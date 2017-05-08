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
#include "ObjectSettingsDialog_SubmeshesPage.hpp"
#include "ApplicationControls.hpp"
#include "ApplicationAccessor.hpp"
#include "StringUtilities.hpp"
#include "Strings.hpp"
#include "Dialogs.hpp"
#include "SuspendCustomControlsHook.hpp"
#include "GeometryState.hpp"
#include "PropertyValues.hpp"

#if defined(__WXMAC__)
    #define CONTROL_BUTTON_SIZE wxSize(STANDARD_BUTTON_WIDTH, -1)
#else
    #define CONTROL_BUTTON_SIZE wxDefaultSize
#endif

using namespace Finjin::Exporter;


//Static initialization---------------------------------------------------------
const wxString ObjectSettingsDialog_SubmeshesPage::TITLE(wxT("Submeshes"));


//Implementation----------------------------------------------------------------
BEGIN_EVENT_TABLE(ObjectSettingsDialog_SubmeshesPage, SettingsPage)
    EVT_CHECKBOX(ObjectSettingsDialog_SubmeshesPage::CUSTOMIZE_SUBMESHES_CHECKBOX, ObjectSettingsDialog_SubmeshesPage::OnCustomizeSubmeshesCheckBox)
    EVT_CHOICE(ObjectSettingsDialog_SubmeshesPage::SUBMESH_TEXTURE_COORDINATE_SETS_CHOICE, ObjectSettingsDialog_SubmeshesPage::OnSubmeshTextureCoordinateSetsChoice)
    EVT_BUTTON(ObjectSettingsDialog_SubmeshesPage::REFRESH_TEXTURE_COORDINATE_SETS_BUTTON, ObjectSettingsDialog_SubmeshesPage::OnRefreshTextureCoordinateSetsButton)
    EVT_BUTTON(ObjectSettingsDialog_SubmeshesPage::CLEAR_TEXTURE_COORDINATE_SETS_BUTTON, ObjectSettingsDialog_SubmeshesPage::OnClearTextureCoordinateSetsButton)
    EVT_TEXT(ObjectSettingsDialog_SubmeshesPage::SUBMESH_NAME_TEXT, ObjectSettingsDialog_SubmeshesPage::OnSubmeshNameText)
    EVT_TEXT(ObjectSettingsDialog_SubmeshesPage::SUBMESH_NAME_TEXT, ObjectSettingsDialog_SubmeshesPage::OnRenderQueueText)
    EVT_SPINNER_VALUE_CHANGED(ObjectSettingsDialog_SubmeshesPage::RENDER_QUEUE_PRIORITY_SPINNER, ObjectSettingsDialog_SubmeshesPage::OnRenderQueuePriorityValueChanged)
    EVT_BUTTON(ObjectSettingsDialog_SubmeshesPage::MOVE_TEXTURE_COORDINATE_SET_UP_BUTTON, ObjectSettingsDialog_SubmeshesPage::OnMoveTextureCoordinateSetUpButton)
    EVT_BUTTON(ObjectSettingsDialog_SubmeshesPage::MOVE_TEXTURE_COORDINATE_SET_DOWN_BUTTON, ObjectSettingsDialog_SubmeshesPage::OnMoveTextureCoordinateSetDownButton)
    EVT_BUTTON(ObjectSettingsDialog_SubmeshesPage::REMOVE_TEXTURE_COORDINATE_SET_DOWN_BUTTON, ObjectSettingsDialog_SubmeshesPage::OnRemoveTextureCoordinateSetButton)
    EVT_BUTTON(ObjectSettingsDialog_SubmeshesPage::TEXTURE_COORDINATE_SET_U_BUTTON, ObjectSettingsDialog_SubmeshesPage::OnTextureCoordinateSetUButton)
    EVT_BUTTON(ObjectSettingsDialog_SubmeshesPage::TEXTURE_COORDINATE_SET_UV_BUTTON, ObjectSettingsDialog_SubmeshesPage::OnTextureCoordinateSetUVButton)
    EVT_BUTTON(ObjectSettingsDialog_SubmeshesPage::TEXTURE_COORDINATE_SET_UVW_BUTTON, ObjectSettingsDialog_SubmeshesPage::OnTextureCoordinateSetUVWButton)
END_EVENT_TABLE()

