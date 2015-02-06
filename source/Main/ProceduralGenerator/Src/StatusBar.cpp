#include "StatusBar.h"

namespace ProceduralTextures
{
	StatusBar::StatusBar( wxWindow * p_pParent, wxWindowID p_id, long p_lStyle, const wxString & p_strName )
		:	wxStatusBar( p_pParent, p_id, p_lStyle, p_strName )
	{
		SetBackgroundColour( p_pParent->GetBackgroundColour() );
		SetForegroundColour( p_pParent->GetForegroundColour() );
		wxStatusBar::SetFieldsCount( 2 );
		wxSize l_size = GetClientSize();
		l_size = wxSize( l_size.x / 2, l_size.y );
		m_pTxtCtrls[0] = new wxStaticText( this, wxID_ANY, wxEmptyString, wxPoint( 0, 0 ), l_size, wxALIGN_CENTER_VERTICAL );
		m_pTxtCtrls[0]->SetBackgroundColour( p_pParent->GetBackgroundColour() );
		m_pTxtCtrls[0]->SetForegroundColour( p_pParent->GetForegroundColour() );
		m_pTxtCtrls[1] = new wxStaticText( this, wxID_ANY, wxEmptyString, wxPoint( l_size.x, 0 ), l_size, wxALIGN_CENTER_VERTICAL );
		m_pTxtCtrls[1]->SetBackgroundColour( p_pParent->GetBackgroundColour() );
		m_pTxtCtrls[1]->SetForegroundColour( p_pParent->GetForegroundColour() );
	}

	StatusBar::~StatusBar()
	{
	}

	void StatusBar::SetStatusText( const wxString & p_strText, int p_iNumber )
	{
		if ( p_iNumber >= 0 && p_iNumber < 2 )
		{
			m_pTxtCtrls[p_iNumber]->SetLabel( p_strText );
		}
		else
		{
			wxStatusBar::SetStatusText( p_strText, p_iNumber );
		}
	}

	BEGIN_EVENT_TABLE( StatusBar, wxStatusBar )
		EVT_SIZE(	StatusBar::_onSize )
	END_EVENT_TABLE()

	void StatusBar::_onSize( wxSizeEvent & p_event )
	{
		wxSize l_size = p_event.GetSize();
		l_size = wxSize( l_size.x / 2, l_size.y );
		m_pTxtCtrls[0]->SetSize( 0, 0, l_size.x, l_size.y );
		m_pTxtCtrls[1]->SetSize( l_size.x, 0, l_size.x, l_size.y );
	}
}
