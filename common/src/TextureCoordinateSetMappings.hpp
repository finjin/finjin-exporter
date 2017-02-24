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


//Includes---------------------------------------------------------------------
#include "MaterialAccessor.hpp"
#include "AssetReference.hpp"


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * Indicates where a texture coordinate set is coming from in the application.
     * This class has two ways of tracking a texture coordinate set, only one of which
     * will used by a particular edition of Finjin:
     * 1)Number - This is 3DS Max's method.
     * 2)Name - This is the method employed by Maya and XSI.
     */
    class TextureCoordinateSetSource
    {
    public:
        /** Indicates an invalid texture coordinate set number. */
        enum {INVALID_SOURCE_NUMBER = 0xffff};

        /** Default constructor. */
        TextureCoordinateSetSource()
        {
            this->number = INVALID_SOURCE_NUMBER;
        }

        /** Constructor that takes a texture coordinate set number. */
        TextureCoordinateSetSource(int number)
        {
            this->number = number;
        }

        /** Constructor that takes a texture coordinate set name. */
        TextureCoordinateSetSource(const wxString& name)
        {
            this->number = INVALID_SOURCE_NUMBER;
            this->name = name;
        }

        /** Formats the texture coordinate source as a string. */
        wxString ToString() const
        {
            if (this->number != INVALID_SOURCE_NUMBER)
                return wxString::Format(wxT("%d"), this->number);
            else
                return this->name;
        }

        /** Determines whether this source equals the other source. */
        bool operator == (const TextureCoordinateSetSource& other) const
        {
            if (this->number != INVALID_SOURCE_NUMBER)
                return this->number == other.number;
            else
                return this->name == other.name;
        }

        /** Determines whether this source doesn't equal the other source. */
        bool operator != (const TextureCoordinateSetSource& other) const
        {
            if (this->number != INVALID_SOURCE_NUMBER)
                return this->number != other.number;
            else
                return this->name != other.name;
        }

    public:
        /** 
         * The source texture coordinate set number. 
         * If not INVALID_SOURCE_NUMBER, this is used. Otherwise name is used.
         */
        int number;

        /** 
         * The name of the source texture coordinate set. 
         * If index is INVALID_SOURCE_NUMBER, this is used. 
         */
        wxString name;
    };

    enum class TextureCoordinateFlags
    {
        NONE = 0,

        /** Indicates the U texture coordinate is valid. */
        U_TEXTURE_COORDS = 0x100,

        /** Indicates the V texture coordinate is valid. U_TEXTURE_COORDS must be specified if this is specified. */
        V_TEXTURE_COORDS = 0x200,

        /** Indicates the W texture coordinate is valid. U_TEXTURE_COORDS and V_TEXTURE_COORDS must be specified if this is specified. */
        W_TEXTURE_COORDS = 0x400            
    };
    FINJIN_ENUM_BITWISE_OPERATIONS(TextureCoordinateFlags)

    /** 
     * Maps a source texture coordinate set to a destination.
     * The destination is a zero-based index, corresponding to the texture coordinate sets are exported.
     */
    class TextureCoordinateSetMapping
    {
    public:
        TextureCoordinateSetMapping()
        {
            this->destinationIndex = 0;
            this->textureCoordinatesFlags = TextureCoordinateFlags::U_TEXTURE_COORDS | TextureCoordinateFlags::V_TEXTURE_COORDS;
        }

        /** Determines if this mapping is less than the other by comparing the destination index. */
        bool operator < (const TextureCoordinateSetMapping& other) const
        {
            return this->destinationIndex < other.destinationIndex;
        }

        /** Determines if this mapping isn't equal to the other mapping. */
        bool operator != (const TextureCoordinateSetMapping& other) const
        {
            return 
                this->source != other.source ||
                this->destinationIndex != other.destinationIndex ||
                this->textureCoordinatesFlags != other.textureCoordinatesFlags;
        }

        /** Sets the texture coordinate flags by parsing the specified string. */
        void SetTextureCoordinateFlagsFromString(const wxString& value)
        {
            this->textureCoordinatesFlags = TextureCoordinateFlags::NONE;
            if (value.empty() || value == wxT("uv"))
                this->textureCoordinatesFlags |= TextureCoordinateFlags::U_TEXTURE_COORDS | TextureCoordinateFlags::V_TEXTURE_COORDS;
            else if (value == wxT("u"))
                this->textureCoordinatesFlags |= TextureCoordinateFlags::U_TEXTURE_COORDS;
            else if (value == wxT("uvw"))
                this->textureCoordinatesFlags |= TextureCoordinateFlags::U_TEXTURE_COORDS | TextureCoordinateFlags::V_TEXTURE_COORDS | TextureCoordinateFlags::W_TEXTURE_COORDS;
        }

        /** Gets the texture coordinate flags as a string. */
        wxString GetTextureCoordinateFlagsString() const
        {
            wxString result;
            if (NoneSet(this->textureCoordinatesFlags) || this->textureCoordinatesFlags == (TextureCoordinateFlags::U_TEXTURE_COORDS | TextureCoordinateFlags::V_TEXTURE_COORDS))
                result = wxT("uv");
            else if (this->textureCoordinatesFlags == TextureCoordinateFlags::U_TEXTURE_COORDS)
                result = wxT("u");
            else if (this->textureCoordinatesFlags == (TextureCoordinateFlags::U_TEXTURE_COORDS | TextureCoordinateFlags::V_TEXTURE_COORDS | TextureCoordinateFlags::W_TEXTURE_COORDS))
                result = wxT("uvw");
            return result;
        }

        /** Gets the number of texture coordinate components. */
        size_t GetComponentCount() const
        {
            size_t result = 0;
            if (NoneSet(this->textureCoordinatesFlags) || this->textureCoordinatesFlags == (TextureCoordinateFlags::U_TEXTURE_COORDS | TextureCoordinateFlags::V_TEXTURE_COORDS))
                result = 2;
            else if (this->textureCoordinatesFlags == TextureCoordinateFlags::U_TEXTURE_COORDS)
                result = 1;
            else if (this->textureCoordinatesFlags == (TextureCoordinateFlags::U_TEXTURE_COORDS | TextureCoordinateFlags::V_TEXTURE_COORDS | TextureCoordinateFlags::W_TEXTURE_COORDS))
                result = 3;
            return result;
        }

    public:
        /** Texture coordinate set source. */
        TextureCoordinateSetSource source;

        /** Texture coordinate set destination index. */
        int destinationIndex;

        /** Texture coordinate flags. */
        TextureCoordinateFlags textureCoordinatesFlags;

    };

    /** All the texture coordinate set mappings for an object. */
    class TextureCoordinateSetMappings
    {
    public:
        /** Determines if this set of mappings is equal to the other set of mappings. */
        bool operator == (const TextureCoordinateSetMappings& other) const
        {
            if (size() != other.size())
                return false;

            for (size_t i = 0; i < size(); i++)
            {
                if (this->mappings[i] != other.mappings[i])
                    return false;
            }

            return true;
        }

        /** Determines if this set of mappings is not equal to the other set of mappings. */
        bool operator != (const TextureCoordinateSetMappings& other) const
        {
            return !(operator == (other));
        }

        /** Gets the mapping at the specified index. */
        TextureCoordinateSetMapping& operator [] (size_t i)
        {
            return this->mappings[i];
        }

        /** Gets the mapping at the specified index. */
        const TextureCoordinateSetMapping& operator [] (size_t i) const
        {
            return this->mappings[i];
        }

        /** Sorts the mappings by their destination index. */
        void Sort()
        {
            std::sort(this->mappings.begin(), this->mappings.end());
        }

        /** 
         * Removes the mapping at the specified index. 
         * @param index [in] - The index of the mapping to remove.
         * @return The index of closest mapping is returned. This is typically used to select
         * the next valid mapping in the user interface. If there are no mappings left, -1 is returned.
         */
        int Remove(int index)
        {
            this->mappings.erase(this->mappings.begin() + index);
            if (index >= (int)this->mappings.size())
                index = (int)this->mappings.size() - 1;
            if (index < 0)
                index = 0;
            RenumberDestinations();
            return !this->mappings.empty() ? index : -1;
        }

        /**
         * Moves the mapping from a specified location, either up or down by one.
         * @param from [in] - The index of the mapping to move.
         * @param direction [in] - Specifies the direction of the move. If < 0, the mapping is moved toward
         * the beginning of the array by one. Otherwise the mapping is moved toward the end of the array by one.
         * @return The new index of the moved mapping is returned. This may be the same as 'from' if the movement
         * was invalid.
         */
        int Move(int from, int direction)
        {
            direction = direction < 0 ? -1 : 1;

            int to = from + direction;
            if (to >= (int)this->mappings.size())
                to = (int)this->mappings.size() - 1;
            if (to < 0)
                to = 0;

            if (to != from)
            {
                std::swap(this->mappings[from], this->mappings[to]);
                std::swap(this->mappings[from].destinationIndex, this->mappings[to].destinationIndex);
                return to;
            }
            else
                return -1;
        }

        /** 
         * Renumbers the mapping destinations. 
         * This is called as necessary to keep a sequential ordering of destinations.
         */
        void RenumberDestinations()
        {
            for (size_t i = 0; i < this->mappings.size(); i++)
                this->mappings[i].destinationIndex = (int)i;
        }

        /** Determines whether this set of mappings is non-empty. */
        bool IsValid() const
        {
            return !empty();
        }

        /** Determines whether this set of mappings is empty. */
        bool empty() const
        {
            return this->mappings.empty();
        }

        /** Gets the number of mappings. */
        size_t size() const
        {
            return this->mappings.size();
        }

        /** Changes the number of mappings. */
        void resize(size_t size)
        {
            bool sizeGreater = size > this->mappings.size();
            this->mappings.resize(size);
            if (sizeGreater)
                RenumberDestinations();
        }

        /** Removes all the mappings. */
        void clear()
        {
            this->mappings.clear();
        }
        
    private:
        typedef std::vector<TextureCoordinateSetMapping> Mappings;
        Mappings mappings;        
    };

    struct TextureNameAlias
    {
        wxString alias;
        wxString textureName;

        bool operator == (const TextureNameAlias& other) const
        {
            return 
                this->alias == other.alias &&
                this->textureName == other.textureName;
        }

        bool operator != (const TextureNameAlias& other) const
        {
            return 
                this->alias != other.alias &&
                this->textureName != other.textureName;
        }
    };

    /** All the texture coordinate set mappings for a submesh. */
    class SubmeshSettings
    {
    public:
        SubmeshSettings()
        {
            this->renderPriority = 0;
        }

        /** Determines if this submesh is equal to the other submesh. */
        bool operator == (const SubmeshSettings& other) const
        {
            return 
                this->material.MaterialEquals(other.material) &&
                this->textureCoordinateSetMappings == other.textureCoordinateSetMappings;
        }

        /** Determines if this submesh is not equal to the other submesh. */
        bool operator != (const SubmeshSettings& other) const
        {
            return 
                !this->material.MaterialEquals(other.material) &&
                this->textureCoordinateSetMappings != other.textureCoordinateSetMappings;
        }

        /** Determines if this submesh is less than the other by comparing the material name. */
        bool operator < (const SubmeshSettings& other) const
        {
            return this->material.GetLocalName() < other.material.GetLocalName();
        }

    public:
        /** The name of the submesh. This is not considered when comparing submeshes. */
        wxString submeshName;

        /* The submesh material. */
        MaterialAccessor material;

        /* The texture coordinate set mappings for the submesh. */
        TextureCoordinateSetMappings textureCoordinateSetMappings;

        /* The texture aliases for the submesh. */
        std::vector<TextureNameAlias> textureNameAliases;

        wxString renderQueueName;
        int renderPriority;
    };

    /** All settings for all the submeshes of a mesh. */
    class SubmeshesSettings
    {
    public:
        SubmeshesSettings()
        {            
        }

        /** 
         * Initializes the settings.
         * @param materials [in] - A collection of materials, each corresponding to a submesh.
         * @param textureCoordinateSetMappings [in] - The default texture coordinate set mappings for a submesh.
         */
        void Initialize
            (
            const MaterialAccessorVector& materials, 
            const TextureCoordinateSetMappings& textureCoordinateSetMappings
            )
        {
            this->submeshesSettings.clear();
            this->submeshesSettings.resize(materials.size());
            for (size_t i = 0; i < materials.size(); i++)
            {
                this->submeshesSettings[i].material = materials[i];
                this->submeshesSettings[i].textureCoordinateSetMappings = textureCoordinateSetMappings;
            }

            Sort();
        }

        /**
         * Synchronizes this set of submesh settings to another set of submesh settings.
         * This is essential during export for removing mappings that no longer exist, and for adding 
         * mappings that do exist but aren't defined.
         * @param other [in] - The mappings to synchronize to.
         * @param deepSync [in] - Indicates whether newly added mappings should be copied from 'other'.
         * If false, then the new mappings are created with only the material (a mapping to nothing).
         */
        void SyncTo(const SubmeshesSettings& other, bool deepSync = false)
        {
            //Remove settings that aren't in 'other'
            for (size_t i = 0; i < size();)
            {
                if (other.FindIndexOfMaterial(this->submeshesSettings[i].material) == -1)
                    this->submeshesSettings.erase(this->submeshesSettings.begin() + i);
                else
                    i++;
            }

            //Add settings that are in 'other' but aren't in 'this'
            for (size_t i = 0; i < other.size(); i++)
            {
                if (FindIndexOfMaterial(other.submeshesSettings[i].material) == -1)
                {
                    if (deepSync)
                        this->submeshesSettings.push_back(other.submeshesSettings[i]);
                    else
                    {
                        SubmeshSettings submeshMapping;
                        submeshMapping.submeshName = other.submeshesSettings[i].submeshName;
                        submeshMapping.material = other.submeshesSettings[i].material;
                        this->submeshesSettings.push_back(submeshMapping);
                    }                    
                }
            }

            //Re-sort
            Sort();
        }

        /** 
         * Finds the index of the setting that uses the specified material.
         * @param material [in] - The material to search for
         * @return The index of the setting is returned for the material. If the material could not be found, -1 is returned.
         */
        int FindIndexOfMaterial(MaterialAccessor material) const
        {
            for (size_t i = 0; i < size(); i++)
            {
                if (this->submeshesSettings[i].material.MaterialEquals(material))
                    return (int)i;
            }
            return -1;
        }

        /** Determines whether this set of settings equals the other set of settings. */
        bool operator == (const SubmeshesSettings& other) const
        {
            if (size() != other.size())
                return false;

            for (size_t i = 0; i < size(); i++)
            {
                if (this->submeshesSettings[i] != other.submeshesSettings[i])
                    return false;
            }

            return true;
        }

        /** Determines whether this set of settings doesn't equal the other set of settings. */
        bool operator != (const SubmeshesSettings& other) const
        {
            return !(operator == (other));
        }

        /** Gets the settings at the specified index. */
        SubmeshSettings& operator [] (size_t i)
        {
            return this->submeshesSettings[i];
        }

        /** Gets the submesh settings at the specified index. */
        const SubmeshSettings& operator [] (size_t i) const
        {
            return this->submeshesSettings[i];
        }

        /** 
         * Sorts the submesh settings.
         * @param deepSort [in] - Indicates whether texture coordinate set mappings of each submesh 
         * should also be sorted. If false, just the submesh mappings are sorted.
         */
        void Sort(bool deepSort = false)
        {
            std::sort(this->submeshesSettings.begin(), this->submeshesSettings.end());

            if (deepSort)
            {
                for (size_t i = 0; i < this->submeshesSettings.size(); i++)
                  this->submeshesSettings[i].textureCoordinateSetMappings.Sort();
            }
        }

        /** Determines whether all the texture coordinate set mappings are the same. */
        bool AllSubmeshMappingsSame() const
        {
            for (size_t i = 1; i < this->submeshesSettings.size(); i++)
            {
                if (this->submeshesSettings[i].textureCoordinateSetMappings != this->submeshesSettings[0].textureCoordinateSetMappings)
                    return false;
            }
            return true;
        }

        /** 
         * Moves the texture coordinate set from the specified index, in the specified direction.
         * @param submeshIndex [in] - Index of the submesh mapping that the move should be applied to. If -1,
         * the move is applied to all submesh mappings. -1 should only be used if it has been verified that all
         * submesh mappings are the same with a call to AllSubmeshMappingsSame().
         * @param from [in] - The index of the mapping to move.
         * @param direction [in] - Specifies the direction of the move. If < 0, the mapping is moved toward
         * the beginning of the array by one. Otherwise the mapping is moved toward the end of the array by one.
         * @return The new index of the moved mapping is returned. This may be the same as 'from' if the movement
         * was invalid.
         */
        int MoveTextureCoordinateSet(int submeshIndex, int from, int direction)
        {
            int result = -1;
            if (submeshIndex < 0)
            {
                for (size_t i = 0; i < this->submeshesSettings.size(); i++)
                {
                    int moveResult = this->submeshesSettings[i].textureCoordinateSetMappings.Move(from, direction);
                    if (result == -1)
                        result = moveResult;
                }                
            }
            else
                result = this->submeshesSettings[submeshIndex].textureCoordinateSetMappings.Move(from, direction);
            return result;
        }

        /** 
         * Renumbers the submesh texture coordinate set mapping destinations. 
         * This is called as necessary to keep a sequential ordering of destinations.
         */
        void RenumberDestinations()
        {
            for (size_t i = 0; i < this->submeshesSettings.size(); i++)
                this->submeshesSettings[i].textureCoordinateSetMappings.RenumberDestinations();
        }

        /** Determines whether this set of submesh settings is non-empty. */
        bool IsValid() const
        {
            return !empty();
        }

        /** Determines whether this set of submesh settings is empty. */
        bool empty() const
        {
            return this->submeshesSettings.empty();
        }

        /** Gets the number of submesh settings. */
        size_t size() const
        {
            return this->submeshesSettings.size();
        }

        /** Changes the number of submesh settings. */
        void resize(size_t size)
        {
            this->submeshesSettings.resize(size);
        }

        /** Removes all the submesh settings. */
        void clear()
        {
            this->submeshesSettings.clear();
        }
        
    private:
        typedef std::vector<SubmeshSettings> SubmeshSettingsVector;
        SubmeshSettingsVector submeshesSettings;
    };

} }
