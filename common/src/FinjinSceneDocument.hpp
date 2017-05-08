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


#pragma once


//Includes----------------------------------------------------------------------
#include "FinjinNoteTrack.hpp"
#include "FinjinColor.hpp"
#include "AssetReference.hpp"
#include "TransformAccessor.hpp"
#include "UserDataTypes.hpp"
#include "finjin/common/WxChunkName.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Common {
    class WxDataChunkWriter;
} }

namespace Finjin { namespace Exporter {

    class FinjinSceneDocument;

    /**
     * Base class from which all sceneDocument document items are derived
     * Not all fields are used by subclasses
     */
    class FinjinSceneDocument_Item
    {
    public:
        FinjinSceneDocument_Item(FinjinSceneDocument* sceneDocument = nullptr);

        virtual ~FinjinSceneDocument_Item();

        virtual void AddChildItem(std::shared_ptr<FinjinSceneDocument_Item> item);

        virtual void Write(WxDataChunkWriter& writer, WxError& error);

    //protected:
        typedef int ExportFlags;
        enum
        {
            NO_FLAGS = 0,

            FIRST_EXPORT_FLAG = 1 << 0,

            EXPORT_TYPE = FIRST_EXPORT_FLAG,
            EXPORT_NAME = FIRST_EXPORT_FLAG << 1,
            EXPORT_ID = FIRST_EXPORT_FLAG << 2,
            EXPORT_USER_DATA = FIRST_EXPORT_FLAG << 3,
            EXPORT_NOTE_TRACKS = FIRST_EXPORT_FLAG << 4,
            EXPORT_FLAGS = FIRST_EXPORT_FLAG << 5,
            LAST_UNUSED_EXPORT_FLAG = FIRST_EXPORT_FLAG << 6
        };

        virtual void Write(WxDataChunkWriter& writer, ExportFlags flags, WxError& error);

    private:
        void ExportUserData(WxDataChunkWriter& writer, WxError& error);
        void ExportNoteTracks(WxDataChunkWriter& writer, WxError& error);

    public:
        /** The sceneDocument document item that owns this item */
        FinjinSceneDocument* sceneDocument;

        FinjinSceneDocument_Item* parent;

        typedef std::vector<std::shared_ptr<FinjinSceneDocument_Item> > ItemList;

        wxString typeName;
        wxString name;
        wxString id;
        struct UserData
        {
            UserData()
            {
                this->userDataClass = nullptr;
            }

            bool IsValid() const
            {
                return !this->propertyInstances.empty() || !this->rawData.empty();
            }

            void operator = (const wxString& rawData);

            wxString className;

            UserDataTypes::Class* userDataClass;
            UserDataTypes::Class::Items userDataClassProperties;

            wxString rawData;

            struct PropertyInstance
            {
                PropertyInstance() {this->prop = nullptr;}
                wxString name;
                wxString value;
                UserDataTypes::Class::Item* prop;
            };
            std::vector<PropertyInstance> propertyInstances;
        };
        UserData userData;
        std::vector<FinjinNoteTrack> noteTracks;
        int childOrder;

        /** Child items. May be empty */
        ItemList childItems;

        std::vector<NamedFlag> namedFlags;
    };

    class FinjinSceneDocument_MovableObject : public FinjinSceneDocument_Item
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_Item)

        FinjinSceneDocument_MovableObject(FinjinSceneDocument* sceneDocument = nullptr);

        void Write(WxDataChunkWriter& writer, WxError& error) override;

        void Write(WxDataChunkWriter& writer, ExportFlags flags, WxError& error) override;