ObjectSettingsDialog_SubmeshesPage::ObjectSettingsDialog_SubmeshesPage(wxWindow* parent, ObjectAccessor object, FinjinObjectSettingsAccessor objectSettings, FinjinSceneSettingsAccessor sceneSettings, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : SettingsPage( parent, id, pos, size, style )
{
    this->object = object;
    this->objectSettings = objectSettings;
    GeometryStateFlags flags = GeometryStateFlags::NONE;
    if (!sceneSettings.GetCreateExtraMaterials())
        flags |= GeometryStateFlags::NO_EXTRA_MATERIALS;
    GeometryState::GetSubmeshesSettings(this->object, this->detectedSubmeshesTextureCoordinateSets, flags);

    wxBoxSizer* topSizer;
    topSizer = new wxBoxSizer(wxVERTICAL);

    wxGridSizer* headerSizer;
    headerSizer = new wxGridSizer( 1, 2, 0, 0 );

    useCustomTextureCoordinateCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Customize"), wxDefaultPosition, wxDefaultSize, 0 );

    headerSizer->Add( useCustomTextureCoordinateCheckbox, 0, wxALL|wxTOP, 5 );

    topSizer->Add( headerSizer, 0, wxEXPAND, 5 );


    wxStaticBoxSizer* textureCoordinateSetsSizer;
    textureCoordinateSetsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Submeshes/Subentities") ), wxVERTICAL );

    wxFlexGridSizer* textureCoordinateSetsGridSizer;
    textureCoordinateSetsGridSizer = new wxFlexGridSizer( 5, 3, 0, 0 );
    textureCoordinateSetsGridSizer->SetFlexibleDirection( wxBOTH );
    textureCoordinateSetsGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    textureCoordinateSetsGridSizer->AddGrowableCol(2);


    textureCoordinateSetsGridSizer->Add( 0, 0, 0, wxEXPAND, 5 );

    activeTextureCoordinateSetLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Submesh/Material"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
    activeTextureCoordinateSetLabel->Wrap( -1 );
    textureCoordinateSetsGridSizer->Add( activeTextureCoordinateSetLabel, 0, wxALL, 5 );

    wxArrayString submeshTextureCoordinateSetChoiceChoices;
    submeshTextureCoordinateSetChoice = new ApplicationChoiceCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), submeshTextureCoordinateSetChoiceChoices, 0 );
    submeshTextureCoordinateSetChoice->SetSelection( 0 );
    submeshTextureCoordinateSetChoice->SetMinSize( wxSize( 140,-1 ) );

    textureCoordinateSetsGridSizer->Add( submeshTextureCoordinateSetChoice, 0, wxLEFT|wxEXPAND, 5 );


    textureCoordinateSetsGridSizer->Add( 0, 0, 0, wxEXPAND, 5 );

    submeshNameLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Submesh Name"), wxDefaultPosition, wxDefaultSize, 0 );
    submeshNameLabel->Wrap( -1 );
    textureCoordinateSetsGridSizer->Add( submeshNameLabel, 0, wxALL, 5 );

    submeshNameText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 140,-1 ), 0 );
    textureCoordinateSetsGridSizer->Add( submeshNameText, 0, wxLEFT|wxRIGHT, 5 );


    textureCoordinateSetsGridSizer->Add( 0, 0, 1, wxEXPAND, 5 );

    renderQueueLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Render Queue"), wxDefaultPosition, wxDefaultSize, 0 );
    renderQueueLabel->Wrap( -1 );
    textureCoordinateSetsGridSizer->Add( renderQueueLabel, 0, wxALL, 5 );

    wxBoxSizer* renderQueueSizer;
    renderQueueSizer = new wxBoxSizer( wxHORIZONTAL );

    renderQueueText = new ApplicationTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 140,-1 ), 0 );
    renderQueueText->SetMaxLength( 0 );
    renderQueueSizer->Add( renderQueueText, 0, wxRIGHT|wxLEFT, 5 );

    renderQueuePriorityLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Priority"), wxDefaultPosition, wxDefaultSize, 0 );
    renderQueuePriorityLabel->Wrap( -1 );
    renderQueueSizer->Add( renderQueuePriorityLabel, 0, wxALL, 5 );

    renderQueuePrioritySpinner = new SpinnerControl(this, wxID_ANY, wxID_ANY, wxDefaultPosition, wxSize(50, -1), 0, 0, 65535, SpinnerControl::INT_UNIT);
    renderQueueSizer->Add( renderQueuePrioritySpinner, 0, wxRIGHT|wxLEFT, 5 );

    textureCoordinateSetsGridSizer->Add( renderQueueSizer, 1, wxEXPAND, 5 );

    textureCoordinateSetsGridSizer->Add( 0, 0, 1, wxEXPAND, 5 );


    textureCoordinateSetsLabel = new ApplicationStaticTextCtrl( this, wxID_ANY, wxT("Texture Coordinate Sets"), wxDefaultPosition, wxDefaultSize, 0 );
    textureCoordinateSetsLabel->Wrap( -1 );
    textureCoordinateSetsGridSizer->Add( textureCoordinateSetsLabel, 0, wxALL, 5 );

    wxBoxSizer* refreshClearButtonsSizer;
    refreshClearButtonsSizer = new wxBoxSizer( wxHORIZONTAL );

    refreshTextureCoordinateSetsButton = new wxButton( this, wxID_ANY, wxT("Refresh"), wxDefaultPosition, wxSize( 70,-1 ), 0 );
    refreshClearButtonsSizer->Add( refreshTextureCoordinateSetsButton, 0, wxLEFT, 5 );

    clearTextureCoordinateSetsButton = new wxButton( this, wxID_ANY, wxT("Clear"), wxDefaultPosition, wxSize( 70,-1 ), 0 );
    refreshClearButtonsSizer->Add( clearTextureCoordinateSetsButton, 0, 0, 5 );

    textureCoordinateSetsGridSizer->Add( refreshClearButtonsSizer, 1, wxEXPAND, 5 );
    textureCoordinateSetsSizer->Add( textureCoordinateSetsGridSizer, 0, wxEXPAND, 5 );

    wxBoxSizer* textureCoordinateSetsListSizer;
    textureCoordinateSetsListSizer = new wxBoxSizer( wxHORIZONTAL );

    textureCoordinateSetsListSizer->Add( 5, 0, 0, wxEXPAND, 5 );

    textureCoordinateSetsList = new ApplicationListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_NO_SORT_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VRULES );
    textureCoordinateSetsListSizer->Add( textureCoordinateSetsList, 1, wxBOTTOM|wxEXPAND, 5 );

    wxBoxSizer* textureCoordinateSetsButtonsSizer;
    textureCoordinateSetsButtonsSizer = new wxBoxSizer( wxVERTICAL );

    moveTextureCoordinateSetUpButton = new wxButton( this, wxID_ANY, wxT("Move Up"), wxDefaultPosition, CONTROL_BUTTON_SIZE, 0 );
    textureCoordinateSetsButtonsSizer->Add( moveTextureCoordinateSetUpButton, 0, wxLEFT, 5 );

    moveTextureCoordinateSetDownButton = new wxButton( this, wxID_ANY, wxT("Move Down"), wxDefaultPosition, CONTROL_BUTTON_SIZE, 0 );
    textureCoordinateSetsButtonsSizer->Add( moveTextureCoordinateSetDownButton, 0, wxLEFT, 5 );

    removeTextureCoordinateSetButton = new wxButton( this, wxID_ANY, wxT("Remove"), wxDefaultPosition, CONTROL_BUTTON_SIZE, 0 );
    textureCoordinateSetsButtonsSizer->Add( removeTextureCoordinateSetButton, 0, wxLEFT|wxTOP, 5 );

    textureCoordinateSetUButton = new wxButton( this, wxID_ANY, wxT("U (1D)"), wxDefaultPosition, CONTROL_BUTTON_SIZE, 0 );
    textureCoordinateSetsButtonsSizer->Add( textureCoordinateSetUButton, 0, wxLEFT|wxTOP, 5 );

    textureCoordinateSetUVButton = new wxButton( this, wxID_ANY, wxT("UV (2D)"), wxDefaultPosition, CONTROL_BUTTON_SIZE, 0 );
    textureCoordinateSetsButtonsSizer->Add( textureCoordinateSetUVButton, 0, wxLEFT, 5 );

    textureCoordinateSetUVWButton = new wxButton( this, wxID_ANY, wxT("UVW (3D)"), wxDefaultPosition, CONTROL_BUTTON_SIZE, 0 );
    textureCoordinateSetsButtonsSizer->Add( textureCoordinateSetUVWButton, 0, wxLEFT, 5 );

    textureCoordinateSetsListSizer->Add( textureCoordinateSetsButtonsSizer, 0, wxEXPAND, 5 );

    textureCoordinateSetsSizer->Add( textureCoordinateSetsListSizer, 1, wxEXPAND, 5 );

    topSizer->Add( textureCoordinateSetsSizer, 1, wxEXPAND|wxTOP, 5 );

    SetSizer(topSizer);

    this->useCustomTextureCoordinateCheckbox->SetId(CUSTOMIZE_SUBMESHES_CHECKBOX);
    this->submeshTextureCoordinateSetChoice->SetId(SUBMESH_TEXTURE_COORDINATE_SETS_CHOICE);
    this->refreshTextureCoordinateSetsButton->SetId(REFRESH_TEXTURE_COORDINATE_SETS_BUTTON);
    this->clearTextureCoordinateSetsButton->SetId(CLEAR_TEXTURE_COORDINATE_SETS_BUTTON);
    this->submeshNameText->SetId(SUBMESH_NAME_TEXT);
    this->renderQueueText->SetId(RENDER_QUEUE_TEXT);
    this->renderQueuePrioritySpinner->SetId(RENDER_QUEUE_PRIORITY_SPINNER);
    this->moveTextureCoordinateSetUpButton->SetId(MOVE_TEXTURE_COORDINATE_SET_UP_BUTTON);
    this->moveTextureCoordinateSetDownButton->SetId(MOVE_TEXTURE_COORDINATE_SET_DOWN_BUTTON);
    this->removeTextureCoordinateSetButton->SetId(REMOVE_TEXTURE_COORDINATE_SET_DOWN_BUTTON);
    this->textureCoordinateSetUButton->SetId(TEXTURE_COORDINATE_SET_U_BUTTON);
    this->textureCoordinateSetUVButton->SetId(TEXTURE_COORDINATE_SET_UV_BUTTON);
    this->textureCoordinateSetUVWButton->SetId(TEXTURE_COORDINATE_SET_UVW_BUTTON);

    this->textureCoordinateSetUVWButton->Show(ApplicationAccessor::Supports3DTextureCoordinates());

    Layout();
}

