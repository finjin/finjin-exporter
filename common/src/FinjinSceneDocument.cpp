//Copyright (c) 2017 Finjin
//
//This file is part of Finjin Exporter (finjin-exporter).
//
//Finjin Exporter is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//Finjin Exporter is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with Finjin Exporter.  If not, see <http://www.gnu.org/licenses/>.


//Includes----------------------------------------------------------------------
#include "FinjinPrecompiled.hpp"
#include "FinjinSceneDocument.hpp"
#include "StringUtilities.hpp"
#include "ApplicationAccessor.hpp"
#include "FinjinVersion.hpp"
#include "FinjinResourceManager.hpp"
#include "StringSplitter.hpp"
#include "ExporterUtilities.hpp"
#include "finjin/common/WxDataChunkWriter.hpp"
#include "finjin/common/WxConfigDocumentReader.hpp"
#include "finjin/engine/StandardAssetDocumentChunkNames.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyNames.hpp"
#include "finjin/engine/StandardAssetDocumentPropertyValues.hpp"

using namespace Finjin::Engine;
using namespace Finjin::Exporter;


//Local functions---------------------------------------------------------------
static bool ChildOrderLess(const std::shared_ptr<FinjinSceneDocument_Item>& item1, const std::shared_ptr<FinjinSceneDocument_Item>& item2)
{
    return item1->childOrder < item2->childOrder;
}

template <size_t count>
std::array<float, count> ParseFloats(const wxString& value)
{
    std::array<float, count> values;

    StringSplitter splitValues(value, wxT(FINJIN_EXPORTER_DEFAULT_VALUE_SEPARATOR_CHAR));
    auto valueCount = std::min(count, splitValues.size());
    for (size_t i = 0; i < valueCount; i++)
        values[i] = StringUtilities::ToFloat(splitValues[i]);
    for (size_t i = valueCount; i < values.size(); i++)
        values[i] = 0;

    return values;
}


//Implementation----------------------------------------------------------------

//FinjinSceneDocument_Item::UserData
void FinjinSceneDocument_Item::UserData::operator = (const wxString& rawData)
{
    this->className.clear();
    this->propertyInstances.clear();

    this->userDataClass = nullptr;

    this->rawData = rawData;

    wxString currentSectionName;

    auto userDataUtf8 = rawData.ToUTF8();
    auto userDataUtf8Length = strlen(userDataUtf8.data());
    WxConfigDocumentReader reader;
    for (auto line = reader.Start(userDataUtf8.data(), userDataUtf8Length); line != nullptr; line = reader.Next())
    {
        switch (line->GetType())
        {
            case WxConfigDocumentLine::Type::SECTION:
            {
                if (this->className.empty())
                {
                    currentSectionName = this->className = line->GetSectionName();

                    this->userDataClass = FinjinResourceManager::GetInstance().GetUserDataTypes().GetClass(this->className);
                    if (this->userDataClass != nullptr)
                    {
                        this->userDataClass->GetDataItems(this->userDataClassProperties);
                        this->propertyInstances.reserve(this->userDataClassProperties.size());
                    }
                }
                break;
            }
            case WxConfigDocumentLine::Type::SCOPE_START:
            {
                break;
            }
            case WxConfigDocumentLine::Type::SCOPE_END:
            {
                currentSectionName.clear();
                return;
            }
            case WxConfigDocumentLine::Type::KEY_AND_VALUE:
            {
                if (!currentSectionName.empty())
                {
                    PropertyInstance newPropInstance;

                    newPropInstance.name = line->GetKey();
                    newPropInstance.value = line->GetValue();

                    if (userDataClass != nullptr)
                    {
                        for (auto& prop : this->userDataClassProperties)
                        {
                            if (prop.name == newPropInstance.name)
                            {
                                newPropInstance.prop = &prop;
                                break;
                            }
                        }
                    }

                    //Normalize newlines if necessary
                    if (newPropInstance.prop == nullptr || newPropInstance.prop->type.type == UserDataTypes::DataType::STRING_DATA_TYPE)
                        newPropInstance.value.Replace(wxT("\r\n"), wxT("\n"));

                    this->propertyInstances.push_back(newPropInstance);
                }

                break;
            }
            default: break;
        }
    }
}

//FinjinSceneDocument_Item
FinjinSceneDocument_Item::FinjinSceneDocument_Item(FinjinSceneDocument* sceneDocument)
{
    this->sceneDocument = sceneDocument;
    this->parent = nullptr;
    this->childOrder = 0;
}

FinjinSceneDocument_Item::~FinjinSceneDocument_Item()
{
}

void FinjinSceneDocument_Item::AddChildItem(std::shared_ptr<FinjinSceneDocument_Item> item)
{
    if (item != nullptr)
    {
        this->childItems.push_back(std::shared_ptr<FinjinSceneDocument_Item>(item));
        item->parent = this;
    }
}