    public:
        TransformAccessor objectOffsetTransform;
    };

    class FinjinSceneDocument_Camera : public FinjinSceneDocument_MovableObject
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_MovableObject)

        FinjinSceneDocument_Camera(FinjinSceneDocument* sceneDocument = nullptr);

        void Write(WxDataChunkWriter& writer, WxError& error) override;

    public:
        float fov;
        bool isOrtho;
        float nearClip;
        float farClip;
        float orthoWidth;
        float orthoHeight;
    };

    class FinjinSceneDocument_Light : public FinjinSceneDocument_MovableObject
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_MovableObject)

        FinjinSceneDocument_Light(FinjinSceneDocument* sceneDocument = nullptr);

        void Write(WxDataChunkWriter& writer, WxError& error) override;

        bool HasEffect(bool checkOn = true) const;

    public:
        bool isOn;
        LightType lightType;
        bool castShadows;
        float power;
        FinjinColor color;
        float range[2];
        float coneRange[2];
    };

    class FinjinSceneDocument_RenderableMovableObject : public FinjinSceneDocument_MovableObject
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_MovableObject)

        FinjinSceneDocument_RenderableMovableObject(FinjinSceneDocument* sceneDocument = nullptr);

        void Write(WxDataChunkWriter& writer, WxError& error) override;

        enum ObjectVisibility
        {
            OBJECT_VISIBILITY_DEFAULT,
            OBJECT_VISIBLE,
            OBJECT_HIDDEN
        };

    //protected:
        enum
        {
            FIRST_EXPORT_FLAG = Super::LAST_UNUSED_EXPORT_FLAG,
            EXPORT_VISIBILITY = FIRST_EXPORT_FLAG,
            EXPORT_RENDER_QUEUE = FIRST_EXPORT_FLAG << 1,
            EXPORT_RENDER_PRIORITY = FIRST_EXPORT_FLAG << 2,
            EXPORT_RENDERING_DISTANCE = FIRST_EXPORT_FLAG << 3,
            EXPORT_CAST_SHADOWS = FIRST_EXPORT_FLAG << 4,
            EXPORT_RECEIVE_SHADOWS = FIRST_EXPORT_FLAG << 5,
            LAST_UNUSED_EXPORT_FLAG = FIRST_EXPORT_FLAG << 6
        };

        void Write(WxDataChunkWriter& writer, ExportFlags flags, WxError& error) override;

    private:
        void WriteObjectVisibility(WxDataChunkWriter& writer, WxError& error);

    public:
        ObjectVisibility visibility;
        wxString renderQueue;
        int renderPriority;
        float renderingDistance;
        bool castShadows;
        bool receiveShadows;
    };

    class FinjinSceneDocument_BoneAttachment : public FinjinSceneDocument_Item
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_Item)

        FinjinSceneDocument_BoneAttachment(FinjinSceneDocument* sceneDocument = nullptr);

        void Write(WxDataChunkWriter& writer, WxError& error) override;

    public:
        wxString boneName;
        TransformAccessor transform;
    };

    class FinjinSceneDocument_BoneAttachments : public FinjinSceneDocument_Item
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_Item)

        FinjinSceneDocument_BoneAttachments(FinjinSceneDocument* sceneDocument = nullptr);

        void Write(WxDataChunkWriter& writer, WxError& error) override;
    };

    class FinjinSceneDocument_Entity : public FinjinSceneDocument_RenderableMovableObject
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_RenderableMovableObject)

        FinjinSceneDocument_Entity(FinjinSceneDocument* sceneDocument = nullptr);
        ~FinjinSceneDocument_Entity();

        void Write(WxDataChunkWriter& writer, WxError& error) override;

        void AddBoneAttachment(std::shared_ptr<FinjinSceneDocument_BoneAttachment> boneAttachment);

    public:
        ResourceReferenceFunction meshRef;
        struct Subentity
        {
            Subentity()
            {
                this->renderPriority = std::numeric_limits<int>::min();
            }

            ResourceReferenceFunction materialRef;
            wxString renderQueue;
            int renderPriority;
        };
        std::vector<Subentity> subentities;
        struct ManualLod
        {
            std::vector<ResourceReferenceFunction> subentityMaterialRefs;
        };
        std::vector<ManualLod> manualLods;
        std::shared_ptr<FinjinSceneDocument_BoneAttachments> boneAttachments;
    };

    class FinjinSceneDocument_StaticAndInstancedGeometryEntity : public FinjinSceneDocument_Item
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_Item)

        FinjinSceneDocument_StaticAndInstancedGeometryEntity(FinjinSceneDocument* sceneDocument = nullptr);

        void Write(WxDataChunkWriter& writer, WxError& error) override;

    public:
        std::vector<wxString> subentityMaterialNames;
        TransformAccessor transform;
    };

    class FinjinSceneDocument_Entities : public FinjinSceneDocument_Item
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_Item)

        FinjinSceneDocument_Entities (FinjinSceneDocument* sceneDocument = nullptr);

        void Write(WxDataChunkWriter& writer, WxError& error) override;
    };

    class FinjinSceneDocument_StaticGeometry : public FinjinSceneDocument_RenderableMovableObject
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_RenderableMovableObject)

        FinjinSceneDocument_StaticGeometry(FinjinSceneDocument* sceneDocument = nullptr);
        ~FinjinSceneDocument_StaticGeometry();

        void AddChildItem(std::shared_ptr<FinjinSceneDocument_Item> item) override;

        void Write(WxDataChunkWriter& writer, WxError& error) override;

    public:
        std::shared_ptr<FinjinSceneDocument_Entities> entities;
        bool isVisible;
        FinjinVector3 position;
        FinjinVector3 dimensions;
    };

    class FinjinSceneDocument_StaticGeometries : public FinjinSceneDocument_Item
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_Item)

        FinjinSceneDocument_StaticGeometries(FinjinSceneDocument* sceneDocument = nullptr);

        void Write(WxDataChunkWriter& writer, WxError& error) override;
    };

    class FinjinSceneDocument_InstancedGeometry : public FinjinSceneDocument_RenderableMovableObject
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_RenderableMovableObject)

        FinjinSceneDocument_InstancedGeometry(FinjinSceneDocument* sceneDocument = nullptr);
        ~FinjinSceneDocument_InstancedGeometry();

        void AddChildItem(std::shared_ptr<FinjinSceneDocument_Item> item) override;

        void Write(WxDataChunkWriter& writer, WxError& error) override;

    public:
        std::shared_ptr<FinjinSceneDocument_Entities> entities;
        bool isVisible;
        int batchCount;
        FinjinVector3 position;
        FinjinVector3 dimensions;
    };

    class FinjinSceneDocument_InstancedGeometries : public FinjinSceneDocument_Item
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_Item)

        FinjinSceneDocument_InstancedGeometries(FinjinSceneDocument* sceneDocument = nullptr);

        void Write(WxDataChunkWriter& writer, WxError& error) override;
    };

    class FinjinSceneDocument_NodeAnimation : public FinjinSceneDocument_Item
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_Item)

        FinjinSceneDocument_NodeAnimation(FinjinSceneDocument* sceneDocument = nullptr);

        void Write(WxDataChunkWriter& writer, WxError& error) override;

    public:
        bool enable;
        bool loop;

        struct Key
        {
            Key()
            {
                this->time = WxTimeDuration::Zero();
                FINJIN_ZERO_ITEM(this->m44);
            }

            const TransformAccessor& GetTransform() const
            {
                return this->transform;
            }

            void SetTransform(const TransformAccessor& t)
            {
                this->transform = t;

                MatrixAccessor transformMatrix = t.GetMatrix();
                transformMatrix.GetExportMatrix44(this->m44);
            }

            WxTimeDuration time;
            float m44[4][4];

        private:
            TransformAccessor transform;
        };
        std::vector<Key> keys;
    };

    class FinjinSceneDocument_NodeAnimations : public FinjinSceneDocument_Item
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_Item)

        FinjinSceneDocument_NodeAnimations(FinjinSceneDocument* sceneDocument = nullptr);

        void Write(WxDataChunkWriter& writer, WxError& error) override;
    };

    class FinjinSceneDocument_Sky : public FinjinSceneDocument_Item
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_Item)

        FinjinSceneDocument_Sky(FinjinSceneDocument* sceneDocument = nullptr);

        void SetAnimations(std::shared_ptr<FinjinSceneDocument_NodeAnimations> animations);
        void AddAnimation(std::shared_ptr<FinjinSceneDocument_NodeAnimation> animation);

        void AddChildItem(std::shared_ptr<FinjinSceneDocument_Item> item) override;

        void Write(WxDataChunkWriter& writer, WxError& error) override;

    public:
        ResourceReferenceFunction meshRef;
        bool enable;
        std::shared_ptr<FinjinSceneDocument_NodeAnimations> animations;
        TransformAccessor transform;
    };

    class FinjinSceneDocument_Clipping : public FinjinSceneDocument_Item
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_Item)

        FinjinSceneDocument_Clipping(FinjinSceneDocument* sceneDocument, float nearClip, float farClip);

    public:
        float nearClip;
        float farClip;
    };

    class FinjinSceneDocument_Shadows : public FinjinSceneDocument_Item
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_Item)

        FinjinSceneDocument_Shadows(FinjinSceneDocument* sceneDocument = nullptr);
        ~FinjinSceneDocument_Shadows();

        void Write(WxDataChunkWriter& writer, WxError& error) override;

    public:
        wxString technique;
        float farDistance;
        FinjinColor color;
    };

    class FinjinSceneDocument_Fog : public FinjinSceneDocument_Item
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_Item)

        FinjinSceneDocument_Fog
            (
            FinjinSceneDocument* sceneDocument,
            FogMode mode,
            float expDensity,
            float linearStart,
            float linearEnd,
            const FinjinColor& color
            );

        void Write(WxDataChunkWriter& writer, WxError& error) override;

    public:
        FogMode mode;
        float expDensity;
        float linearStart;
        float linearEnd;
        FinjinColor color;
    };

    class FinjinSceneDocument_Environment : public FinjinSceneDocument_Item
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_Item)

        FinjinSceneDocument_Environment(FinjinSceneDocument* sceneDocument = nullptr);
        ~FinjinSceneDocument_Environment();

        void SetRange(std::shared_ptr<FinjinSceneDocument_Clipping> range);

        void SetFog(std::shared_ptr<FinjinSceneDocument_Fog> fog);

        void SetShadows(std::shared_ptr<FinjinSceneDocument_Shadows> shadows);

        void AddSky(std::shared_ptr<FinjinSceneDocument_Sky> sky);

        void AddChildItem(std::shared_ptr<FinjinSceneDocument_Item> item) override;

        void Write(WxDataChunkWriter& writer, WxError& error) override;

    public:
        float environmentNear;
        float environmentFar;
        FinjinColor ambientLightColor;
        FinjinColor backgroundColor;
        std::shared_ptr<FinjinSceneDocument_Clipping> range;
        std::shared_ptr<FinjinSceneDocument_Fog> fog;
        std::shared_ptr<FinjinSceneDocument_Shadows> shadows;
        ItemList skies;
    };

    class FinjinSceneDocument_TrackTarget : public FinjinSceneDocument_Item
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_Item)

        FinjinSceneDocument_TrackTarget(FinjinSceneDocument* sceneDocument = nullptr);
        ~FinjinSceneDocument_TrackTarget();

        void Write(WxDataChunkWriter& writer, WxError& error) override;

    public:
        wxString nodeName;
        FinjinVector3 localDirection;
    };

    class FinjinSceneDocument_Physical : public FinjinSceneDocument_MovableObject
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_MovableObject)

        FinjinSceneDocument_Physical(FinjinSceneDocument* sceneDocument = nullptr);

        void Write(WxDataChunkWriter& writer, WxError& error) override;

    public:
        wxString type; //TODO: Remove this?
        TransformAccessor transform;

        /** A simple bounding volume, centered around the owner's origin */
        struct Shape
        {
            Shape()
            {
                this->type = NONE;
                this->radius = 0;
            }

            enum Type
            {
                NONE,
                PLANE,
                SPHERE,
                BOX,
                CYLINDER,
                CAPSULE,
                MESH
            };

            /** The bounding volume type */
            Type type;

            FinjinVector4 plane;

            /** The bounding radius. Used when 'type' is SPHERE, CYLINDER, or CAPSULE */
            float radius;

            /**
             * The size. All elements are used when 'type' is BOX, 'x' is used when CYLINDER or CAPSULE.
             * Note that when type is CAPSULE, the size given is the distance along the noncurved part of the capsule.
             */
            FinjinVector3 size;

            /** A single face */
            struct Face
            {
                FinjinVector3 vertex[3];
            };

            /** Faces of the mesh bounding volume. Used when 'type' is MESH */
            std::vector<Face> meshFaces;
        };
        Shape shape;
    };

    class FinjinSceneDocument_SceneNode : public FinjinSceneDocument_Item
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_Item)

        FinjinSceneDocument_SceneNode(FinjinSceneDocument* sceneDocument = nullptr);
        ~FinjinSceneDocument_SceneNode();

        void AddAttachedObject(std::shared_ptr<FinjinSceneDocument_MovableObject> object);

        void SetAnimations(std::shared_ptr<FinjinSceneDocument_NodeAnimations> animations);
        void AddAnimation(std::shared_ptr<FinjinSceneDocument_NodeAnimation> animation);

        void AddChildItem(std::shared_ptr<FinjinSceneDocument_Item> item) override;

        void Write(WxDataChunkWriter& writer, WxError& error) override;

        bool IsPrefabRoot() const;
        const FinjinSceneDocument_SceneNode* GetPrefabRoot() const;
        FinjinSceneDocument_SceneNode* GetPrefabRoot();

        enum NodeVisibility
        {
            NODE_VISIBILITY_DEFAULT,
            NODE_VISIBLE,
            NODE_HIDDEN,
            NODE_TREE_VISIBLE,
            NODE_TREE_HIDDEN
        };

    public:
        NodeVisibility visibility;

        bool isPrefab;
        wxString prefabName;
        ResourceReferenceFunction prefabRef;

        TransformAccessor transform;

        std::shared_ptr<FinjinSceneDocument_NodeAnimations> animations;

        std::vector<std::shared_ptr<FinjinSceneDocument_MovableObject> > attachedObjects;

        std::vector<std::shared_ptr<FinjinSceneDocument_SceneNode> > prefabChildTable;

        size_t nodeIndex;
        size_t parentNodeIndex;
        size_t childNodeCount;
    };

    class FinjinSceneDocument_SceneNodes : public FinjinSceneDocument_Item
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_Item)

        FinjinSceneDocument_SceneNodes(FinjinSceneDocument* sceneDocument = nullptr);

        void Write(WxDataChunkWriter& writer, WxError& error) override;
    };

    class FinjinSceneDocument : public FinjinSceneDocument_Item
    {
    public:
        FINJIN_EXPORTER_DEFINE_SUPERCLASS(FinjinSceneDocument_Item)

        FinjinSceneDocument();
        ~FinjinSceneDocument();

        FinjinSceneDocument_Environment* GetEnvironment();
        void SetEnvironment(std::shared_ptr<FinjinSceneDocument_Environment> environment);

        void AddNode(std::shared_ptr<FinjinSceneDocument_SceneNode> node);

        FinjinSceneDocument_StaticGeometries* GetStaticGeometries();
        void AddStaticGeometry(std::shared_ptr<FinjinSceneDocument_StaticGeometry> staticGeometry);

        FinjinSceneDocument_InstancedGeometries* GetInstancedGeometries();
        void AddInstancedGeometry(std::shared_ptr<FinjinSceneDocument_InstancedGeometry> instancedGeometry);

        void AddChildItem(std::shared_ptr<FinjinSceneDocument_Item> item) override;

        void Write(WxDataChunkWriter& writer, WxError& error) override;

    public:
        float unitsPerMeter;
        wxString unitType;

        wxString sceneManager;

    protected:
        std::shared_ptr<FinjinSceneDocument_Environment> environment;
        std::shared_ptr<FinjinSceneDocument_SceneNodes> sceneNodes;
        std::shared_ptr<FinjinSceneDocument_StaticGeometries> staticGeometries;
        std::shared_ptr<FinjinSceneDocument_InstancedGeometries> instancedGeometries;
        ItemList prefabs;
    };

    /** A utility class used in the construction of an FinjinSceneDocument */
    struct FinjinSceneDocumentParentItems
    {
        FinjinSceneDocumentParentItems()
        {
            this->node = nullptr;
            this->object = nullptr;
        }

        FinjinSceneDocumentParentItems(FinjinSceneDocument_SceneNode* node, FinjinSceneDocument_Item* object)
        {
            this->node = node;
            this->object = object;
        }

        bool IsValid() const
        {
            return this->node != nullptr || this->object != nullptr;
        }

        /** Gets the best parent item, favoring the member 'object' */
        FinjinSceneDocument_Item* GetBestParentObject()
        {
            if (this->object != nullptr)
                return this->object;
            else
                return this->node;
        }

        /** Gets the best parent item, favoring the member 'node' */
        FinjinSceneDocument_Item* GetBestParentNode()
        {
            if (this->node != nullptr)
                return this->node;
            else
                return this->object;
        }

        bool AddAttachedObject(std::shared_ptr<FinjinSceneDocument_MovableObject> object)
        {
            auto result = true;

            if (this->node != nullptr)
                this->node->AddAttachedObject(object);
            else if (this->object != nullptr)
                this->object->AddChildItem(object);
            else
                result = false;

            return result;
        }

        FinjinSceneDocument_SceneNode* node;
        FinjinSceneDocument_Item* object;
    };

} }
