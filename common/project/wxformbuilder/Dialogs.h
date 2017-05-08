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


#ifndef __DIALOGS_H__
#define __DIALOGS_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/radiobut.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/statbox.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/hyperlink.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/panel.h>
#include <wx/listctrl.h>
#include <wx/clrpicker.h>
#include <wx/scrolwin.h>
#include <wx/gauge.h>
#include <wx/listbox.h>

///////////////////////////////////////////////////////////////////////////

namespace Finjin
{
    ///////////////////////////////////////////////////////////////////////////////
    /// Class InstallationDialogBase
    ///////////////////////////////////////////////////////////////////////////////
    class InstallationDialogBase : public wxDialog
    {
        private:

        protected:
            wxRadioButton* installRadioButton;
            wxRadioButton* uninstallRadioButton;
            wxStaticText* aboutLabel;
            wxCheckBox* m_checkBox1;
            wxCheckBox* m_checkBox2;
            wxCheckBox* m_checkBox3;
            wxCheckBox* m_checkBox4;
            wxCheckBox* m_checkBox11;
            wxCheckBox* m_checkBox21;
            wxCheckBox* m_checkBox31;
            wxCheckBox* m_checkBox41;
            wxCheckBox* showDetailsCheckBox;
            wxButton* installButton;
            wxButton* closeButton;

        public:

            InstallationDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Install Finjin"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 200,463 ), long style = wxDEFAULT_DIALOG_STYLE );
            ~InstallationDialogBase();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class AboutDialog
    ///////////////////////////////////////////////////////////////////////////////
    class AboutDialog : public wxDialog
    {
        private:

        protected:
            wxStaticBitmap* logoBitmap;
            wxStaticText* descriptionText;
            wxHyperlinkCtrl* homeHyperlink;
            wxHyperlinkCtrl* githubHyperlink;
            wxStaticText* versionText;
            wxNotebook* notebook;
            wxButton* closeButton;

        public:

            AboutDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("About Finjin"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 320,473 ), long style = wxDEFAULT_DIALOG_STYLE );
            ~AboutDialog();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class GlobalSettingsDialog
    ///////////////////////////////////////////////////////////////////////////////
    class GlobalSettingsDialog : public wxDialog
    {
        private:

        protected:
            wxNotebook* notebook;
            wxButton* okButton;
            wxButton* cancelButton;

        public:

            GlobalSettingsDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Finjin Global Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 608,599 ), long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU );
            ~GlobalSettingsDialog();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class SceneSettingsDialog
    ///////////////////////////////////////////////////////////////////////////////
    class SceneSettingsDialog : public wxDialog
    {
        private:

        protected:
            wxNotebook* tabControl;
            wxButton* closeButton;

        public:

            SceneSettingsDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Finjin - Scene Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 493,587 ), long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU );
            ~SceneSettingsDialog();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class UserDataSettingsDialog
    ///////////////////////////////////////////////////////////////////////////////
    class UserDataSettingsDialog : public wxDialog
    {
        private:

        protected:
            wxButton* closeButton;

        public:

            UserDataSettingsDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Finjin - User Data Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 493,587 ), long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU );
            ~UserDataSettingsDialog();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class ObjectSettingsDialog
    ///////////////////////////////////////////////////////////////////////////////
    class ObjectSettingsDialog : public wxDialog
    {
        private:

        protected:
            wxNotebook* notebook;
            wxButton* closeButton;

        public:

            ObjectSettingsDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Finjin - Object Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 493,628 ), long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU );
            ~ObjectSettingsDialog();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class GlobalSettingsDialog_AnimationsPage
    ///////////////////////////////////////////////////////////////////////////////
    class GlobalSettingsDialog_AnimationsPage : public wxPanel
    {
        private:

        protected:
            wxStaticText* skeletonAnimationSamplingLabel;
            wxTextCtrl* skeletonAnimationSamplingText;
            wxChoice* skeletonAnimationSamplingChoice;
            wxStaticText* vertexAnimationSamplingLabel;
            wxTextCtrl* vertexAnimationSamplingText;
            wxChoice* vertexAnimationSamplingChoice;
            wxStaticText* nodeAnimationSamplingLabel;
            wxTextCtrl* nodeAnimationSamplingText;
            wxChoice* nodeAnimationSamplingChoice;

        public:

            GlobalSettingsDialog_AnimationsPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 593,304 ), long style = wxTAB_TRAVERSAL );
            ~GlobalSettingsDialog_AnimationsPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class GlobalSettingsDialog_FlagsPage
    ///////////////////////////////////////////////////////////////////////////////
    class GlobalSettingsDialog_FlagsPage : public wxPanel
    {
        private:

        protected:
            wxButton* addButton;
            wxTextCtrl* flagsText;

        public:

            GlobalSettingsDialog_FlagsPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 526,568 ), long style = wxTAB_TRAVERSAL );
            ~GlobalSettingsDialog_FlagsPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class GlobalSettingsDialog_GeneralPage
    ///////////////////////////////////////////////////////////////////////////////
    class GlobalSettingsDialog_GeneralPage : public wxPanel
    {
        private:

        protected:
            wxStaticText* textBlobFormatLabel;
            wxChoice* textBlobFormatChoice;
            wxStaticText* blobTextFormatDefaultLabel;
            wxStaticText* maxBytesPerLineLabel;
            wxTextCtrl* maxBytesPerLineSpinner;
            wxStaticText* alwaysExportLabel;
            wxCheckBox* alwaysExportTextCheckbox;
            wxCheckBox* alwaysExportBinaryCheckbox;
            wxCheckBox* alwaysExportConfigCheckbox;
            wxCheckBox* alwaysExportJsonCheckbox;
            wxStaticText* embedLabel;
            wxCheckBox* embedMeshesCheckbox;
            wxCheckBox* embedMaterialsCheckbox;
            wxCheckBox* embedTexturesCheckbox;
            wxCheckBox* embedPrefabsCheckbox;
            wxStaticText* exportLabel;
            wxCheckBox* exportApplicationNameCheckbox;
            wxCheckBox* exportAuthorCheckbox;
            wxCheckBox* exportTimestampCheckbox;
            wxCheckBox* exporterVersionCheckbox;
            wxStaticText* exportDirectoryDescriptionLabel;
            wxStaticText* exportDirectoryLabel;
            wxTextCtrl* exportDirectoryText;
            wxRadioButton* finjinViewerButton;
            wxCheckBox* runExternalViewerAfterExportCheckbox;

        public:

            GlobalSettingsDialog_GeneralPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 593,512 ), long style = wxTAB_TRAVERSAL );
            ~GlobalSettingsDialog_GeneralPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class GlobalSettingsDialog_MaterialsPage
    ///////////////////////////////////////////////////////////////////////////////
    class GlobalSettingsDialog_MaterialsPage : public wxPanel
    {
        private:

        protected:
            wxStaticText* noMaterialNameLabel;
            wxTextCtrl* noMaterialNameText;
            wxStaticText* noMaterialNameDescriptionLabel;
            wxCheckBox* includeParentMaterialInNameCheckbox;
            wxCheckBox* createExtraMaterialsCheckbox;
            wxCheckBox* copyBitmapsToExportDirectoryCheckbox;
            wxCheckBox* convertBitmapsToTexturesCheckbox;

        public:

            GlobalSettingsDialog_MaterialsPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 593,360 ), long style = wxTAB_TRAVERSAL );
            ~GlobalSettingsDialog_MaterialsPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class GlobalSettingsDialog_MeshesPage
    ///////////////////////////////////////////////////////////////////////////////
    class GlobalSettingsDialog_MeshesPage : public wxPanel
    {
        private:

        protected:
            wxStaticText* submeshNamingLabel;
            wxChoice* submeshNamingChoice;
            wxTextCtrl* submeshNamingCustomText;
            wxCheckBox* alwaysExportVertexFormatInMeshCheckbox;
            wxStaticText* baseDirectoryLabel;
            wxChoice* baseDirectoryChoice;
            wxTextCtrl* baseDirectoryText;
            wxButton* baseDirectoryButton;
            wxStaticText* vertexFormatFileLabel;
            wxTextCtrl* vertexFormatFileText;
            wxButton* vertexFormatFileButton;

        public:

            GlobalSettingsDialog_MeshesPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 593,538 ), long style = wxTAB_TRAVERSAL );
            ~GlobalSettingsDialog_MeshesPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class GlobalSettingsDialog_ScenePage
    ///////////////////////////////////////////////////////////////////////////////
    class GlobalSettingsDialog_ScenePage : public wxPanel
    {
        private:

        protected:
            wxStaticText* sceneScaleLabel;
            wxChoice* sceneScaleChoice;
            wxStaticText* divideByLabel;
            wxTextCtrl* dividedBySpinner;

        public:

            GlobalSettingsDialog_ScenePage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 593,185 ), long style = wxTAB_TRAVERSAL );
            ~GlobalSettingsDialog_ScenePage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class GlobalSettingsDialog_UserDataClassPage
    ///////////////////////////////////////////////////////////////////////////////
    class GlobalSettingsDialog_UserDataClassPage : public wxPanel
    {
        private:

        protected:
            wxStaticText* baseDirectoryLabel;
            wxChoice* baseDirectoryChoice;
            wxTextCtrl* baseDirectoryText;
            wxButton* baseDirectoryButton;
            wxStaticText* resourceLocationsDescriptionLabel;
            wxTextCtrl* resourceLocationsText;
            wxButton* addDirectoryButton;
            wxButton* addFileButton;

        public:

            GlobalSettingsDialog_UserDataClassPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 593,374 ), long style = wxTAB_TRAVERSAL );
            ~GlobalSettingsDialog_UserDataClassPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class ObjectSettingsDialog_GeneralPage
    ///////////////////////////////////////////////////////////////////////////////
    class ObjectSettingsDialog_GeneralPage : public wxPanel
    {
        private:

        protected:
            wxStaticText* typeLabel;
            wxChoice* typeChoice;
            wxStaticText* defaultTypeLabel;
            wxCheckBox* isPrefabCheckbox;
            wxCheckBox* isPrefabInstanceCheckbox;
            wxStaticText* visibilityLabel;
            wxChoice* visibilityChoice;
            wxCheckBox* visibilityAffectObjectOnlyCheckbox;
            wxStaticText* renderQueueLabel;
            wxTextCtrl* renderQueueText;
            wxStaticText* renderQueuePriorityLabel;
            wxTextCtrl* renderQueuePriorityText;
            wxStaticText* renderDistanceLabel;
            wxTextCtrl* renderDistanceText;
            wxStaticText* exportedParentLabel;
            wxTextCtrl* exportedParentText;
            wxButton* exportedParentButton;
            wxCheckBox* useRootAsExportedParentCheckbox;
            wxCheckBox* isAttachedObjectCheckbox;
            wxCheckBox* ignoreChildrenCheckbox;
            wxStaticText* sortChildrenLabel;
            wxChoice* sortChildrenChoice;
            wxStaticText* childOrderLabel;
            wxTextCtrl* childOrderText;
            wxStaticBoxSizer* noteTrackNamesSizer;
            wxTextCtrl* noteTrackNamesText;
            wxButton* addFlagButton;
            wxTextCtrl* flagsText;

        public:

            ObjectSettingsDialog_GeneralPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 491,611 ), long style = wxTAB_TRAVERSAL );
            ~ObjectSettingsDialog_GeneralPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class ObjectSettingsDialog_MeshPage
    ///////////////////////////////////////////////////////////////////////////////
    class ObjectSettingsDialog_MeshPage : public wxPanel
    {
        private:

        protected:
            wxStaticText* meshNameLabel;
            wxTextCtrl* meshNameText;
            wxStaticText* primitiveTypeLabel;
            wxChoice* primitiveTypeChoice;
            wxStaticText* pointSizeLabel;
            wxTextCtrl* pointSizeSpinner;
            wxCheckBox* exportVertexColorsCheckbox;
            wxCheckBox* animateBoundingVolumesCheckbox;
            wxCheckBox* checkMeshInstanceCheckbox;
            wxCheckBox* exportTangentsCheckbox;
            wxCheckBox* exportBinormalsCheckbox;

        public:

            ObjectSettingsDialog_MeshPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 494,485 ), long style = wxTAB_TRAVERSAL );
            ~ObjectSettingsDialog_MeshPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class ObjectSettingsDialog_SubmeshesPage
    ///////////////////////////////////////////////////////////////////////////////
    class ObjectSettingsDialog_SubmeshesPage : public wxPanel
    {
        private:

        protected:
            wxCheckBox* useCustomTextureCoordinateCheckbox;
            wxStaticText* activeTextureCoordinateSetLabel;
            wxChoice* submeshTextureCoordinateSetChoice;
            wxStaticText* submeshNameLabel;
            wxTextCtrl* submeshNameText;
            wxStaticText* renderQueueLabel;
            wxTextCtrl* renderQueueText;
            wxStaticText* renderQueuePriorityLabel;
            wxTextCtrl* renderQueuePriorityText;
            wxStaticText* textureCoordinateSetsLabel;
            wxButton* refreshTextureCoordinateSetsButton;
            wxButton* clearTextureCoordinateSetsButton;
            wxListCtrl* textureCoordinateSetsList;
            wxButton* moveTextureCoordinateSetUpButton;
            wxButton* moveTextureCoordinateSetDownButton;
            wxButton* removeTextureCoordinateSetButton;
            wxButton* textureCoordinateSetUButton;
            wxButton* textureCoordinateSetUVButton;
            wxButton* textureCoordinateSetUVWButton;

        public:

            ObjectSettingsDialog_SubmeshesPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 494,726 ), long style = wxTAB_TRAVERSAL );
            ~ObjectSettingsDialog_SubmeshesPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class ObjectSettingsDialog_MeshLodPage
    ///////////////////////////////////////////////////////////////////////////////
    class ObjectSettingsDialog_MeshLodPage : public wxPanel
    {
        private:

        protected:
            wxListCtrl* manualLodList;
            wxButton* addManualLodButton;
            wxButton* editManualLodButton;
            wxButton* removeManualLodButton;
            wxCheckBox* calculateManualDistancesCheckbox;

        public:

            ObjectSettingsDialog_MeshLodPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 494,573 ), long style = wxTAB_TRAVERSAL );
            ~ObjectSettingsDialog_MeshLodPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class ObjectSettingsDialog_EntityPage
    ///////////////////////////////////////////////////////////////////////////////
    class ObjectSettingsDialog_EntityPage : public wxPanel
    {
        private:

        protected:
            wxCheckBox* exportMeshCheckbox;
            wxStaticText* skeletonAnimationBlendingLabel;
            wxChoice* skeletonAnimationBlendingChoice;

        public:

            ObjectSettingsDialog_EntityPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 494,573 ), long style = wxTAB_TRAVERSAL );
            ~ObjectSettingsDialog_EntityPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class ObjectSettingsDialog_PrefabInstancePage
    ///////////////////////////////////////////////////////////////////////////////
    class ObjectSettingsDialog_PrefabInstancePage : public wxPanel
    {
        private:

        protected:
            wxStaticText* modelNameLabel;
            wxTextCtrl* modelNameText;

        public:

            ObjectSettingsDialog_PrefabInstancePage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
            ~ObjectSettingsDialog_PrefabInstancePage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class ObjectSettingsDialog_SkyPage
    ///////////////////////////////////////////////////////////////////////////////
    class ObjectSettingsDialog_SkyPage : public wxPanel
    {
        private:

        protected:
            wxCheckBox* enableCheckbox;

        public:

            ObjectSettingsDialog_SkyPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 494,573 ), long style = wxTAB_TRAVERSAL );
            ~ObjectSettingsDialog_SkyPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class ObjectSettingsDialog_LightPage
    ///////////////////////////////////////////////////////////////////////////////
    class ObjectSettingsDialog_LightPage : public wxPanel
    {
        private:

        protected:
            wxStaticText* specularColorLabel;
            wxColourPickerCtrl* specularColor;
            wxStaticText* constantAttenuationLabel;
            wxTextCtrl* constantAttenuationText;
            wxStaticText* linearAttenuationLabel;
            wxTextCtrl* linearAttenuationText;
            wxStaticText* quadraticAttenuationLabel;
            wxTextCtrl* quadraticAttenuationText;

        public:

            ObjectSettingsDialog_LightPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
            ~ObjectSettingsDialog_LightPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class ObjectSettingsDialog_MeshAnimationsPage
    ///////////////////////////////////////////////////////////////////////////////
    class ObjectSettingsDialog_MeshAnimationsPage : public wxPanel
    {
        private:

        protected:
            wxStaticText* meshAnimationTypeLabel;
            wxChoice* meshAnimationTypeChoice;
            wxStaticText* selectedMeshAnimationTypeLabel;
            wxCheckBox* exportSkeletonCheckbox;
            wxCheckBox* exportedMeshDeformedBySkeletonCheckbox;
            wxCheckBox* embedSkeletonCheckbox;
            wxCheckBox* linkSkeletonToObjectCheckbox;
            wxStaticText* referencePoseLabel;
            wxChoice* referencePoseChoice;
            wxTextCtrl* referencePoseTimeSpinner;
            wxStaticText* referencePoseNoteLabel;
            wxStaticText* skeletonNameLabel;
            wxTextCtrl* skeletonNameText;
            wxStaticText* animatedRootLabel;
            wxTextCtrl* animatedRootText;
            wxButton* animatedRootChooseBoneButton;
            wxButton* animatedRootChooseObjectButton;
            wxStaticText* skeletonAnimationSamplingLabel;
            wxTextCtrl* skeletonAnimationSamplingText;
            wxChoice* skeletonAnimationSamplingChoice;
            wxCheckBox* removeBonesWithNoInfluenceCheckBox;
            wxCheckBox* exportPosesCheckbox;
            wxStaticText* vertexAnimationSamplingLabel;
            wxTextCtrl* vertexAnimationSamplingText;
            wxChoice* vertexAnimationSamplingChoice;
            wxButton* addMeshAnimationButton;
            wxButton* editMeshAnimationButton;
            wxButton* copyMeshAnimationButton;
            wxButton* removeMeshAnimationButton;
            wxButton* syncToBipedMixerButton;
            wxListCtrl* meshAnimationsList;

        public:

            ObjectSettingsDialog_MeshAnimationsPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 570,603 ), long style = wxTAB_TRAVERSAL );
            ~ObjectSettingsDialog_MeshAnimationsPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class ObjectSettingsDialog_NodeAnimationsPage
    ///////////////////////////////////////////////////////////////////////////////
    class ObjectSettingsDialog_NodeAnimationsPage : public wxPanel
    {
        private:

        protected:
            wxStaticText* defaultAnimationSamplingLabel;
            wxTextCtrl* defaultAnimationSamplingText;
            wxChoice* defaultAnimationSamplingChoice;
            wxButton* addNodeAnimationButton;
            wxButton* editNodeAnimationButton;
            wxButton* copyNodeAnimationButton;
            wxButton* removeNodeAnimationButton;
            wxListCtrl* nodeAnimationsList;

        public:

            ObjectSettingsDialog_NodeAnimationsPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,515 ), long style = wxTAB_TRAVERSAL );
            ~ObjectSettingsDialog_NodeAnimationsPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class ObjectSettingsDialog_PhysicalPage
    ///////////////////////////////////////////////////////////////////////////////
    class ObjectSettingsDialog_PhysicalPage : public wxPanel
    {
        private:

        protected:
            wxStaticText* itemTypeLabel;
            wxTextCtrl* itemTypeText;

        public:

            ObjectSettingsDialog_PhysicalPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
            ~ObjectSettingsDialog_PhysicalPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class SceneSettingsDialog_GeneralPage
    ///////////////////////////////////////////////////////////////////////////////
    class SceneSettingsDialog_GeneralPage : public wxPanel
    {
        private:

        protected:
            wxStaticText* sceneManagerLabel;
            wxTextCtrl* customSceneManagerText;
            wxStaticText* sceneScaleLabel;
            wxChoice* sceneScaleChoice;
            wxStaticText* divideByLabel;
            wxTextCtrl* dividedBySpinner;
            wxCheckBox* exportHierarchyCheckbox;
            wxCheckBox* exportHiddenObjectsCheckbox;
            wxButton* addFlagButton;
            wxTextCtrl* queryFlagAliasesText;

        public:

            SceneSettingsDialog_GeneralPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 526,568 ), long style = wxTAB_TRAVERSAL );
            ~SceneSettingsDialog_GeneralPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class SceneSettingsDialog_MeshesPage
    ///////////////////////////////////////////////////////////////////////////////
    class SceneSettingsDialog_MeshesPage : public wxPanel
    {
        private:

        protected:
            wxCheckBox* exportVertexColorsCheckbox;
            wxCheckBox* useExplicitVertexColorCheckbox;
            wxCheckBox* animateBoundingVolumesCheckbox;
            wxCheckBox* checkMeshInstancesCheckbox;
            wxCheckBox* alwaysCreateNewSubmeshesWhenMergingCheckBox;
            wxCheckBox* exportTangentsCheckbox;
            wxCheckBox* exportBinormalsCheckbox;

        public:

            SceneSettingsDialog_MeshesPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 509,683 ), long style = wxTAB_TRAVERSAL );
            ~SceneSettingsDialog_MeshesPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class SceneSettingsDialog_MaterialsPage
    ///////////////////////////////////////////////////////////////////////////////
    class SceneSettingsDialog_MaterialsPage : public wxPanel
    {
        private:

        protected:
            wxButton* addAdditionalMaterialButton;
            wxTextCtrl* addAdditionalMaterialsText;
            wxButton* addIgnoreMaterialButton;
            wxTextCtrl* ignoreMaterialsText;

        public:

            SceneSettingsDialog_MaterialsPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 619,626 ), long style = wxTAB_TRAVERSAL );
            ~SceneSettingsDialog_MaterialsPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class SceneSettingsDialog_BitmapsPage
    ///////////////////////////////////////////////////////////////////////////////
    class SceneSettingsDialog_BitmapsPage : public wxPanel
    {
        private:

        protected:
            wxCheckBox* copyBitmapsToExportDirectoryCheckbox;
            wxCheckBox* convertBitmapsToTexturesCheckbox;

        public:

            SceneSettingsDialog_BitmapsPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 611,626 ), long style = wxTAB_TRAVERSAL );
            ~SceneSettingsDialog_BitmapsPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class SceneSettingsDialog_EnvironmentPage
    ///////////////////////////////////////////////////////////////////////////////
    class SceneSettingsDialog_EnvironmentPage : public wxPanel
    {
        private:

        protected:
            wxCheckBox* exportCheckbox;
            wxStaticText* ambientLightColorLabel;
            wxColourPickerCtrl* ambientLightColor;
            wxChoice* ambientLightColorSyncChoice;
            wxStaticText* backgroundColorLabel;
            wxColourPickerCtrl* backgroundColor;
            wxChoice* backgroundColorSyncChoice;
            wxStaticText* nearDistanceLabel;
            wxTextCtrl* nearDistanceSpinner;
            wxStaticText* farLabel;
            wxRadioButton* farDistanceButton;
            wxTextCtrl* farDistanceSpinner;
            wxRadioButton* farBoxObjectButton;
            wxTextCtrl* farBoxObjectText;
            wxButton* chooseFarBoxObjectButton;
            wxStaticText* fogModeLabel;
            wxChoice* fogModeChoice;
            wxStaticText* fogDensityLabel;
            wxTextCtrl* fogDensitySpinner;
            wxStaticText* fogStartLabel;
            wxTextCtrl* fogStartSpinner;
            wxStaticText* fogEndLabel;
            wxTextCtrl* fogEndSpinner;
            wxStaticText* fogColorLabel;
            wxColourPickerCtrl* fogColor;
            wxChoice* fogSyncChoice;
            wxStaticText* shadowTechniqueLabel;
            wxTextCtrl* shadowTechniqueText;
            wxStaticText* shadowFarDistanceLabel;
            wxTextCtrl* shadowFarDistanceSpinner;
            wxStaticText* shadowColorLabel;
            wxColourPickerCtrl* shadowColor;

        public:

            SceneSettingsDialog_EnvironmentPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 648,836 ), long style = wxTAB_TRAVERSAL );
            ~SceneSettingsDialog_EnvironmentPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class SceneSettingsDialog_DirectoriesPage
    ///////////////////////////////////////////////////////////////////////////////
    class SceneSettingsDialog_DirectoriesPage : public wxPanel
    {
        private:

        protected:
            wxStaticText* exportToDefaultSceneFileLabel;
            wxCheckBox* exportToDefaultSceneFileCheckbox;
            wxStaticText* baseDirectoryLabel;
            wxChoice* baseDirectoryChoice;
            wxTextCtrl* baseDirectoryText;
            wxButton* baseDirectoryButton;
            wxStaticText* defaultSceneFileNameLabel;
            wxTextCtrl* defaultSceneFileNameText;
            wxButton* defaultSceneFileNameButton;

        public:

            SceneSettingsDialog_DirectoriesPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 640,645 ), long style = wxTAB_TRAVERSAL );
            ~SceneSettingsDialog_DirectoriesPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class SceneSettingsDialog_AnimationsPage
    ///////////////////////////////////////////////////////////////////////////////
    class SceneSettingsDialog_AnimationsPage : public wxPanel
    {
        private:

        protected:
            wxStaticText* skeletonAnimationSamplingLabel;
            wxTextCtrl* skeletonAnimationSamplingText;
            wxChoice* skeletonAnimationSamplingChoice;
            wxStaticText* vertexAnimationSamplingLabel;
            wxTextCtrl* vertexAnimationSamplingText;
            wxChoice* vertexAnimationSamplingChoice;
            wxStaticText* nodeAnimationSamplingLabel;
            wxTextCtrl* nodeAnimationSamplingText;
            wxChoice* nodeAnimationSamplingChoice;

        public:

            SceneSettingsDialog_AnimationsPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,339 ), long style = wxTAB_TRAVERSAL );
            ~SceneSettingsDialog_AnimationsPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class SceneSettingsDialog_EventsPage
    ///////////////////////////////////////////////////////////////////////////////
    class SceneSettingsDialog_EventsPage : public wxPanel
    {
        private:

        protected:
            wxStaticText* descriptionLabel;
            wxTextCtrl* beforeExportText;
            wxTextCtrl* afterExportText;
            wxStaticText* macrosLabel;
            wxTextCtrl* macrosText;

        public:

            SceneSettingsDialog_EventsPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,562 ), long style = wxTAB_TRAVERSAL );
            ~SceneSettingsDialog_EventsPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class UserDataSettingsPage
    ///////////////////////////////////////////////////////////////////////////////
    class UserDataSettingsPage : public wxPanel
    {
        private:

        protected:
            wxStaticText* idLabel;
            wxTextCtrl* idText;
            wxButton* generateIDButton;
            wxStaticText* userDataClassLabel;
            wxChoice* userDataClassChoice;
            wxButton* configureUserDataClassButton;
            wxTextCtrl* userDataText;
            wxScrolledWindow* userDataControlsWindow;

        public:

            UserDataSettingsPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,458 ), long style = wxTAB_TRAVERSAL );
            ~UserDataSettingsPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class AboutDialog_AcknowledgmentsPage
    ///////////////////////////////////////////////////////////////////////////////
    class AboutDialog_AcknowledgmentsPage : public wxPanel
    {
        private:

        protected:
            wxStaticText* thirdPartyLibrariesLabel;
            wxTextCtrl* thirdPartyLibrariesText;

            // Virtual event handlers, overide them in your derived class
            virtual void OnTextURL( wxTextUrlEvent& event ) { event.Skip(); }


        public:

            AboutDialog_AcknowledgmentsPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 300,180 ), long style = wxTAB_TRAVERSAL );
            ~AboutDialog_AcknowledgmentsPage();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class ProgressDialog
    ///////////////////////////////////////////////////////////////////////////////
    class ProgressDialog : public wxDialog
    {
        private:

        protected:
            wxStaticText* totalProgressLabel;
            wxGauge* totalProgressGauge;
            wxStaticText* progressLabel;
            wxGauge* progressGauge;
            wxTextCtrl* progressText;
            wxButton* okButton;

        public:

            ProgressDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Progress"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 498,318 ), long style = wxCAPTION|wxMAXIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU );
            ~ProgressDialog();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class NodeAnimationSettingsDialog
    ///////////////////////////////////////////////////////////////////////////////
    class NodeAnimationSettingsDialog : public wxDialog
    {
        private:

        protected:
            wxStaticText* nameLabel;
            wxTextCtrl* nameText;
            wxStaticText* startEndFramesLabel;
            wxTextCtrl* startFrameText;
            wxStaticText* inBetweenFrameLabel;
            wxTextCtrl* endFrameText;
            wxStaticText* timeScalingLabel;
            wxChoice* timeScalingChoice;
            wxTextCtrl* timeScalingText;
            wxStaticText* samplingLabel;
            wxTextCtrl* samplingText;
            wxChoice* samplingChoice;
            wxStaticText* translationInterpolationLabel;
            wxStaticText* rotationInterpolationLabel;
            wxChoice* translationInterpolationChoice;
            wxChoice* rotationInterpolationChoice;
            wxCheckBox* embedAnimationCheckbox;
            wxCheckBox* linkToObjectCheckbox;
            wxCheckBox* enabledCheckbox;
            wxCheckBox* loopedCheckbox;
            wxCheckBox* useDefinedAnimationStartTimeCheckbox;
            wxCheckBox* copyFirstAnimationKeyToLastCheckbox;
            wxButton* okButton;
            wxButton* cancelButton;

        public:

            NodeAnimationSettingsDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Finjin - Animation Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 316,355 ), long style = wxDEFAULT_DIALOG_STYLE );
            ~NodeAnimationSettingsDialog();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class MeshAnimationSettingsDialog
    ///////////////////////////////////////////////////////////////////////////////
    class MeshAnimationSettingsDialog : public wxDialog
    {
        private:

        protected:
            wxStaticText* nameLabel;
            wxTextCtrl* nameText;
            wxStaticText* trackLabel;
            wxChoice* trackChoice;
            wxStaticText* startEndFramesLabel;
            wxTextCtrl* startFrameText;
            wxStaticText* inBetweenFrameLabel;
            wxTextCtrl* endFrameText;
            wxStaticText* timeScalingLabel;
            wxChoice* timeScalingChoice;
            wxTextCtrl* timeScalingText;
            wxStaticText* samplingLabel;
            wxTextCtrl* samplingText;
            wxChoice* samplingChoice;
            wxStaticText* morphWholeObjectLabel;
            wxChoice* morphWholeObjectChoice;
            wxCheckBox* embedAnimationCheckbox;
            wxCheckBox* linkToObjectCheckbox;
            wxCheckBox* copyFirstAnimationKeyToLastCheckbox;
            wxCheckBox* exportAsMorphCheckbox;
            wxCheckBox* overrideAnimatedRootStartTimeCheckbox;
            wxTextCtrl* animatedRootStartTimeText;
            wxStaticBoxSizer* skeletonAnimationSizer;
            wxStaticText* removeBoneTranslationsLabel;
            wxButton* addRemoveTranslationsBoneButton;
            wxTextCtrl* removeBoneTranslationsText;
            wxStaticText* includeExcludeBonesLabel;
            wxButton* includeExcludeBonesButton;
            wxTextCtrl* includeExcludeBonesText;
            wxButton* okButton;
            wxButton* cancelButton;

        public:

            MeshAnimationSettingsDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Finjin - Animation Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 339,710 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
            ~MeshAnimationSettingsDialog();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class ConvertToFramesDialog
    ///////////////////////////////////////////////////////////////////////////////
    class ConvertToFramesDialog : public wxDialog
    {
        private:

        protected:
            wxStaticText* sourceFileDescriptionLabel;
            wxTextCtrl* sourceFileText;
            wxButton* sourceFileButton;
            wxStaticText* destinationDirectoryDescriptionLabel;
            wxTextCtrl* destinationDirectoryText;
            wxButton* destinationDirectoryButton;
            wxStaticText* destinationBaseFileNameDescriptionLabel;
            wxTextCtrl* destinationBaseFileNameText;
            wxRadioButton* originalSizeButton;
            wxRadioButton* customSizeButton;
            wxTextCtrl* customWidthText;
            wxStaticText* customSizeXLabel;
            wxTextCtrl* customHeightText;
            wxButton* okButton;
            wxButton* cancelButton;

        public:

            ConvertToFramesDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Convert to Frames"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 361,363 ), long style = wxDEFAULT_DIALOG_STYLE );
            ~ConvertToFramesDialog();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class FlagAliasDialog
    ///////////////////////////////////////////////////////////////////////////////
    class FlagAliasDialog : public wxDialog
    {
        private:

        protected:
            wxStaticText* nameLabel;
            wxTextCtrl* nameText;
            wxStaticText* bitLabel;
            wxTextCtrl* bitText;
            wxButton* okButton;
            wxButton* cancelButton;

        public:

            FlagAliasDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Flag Alias"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 207,123 ), long style = wxDEFAULT_DIALOG_STYLE );
            ~FlagAliasDialog();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class FlagDialog
    ///////////////////////////////////////////////////////////////////////////////
    class FlagDialog : public wxDialog
    {
        private:

        protected:
            wxStaticText* nameText;
            wxChoice* nameChoice;
            wxButton* okButton;
            wxButton* cancelButton;

        public:

            FlagDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Flag"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 217,123 ), long style = wxDEFAULT_DIALOG_STYLE );
            ~FlagDialog();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class IncludeExcludeBoneDialog
    ///////////////////////////////////////////////////////////////////////////////
    class IncludeExcludeBoneDialog : public wxDialog
    {
        private:

        protected:
            wxStaticText* boneLabel;
            wxTextCtrl* boneEdit;
            wxButton* boneButton;
            wxRadioButton* includeButton;
            wxRadioButton* excludeButton;
            wxCheckBox* applyRecursiveCheckbox;
            wxButton* okButton;
            wxButton* cancelButton;

        public:

            IncludeExcludeBoneDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Include/Exclude Bone"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 300,136 ), long style = wxDEFAULT_DIALOG_STYLE );
            ~IncludeExcludeBoneDialog();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class RenameDialog
    ///////////////////////////////////////////////////////////////////////////////
    class RenameDialog : public wxDialog
    {
        private:

        protected:
            wxStaticText* fromLabel;
            wxTextCtrl* fromText;
            wxStaticText* toLabel;
            wxTextCtrl* toText;
            wxButton* okButton;
            wxButton* cancelButton;

        public:

            RenameDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Rename"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 307,99 ), long style = wxDEFAULT_DIALOG_STYLE );
            ~RenameDialog();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class AddDialog
    ///////////////////////////////////////////////////////////////////////////////
    class AddDialog : public wxDialog
    {
        private:

        protected:
            wxStaticText* addLabel;
            wxTextCtrl* addText;
            wxButton* okButton;
            wxButton* cancelButton;

        public:

            AddDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Add"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 307,93 ), long style = wxDEFAULT_DIALOG_STYLE );
            ~AddDialog();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class RenderCubeMapDialog
    ///////////////////////////////////////////////////////////////////////////////
    class RenderCubeMapDialog : public wxDialog
    {
        private:

        protected:
            wxStaticText* informationLabel;
            wxStaticText* cubeFaceSizeLabel;
            wxTextCtrl* cubeFaceSizeEdit;
            wxRadioButton* separateFilesButton;
            wxStaticText* destinationDirectoryDescriptionLabel;
            wxTextCtrl* destinationDirectoryText;
            wxButton* destinationDirectoryButton;
            wxStaticText* destinationBaseFileNameDescriptionLabel;
            wxTextCtrl* destinationBaseFileNameText;
            wxRadioButton* ddsFileButton;
            wxTextCtrl* ddsFileText;
            wxButton* ddsFileButton;
            wxButton* okButton;
            wxButton* cancelButton;

        public:

            RenderCubeMapDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Render Cube Map"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 369,326 ), long style = wxDEFAULT_DIALOG_STYLE );
            ~RenderCubeMapDialog();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class RenderTerrainMapDialog
    ///////////////////////////////////////////////////////////////////////////////
    class RenderTerrainMapDialog : public wxDialog
    {
        private:

        protected:
            wxStaticText* informationLabel;
            wxStaticText* dimensionsLabel;
            wxTextCtrl* widthEdit;
            wxStaticText* dimensionsXLabel;
            wxTextCtrl* heightText;
            wxCheckBox* squareCheckbox;
            wxCheckBox* rotate90DegreesClockwiseCheckbox;
            wxStaticText* bytesPerPixelLabel;
            wxTextCtrl* bytesPerPixelEdit;
            wxStaticText* destinationFileLabel;
            wxTextCtrl* destinationFileText;
            wxButton* destinationFileButton;
            wxButton* okButton;
            wxButton* cancelButton;

        public:

            RenderTerrainMapDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Render Map"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 381,192 ), long style = wxDEFAULT_DIALOG_STYLE );
            ~RenderTerrainMapDialog();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class TextInputDialog
    ///////////////////////////////////////////////////////////////////////////////
    class TextInputDialog : public wxDialog
    {
        private:

        protected:
            wxStaticText* descriptionalLabel;
            wxTextCtrl* inputText;
            wxButton* okButton;
            wxButton* cancelButton;

        public:

            TextInputDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Text Input"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 302,148 ), long style = wxDEFAULT_DIALOG_STYLE );
            ~TextInputDialog();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class ManualLodDialog
    ///////////////////////////////////////////////////////////////////////////////
    class ManualLodDialog : public wxDialog
    {
        private:

        protected:
            wxStaticText* objectLabel;
            wxTextCtrl* objectEdit;
            wxButton* objectButton;
            wxStaticText* distanceLabel;
            wxTextCtrl* distanceText;
            wxButton* okButton;
            wxButton* cancelButton;

        public:

            ManualLodDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Manual LOD"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 319,120 ), long style = wxDEFAULT_DIALOG_STYLE );
            ~ManualLodDialog();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class ChooseObjectsDialog
    ///////////////////////////////////////////////////////////////////////////////
    class ChooseObjectsDialog : public wxDialog
    {
        private:

        protected:
            wxStaticText* nameLabel;
            wxTextCtrl* nameTextBox;
            wxCheckBox* showFullNamesCheckbox;
            wxListBox* objectsListBox;
            wxCheckBox* meshesCheckBox;
            wxCheckBox* planesCheckBox;
            wxCheckBox* lightsCheckBox;
            wxCheckBox* camerasCheckBox;
            wxCheckBox* otherCheckBox;
            wxButton* allButton;
            wxButton* noneButton;
            wxButton* invertButton;
            wxButton* okButton;
            wxButton* cancelButton;

        public:

            ChooseObjectsDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Choose Objects"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 380,456 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
            ~ChooseObjectsDialog();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class ChooseFilesDialog
    ///////////////////////////////////////////////////////////////////////////////
    class ChooseFilesDialog : public wxDialog
    {
        private:

        protected:
            wxStaticText* nameLabel;
            wxTextCtrl* nameTextBox;
            wxListBox* filesListBox;
            wxButton* okButton;
            wxButton* cancelButton;

        public:

            ChooseFilesDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Choose a File"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 380,456 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
            ~ChooseFilesDialog();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class ChooseMaterialsDialog
    ///////////////////////////////////////////////////////////////////////////////
    class ChooseMaterialsDialog : public wxDialog
    {
        private:

        protected:
            wxStaticText* nameLabel;
            wxTextCtrl* nameTextBox;
            wxCheckBox* showFullNamesCheckbox;
            wxListBox* objectsListBox;
            wxButton* allButton;
            wxButton* noneButton;
            wxButton* invertButton;
            wxButton* okButton;
            wxButton* cancelButton;

        public:

            ChooseMaterialsDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Choose Materials"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 380,456 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
            ~ChooseMaterialsDialog();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class FinjinMaterialFileRemappingDialog
    ///////////////////////////////////////////////////////////////////////////////
    class FinjinMaterialFileRemappingDialog : public wxDialog
    {
        private:

        protected:
            wxStaticText* invalidFilesLabel;
            wxListBox* invalidFilesListBox;
            wxStaticText* directoriesLabel;
            wxTextCtrl* directoriesText;
            wxButton* addButton;
            wxButton* searchButton;
            wxButton* okButton;
            wxButton* cancelButton;

        public:

            FinjinMaterialFileRemappingDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Finjin Material File Error"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,500 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
            ~FinjinMaterialFileRemappingDialog();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class FinjinLogFileDialog
    ///////////////////////////////////////////////////////////////////////////////
    class FinjinLogFileDialog : public wxDialog
    {
        private:

        protected:
            wxTextCtrl* logText;
            wxButton* okButton;

        public:

            FinjinLogFileDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Finjin Log"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,500 ), long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER );
            ~FinjinLogFileDialog();

    };

    ///////////////////////////////////////////////////////////////////////////////
    /// Class FinjinInstallationDetailsDialog
    ///////////////////////////////////////////////////////////////////////////////
    class FinjinInstallationDetailsDialog : public wxDialog
    {
        private:

        protected:
            wxTextCtrl* resultsText;
            wxButton* okButton;

        public:

            FinjinInstallationDetailsDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Finjin Installation Details"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 711,500 ), long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER );
            ~FinjinInstallationDetailsDialog();

    };

} // namespace Finjin

#endif //__DIALOGS_H__
