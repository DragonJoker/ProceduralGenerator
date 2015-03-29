#include "ProjectListDialog.h"

#pragma warning( push )
#pragma warning( disable:4996 )
#include <wx/button.h>
#include <wx/statline.h>
#include <wx/sizer.h>
#pragma warning( pop )

#include <GeneratorPrerequisites.h>

#include "RenderPanel.h"

using namespace ProceduralTextures;

namespace ProceduralGenerator
{
	ProjectListDialog::ProjectListDialog( wxWindow * p_parent, wxArrayString const & p_choices )
		: wxDialog( p_parent, wxID_ANY, _( "Choose a generator" ), wxDefaultPosition, wxSize( 400, 20 + DEFAULT_HEIGHT * 6 ) )
		, m_pComboProjets( NULL )
		, m_pStaticResolution( NULL )
		, m_pComboWidth( NULL )
		, m_pComboHeight( NULL )
	{
		wxStaticLine * l_pLine = NULL;
		wxStaticText * l_pDesc = NULL;
		wxButton * l_pOk = NULL;
		wxButton * l_pCancel = NULL;
		SetClientSize( wxSize( 400, 20 + DEFAULT_HEIGHT * 6 ) );
		wxArrayString l_arrayResolutions;
#if !defined( NDEBUG )
		l_arrayResolutions.push_back( wxT( "256" ) );
		l_arrayResolutions.push_back( wxT( "384" ) );
#endif
		l_arrayResolutions.push_back( wxT( "512" ) );
		l_arrayResolutions.push_back( wxT( "768" ) );
		l_arrayResolutions.push_back( wxT( "1024" ) );
		l_arrayResolutions.push_back( wxT( "1080" ) );
		l_arrayResolutions.push_back( wxT( "1152" ) );
		l_arrayResolutions.push_back( wxT( "1280" ) );
		l_arrayResolutions.push_back( wxT( "1920" ) );

		if ( p_choices.size() > 0 )
		{
			l_pDesc = new wxStaticText( this, wxID_ANY, _( "Generator" ) );
			m_pComboProjets = new wxComboBox( this, pldProjects, p_choices[0], wxDefaultPosition, wxDefaultSize, p_choices, wxCB_READONLY );
			m_pStaticResolution = new wxStaticText( this, wxID_ANY, _( "Screen Size" ) );
			m_pComboWidth = new wxComboBox( this, wxID_ANY, l_arrayResolutions[0], wxDefaultPosition, wxDefaultSize, l_arrayResolutions, wxCB_READONLY );
			m_pComboHeight = new wxComboBox( this, wxID_ANY, l_arrayResolutions[0],	wxDefaultPosition, wxDefaultSize, l_arrayResolutions, wxCB_READONLY );
		}

		l_pLine = new wxStaticLine( this, wxID_ANY );
		l_pOk = new wxButton( this, pldOK, _( "OK" ) );
		l_pCancel = new wxButton( this, pldCancel, _( "Cancel" ) );
		wxBoxSizer * l_pSizer = new wxBoxSizer( wxVERTICAL );
		wxBoxSizer * l_pSizerDim = new wxBoxSizer( wxHORIZONTAL );
		wxBoxSizer * l_pSizerBtn = new wxBoxSizer( wxHORIZONTAL );

		if ( m_pComboWidth )
		{
			l_pSizerDim->Add( m_pComboWidth, wxSizerFlags( 0 ) );
		}

		if ( m_pComboHeight )
		{
			l_pSizerDim->Add( m_pComboHeight, wxSizerFlags( 0 ) );
		}

		l_pSizerBtn->Add( l_pOk, wxSizerFlags( 0 ) );
#if wxCHECK_VERSION( 2, 8, 12 )
		l_pSizerBtn->AddSpacer(	100 );
#else
		l_pSizerBtn->Add( 100, 0, wxSizerFlags( 1 ) );
#endif
		l_pSizerBtn->Add( l_pCancel, wxSizerFlags( 0 ) );

		if ( l_pDesc )
		{
			l_pSizer->Add( l_pDesc, wxSizerFlags( 0 ).Expand().Border( wxALL, 5 ) );
		}

		if ( m_pComboProjets )
		{
			l_pSizer->Add( m_pComboProjets, wxSizerFlags( 0 ).Expand().Border( wxALL, 5 ) );
		}

		if ( m_pStaticResolution )
		{
			l_pSizer->Add( m_pStaticResolution, wxSizerFlags( 0 ).Expand().Border( wxALL, 5 ) );
		}

		l_pSizer->Add(	l_pSizerDim, wxSizerFlags( 0 ).Expand().Border( wxALL, 5 ) );
		l_pSizer->Add(	l_pLine, wxSizerFlags( 0 ).Expand().Border( wxALL, 5 ) );
		l_pSizer->Add(	l_pSizerBtn, wxSizerFlags( 0 ).Expand().Border( wxALL, 5 ) );
		SetSizer( l_pSizer );
		l_pSizer->SetSizeHints( this );
	}

	ProjectListDialog::~ProjectListDialog()
	{
	}

	wxString ProjectListDialog::GetProject()
	{
		return m_pComboProjets->GetValue();
	}

	wxSize ProjectListDialog::GetResolution()
	{
		wxSize l_sizeReturn;
		int l_iWidth = atoi( m_pComboWidth->GetValue().char_str() );
		int l_iHeight = atoi( m_pComboHeight->GetValue().char_str() );
		l_sizeReturn.Set( l_iWidth, l_iHeight );
		return l_sizeReturn;
	}

	BEGIN_EVENT_TABLE( ProjectListDialog, wxDialog )
		EVT_CLOSE(	ProjectListDialog::_onClose )
		EVT_BUTTON(	pldOK,			ProjectListDialog::_onOK )
		EVT_BUTTON(	pldCancel,		ProjectListDialog::_onCancel )
		EVT_COMBOBOX(	pldProjects,	ProjectListDialog::_onProjects )
	END_EVENT_TABLE()

	void ProjectListDialog::_onClose( wxCloseEvent & /*p_event*/ )
	{
		EndModal( wxID_CANCEL );
	}

	void ProjectListDialog::_onOK( wxCommandEvent & /*p_event*/ )
	{
		EndModal( wxID_OK );
	}

	void ProjectListDialog::_onCancel( wxCommandEvent & /*p_event*/ )
	{
		EndModal( wxID_CANCEL );
	}

	void ProjectListDialog::_onProjects( wxCommandEvent & p_event )
	{
		wxString l_project = p_event.GetString();

		if ( l_project.find( _( "Image Effects" ) ) != wxString::npos
				|| l_project.find( _( "Shader Effects" ) ) != wxString::npos
				|| l_project.find( _( "Text Effects" ) ) != wxString::npos
				|| l_project.find( _( "Shader Webcam" ) ) != wxString::npos )
		{
			m_pComboHeight->Hide();
			m_pComboWidth->Hide();
			m_pStaticResolution->Hide();
		}
		else
		{
			m_pComboHeight->Show();
			m_pComboWidth->Show();
			m_pStaticResolution->Show();
		}
	}
}