ObjectSettingsDialog_SubmeshesPage::~ObjectSettingsDialog_SubmeshesPage()
{
}

bool ObjectSettingsDialog_SubmeshesPage::GetGUIData()
{
    //Customize texture coordinate sets?
    auto useCustomSubmeshes = this->useCustomTextureCoordinateCheckbox->GetValue();
    this->objectSettings.SetUseCustomSubmeshes(useCustomSubmeshes);

    //Texture coordinate sets list
    //if (useCustomSubmeshes)
        this->objectSettings.SetSubmeshesSettings(this->submeshes);
    /*else
    {
        SubmeshesSettings nullSubmeshesSettings;
        this->objectSettings.SetSubmeshesSettings(nullSubmeshesSettings);
    }*/

    return true;
}

bool ObjectSettingsDialog_SubmeshesPage::SetGUIData()
{
    //Customize texture coordinate sets?
    this->useCustomTextureCoordinateCheckbox->SetValue(this->objectSettings.GetUseCustomSubmeshes());

    //Submeshes----------------------------------------------------------------------
    this->objectSettings.GetSubmeshesSettings(this->submeshes);
    this->submeshes.SyncTo(this->detectedSubmeshesTextureCoordinateSets);//, this->objectSettings.GetUseCustomSubmeshes());
    this->submeshTextureCoordinateSetChoice->Freeze();
    this->submeshTextureCoordinateSetChoice->Append(Strings::ALL);
    for (int i = 0; i < (int)this->submeshes.size(); i++)
    {
        wxString materialName = this->submeshes[i].material.GetLocalName();
        if (materialName.empty())
            materialName = Strings::NO_MATERIAL;
        this->submeshTextureCoordinateSetChoice->Append(materialName);
        this->submeshTextureCoordinateSetChoice->SetClientData(i + 1, &this->submeshes[i].material);
    }
    this->submeshTextureCoordinateSetChoice->Thaw();
    if (this->submeshes.AllSubmeshMappingsSame())
        this->submeshTextureCoordinateSetChoice->SetSelection(0);
    else
        this->submeshTextureCoordinateSetChoice->SetSelection(1);

    //Texture coordinate sets list
    this->textureCoordinateSetsList->InsertColumn(0, wxT("Source ") + ApplicationAccessor::TEXTURE_COORDINATE_SET_NAME, wxLIST_FORMAT_LEFT, 225);
    this->textureCoordinateSetsList->InsertColumn(1, wxT("Destination Number"), wxLIST_FORMAT_LEFT, 120);
    this->textureCoordinateSetsList->InsertColumn(2, wxT("Type"), wxLIST_FORMAT_LEFT, 65);
    UpdateSubmeshControls();

    return true;
}

