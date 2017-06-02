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
#include "UserDataTypes.hpp"
#include "FinjinSceneSettingsAccessor.hpp"
#include "finjin/common/WxNumericStruct.hpp"


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    enum class ManagedResourceType
    {
        NONE = 0,

        VERTEX_FORMATS = 1 << 0,
        USER_DATA_TYPES = 1 << 1,

        ALL = VERTEX_FORMATS | USER_DATA_TYPES
    };
    FINJIN_ENUM_BITWISE_OPERATIONS(ManagedResourceType)

    /**
     * Manages the resource locations stored in the Finjin scene settings.
     * There are two types of resources that this class manages:
     *   1)User data types and their file locations.
     */
    class FinjinResourceManager
    {
    public:
        static FinjinResourceManager& GetInstance();

        /** Interface for a class that receives notifications with the user data types are changed. */
        class UserDataTypesChangedListener
        {
        public:
            virtual ~UserDataTypesChangedListener() {}
            virtual void UserDataTypesChanging() {}
            virtual void UserDataTypesChanged() = 0;
        };

        void AddUserDataTypesChangedListener(UserDataTypesChangedListener* listener);
        void RemoveUserDataTypesChangedListener(UserDataTypesChangedListener* listener);

        void UpdateGlobalResources();

        void UpdateVertexFormats();

        /**
         * Updates the user data types, triggering notifications for all the user data type listeners.
         * @param sceneSettings [in] - The scene settings.
         */
        void UpdateUserDataTypes();

        /** Gets all the user data types. */
        UserDataTypes& GetUserDataTypes();

        wxString FindVertexFormatName(const std::vector<WxGpuVertexFormatStruct::Element>& vertexFormatElements) const;

        /**
         * Refreshes the specified resources, triggering notifications as necessary.
         * @param resourceTypes [in] - Flags indicating which types of resources to refresh.
         * @param force [in] - Indicates whether the resources should be forced to be refreshed, even
         * if the resource locations have not been changed.
         * @param sceneSettings [in] - The scene settings.
         */
        void RefreshSceneResources
            (
            ManagedResourceType resourceTypes = ManagedResourceType::ALL,
            bool force = false,
            FinjinSceneSettingsAccessor sceneSettings = FinjinSceneSettingsAccessor()
            );

        /**
         * Clears the specified resources, triggering notifications as necessary.
         * @param resourceTypes [in] - Flags indicating which types of resources to refresh.
         */
        void ClearSceneResources(ManagedResourceType resourceTypes = ManagedResourceType::ALL);

        bool ValidateVertexFormatLocation();

        /**
         * Validates the user data type locations, displaying a dialog if there are any invalid locations.
         * Calling this does not change any resources.
         * @param sceneSettings [in] - The scene settings.
         * @return If the user data type locations were all valid, true is returned. Otherwise, false is returned.
         */
        bool ValidateUserDataTypesLocations();

        /**
         * Validates all the specified resources.
         * As invalid resources are found, the user is presented with appropriate dialogs that allow them to
         * fix the invalid locations.
         * @param resourceTypes [in] - Flags indicating which types of resources to validate.
         * @param sceneSettings [in] - The scene settings.
         * @return The validated resource types are returned.
         */
        ManagedResourceType ValidateResourceLocations(ManagedResourceType resourceTypes = ManagedResourceType::ALL, FinjinSceneSettingsAccessor sceneSettings = FinjinSceneSettingsAccessor());

    private:
        static void GetInvalidUserDataTypesLocations
            (
            std::vector<UserDataTypesLocation>& invalidLocations,
            const std::vector<UserDataTypesLocation>& locations,
            const wxString& baseDirectory
            );

        void NotifyUserDataTypesChangingListeners();
        void NotifyUserDataTypesChangedListeners();

    private:
        std::vector<WxGpuVertexFormatStruct> vertexFormats;

        /** Listeners that are called when the user data types change. */
        std::list<UserDataTypesChangedListener*> userDataTypesChangedListeners;

        /** The user data types for the scene. */
        UserDataTypes userDataTypes;
    };

} };