void FinjinSceneDocument_Item::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    if (!this->childItems.empty())
    {
        for (auto child : this->childItems)
        {
            child->Write(writer, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }
}

void FinjinSceneDocument_Item::Write(WxDataChunkWriter& writer, ExportFlags flags, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    //Type
    if (flags & EXPORT_TYPE)
    {
        if (!this->typeName.empty())
        {
            writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, this->typeName, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }

    //Name
    if (flags & EXPORT_NAME)
    {
        if (!this->name.empty())
        {
            writer.WriteString(StandardAssetDocumentPropertyNames::NAME, this->name, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }

    //ID
    if (flags & EXPORT_ID)
    {
        if (!this->id.empty())
        {
            WxUuid parsedID;
            if (WxUuid::Parse(parsedID, this->id))
            {
                writer.WriteUuid(StandardAssetDocumentPropertyNames::ID, parsedID, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }
        }
    }

    //User data
    if (flags & EXPORT_USER_DATA)
    {
        ExportUserData(writer, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Note tracks
    if (flags & EXPORT_NOTE_TRACKS)
    {
        ExportNoteTracks(writer, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Flags
    if (flags & EXPORT_FLAGS)
    {
        if (!this->namedFlags.empty())
        {
            writer.WriteChunk(StandardAssetDocumentChunkNames::FLAGS, [this](WxDataChunkWriter& writer, WxError& error)
            {
                FINJIN_WX_ERROR_METHOD_START(error);

                //Count
                writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, this->namedFlags.size(), error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                //Values
                writer.WriteStridedUInt8s(StandardAssetDocumentPropertyNames::VALUES, &this->namedFlags[0].bit, this->namedFlags.size(), WxDataChunkWriteStride(1, sizeof(NamedFlag)), error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }
}

void FinjinSceneDocument_Item::ExportUserData(WxDataChunkWriter& writer, WxError& error)
{
    if (!this->userData.IsValid())
        return;

    writer.WriteChunk(StandardAssetDocumentChunkNames::USER_DATA, [this](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        if (!this->userData.propertyInstances.empty())
        {
            auto allPropertiesDefined = true;
            for (auto& propInstance : this->userData.propertyInstances)
            {
                if (propInstance.prop == nullptr)
                {
                    allPropertiesDefined = false;
                    break;
                }
            }

            if (!allPropertiesDefined)
            {
                writer.WriteCount(StandardAssetDocumentPropertyNames::USER_DATA_STRING_PROPERTY_COUNT, this->userData.propertyInstances.size(), error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }

            writer.WriteString(StandardAssetDocumentPropertyNames::USER_DATA_CLASS_NAME, this->userData.className, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            for (auto& propInstance : this->userData.propertyInstances)
            {
                auto keyUtf8 = propInstance.name.ToUTF8();
                WxChunkPropertyName propertyName(keyUtf8.data());

                if (allPropertiesDefined)
                {
                    switch (propInstance.prop->type.type)
                    {
                        case UserDataTypes::DataType::STRING_DATA_TYPE:
                        {
                            if (!propInstance.value.empty())
                            {
                                writer.WriteString(propertyName, propInstance.value, error);
                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                            }
                            break;
                        }
                        case UserDataTypes::DataType::INT_DATA_TYPE:
                        {
                            auto i = StringUtilities::ToInt(propInstance.value);
                            writer.WriteInt32(propertyName, i, error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                            break;
                        }
                        case UserDataTypes::DataType::FLOAT_DATA_TYPE:
                        {
                            auto f = StringUtilities::ToFloat(propInstance.value);
                            writer.WriteFloat(propertyName, f, error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                            break;
                        }
                        case UserDataTypes::DataType::BOOL_DATA_TYPE:
                        {
                            auto b = StringUtilities::ToBool(propInstance.value);
                            writer.WriteBool(propertyName, b, error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                            break;
                        }
                        case UserDataTypes::DataType::RGB_DATA_TYPE:
                        {
                            auto values = ParseFloats<3>(propInstance.value);
                            writer.WriteFloats(propertyName, values.data(), values.size(), error);
                            break;
                        }
                        case UserDataTypes::DataType::RGBA_DATA_TYPE:
                        {
                            auto values = ParseFloats<4>(propInstance.value);
                            writer.WriteFloats(propertyName, values.data(), values.size(), error);
                            break;
                        }
                    }
                }
                else
                {
                    if (!propInstance.value.empty())
                    {
                        writer.WriteString(propertyName, propInstance.value, error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                    }
                }
            }
        }
        else
        {
            wxString className;
            wxString currentSectionName;

            auto userDataUtf8 = this->userData.rawData.ToUTF8();
            auto userDataUtf8Length = strlen(userDataUtf8.data());
            WxConfigDocumentReader reader;
            for (auto line = reader.Start(userDataUtf8.data(), userDataUtf8Length); line != nullptr; line = reader.Next())
            {
                switch (line->GetType())
                {
                    case WxConfigDocumentLine::Type::SECTION:
                    {
                        if (className.empty())
                        {
                            writer.WriteCount(StandardAssetDocumentPropertyNames::USER_DATA_STRING_PROPERTY_COUNT, reader.GetItemCount(wxEmptyString), error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                            currentSectionName = className = line->GetSectionName();
                            writer.WriteString(StandardAssetDocumentPropertyNames::USER_DATA_CLASS_NAME, currentSectionName, error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                        }
                        break;
                    }
                    case WxConfigDocumentLine::Type::SCOPE_START:
                    {
                        break;
                    }
                    case WxConfigDocumentLine::Type::SCOPE_END:
                    {
                        currentSectionName.clear();
                        break;
                    }
                    case WxConfigDocumentLine::Type::KEY_AND_VALUE:
                    {
                        if (!currentSectionName.empty())
                        {
                            auto key = line->GetKey();
                            auto value = line->GetValue();
                            if (!value.empty())
                            {
                                auto keyUtf8 = key.ToUTF8();
                                WxChunkPropertyName propertyName(keyUtf8.data());

                                writer.WriteString(propertyName, value, error);
                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                            }
                        }

                        break;
                    }
                    default: break;
                }
            }
        }
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

void FinjinSceneDocument_Item::ExportNoteTracks(WxDataChunkWriter& writer, WxError& error)
{
    if (this->noteTracks.empty())
        return;

    writer.WriteChunk(StandardAssetDocumentChunkNames::NOTE_TRACKS, [this](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        //Count
        writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, this->noteTracks.size(), error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

        //Each note track
        for (size_t trackIndex = 0; trackIndex < this->noteTracks.size(); trackIndex++)
        {
            auto& noteTrack = this->noteTracks[trackIndex];

            //Note track
            WxChunkName noteTrackChunkName(StandardAssetDocumentChunkNames::NOTE_TRACK, trackIndex);
            writer.WriteChunk(noteTrackChunkName, [this, &noteTrack](WxDataChunkWriter& writer, WxError& error)
            {
                FINJIN_WX_ERROR_METHOD_START(error);

                //Name
                if (!noteTrack.name.empty())
                {
                    writer.WriteString(StandardAssetDocumentPropertyNames::NAME, noteTrack.name, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }

                //Keys
                if (!noteTrack.keys.empty())
                {
                    writer.WriteChunk(StandardAssetDocumentChunkNames::KEYS, [this, &noteTrack](WxDataChunkWriter& writer, WxError& error)
                    {
                        FINJIN_WX_ERROR_METHOD_START(error);

                        //Count
                        writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, noteTrack.keys.size(), error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                        //Each key
                        for (size_t keyIndex = 0; keyIndex < noteTrack.keys.size(); keyIndex++)
                        {
                            auto& key = noteTrack.keys[keyIndex];

                            WxChunkName keyChunkName(StandardAssetDocumentChunkNames::KEY, keyIndex);
                            writer.WriteChunk(keyChunkName, [this, &key](WxDataChunkWriter& writer, WxError& error)
                            {
                                //Time
                                writer.WriteTimeDuration(StandardAssetDocumentPropertyNames::TIME, key.time, error);
                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                                //Text
                                writer.WriteString(StandardAssetDocumentPropertyNames::CONTENT, key.text, error);
                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                            }, error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                        }
                    }, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }
            }, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

//FinjinSceneDocument_MovableObject
FinjinSceneDocument_MovableObject::FinjinSceneDocument_MovableObject(FinjinSceneDocument* sceneDocument) : Super(sceneDocument)
{
}

void FinjinSceneDocument_MovableObject::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    Super::Write(writer, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

void FinjinSceneDocument_MovableObject::Write(WxDataChunkWriter& writer, ExportFlags flags, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    Super::Write(writer, flags, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    ExporterUtilities::WriteTransformProperties(writer, this->objectOffsetTransform, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

//FinjinSceneDocument_Camera
FinjinSceneDocument_Camera::FinjinSceneDocument_Camera(FinjinSceneDocument* sceneDocument) : Super(sceneDocument)
{
    this->typeName = StandardAssetDocumentPropertyValues::TypeName::CAMERA;

    this->fov = 0;
    this->isOrtho = false;
    this->orthoWidth = this->orthoHeight = 50;
    this->nearClip = this->farClip = 0;
}

void FinjinSceneDocument_Camera::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    Super::Write
        (
        writer,
        EXPORT_TYPE |
        EXPORT_NAME |
        EXPORT_ID |
        EXPORT_USER_DATA |
        EXPORT_NOTE_TRACKS,
        error
        );
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Field of view
    writer.WriteFloat(StandardAssetDocumentPropertyNames::FOV, this->fov, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Ortho
    if (this->isOrtho)
    {
        writer.WriteString(StandardAssetDocumentPropertyNames::PROJECTION_TYPE, StandardAssetDocumentPropertyValues::CameraProjection::ORTHOGRAPHIC, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

        float size[2] = {this->orthoWidth, this->orthoHeight};
        writer.WriteFloats(StandardAssetDocumentPropertyNames::ORTHO_SIZE, size, 2, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Near/far clip
    if (this->nearClip != this->farClip)
    {
        float clipping[2] = {this->nearClip, this->farClip};
        writer.WriteFloats(StandardAssetDocumentPropertyNames::CLIPPING, clipping, 2, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    Super::Write(writer, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

//FinjinSceneDocument_Light
FinjinSceneDocument_Light::FinjinSceneDocument_Light(FinjinSceneDocument* sceneDocument) : Super(sceneDocument)
{
    this->typeName = StandardAssetDocumentPropertyValues::TypeName::LIGHT;

    this->isOn = false;
    this->lightType = LightType::DIRECTIONAL_LIGHT;
    this->castShadows = false;
    this->power = 1;
    this->range[0] = 0;
    this->range[1] = 10000;
    this->coneRange[0] = this->coneRange[1] = 0;
}

void FinjinSceneDocument_Light::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    Super::Write
        (
        writer,
        EXPORT_TYPE |
        EXPORT_NAME |
        EXPORT_ID |
        EXPORT_USER_DATA |
        EXPORT_NOTE_TRACKS,
        error
        );
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Light type
    wxString lightType;
    switch (this->lightType)
    {
        case LightType::SPOT_LIGHT: lightType = StandardAssetDocumentPropertyValues::LightType::SPOT; break;
        case LightType::POINT_LIGHT: lightType = StandardAssetDocumentPropertyValues::LightType::POINT; break;
        case LightType::DIRECTIONAL_LIGHT: lightType = StandardAssetDocumentPropertyValues::LightType::DIRECTIONAL; break;
    }
    if (!lightType.empty())
    {
        writer.WriteString(StandardAssetDocumentPropertyNames::LIGHT_TYPE, lightType, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Active?
    writer.WriteBool(StandardAssetDocumentPropertyNames::ACTIVE, this->isOn, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Cast shadows
    writer.WriteBool(StandardAssetDocumentPropertyNames::CAST_SHADOWS, this->castShadows, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Power
    writer.WriteFloat(StandardAssetDocumentPropertyNames::POWER, this->power, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Diffuse color
    writer.WriteFloats(StandardAssetDocumentPropertyNames::COLOR, &this->color.r, 4, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Light range
    if (this->range[1] > 0)
    {
        writer.WriteFloats(StandardAssetDocumentPropertyNames::RANGE, this->range, 2, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Spotlight cone
    if (this->lightType == LightType::SPOT_LIGHT)
    {
        writer.WriteFloats(StandardAssetDocumentPropertyNames::CONE_RANGE, this->coneRange, 2, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    Super::Write(writer, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

bool FinjinSceneDocument_Light::HasEffect(bool checkOn) const
{
    return (!checkOn || this->isOn) && this->power > 0 && !this->color.IsBlack();
}

//FinjinSceneDocument_RenderableMovableObject
FinjinSceneDocument_RenderableMovableObject::FinjinSceneDocument_RenderableMovableObject(FinjinSceneDocument* sceneDocument) : Super(sceneDocument)
{
    this->visibility = OBJECT_VISIBILITY_DEFAULT;
    this->renderPriority = 0;
    this->renderingDistance = 0;
    this->castShadows = true;
    this->receiveShadows = true;
}

void FinjinSceneDocument_RenderableMovableObject::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    Super::Write(writer, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

void FinjinSceneDocument_RenderableMovableObject::Write(WxDataChunkWriter& writer, ExportFlags flags, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    Super::Write(writer, flags, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Visibility
    if (flags & EXPORT_VISIBILITY)
    {
        WriteObjectVisibility(writer, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Render queue
    if (flags & EXPORT_RENDER_QUEUE)
    {
        if (!this->renderQueue.empty())
        {
            writer.WriteString(StandardAssetDocumentPropertyNames::RENDER_QUEUE, this->renderQueue, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }

    //Render priority
    if (flags & EXPORT_RENDER_PRIORITY)
    {
        if (this->renderPriority != 0)
        {
            writer.WriteInt32(StandardAssetDocumentPropertyNames::RENDER_PRIORITY, this->renderPriority, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }

    //Rendering distance
    if (flags & EXPORT_RENDERING_DISTANCE)
    {
        if (this->renderingDistance != 0)
        {
            writer.WriteFloat(StandardAssetDocumentPropertyNames::RENDER_DISTANCE, this->renderingDistance, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }

    //Casts shadows?
    if (flags & EXPORT_CAST_SHADOWS)
    {
        writer.WriteBool(StandardAssetDocumentPropertyNames::CAST_SHADOWS, this->castShadows, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Receives shadows?
    if (flags & EXPORT_RECEIVE_SHADOWS)
    {
        writer.WriteBool(StandardAssetDocumentPropertyNames::RECEIVE_SHADOWS, this->receiveShadows, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
}

void FinjinSceneDocument_RenderableMovableObject::WriteObjectVisibility(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    switch (this->visibility)
    {
        case OBJECT_VISIBLE:
        {
            writer.WriteBool(StandardAssetDocumentPropertyNames::VISIBLE, true, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            break;
        }
        case OBJECT_HIDDEN:
        {
            writer.WriteBool(StandardAssetDocumentPropertyNames::VISIBLE, false, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            break;
        }
        default: break;
    }
}

//FinjinSceneDocument_BoneAttachment
FinjinSceneDocument_BoneAttachment::FinjinSceneDocument_BoneAttachment(FinjinSceneDocument* sceneDocument) : Super(sceneDocument)
{
}

void FinjinSceneDocument_BoneAttachment::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    //Name
    Super::Write(writer, EXPORT_NAME, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Bone name
    writer.WriteString(StandardAssetDocumentPropertyNames::BONE, this->boneName, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Transform
    ExporterUtilities::WriteTransformProperties(writer, this->transform, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    Super::Write(writer, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

//FinjinSceneDocument_BoneAttachments
FinjinSceneDocument_BoneAttachments::FinjinSceneDocument_BoneAttachments(FinjinSceneDocument* sceneDocument) : Super(sceneDocument)
{
}

void FinjinSceneDocument_BoneAttachments::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    writer.WriteChunk(StandardAssetDocumentChunkNames::BONE_ATTACHMENTS, [this](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        //Count
        writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, this->childItems.size(), error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

        //Each bone attachment
        for (size_t attachmentIndex = 0; attachmentIndex < this->childItems.size(); attachmentIndex++)
        {
            auto attachment = std::static_pointer_cast<FinjinSceneDocument_BoneAttachment>(this->childItems[attachmentIndex]);
            WxChunkName chunkName(StandardAssetDocumentChunkNames::BONE_ATTACHMENT, attachmentIndex);
            writer.WriteChunk(chunkName, [attachment](WxDataChunkWriter& writer, WxError& error)
            {
                FINJIN_WX_ERROR_METHOD_START(error);

                attachment->Write(writer, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

//FinjinSceneDocument_Entity
FinjinSceneDocument_Entity::FinjinSceneDocument_Entity(FinjinSceneDocument* sceneDocument) : Super(sceneDocument)
{
    this->typeName = StandardAssetDocumentPropertyValues::TypeName::ENTITY;
}

FinjinSceneDocument_Entity::~FinjinSceneDocument_Entity()
{
}

void FinjinSceneDocument_Entity::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    Super::Write
        (
        writer,
        EXPORT_TYPE |
        EXPORT_NAME |
        EXPORT_ID |
        EXPORT_USER_DATA |
        EXPORT_FLAGS |
        EXPORT_VISIBILITY |
        EXPORT_RENDER_QUEUE |
        EXPORT_RENDER_PRIORITY |
        EXPORT_RENDERING_DISTANCE |
        EXPORT_NOTE_TRACKS |
        EXPORT_CAST_SHADOWS |
        EXPORT_RECEIVE_SHADOWS,
        error
        );
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Mesh file
    writer.WriteString(StandardAssetDocumentPropertyNames::MESH_REF, this->meshRef().ToUriString(), error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Subentities
    if (!this->subentities.empty())
    {
        writer.WriteChunk(StandardAssetDocumentChunkNames::SUBENTITIES, [this](WxDataChunkWriter& writer, WxError& error)
        {
            FINJIN_WX_ERROR_METHOD_START(error);

            //Count
            writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, this->subentities.size(), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Each subentity
            for (size_t subentityIndex = 0; subentityIndex < this->subentities.size(); subentityIndex++)
            {
                WxChunkName subentityChunkName(StandardAssetDocumentChunkNames::SUBENTITY, subentityIndex);
                writer.WriteChunk(subentityChunkName, [this, subentityIndex](WxDataChunkWriter& writer, WxError& error)
                {
                    FINJIN_WX_ERROR_METHOD_START(error);

                    //Material ref
                    writer.WriteString(StandardAssetDocumentPropertyNames::MATERIAL_REF, this->subentities[subentityIndex].materialRef().ToUriString(), error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                    //Render queue
                    if (!this->subentities[subentityIndex].renderQueue.empty())
                    {
                        writer.WriteString(StandardAssetDocumentPropertyNames::RENDER_QUEUE, this->subentities[subentityIndex].renderQueue, error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                    }

                    //Render queue priority
                    if (this->subentities[subentityIndex].renderPriority != std::numeric_limits<int>::min())
                    {
                        writer.WriteInt32(StandardAssetDocumentPropertyNames::RENDER_PRIORITY, this->subentities[subentityIndex].renderPriority, error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                    }
                }, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Manual LODs
    if (!this->manualLods.empty())
    {
        writer.WriteChunk(StandardAssetDocumentChunkNames::MANUAL_LODS, [this](WxDataChunkWriter& writer, WxError& error)
        {
            FINJIN_WX_ERROR_METHOD_START(error);

            //Count
            writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, this->manualLods.size(), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Each manual LOD
            for (size_t manualLodIndex = 0; manualLodIndex < this->manualLods.size(); manualLodIndex++)
            {
                auto& manualLod = this->manualLods[manualLodIndex];

                //Manual LOD
                WxChunkName manualLodChunkName(StandardAssetDocumentChunkNames::MANUAL_LOD, manualLodIndex);
                writer.WriteChunk(manualLodChunkName, [this, &manualLod](WxDataChunkWriter& writer, WxError& error)
                {
                    FINJIN_WX_ERROR_METHOD_START(error);

                    //Subentities
                    writer.WriteChunk(StandardAssetDocumentChunkNames::SUBENTITIES, [this, &manualLod](WxDataChunkWriter& writer, WxError& error)
                    {
                        FINJIN_WX_ERROR_METHOD_START(error);

                        //Count
                        writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, manualLod.subentityMaterialRefs.size(), error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                        //Each subentity
                        for (size_t subentityIndex = 0; subentityIndex < manualLod.subentityMaterialRefs.size(); subentityIndex++)
                        {
                            WxChunkName subentityChunkName(StandardAssetDocumentChunkNames::SUBENTITY, subentityIndex);
                            writer.WriteChunk(subentityChunkName, [this, &manualLod, subentityIndex](WxDataChunkWriter& writer, WxError& error)
                            {
                                FINJIN_WX_ERROR_METHOD_START(error);

                                //Material ref
                                writer.WriteString(StandardAssetDocumentPropertyNames::MATERIAL_REF, manualLod.subentityMaterialRefs[subentityIndex]().ToUriString(), error);
                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                            }, error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                        }
                    }, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Bone attachments
    if (this->boneAttachments != nullptr)
    {
        this->boneAttachments->Write(writer, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    Super::Write(writer, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

void FinjinSceneDocument_Entity::AddBoneAttachment(std::shared_ptr<FinjinSceneDocument_BoneAttachment> boneAttachment)
{
    if (boneAttachment != nullptr)
    {
        if (this->boneAttachments == nullptr)
            this->boneAttachments.reset(new FinjinSceneDocument_BoneAttachments(this->sceneDocument));
        this->boneAttachments->AddChildItem(boneAttachment);
    }
}

//FinjinSceneDocument_StaticAndInstancedGeometryEntity
FinjinSceneDocument_StaticAndInstancedGeometryEntity::FinjinSceneDocument_StaticAndInstancedGeometryEntity(FinjinSceneDocument* sceneDocument) : Super(sceneDocument)
{
}

void FinjinSceneDocument_StaticAndInstancedGeometryEntity::Write(WxDataChunkWriter& writer, WxError& error)
{
    //TODO: Implement this or remove this
    /*
    DataElementPtr entityElement = documentSet.AddChildElement(StandardAssetDocumentPropertyNames::ENTITY);
    documentSet.PushDataElement(entityElement);

    //Mesh file
    entityElement->SetAttribute(StandardAssetDocumentPropertyNames::MESH_FILE, this->meshFile);

    //Subentities
    if (!this->subentityMaterialNames.empty())
    {
        DataElementPtr subentitiesElement = entityElement->AddChildElement(StandardAssetDocumentPropertyNames::SUBENTITIES);
        for (size_t i = 0; i < this->subentityMaterialNames.size(); i++)
        {
            DataElementPtr subentityElement = subentitiesElement->AddChildElement(StandardAssetDocumentPropertyNames::SUBENTITY);
            subentityElement->SetAttribute(StandardAssetDocumentPropertyNames::INDEX, i);
            subentityElement->SetAttribute(StandardAssetDocumentPropertyNames::MATERIAL_NAME, this->subentityMaterialNames[i]);
        }
    }

    //Position/rotation/scale
    entityElement->AddChildElement(StandardAssetDocumentPropertyNames::POSITION, this->position);
    entityElement->AddQuaternionChildElement(StandardAssetDocumentPropertyNames::ROTATION, this->rotation);
    entityElement->AddChildElement(StandardAssetDocumentPropertyNames::SCALE, this->scale);

    Super::Export(documentSet);

    documentSet.PopDataElement();*/
}

//FinjinSceneDocument_Entities
FinjinSceneDocument_Entities::FinjinSceneDocument_Entities(FinjinSceneDocument* sceneDocument) : Super(sceneDocument)
{
}

void FinjinSceneDocument_Entities::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    writer.WriteChunk(StandardAssetDocumentChunkNames::ENTITIES, [this](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        Super::Write(writer, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}


//FinjinSceneDocument_StaticGeometry
FinjinSceneDocument_StaticGeometry::FinjinSceneDocument_StaticGeometry(FinjinSceneDocument* sceneDocument) : Super(sceneDocument)
{
    this->isVisible = true;
}

FinjinSceneDocument_StaticGeometry::~FinjinSceneDocument_StaticGeometry()
{
}

void FinjinSceneDocument_StaticGeometry::AddChildItem(std::shared_ptr<FinjinSceneDocument_Item> item)
{
    if (FINJIN_EXPORTER_OBJECT_IS_TYPE_OF(item.get(), FinjinSceneDocument_StaticAndInstancedGeometryEntity))
    {
        if (this->entities == nullptr)
            this->entities.reset(new FinjinSceneDocument_Entities(this->sceneDocument));
        this->entities->AddChildItem(item);
    }
    else
        Super::AddChildItem(item);
}

void FinjinSceneDocument_StaticGeometry::Write(WxDataChunkWriter& writer, WxError& error)
{
    //TODO: Implement this or remove this
    /*
    DataElementPtr staticGeometryElement = documentSet.AddChildElement(StandardAssetDocumentChunkNames::STATIC_GEOMETRY);
    documentSet.PushDataElement(staticGeometryElement);

    //Visibility
    staticGeometryElement->SetAttribute(StandardAssetDocumentPropertyNames::VISIBLE, this->isVisible);

    //Origin
    staticGeometryElement->AddChildElement(StandardAssetDocumentPropertyNames::ORIGIN, this->position);

    //Dimensions
    staticGeometryElement->AddChildElement(StandardAssetDocumentPropertyNames::SIZE, this->dimensions);

    Super::Export
        (
        documentSet,
        EXPORT_TYPE |
        EXPORT_NAME |
        EXPORT_RENDER_QUEUE |
        EXPORT_RENDER_PRIORITY |
        EXPORT_RENDERING_DISTANCE |
        EXPORT_CAST_SHADOWS
        );

    if (this->entities != nullptr)
        this->entities->Export(documentSet);

    documentSet.PopDataElement();*/
}

//FinjinSceneDocument_StaticGeometries
FinjinSceneDocument_StaticGeometries::FinjinSceneDocument_StaticGeometries(FinjinSceneDocument* sceneDocument) : Super(sceneDocument)
{
}

void FinjinSceneDocument_StaticGeometries::Write(WxDataChunkWriter& writer, WxError& error)
{
    //TODO: Implement this or remove this
    /*
    DataElementPtr staticGeometriesElement = documentSet.AddChildElement(StandardAssetDocumentPropertyNames::STATIC_GEOMETRIES);
    documentSet.PushDataElement(staticGeometriesElement);

    Super::Export(documentSet);

    documentSet.PopDataElement();*/
}

//FinjinSceneDocument_InstancedGeometry
FinjinSceneDocument_InstancedGeometry::FinjinSceneDocument_InstancedGeometry(FinjinSceneDocument* sceneDocument) : Super(sceneDocument)
{
    this->isVisible = true;
    this->batchCount = 0;
}

FinjinSceneDocument_InstancedGeometry::~FinjinSceneDocument_InstancedGeometry()
{
}

void FinjinSceneDocument_InstancedGeometry::AddChildItem(std::shared_ptr<FinjinSceneDocument_Item> item)
{
    if (FINJIN_EXPORTER_OBJECT_IS_TYPE_OF(item.get(), FinjinSceneDocument_StaticAndInstancedGeometryEntity))
    {
        if (this->entities == nullptr)
            this->entities.reset(new FinjinSceneDocument_Entities(this->sceneDocument));
        this->entities->AddChildItem(item);
    }
    else
        Super::AddChildItem(item);
}

void FinjinSceneDocument_InstancedGeometry::Write(WxDataChunkWriter& writer, WxError& error)
{
    //TODO: Implement this or remove this
    /*
    DataElementPtr instancedGeometryElement = documentSet.AddChildElement(StandardAssetDocumentPropertyNames::INSTANCED_GEOMETRY);
    documentSet.PushDataElement(instancedGeometryElement);

    //Visibility
    instancedGeometryElement->SetAttribute(StandardAssetDocumentPropertyNames::VISIBLE, this->isVisible);

    //Batch count
    if (this->batchCount > 0)
        instancedGeometryElement->SetAttribute(StandardAssetDocumentPropertyNames::BATCH_COUNT, this->batchCount);

    //Origin
    instancedGeometryElement->AddChildElement(StandardAssetDocumentPropertyNames::ORIGIN, this->position);

    //Dimensions
    instancedGeometryElement->AddChildElement(StandardAssetDocumentPropertyNames::SIZE, this->dimensions);

    Super::Export
        (
        documentSet,
        EXPORT_TYPE |
        EXPORT_NAME |
        EXPORT_RENDER_QUEUE |
        EXPORT_RENDER_PRIORITY |
        EXPORT_RENDERING_DISTANCE |
        EXPORT_CAST_SHADOWS
        );

    if (this->entities != nullptr)
        this->entities->Export(documentSet);

    documentSet.PopDataElement();*/
}

//FinjinSceneDocument_InstancedGeometries
FinjinSceneDocument_InstancedGeometries::FinjinSceneDocument_InstancedGeometries(FinjinSceneDocument* sceneDocument) : Super(sceneDocument)
{
}

void FinjinSceneDocument_InstancedGeometries::Write(WxDataChunkWriter& writer, WxError& error)
{
    //TODO: Implement this or remove this
    /*
    DataElementPtr instancedGeometriesElement = documentSet.AddChildElement(StandardAssetDocumentPropertyNames::INSTANCED_GEOMETRIES);
    documentSet.PushDataElement(instancedGeometriesElement);

    Super::Export(documentSet);

    documentSet.PopDataElement();*/
}

//FinjinSceneDocument_Sky
FinjinSceneDocument_Sky::FinjinSceneDocument_Sky(FinjinSceneDocument* sceneDocument) : Super(sceneDocument)
{
    this->typeName = StandardAssetDocumentPropertyValues::TypeName::SKY;
    this->enable = true;
}

void FinjinSceneDocument_Sky::SetAnimations(std::shared_ptr<FinjinSceneDocument_NodeAnimations> animations)
{
    this->animations = animations;
}

void FinjinSceneDocument_Sky::AddAnimation(std::shared_ptr<FinjinSceneDocument_NodeAnimation> animation)
{
    if (animation != nullptr)
    {
        if (this->animations == nullptr)
            this->animations.reset(new FinjinSceneDocument_NodeAnimations(this->sceneDocument));
        this->animations->AddChildItem(animation);
    }
}

void FinjinSceneDocument_Sky::AddChildItem(std::shared_ptr<FinjinSceneDocument_Item> item)
{
    if (item != nullptr)
    {
        if (FINJIN_EXPORTER_OBJECT_IS_TYPE_OF(item.get(), FinjinSceneDocument_NodeAnimations))
            SetAnimations(std::static_pointer_cast<FinjinSceneDocument_NodeAnimations>(animations));
        else if (FINJIN_EXPORTER_OBJECT_IS_TYPE_OF(item.get(), FinjinSceneDocument_NodeAnimation))
            AddAnimation(std::static_pointer_cast<FinjinSceneDocument_NodeAnimation>(item));
        else
            Super::AddChildItem(item);
    }
}

void FinjinSceneDocument_Sky::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    //Type
    writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, this->typeName, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Transform
    ExporterUtilities::WriteTransformProperties(writer, this->transform, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Enable
    writer.WriteBool(StandardAssetDocumentPropertyNames::ENABLE, this->enable, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Mesh file
    writer.WriteString(StandardAssetDocumentPropertyNames::MESH_REF, this->meshRef().ToUriString(), error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Animations
    if (this->animations != nullptr)
    {
        //Export
        this->animations->Write(writer, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
}

//FinjinSceneDocument_Clipping
FinjinSceneDocument_Clipping::FinjinSceneDocument_Clipping(FinjinSceneDocument* sceneDocument, float nearClip, float farClip)
{
    this->nearClip = nearClip;
    this->farClip = farClip;
}

//FinjinSceneDocument_Shadows
FinjinSceneDocument_Shadows::FinjinSceneDocument_Shadows(FinjinSceneDocument* sceneDocument) : Super(sceneDocument)
{
    this->farDistance = 0;
}

FinjinSceneDocument_Shadows::~FinjinSceneDocument_Shadows()
{
}

void FinjinSceneDocument_Shadows::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    if (!this->technique.empty())
    {
        writer.WriteChunk(StandardAssetDocumentChunkNames::SHADOWS, [this](WxDataChunkWriter& writer, WxError& error)
        {
            FINJIN_WX_ERROR_METHOD_START(error);

            //Technique
            writer.WriteString(StandardAssetDocumentPropertyNames::TECHNIQUE, this->technique, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Far distance
            writer.WriteFloat(StandardAssetDocumentPropertyNames::FAR_DISTANCE, this->farDistance, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Color
            writer.WriteFloats(StandardAssetDocumentPropertyNames::COLOR, &this->color.r, 4, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
}

//FinjinSceneDocument_Fog
FinjinSceneDocument_Fog::FinjinSceneDocument_Fog
    (
    FinjinSceneDocument* sceneDocument,
    FogMode mode,
    float expDensity,
    float linearStart,
    float linearEnd,
    const FinjinColor& color
    ) : Super(sceneDocument)
{
    this->mode = mode;
    this->expDensity = expDensity;
    this->linearStart = linearStart;
    this->linearEnd = linearEnd;
    this->color = color;
}

void FinjinSceneDocument_Fog::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    if (this->mode != FogMode::NONE)
    {
        writer.WriteChunk(StandardAssetDocumentChunkNames::FOG, [this](WxDataChunkWriter& writer, WxError& error)
        {
            FINJIN_WX_ERROR_METHOD_START(error);

            if (this->mode == FogMode::EXP || this->mode == FogMode::EXP2)
            {
                //Mode
                writer.WriteString(StandardAssetDocumentPropertyNames::MODE, this->mode == FogMode::EXP ? StandardAssetDocumentPropertyValues::FogMode::EXP : StandardAssetDocumentPropertyValues::FogMode::EXP2, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                //Density
                writer.WriteFloat(StandardAssetDocumentPropertyNames::EXP_DENSITY, this->expDensity, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }
            else if (this->mode == FogMode::LINEAR)
            {
                //Mode
                writer.WriteString(StandardAssetDocumentPropertyNames::MODE, StandardAssetDocumentPropertyValues::FogMode::LINEAR, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                //Range
                float range[2] = { this->linearStart , this->linearEnd };
                writer.WriteFloats(StandardAssetDocumentPropertyNames::RANGE, range, 2, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }

            //Color
            writer.WriteFloats(StandardAssetDocumentPropertyNames::COLOR, &this->color.r, 4, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            Super::Write(writer, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
}

//FinjinSceneDocument_Environment
FinjinSceneDocument_Environment::FinjinSceneDocument_Environment(FinjinSceneDocument* sceneDocument) : Super(sceneDocument)
{
}

FinjinSceneDocument_Environment::~FinjinSceneDocument_Environment()
{
}

void FinjinSceneDocument_Environment::SetRange(std::shared_ptr<FinjinSceneDocument_Clipping> range)
{
    this->range = range;
    if (this->range != nullptr)
        this->range->parent = this;
}

void FinjinSceneDocument_Environment::SetFog(std::shared_ptr<FinjinSceneDocument_Fog> fog)
{
    this->fog = fog;
    if (this->fog != nullptr)
        this->fog->parent = this;
}

void FinjinSceneDocument_Environment::SetShadows(std::shared_ptr<FinjinSceneDocument_Shadows> shadows)
{
    this->shadows = shadows;
    if (this->shadows != nullptr)
        this->shadows->parent = this;
}

void FinjinSceneDocument_Environment::AddSky(std::shared_ptr<FinjinSceneDocument_Sky> sky)
{
    if (sky != nullptr)
    {
        this->skies.push_back(sky);
        sky->parent = this;
    }
}

void FinjinSceneDocument_Environment::AddChildItem(std::shared_ptr<FinjinSceneDocument_Item> item)
{
    if (item != nullptr)
    {
        if (FINJIN_EXPORTER_OBJECT_IS_TYPE_OF(item.get(), FinjinSceneDocument_Clipping))
            SetRange(std::static_pointer_cast<FinjinSceneDocument_Clipping>(item));
        else if (FINJIN_EXPORTER_OBJECT_IS_TYPE_OF(item.get(), FinjinSceneDocument_Fog))
            SetFog(std::static_pointer_cast<FinjinSceneDocument_Fog>(item));
        else if (FINJIN_EXPORTER_OBJECT_IS_TYPE_OF(item.get(), FinjinSceneDocument_Shadows))
            SetShadows(std::static_pointer_cast<FinjinSceneDocument_Shadows>(item));
        else if (FINJIN_EXPORTER_OBJECT_IS_TYPE_OF(item.get(), FinjinSceneDocument_Sky))
            AddSky(std::static_pointer_cast<FinjinSceneDocument_Sky>(item));
        else
            Super::AddChildItem(item);
    }
}

void FinjinSceneDocument_Environment::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    writer.WriteChunk(StandardAssetDocumentChunkNames::ENVIRONMENT, [this](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        //Ambient light color
        writer.WriteFloats(StandardAssetDocumentPropertyNames::AMBIENT_COLOR, &this->ambientLightColor.r, 4, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

        //Background color
        writer.WriteFloats(StandardAssetDocumentPropertyNames::BACKGROUND_COLOR, &this->backgroundColor.r, 4, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

        //Range
        if (this->range != nullptr)
        {
            float clipping[2] = { this->range->nearClip, this->range->farClip };
            writer.WriteFloats(StandardAssetDocumentPropertyNames::RANGE, clipping, 2, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }

        //Fog
        if (this->fog != nullptr)
        {
            this->fog->Write(writer, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }

        //Shadows
        if (this->shadows != nullptr)
        {
            this->shadows->Write(writer, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }

        //Skies
        if (!this->skies.empty())
        {
            writer.WriteChunk(StandardAssetDocumentChunkNames::SKY_NODES, [this](WxDataChunkWriter& writer, WxError& error)
            {
                FINJIN_WX_ERROR_METHOD_START(error);

                //Count
                writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, this->childItems.size(), error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                //Each sky
                std::stable_sort(this->skies.begin(), this->skies.end(), ChildOrderLess);//Sort by child order
                for (size_t nodeIndex = 0; nodeIndex < this->skies.size(); nodeIndex++)
                {
                    auto sky = this->skies[nodeIndex];

                    WxChunkName skyNodeChunkName(StandardAssetDocumentChunkNames::SKY_NODE, nodeIndex);
                    writer.WriteChunk(skyNodeChunkName, [this, sky](WxDataChunkWriter& writer, WxError& error)
                    {
                        FINJIN_WX_ERROR_METHOD_START(error);

                        sky->Write(writer, error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                    }, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }
            }, error);
            FINJIN_WX_ERROR_METHOD_START(error);
        }

        Super::Write(writer, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

//FinjinSceneDocument_TrackTarget
FinjinSceneDocument_TrackTarget::FinjinSceneDocument_TrackTarget(FinjinSceneDocument* sceneDocument) : Super(sceneDocument)
{
    this->typeName = StandardAssetDocumentPropertyValues::TypeName::TRACK_TARGET;

    this->localDirection.Set(0, 0, -1);
}

FinjinSceneDocument_TrackTarget::~FinjinSceneDocument_TrackTarget()
{
}

void FinjinSceneDocument_TrackTarget::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    writer.WriteChunk(StandardAssetDocumentChunkNames::TRACK_TARGET, [this](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        //Node name
        writer.WriteString(StandardAssetDocumentPropertyNames::NODE, this->nodeName, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

        //Local direction
        writer.WriteFloats(StandardAssetDocumentPropertyNames::LOCAL_DIRECTION, &this->localDirection.x, 3, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

        Super::Write(writer, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

//FinjinSceneDocument_Physical
FinjinSceneDocument_Physical::FinjinSceneDocument_Physical(FinjinSceneDocument* sceneDocument) : Super(sceneDocument)
{
    this->typeName = StandardAssetDocumentPropertyValues::TypeName::PHYSICAL;
}

void FinjinSceneDocument_Physical::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    writer.WriteChunk(StandardAssetDocumentChunkNames::PHYSICAL, [this](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        Super::Write
            (
            writer,
            EXPORT_TYPE |
            EXPORT_NAME |
            EXPORT_ID |
            EXPORT_USER_DATA |
            EXPORT_NOTE_TRACKS,
            error
            );
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

        //Item type
        /*if (!this->type.empty())
        {
            writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, this->type, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }*/

        //Bounding volume
        switch (this->shape.type)
        {
            case Shape::PLANE:
            {
                //Plane
                writer.WriteChunk(StandardAssetDocumentChunkNames::SHAPE, [this](WxDataChunkWriter& writer, WxError& error)
                {
                    FINJIN_WX_ERROR_METHOD_START(error);

                    //Type
                    writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, StandardAssetDocumentPropertyValues::Shape::PLANE, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                    //Plane
                    writer.WriteFloats(StandardAssetDocumentPropertyNames::PLANE, &this->shape.plane.x, 4, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                break;
            }
            case Shape::SPHERE:
            {
                //Sphere bounding volume
                writer.WriteChunk(StandardAssetDocumentChunkNames::SHAPE, [this](WxDataChunkWriter& writer, WxError& error)
                {
                    FINJIN_WX_ERROR_METHOD_START(error);

                    //Type
                    writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, StandardAssetDocumentPropertyValues::Shape::SPHERE, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                    //Radius
                    writer.WriteFloat(StandardAssetDocumentPropertyNames::RADIUS, this->shape.radius, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                break;
            }
            case Shape::BOX:
            {
                //Box bounding volume
                writer.WriteChunk(StandardAssetDocumentChunkNames::SHAPE, [this](WxDataChunkWriter& writer, WxError& error)
                {
                    FINJIN_WX_ERROR_METHOD_START(error);

                    //Type
                    writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, StandardAssetDocumentPropertyValues::Shape::BOX, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                    //Size
                    writer.WriteFloats(StandardAssetDocumentPropertyNames::SIZE, &this->shape.size.x, 3, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                break;
            }
            case Shape::CYLINDER:
            case Shape::CAPSULE:
            {
                //Cylinder or capsule volume
                writer.WriteChunk(StandardAssetDocumentChunkNames::SHAPE, [this](WxDataChunkWriter& writer, WxError& error)
                {
                    FINJIN_WX_ERROR_METHOD_START(error);

                    //Type
                    writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, this->shape.type == Shape::CYLINDER ? StandardAssetDocumentPropertyValues::Shape::CYLINDER : StandardAssetDocumentPropertyValues::Shape::CAPSULE, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                    //Height
                    writer.WriteFloat(StandardAssetDocumentPropertyNames::HEIGHT, this->shape.size.x, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                    //Radius
                    writer.WriteFloat(StandardAssetDocumentPropertyNames::RADIUS, this->shape.radius, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                break;
            }
            case Shape::MESH:
            {
                writer.WriteChunk(StandardAssetDocumentChunkNames::SHAPE, [this](WxDataChunkWriter& writer, WxError& error)
                {
                    FINJIN_WX_ERROR_METHOD_START(error);

                    //Type
                    writer.WriteString(StandardAssetDocumentPropertyNames::TYPE, StandardAssetDocumentPropertyValues::Shape::MESH, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                    //Primitive type
                    writer.WriteString(StandardAssetDocumentPropertyNames::PRIMITIVE_TYPE, StandardAssetDocumentPropertyValues::PrimitiveType::TRIANGLE_LIST, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                    //Vertex count
                    writer.WriteCount(StandardAssetDocumentPropertyNames::VERTEX_COUNT, this->shape.meshFaces.size() * 3, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                    //Vertices - # faces * 3 vertices per face * 3 floats per vertex
                    writer.WriteStridedFloats(StandardAssetDocumentPropertyNames::VERTEX, &this->shape.meshFaces[0].vertex[0].x, this->shape.meshFaces.size() * 3 * 3, WxDataChunkWriteStride(3 * 3, sizeof(Shape::Face)), error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                break;
            }
            default: break;
        }

        Super::Write(writer, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

//FinjinSceneDocument_NodeAnimation
FinjinSceneDocument_NodeAnimation::FinjinSceneDocument_NodeAnimation(FinjinSceneDocument* sceneDocument) : Super(sceneDocument)
{
    this->enable = false;
    this->loop = false;
}

void FinjinSceneDocument_NodeAnimation::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    //Name
    Super::Write(writer, EXPORT_NAME, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Enable
    writer.WriteBool(StandardAssetDocumentPropertyNames::ENABLE, this->enable, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Loop
    writer.WriteBool(StandardAssetDocumentPropertyNames::LOOP, this->loop, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Keys
    if (!this->keys.empty())
    {
        //Animation length
        auto length = WxTimeDuration::GetZero();
        if (this->keys.size() > 1)
            length = this->keys[this->keys.size() - 1].time - this->keys[0].time;
        writer.WriteTimeDuration(StandardAssetDocumentPropertyNames::LENGTH, length, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

        //Keys
        writer.WriteChunk(StandardAssetDocumentChunkNames::NODE_ANIMATION_KEYS, [this](WxDataChunkWriter& writer, WxError& error)
        {
            FINJIN_WX_ERROR_METHOD_START(error);

            //Count
            writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, this->keys.size(), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            auto& firstKey = this->keys[0];

            //Times
            writer.WriteStridedTimeDurations(StandardAssetDocumentPropertyNames::TIME, &firstKey.time, this->keys.size(), WxDataChunkWriteStride(1, sizeof(Key)), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Node transformations
            writer.WriteStridedFloats(StandardAssetDocumentPropertyNames::TRANSFORM, &firstKey.m44[0][0], this->keys.size() * 16, WxDataChunkWriteStride(16, sizeof(Key)), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    Super::Write(writer, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

//FinjinSceneDocument_NodeAnimations
FinjinSceneDocument_NodeAnimations::FinjinSceneDocument_NodeAnimations(FinjinSceneDocument* sceneDocument) : Super(sceneDocument)
{
}

void FinjinSceneDocument_NodeAnimations::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    writer.WriteChunk(StandardAssetDocumentChunkNames::NODE_ANIMATIONS, [this](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        //Count
        writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, this->childItems.size(), error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

        //Each animation
        for (size_t animationIndex = 0; animationIndex < this->childItems.size(); animationIndex++)
        {
            auto animation = std::static_pointer_cast<FinjinSceneDocument_NodeAnimation>(this->childItems[animationIndex]);

            WxChunkName animationChunkName(StandardAssetDocumentChunkNames::NODE_ANIMATION, animationIndex);
            writer.WriteChunk(animationChunkName, [animation](WxDataChunkWriter& writer, WxError& error)
            {
                FINJIN_WX_ERROR_METHOD_START(error);

                animation->Write(writer, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }

    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

//FinjinSceneDocument_SceneNode
FinjinSceneDocument_SceneNode::FinjinSceneDocument_SceneNode(FinjinSceneDocument* sceneDocument) : Super(sceneDocument)
{
    this->typeName = StandardAssetDocumentPropertyValues::TypeName::SCENE_NODE;

    this->visibility = NODE_VISIBILITY_DEFAULT;
    this->isPrefab = false;

    this->nodeIndex = (size_t)-1;
    this->parentNodeIndex = (size_t)-1;
    this->childNodeCount = 0;
}

FinjinSceneDocument_SceneNode::~FinjinSceneDocument_SceneNode()
{
}

void FinjinSceneDocument_SceneNode::AddAttachedObject(std::shared_ptr<FinjinSceneDocument_MovableObject> object)
{
    if (object != nullptr)
    {
        this->attachedObjects.push_back(std::shared_ptr<FinjinSceneDocument_MovableObject>(object));
        object->parent = this;
    }
}

void FinjinSceneDocument_SceneNode::SetAnimations(std::shared_ptr<FinjinSceneDocument_NodeAnimations> animations)
{
    this->animations = animations;
}

void FinjinSceneDocument_SceneNode::AddAnimation(std::shared_ptr<FinjinSceneDocument_NodeAnimation> animation)
{
    if (animation != nullptr)
    {
        if (this->animations == nullptr)
            this->animations.reset(new FinjinSceneDocument_NodeAnimations(this->sceneDocument));
        this->animations->AddChildItem(animation);
    }
}

void FinjinSceneDocument_SceneNode::AddChildItem(std::shared_ptr<FinjinSceneDocument_Item> item)
{
    if (item != nullptr)
    {
        if (FINJIN_EXPORTER_OBJECT_IS_TYPE_OF(item.get(), FinjinSceneDocument_MovableObject))
            AddAttachedObject(std::static_pointer_cast<FinjinSceneDocument_MovableObject>(item));
        else if (FINJIN_EXPORTER_OBJECT_IS_TYPE_OF(item.get(), FinjinSceneDocument_NodeAnimations))
            SetAnimations(std::static_pointer_cast<FinjinSceneDocument_NodeAnimations>(animations));
        else if (FINJIN_EXPORTER_OBJECT_IS_TYPE_OF(item.get(), FinjinSceneDocument_NodeAnimation))
            AddAnimation(std::static_pointer_cast<FinjinSceneDocument_NodeAnimation>(item));
        else if (FINJIN_EXPORTER_OBJECT_IS_TYPE_OF(item.get(), FinjinSceneDocument_SceneNode))
        {
            auto node = std::static_pointer_cast<FinjinSceneDocument_SceneNode>(item);
            if (!node->IsPrefabRoot())
            {
                //Not a prefab root, so the node will be added to this one
                node->parent = this;
                node->parentNodeIndex = this->nodeIndex;
                this->childNodeCount++;
            }
            this->sceneDocument->AddChildItem(item);
        }
        else
            Super::AddChildItem(item);
    }
}

void FinjinSceneDocument_SceneNode::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    ExportFlags exportFlags = EXPORT_TYPE | EXPORT_NAME;
    if (this->attachedObjects.empty())
    {
        //User data and note tracks for an empty node
        exportFlags |= EXPORT_ID | EXPORT_USER_DATA | EXPORT_NOTE_TRACKS;
    }
    Super::Write(writer, exportFlags, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Node visibility
    switch (this->visibility)
    {
        case NODE_VISIBLE:
        {
            writer.WriteString(StandardAssetDocumentPropertyNames::VISIBILITY, StandardAssetDocumentPropertyValues::NodeVisibility::VISIBLE, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            break;
        }
        case NODE_HIDDEN:
        {
            writer.WriteString(StandardAssetDocumentPropertyNames::VISIBILITY, StandardAssetDocumentPropertyValues::NodeVisibility::HIDDEN, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            break;
        }
        case NODE_TREE_VISIBLE:
        {
            writer.WriteString(StandardAssetDocumentPropertyNames::VISIBILITY, StandardAssetDocumentPropertyValues::NodeVisibility::TREE_VISIBLE, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            break;
        }
        case NODE_TREE_HIDDEN:
        {
            writer.WriteString(StandardAssetDocumentPropertyNames::VISIBILITY, StandardAssetDocumentPropertyValues::NodeVisibility::TREE_HIDDEN, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            break;
        }
        default: break;
    }

    //Instanced prefab name
    if (this->prefabRef != nullptr)
    {
        writer.WriteString(StandardAssetDocumentPropertyNames::PREFAB_REF, this->prefabRef().ToUriString(), error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Position/rotation/scale
    ExporterUtilities::WriteTransformProperties(writer, this->transform, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

    //Attached objects
    if (!this->attachedObjects.empty())
    {
        writer.WriteChunk(StandardAssetDocumentChunkNames::OBJECTS, [this](WxDataChunkWriter& writer, WxError& error)
        {
            //Count
            writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, this->attachedObjects.size(), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Each object
            for (size_t objIndex = 0; objIndex < this->attachedObjects.size(); objIndex++)
            {
                auto attachedObject = this->attachedObjects[objIndex];

                WxChunkName objectChunkName(StandardAssetDocumentChunkNames::OBJECT, objIndex);
                writer.WriteChunk(objectChunkName, [attachedObject](WxDataChunkWriter& writer, WxError& error)
                {
                    FINJIN_WX_ERROR_METHOD_START(error);

                    attachedObject->Write(writer, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Animations
    if (this->animations != nullptr)
    {
        this->animations->Write(writer, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Parent index
    if (this->parentNodeIndex != (size_t)-1)
    {
        writer.WriteCount(StandardAssetDocumentPropertyNames::PARENT, this->parentNodeIndex, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Child count
    if (this->childNodeCount > 0)
    {
        writer.WriteCount(StandardAssetDocumentPropertyNames::CHILD_COUNT, this->childNodeCount, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Child nodes
    Super::Write(writer, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}

bool FinjinSceneDocument_SceneNode::IsPrefabRoot() const
{
    return this->isPrefab && GetPrefabRoot() == nullptr;
}

const FinjinSceneDocument_SceneNode* FinjinSceneDocument_SceneNode::GetPrefabRoot() const
{
    for (FinjinSceneDocument_Item* node = this->parent; node != nullptr; node = node->parent)
    {
        if (!FINJIN_EXPORTER_OBJECT_IS_TYPE_OF(node, FinjinSceneDocument_SceneNode))
            break;

        auto sceneNode = static_cast<FinjinSceneDocument_SceneNode*>(node);
        if (sceneNode->isPrefab)
            return sceneNode;
    }

    return nullptr;
}

FinjinSceneDocument_SceneNode* FinjinSceneDocument_SceneNode::GetPrefabRoot()
{
    for (FinjinSceneDocument_Item* node = this->parent; node != nullptr; node = node->parent)
    {
        if (!FINJIN_EXPORTER_OBJECT_IS_TYPE_OF(node, FinjinSceneDocument_SceneNode))
            break;

        auto sceneNode = static_cast<FinjinSceneDocument_SceneNode*>(node);
        if (sceneNode->isPrefab)
            return sceneNode;
    }

    return nullptr;
}

//FinjinSceneDocument_SceneNodes
FinjinSceneDocument_SceneNodes::FinjinSceneDocument_SceneNodes(FinjinSceneDocument* sceneDocument) : Super(sceneDocument)
{
}

void FinjinSceneDocument_SceneNodes::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    if (!this->childItems.empty())
    {
        writer.WriteChunk(StandardAssetDocumentChunkNames::SCENE_NODES, [this](WxDataChunkWriter& writer, WxError& error)
        {
            FINJIN_WX_ERROR_METHOD_START(error);

            //Count
            writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, this->childItems.size(), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Each node
            for (size_t nodeIndex = 0; nodeIndex < this->childItems.size(); nodeIndex++)
            {
                auto node = std::static_pointer_cast<FinjinSceneDocument_SceneNode>(this->childItems[nodeIndex]);

                WxChunkName sceneNodeChunkName(StandardAssetDocumentChunkNames::SCENE_NODE, nodeIndex);
                writer.WriteChunk(sceneNodeChunkName, [node](WxDataChunkWriter& writer, WxError& error)
                {
                    FINJIN_WX_ERROR_METHOD_START(error);

                    node->Write(writer, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }
}

//FinjinSceneDocument
FinjinSceneDocument::FinjinSceneDocument()
{
    this->unitsPerMeter = 1;
}

FinjinSceneDocument::~FinjinSceneDocument()
{
}

FinjinSceneDocument_Environment* FinjinSceneDocument::GetEnvironment()
{
    return this->environment.get();
}

void FinjinSceneDocument::SetEnvironment(std::shared_ptr<FinjinSceneDocument_Environment> environment)
{
    this->environment = environment;
    if (this->environment != nullptr)
        this->environment->parent = this;
}

void FinjinSceneDocument::AddNode(std::shared_ptr<FinjinSceneDocument_SceneNode> node)
{
    auto prefabRoot = node->GetPrefabRoot();
    if (node->isPrefab && prefabRoot == nullptr)
    {
        //Prefab root
        node->parent = this;
        node->nodeIndex = this->prefabs.size();
        this->prefabs.push_back(std::shared_ptr<FinjinSceneDocument_Item>(node));
    }
    else if (prefabRoot != nullptr)
    {
        //Part of a prefab
        node->nodeIndex = 1 + prefabRoot->prefabChildTable.size();
        prefabRoot->prefabChildTable.push_back(node);
    }
    else
    {
        //Regular node
        if (this->sceneNodes == nullptr)
            this->sceneNodes.reset(new FinjinSceneDocument_SceneNodes(this));
        node->nodeIndex = this->sceneNodes->childItems.size();
        this->sceneNodes->AddChildItem(node);
    }
}

FinjinSceneDocument_StaticGeometries* FinjinSceneDocument::GetStaticGeometries()
{
    return this->staticGeometries.get();
}

void FinjinSceneDocument::AddStaticGeometry(std::shared_ptr<FinjinSceneDocument_StaticGeometry> staticGeometry)
{
    if (staticGeometry != nullptr)
    {
        if (this->staticGeometries == nullptr)
            this->staticGeometries.reset(new FinjinSceneDocument_StaticGeometries(this));
        this->staticGeometries->AddChildItem(staticGeometry);
    }
}

FinjinSceneDocument_InstancedGeometries* FinjinSceneDocument::GetInstancedGeometries()
{
    return this->instancedGeometries.get();
}

void FinjinSceneDocument::AddInstancedGeometry(std::shared_ptr<FinjinSceneDocument_InstancedGeometry> instancedGeometry)
{
    if (instancedGeometry != nullptr)
    {
        if (this->instancedGeometries == nullptr)
            this->instancedGeometries.reset(new FinjinSceneDocument_InstancedGeometries(this));
        this->instancedGeometries->AddChildItem(instancedGeometry);
    }
}

void FinjinSceneDocument::AddChildItem(std::shared_ptr<FinjinSceneDocument_Item> item)
{
    if (item != nullptr)
    {
        if (FINJIN_EXPORTER_OBJECT_IS_TYPE_OF(item.get(), FinjinSceneDocument_SceneNode))
            AddNode(std::static_pointer_cast<FinjinSceneDocument_SceneNode>(item));
        else if (FINJIN_EXPORTER_OBJECT_IS_TYPE_OF(item.get(), FinjinSceneDocument_Environment))
            SetEnvironment(std::static_pointer_cast<FinjinSceneDocument_Environment>(item));
        else if (FINJIN_EXPORTER_OBJECT_IS_TYPE_OF(item.get(), FinjinSceneDocument_StaticGeometry))
            AddStaticGeometry(std::static_pointer_cast<FinjinSceneDocument_StaticGeometry>(item));
        else if (FINJIN_EXPORTER_OBJECT_IS_TYPE_OF(item.get(), FinjinSceneDocument_InstancedGeometry))
            AddInstancedGeometry(std::static_pointer_cast<FinjinSceneDocument_InstancedGeometry>(item));
        else
            Super::AddChildItem(item);
    }
}

void FinjinSceneDocument::Write(WxDataChunkWriter& writer, WxError& error)
{
    FINJIN_WX_ERROR_METHOD_START(error);

    //Prefabs
    if (!this->prefabs.empty())
    {
        writer.WriteChunk(StandardAssetDocumentChunkNames::PREFABS, [this](WxDataChunkWriter& writer, WxError& error)
        {
            FINJIN_WX_ERROR_METHOD_START(error);

            //Count
            writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, this->prefabs.size(), error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Each prefab
            for (size_t prefabIndex = 0; prefabIndex < this->prefabs.size(); prefabIndex++)
            {
                auto prefabNode = std::static_pointer_cast<FinjinSceneDocument_SceneNode>(this->prefabs[prefabIndex]);

                writer.SetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME, prefabNode->name);

                writer.WriteChunk(WxChunkName(StandardAssetDocumentChunkNames::PREFAB, prefabIndex), [this, prefabNode](WxDataChunkWriter& writer, WxError& error)
                {
                    FINJIN_WX_ERROR_METHOD_START(error);

                    //Name
                    writer.WriteString(StandardAssetDocumentPropertyNames::NAME, writer.GetContextString(WxDataChunkWriter::ContextIndex::OBJECT_NAME), error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                    //Scene nodes
                    writer.WriteChunk(StandardAssetDocumentChunkNames::SCENE_NODES, [this, prefabNode](WxDataChunkWriter& writer, WxError& error)
                    {
                        FINJIN_WX_ERROR_METHOD_START(error);

                        //Count
                        writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, 1 + prefabNode->prefabChildTable.size(), error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                        size_t sceneNodeIndex = 0;

                        //Root node
                        writer.WriteChunk(WxChunkName(StandardAssetDocumentChunkNames::SCENE_NODE, sceneNodeIndex++), [prefabNode](WxDataChunkWriter& writer, WxError& error)
                        {
                            FINJIN_WX_ERROR_METHOD_START(error);

                            prefabNode->Write(writer, error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                        }, error);
                        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

                        //Descendant nodes
                        for (auto prefabChildNode : prefabNode->prefabChildTable)
                        {
                            writer.WriteChunk(WxChunkName(StandardAssetDocumentChunkNames::SCENE_NODE, sceneNodeIndex++), [prefabChildNode](WxDataChunkWriter& writer, WxError& error)
                            {
                                FINJIN_WX_ERROR_METHOD_START(error);

                                prefabChildNode->Write(writer, error);
                                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                            }, error);
                            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                        }
                    }, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }

    //Subscene
    writer.WriteChunk(StandardAssetDocumentChunkNames::SUBSCENES, [this](WxDataChunkWriter& writer, WxError& error)
    {
        FINJIN_WX_ERROR_METHOD_START(error);

        //Units per meter
        writer.WriteFloat(StandardAssetDocumentPropertyNames::UNITS_PER_METER, this->unitsPerMeter, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

        //Unit type
        if (!this->unitType.empty())
        {
            writer.WriteString(StandardAssetDocumentPropertyNames::UNIT_TYPE, this->unitType, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }

        //Count
        writer.WriteCount(StandardAssetDocumentPropertyNames::COUNT, 1, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)

        //Each subscene (there is only one)
        writer.WriteChunk(WxChunkName(StandardAssetDocumentChunkNames::SUBSCENE, 0), [this](WxDataChunkWriter& writer, WxError& error)
        {
            FINJIN_WX_ERROR_METHOD_START(error);

            //ID
            if (!this->id.empty())
            {
                WxUuid parsedID;
                if (WxUuid::Parse(parsedID, this->id))
                {
                    writer.WriteUuid(StandardAssetDocumentPropertyNames::ID, parsedID, error);
                    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
                }
            }

            //User data and flags
            Super::Write(writer, EXPORT_USER_DATA | EXPORT_FLAGS, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)

            //Scene manager
            if (!this->sceneManager.empty())
            {
                writer.WriteString(StandardAssetDocumentPropertyNames::SCENE_MANAGER, this->sceneManager, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }

            //Environment
            if (this->environment != nullptr)
            {
                this->environment->Write(writer, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }

            //Static geometries
            if (this->staticGeometries != nullptr)
            {
                this->staticGeometries->Write(writer, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }

            //Instanced geometries
            if (this->instancedGeometries != nullptr)
            {
                this->instancedGeometries->Write(writer, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }

            //Scene nodes
            if (this->sceneNodes != nullptr)
            {
                this->sceneNodes->Write(writer, error);
                FINJIN_WX_DEFAULT_ERROR_CHECK(error)
            }

            Super::Write(writer, error);
            FINJIN_WX_DEFAULT_ERROR_CHECK(error)
        }, error);
        FINJIN_WX_DEFAULT_ERROR_CHECK(error)
    }, error);
    FINJIN_WX_DEFAULT_ERROR_CHECK(error)
}