void ObjectSettingsDialog_SubmeshesPage::OnCustomizeSubmeshesCheckBox(wxCommandEvent& event)
{
    UpdateSubmeshControls();
}

void ObjectSettingsDialog_SubmeshesPage::OnSubmeshTextureCoordinateSetsChoice(wxCommandEvent& event)
{
    int submeshSelection = this->submeshTextureCoordinateSetChoice->GetSelection();
    if (submeshSelection == 0)
    {
        if (!this->submeshes.AllSubmeshMappingsSame())
        {
            Dialogs::ShowMessage(Strings::SUBMESH_TEXTURE_COORDINATE_SETS_NOT_SAME_FOR_EDIT, Strings::FINJIN_ERROR, wxOK | wxICON_EXCLAMATION);
            this->submeshTextureCoordinateSetChoice->SetSelection(1);
        }
    }

    UpdateSubmeshControls();
}

void ObjectSettingsDialog_SubmeshesPage::OnRefreshTextureCoordinateSetsButton(wxCommandEvent& event)
{
    int submeshSelection = this->submeshTextureCoordinateSetChoice->GetSelection();
    if (submeshSelection == 0)
    {
        for (size_t i = 0; i < this->submeshes.size(); i++)
            this->submeshes[i].textureCoordinateSetMappings = this->detectedSubmeshesTextureCoordinateSets[i].textureCoordinateSetMappings;
    }
    else
    {
        submeshSelection--;
        this->submeshes[submeshSelection].textureCoordinateSetMappings = this->detectedSubmeshesTextureCoordinateSets[submeshSelection].textureCoordinateSetMappings;
    }

    UpdateSubmeshControls();
}

void ObjectSettingsDialog_SubmeshesPage::OnClearTextureCoordinateSetsButton(wxCommandEvent& event)
{
    for (size_t i = 0; i < this->submeshes.size(); i++)
        this->submeshes[i].textureCoordinateSetMappings.clear();
    UpdateSubmeshControls();
}

void ObjectSettingsDialog_SubmeshesPage::OnSubmeshNameText(wxCommandEvent& event)
{
    int submeshSelection = this->submeshTextureCoordinateSetChoice->GetSelection();
    if (submeshSelection > 0)
    {
        submeshSelection--;
        this->submeshes[submeshSelection].submeshName = this->submeshNameText->GetValue();
    }
}

void ObjectSettingsDialog_SubmeshesPage::OnRenderQueueText(wxCommandEvent& event)
{
    int submeshSelection = this->submeshTextureCoordinateSetChoice->GetSelection();
    if (submeshSelection > 0)
    {
        submeshSelection--;
        this->submeshes[submeshSelection].renderQueueName = this->renderQueueText->GetValue();
    }
}

void ObjectSettingsDialog_SubmeshesPage::OnRenderQueuePriorityValueChanged(SpinnerControlEvent& event)
{
    int submeshSelection = this->submeshTextureCoordinateSetChoice->GetSelection();
    if (submeshSelection > 0)
    {
        submeshSelection--;
        this->submeshes[submeshSelection].renderPriority = this->renderQueuePrioritySpinner->GetIntValue();
    }
}

void ObjectSettingsDialog_SubmeshesPage::OnMoveTextureCoordinateSetUpButton(wxCommandEvent& event)
{
    MoveTextureCoordinateSet(-1);
}

void ObjectSettingsDialog_SubmeshesPage::OnMoveTextureCoordinateSetDownButton(wxCommandEvent& event)
{
    MoveTextureCoordinateSet(1);
}

void ObjectSettingsDialog_SubmeshesPage::OnRemoveTextureCoordinateSetButton(wxCommandEvent& event)
{
    int textureCoordinateSetSelection = this->textureCoordinateSetsList->GetSelection();
    if (textureCoordinateSetSelection >= 0)
    {
        int submeshSelection = this->submeshTextureCoordinateSetChoice->GetSelection();
        if (submeshSelection == 0)
        {
            for (size_t i = 0; i < this->submeshes.size(); i++)
                this->submeshes[i].textureCoordinateSetMappings.Remove(textureCoordinateSetSelection);
        }
        else
        {
            submeshSelection--;
            this->submeshes[submeshSelection].textureCoordinateSetMappings.Remove(textureCoordinateSetSelection);
        }

        UpdateSubmeshControls();
    }
}

void ObjectSettingsDialog_SubmeshesPage::OnTextureCoordinateSetUButton(wxCommandEvent& event)
{
    SetTextureCoordinateType(TextureCoordinateFlags::U_TEXTURE_COORDS);
}

void ObjectSettingsDialog_SubmeshesPage::OnTextureCoordinateSetUVButton(wxCommandEvent& event)
{
    SetTextureCoordinateType(TextureCoordinateFlags::U_TEXTURE_COORDS | TextureCoordinateFlags::V_TEXTURE_COORDS);
}

void ObjectSettingsDialog_SubmeshesPage::OnTextureCoordinateSetUVWButton(wxCommandEvent& event)
{
    SetTextureCoordinateType(TextureCoordinateFlags::U_TEXTURE_COORDS | TextureCoordinateFlags::V_TEXTURE_COORDS | TextureCoordinateFlags::W_TEXTURE_COORDS);
}

void ObjectSettingsDialog_SubmeshesPage::UpdateSubmeshControls()
{
    bool enable = this->useCustomTextureCoordinateCheckbox->GetValue();

    this->activeTextureCoordinateSetLabel->Enable(enable);
    this->submeshTextureCoordinateSetChoice->Enable(enable);
    this->submeshNameLabel->Enable(enable);
    this->submeshNameText->Enable(enable);
    this->renderQueueLabel->Enable(enable);
    this->renderQueueText->Enable(enable);
    this->renderQueuePrioritySpinner->Enable(enable);
    this->renderQueuePriorityLabel->Enable(enable);
    this->textureCoordinateSetsLabel->Enable(enable);
    this->refreshTextureCoordinateSetsButton->Enable(enable);
    this->clearTextureCoordinateSetsButton->Enable(enable);
    this->textureCoordinateSetsList->Enable(enable);
    this->moveTextureCoordinateSetUpButton->Enable(enable);
    this->moveTextureCoordinateSetDownButton->Enable(enable);
    this->removeTextureCoordinateSetButton->Enable(enable);
    this->textureCoordinateSetUButton->Enable(enable);
    this->textureCoordinateSetUVButton->Enable(enable);
    this->textureCoordinateSetUVWButton->Enable(enable);

    if (enable)
    {
        wxString allSubmeshesNameText;
        for (size_t i = 0; i < this->submeshes.size(); i++)
        {
            if (i > 0)
                allSubmeshesNameText += wxT(",");
            allSubmeshesNameText += this->submeshes[i].submeshName;
        }

        bool isSubmeshSelected = false;
        int submeshSelection = this->submeshTextureCoordinateSetChoice->GetSelection();
        if (submeshSelection > 0)
        {
            submeshSelection--;
            isSubmeshSelected = true;
        }

        if (submeshSelection < (int)this->submeshes.size())
        {
            int textureCoordinateSetSelection = this->textureCoordinateSetsList->GetSelection();
            if (textureCoordinateSetSelection < 0)
                textureCoordinateSetSelection = 0;

            this->submeshNameText->SetValue(isSubmeshSelected ? this->submeshes[submeshSelection].submeshName : allSubmeshesNameText);
            this->submeshNameLabel->Enable(isSubmeshSelected);
            this->submeshNameText->Enable(isSubmeshSelected);

            this->renderQueueLabel->Enable(isSubmeshSelected);

            this->renderQueueText->Enable(isSubmeshSelected);
            this->renderQueueText->SetValue(this->submeshes[submeshSelection].renderQueueName);

            this->renderQueuePriorityLabel->Enable(isSubmeshSelected);
            this->renderQueuePrioritySpinner->Enable(isSubmeshSelected);

            this->renderQueuePriorityLabel->Enable(isSubmeshSelected);
            this->renderQueuePrioritySpinner->Enable(isSubmeshSelected);
            this->renderQueuePrioritySpinner->SetValue(isSubmeshSelected ? this->submeshes[submeshSelection].renderPriority : 0);

            this->textureCoordinateSetsList->DeleteAllItems();

            for (size_t i = 0; i < this->submeshes[submeshSelection].textureCoordinateSetMappings.size(); i++)
            {
                this->textureCoordinateSetsList->InsertItem((int)i, wxEmptyString);
                UpdateTextureCoordinateSetGUIData((int)i);
            }

            if (textureCoordinateSetSelection >= (int)this->submeshes[submeshSelection].textureCoordinateSetMappings.size())
                textureCoordinateSetSelection = (int)this->submeshes[submeshSelection].textureCoordinateSetMappings.size() - 1;
            if (textureCoordinateSetSelection >= 0)
                this->textureCoordinateSetsList->SetSelection(textureCoordinateSetSelection);
        }
    }
    else
        this->textureCoordinateSetsList->DeleteAllItems();
}

void ObjectSettingsDialog_SubmeshesPage::MoveTextureCoordinateSet(int direction)
{
    int from = this->textureCoordinateSetsList->GetSelection();
    if (from >= 0)
    {
        int submeshSelection = this->submeshTextureCoordinateSetChoice->GetSelection();
        int to = 0;
        if (submeshSelection == 0)
        {
            for (size_t i = 0; i < this->submeshes.size(); i++)
                to = this->submeshes[i].textureCoordinateSetMappings.Move(from, direction);
            if (to >= 0)
            {
                UpdateTextureCoordinateSetGUIData(from);
                UpdateTextureCoordinateSetGUIData(to);
                this->textureCoordinateSetsList->SetSelection(to);
            }
        }
        else
        {
            submeshSelection--;
            to = this->submeshes[submeshSelection].textureCoordinateSetMappings.Move(from, direction);
        }

        if (to >= 0)
        {
            UpdateTextureCoordinateSetGUIData(from);
            UpdateTextureCoordinateSetGUIData(to);
            this->textureCoordinateSetsList->SetSelection(to);
        }
    }
}

void ObjectSettingsDialog_SubmeshesPage::SetTextureCoordinateType(TextureCoordinateFlags flags)
{
    int textureCoordinateSetSelection = this->textureCoordinateSetsList->GetSelection();
    if (textureCoordinateSetSelection >= 0)
    {
        int submeshSelection = this->submeshTextureCoordinateSetChoice->GetSelection();
        if (submeshSelection == 0)
        {
            for (size_t i = 0; i < this->submeshes.size(); i++)
                this->submeshes[i].textureCoordinateSetMappings[textureCoordinateSetSelection].textureCoordinatesFlags = flags;
        }
        else
        {
            submeshSelection--;
            this->submeshes[submeshSelection].textureCoordinateSetMappings[textureCoordinateSetSelection].textureCoordinatesFlags = flags;
        }

        UpdateTextureCoordinateSetGUIData(textureCoordinateSetSelection);
    }
}

void ObjectSettingsDialog_SubmeshesPage::UpdateTextureCoordinateSetGUIData(int i)
{
    int submeshSelection = this->submeshTextureCoordinateSetChoice->GetSelection();
    if (submeshSelection > 0)
        submeshSelection--;
    if (submeshSelection < (int)this->submeshes.size())
    {
        auto& set = this->submeshes[submeshSelection].textureCoordinateSetMappings[i];

        this->textureCoordinateSetsList->SetItem(i, 0, set.source.ToString());
        this->textureCoordinateSetsList->SetItem(i, 1, StringUtilities::ToString(set.destinationIndex + 1));

        wxString type;
        if (AnySet(set.textureCoordinatesFlags & TextureCoordinateFlags::U_TEXTURE_COORDS))
        {
            type += wxT("u");
            if (AnySet(set.textureCoordinatesFlags & TextureCoordinateFlags::V_TEXTURE_COORDS))
            {
                type += wxT("v");
                if (AnySet(set.textureCoordinatesFlags & TextureCoordinateFlags::W_TEXTURE_COORDS))
                    type += wxT("w");
            }
        }
        if (type.empty())
            type = wxT("uv");
        this->textureCoordinateSetsList->SetItem(i, 2, type);
    }
}
