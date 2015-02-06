/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/ )

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option ) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___GENERATOR_CONFIG_PANEL_H___
#define ___GENERATOR_CONFIG_PANEL_H___

#include "SpecificControlParameters.h"
#include "ProceduralGenerator.h"

namespace ProceduralTextures
{
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\brief		The description of a panel describing options available with a generator.
	\remark		Each generator that needs a config panel must derive this class
				This file is header only because of Windows DLLs
	*/
	class ConfigPanel
		: public wxPanel
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_arrayTypes	The array of wxEventType, used to make the link between wxEventType and eEVENT_TYPE
		 *\param[in]	p_pGenerator	The generator
		 *\param[in]	p_id			The panel ID
		 *\param[in]	p_ptPosition	The panel position
		 *\param[in]	p_size			The panel dimensions
		 */
		ConfigPanel( wxEventType const( &p_arrayTypes )[eEVENT_TYPE_COUNT], ProceduralGenerator * p_pGenerator, wxWindow * p_pWindow, wxWindowID p_id, const wxPoint & p_ptPosition = wxDefaultPosition, const wxSize & p_size = wxDefaultSize )
			:   wxPanel( p_pWindow, p_id, p_ptPosition, p_size )
			,	m_pGenerator( p_pGenerator )
		{
			for ( int i = 0 ; i < eEVENT_TYPE_COUNT ; i++ )
			{
				m_arrayTypes[i] = p_arrayTypes[i];
			}

			SetBackgroundColour( p_pWindow->GetBackgroundColour() );
			SetForegroundColour( p_pWindow->GetForegroundColour() );
			CreateSpecifics();
		}
		/**
		 *\brief		Destructor
		 */
		virtual ~ConfigPanel()
		{
		}
		/**
		 *\brief		Function used to effectively create the panel's controls
		 */
		void CreateSpecifics()
		{
			std::for_each( m_pGenerator->BeginCtrls(), m_pGenerator->EndCtrls(), [&]( ControlInfos & p_ctrlInfos )
			{
				switch ( p_ctrlInfos.m_eType )
				{
				case eCONTROL_TYPE_STATIC:
					DoCreateStatic( p_ctrlInfos );
					break;

				case eCONTROL_TYPE_EDIT:
					DoCreateEdit( p_ctrlInfos );
					break;

				case eCONTROL_TYPE_SLIDER:
					DoCreateSlider( p_ctrlInfos );
					break;

				case eCONTROL_TYPE_COMBO:
					DoCreateCombo( p_ctrlInfos );
					break;

				case eCONTROL_TYPE_BUTTON:
					DoCreateButton( p_ctrlInfos );
					break;

				case eCONTROL_TYPE_FILE_BUTTON:
					DoCreateFileButton( p_ctrlInfos );
					break;
				}
			} );
		}

	private:
		void DoCreateStatic( ControlInfos & p_ctrlInfos )
		{
			wxString l_strText;
			SpecificControlParameters< eCONTROL_TYPE_STATIC > * l_pSpec = NULL;

			if ( p_ctrlInfos.m_pSpecific && p_ctrlInfos.m_pSpecific->m_eCtrlType == eCONTROL_TYPE_STATIC )
			{
				l_pSpec = static_cast< SpecificControlParameters< eCONTROL_TYPE_STATIC > * >( p_ctrlInfos.m_pSpecific );
				l_strText = l_pSpec->m_strText;
				l_pSpec->m_pControl = new wxStaticText( this, p_ctrlInfos.m_iId, l_strText, p_ctrlInfos.m_ptPosition, p_ctrlInfos.m_szSize, p_ctrlInfos.m_iStyle );
				l_pSpec->m_pControl->SetForegroundColour( *wxWHITE );

				if ( p_ctrlInfos.m_pfnFunction )
				{
					l_pSpec->m_pControl->Connect( p_ctrlInfos.m_iId, m_arrayTypes[p_ctrlInfos.m_eEventType], p_ctrlInfos.m_pfnFunction, NULL, m_pGenerator );
				}

				if ( ! p_ctrlInfos.m_bVisible )
				{
					l_pSpec->m_pControl->Hide();
				}
			}
		}
		void DoCreateEdit( ControlInfos & p_ctrlInfos )
		{
			wxString l_strText;
			SpecificControlParameters< eCONTROL_TYPE_EDIT > * l_pSpec = NULL;

			if ( p_ctrlInfos.m_pSpecific && p_ctrlInfos.m_pSpecific->m_eCtrlType == eCONTROL_TYPE_EDIT )
			{
				l_pSpec = static_cast< SpecificControlParameters< eCONTROL_TYPE_EDIT > * >( p_ctrlInfos.m_pSpecific );
				l_strText = l_pSpec->m_strText;
				l_pSpec->m_pControl = new wxTextCtrl( this, p_ctrlInfos.m_iId, l_strText, p_ctrlInfos.m_ptPosition, p_ctrlInfos.m_szSize, p_ctrlInfos.m_iStyle );

				if ( p_ctrlInfos.m_pfnFunction )
				{
					l_pSpec->m_pControl->Connect( p_ctrlInfos.m_iId, m_arrayTypes[p_ctrlInfos.m_eEventType], p_ctrlInfos.m_pfnFunction, NULL, m_pGenerator );
				}

				if ( ! p_ctrlInfos.m_bVisible )
				{
					l_pSpec->m_pControl->Hide();
				}
			}
		}
		void DoCreateSlider( ControlInfos & p_ctrlInfos )
		{
			int l_iMin = 0;
			int l_iMax = 100;
			int l_iVal = 0;
			SpecificControlParameters< eCONTROL_TYPE_SLIDER > * l_pSpec = NULL;

			if ( p_ctrlInfos.m_pSpecific && p_ctrlInfos.m_pSpecific->m_eCtrlType == eCONTROL_TYPE_SLIDER )
			{
				l_pSpec = static_cast< SpecificControlParameters< eCONTROL_TYPE_SLIDER > * >( p_ctrlInfos.m_pSpecific );
				l_iMin = l_pSpec->m_iMin;
				l_iMax = l_pSpec->m_iMax;
				l_iVal = l_pSpec->m_iValue;
				l_pSpec->m_pControl = new wxSlider( this, p_ctrlInfos.m_iId, l_iVal, l_iMin, l_iMax, p_ctrlInfos.m_ptPosition, p_ctrlInfos.m_szSize, p_ctrlInfos.m_iStyle );

				if ( p_ctrlInfos.m_pfnFunction )
				{
					l_pSpec->m_pControl->Connect( p_ctrlInfos.m_iId, m_arrayTypes[p_ctrlInfos.m_eEventType], p_ctrlInfos.m_pfnFunction, NULL, m_pGenerator );
				}

				if ( ! p_ctrlInfos.m_bVisible )
				{
					l_pSpec->m_pControl->Hide();
				}
			}
		}
		void DoCreateCombo( ControlInfos & p_ctrlInfos )
		{
			wxArrayString l_arrayValues;
			wxString l_strValue;
			SpecificControlParameters< eCONTROL_TYPE_COMBO > * l_pSpec = NULL;

			if ( p_ctrlInfos.m_pSpecific && p_ctrlInfos.m_pSpecific->m_eCtrlType == eCONTROL_TYPE_COMBO )
			{
				l_pSpec = static_cast< SpecificControlParameters< eCONTROL_TYPE_COMBO > * >( p_ctrlInfos.m_pSpecific );
				l_arrayValues = l_pSpec->m_arrayValues;
				l_strValue = l_pSpec->m_strValue;
				l_pSpec->m_pControl = new wxComboBox( this, p_ctrlInfos.m_iId, l_strValue, p_ctrlInfos.m_ptPosition, p_ctrlInfos.m_szSize, l_arrayValues, p_ctrlInfos.m_iStyle );

				if ( p_ctrlInfos.m_pfnFunction )
				{
					l_pSpec->m_pControl->Connect( p_ctrlInfos.m_iId, m_arrayTypes[p_ctrlInfos.m_eEventType], p_ctrlInfos.m_pfnFunction, NULL, m_pGenerator );
				}

				if ( ! p_ctrlInfos.m_bVisible )
				{
					l_pSpec->m_pControl->Hide();
				}
			}
		}
		void DoCreateButton( ControlInfos & p_ctrlInfos )
		{
			wxString l_strText;
			SpecificControlParameters< eCONTROL_TYPE_BUTTON > * l_pSpec = NULL;

			if ( p_ctrlInfos.m_pSpecific && p_ctrlInfos.m_pSpecific->m_eCtrlType == eCONTROL_TYPE_BUTTON )
			{
				l_pSpec = static_cast< SpecificControlParameters< eCONTROL_TYPE_BUTTON > * >( p_ctrlInfos.m_pSpecific );
				l_strText = l_pSpec->m_strText;
				l_pSpec->m_pControl = new wxButton( this, p_ctrlInfos.m_iId, l_strText, p_ctrlInfos.m_ptPosition, p_ctrlInfos.m_szSize, p_ctrlInfos.m_iStyle );
				l_pSpec->m_pControl->SetClientData( l_pSpec );

				if ( p_ctrlInfos.m_pfnFunction )
				{
					if ( p_ctrlInfos.m_eEventType == eEVENT_TYPE_LOAD_TEXT_FILEA )
					{
						Connect( p_ctrlInfos.m_iId, m_arrayTypes[eEVENT_TYPE_BUTTON_CLIKED], wxCommandEventHandler( ConfigPanel::OnLoadTextFileA ) );
						m_pGenerator->Connect( eEVENT_TEXTA, wxEVT_NULL, p_ctrlInfos.m_pfnFunction, NULL, m_pGenerator );
					}
					else if ( p_ctrlInfos.m_eEventType == eEVENT_TYPE_LOAD_TEXT_FILEB )
					{
						Connect( p_ctrlInfos.m_iId, m_arrayTypes[eEVENT_TYPE_BUTTON_CLIKED], wxCommandEventHandler( ConfigPanel::OnLoadTextFileB ) );
						m_pGenerator->Connect( eEVENT_TEXTB, wxEVT_NULL, p_ctrlInfos.m_pfnFunction, NULL, m_pGenerator );
					}
					else if ( p_ctrlInfos.m_eEventType == eEVENT_TYPE_LOAD_IMAGEA )
					{
						Connect( p_ctrlInfos.m_iId, m_arrayTypes[eEVENT_TYPE_BUTTON_CLIKED], wxCommandEventHandler( ConfigPanel::OnLoadImageA ) );
						m_pGenerator->Connect( eEVENT_IMAGEA, wxEVT_NULL, p_ctrlInfos.m_pfnFunction, NULL, m_pGenerator );
					}
					else if ( p_ctrlInfos.m_eEventType == eEVENT_TYPE_LOAD_IMAGEB )
					{
						Connect( p_ctrlInfos.m_iId, m_arrayTypes[eEVENT_TYPE_BUTTON_CLIKED], wxCommandEventHandler( ConfigPanel::OnLoadImageB ) );
						m_pGenerator->Connect( eEVENT_IMAGEB, wxEVT_NULL, p_ctrlInfos.m_pfnFunction, NULL, m_pGenerator );
					}
					else if ( p_ctrlInfos.m_eEventType == eEVENT_TYPE_LOAD_BINARY_FILEA )
					{
						Connect( p_ctrlInfos.m_iId, m_arrayTypes[eEVENT_TYPE_BUTTON_CLIKED], wxCommandEventHandler( ConfigPanel::OnLoadBinaryFileA ) );
						m_pGenerator->Connect( eEVENT_BINARYA, wxEVT_NULL, p_ctrlInfos.m_pfnFunction, NULL, m_pGenerator );
					}
					else if ( p_ctrlInfos.m_eEventType == eEVENT_TYPE_LOAD_BINARY_FILEB )
					{
						Connect( p_ctrlInfos.m_iId, m_arrayTypes[eEVENT_TYPE_BUTTON_CLIKED], wxCommandEventHandler( ConfigPanel::OnLoadBinaryFileB ) );
						m_pGenerator->Connect( eEVENT_BINARYB, wxEVT_NULL, p_ctrlInfos.m_pfnFunction, NULL, m_pGenerator );
					}
					else if ( p_ctrlInfos.m_eEventType == eEVENT_TYPE_LOAD_COLOURA )
					{
						Connect( p_ctrlInfos.m_iId, m_arrayTypes[eEVENT_TYPE_BUTTON_CLIKED], wxCommandEventHandler( ConfigPanel::OnLoadColourA ) );
						m_pGenerator->Connect( eEVENT_COLOURA, wxEVT_NULL, p_ctrlInfos.m_pfnFunction, NULL, m_pGenerator );
					}
					else if ( p_ctrlInfos.m_eEventType == eEVENT_TYPE_LOAD_COLOURB )
					{
						Connect( p_ctrlInfos.m_iId, m_arrayTypes[eEVENT_TYPE_BUTTON_CLIKED], wxCommandEventHandler( ConfigPanel::OnLoadColourB ) );
						m_pGenerator->Connect( eEVENT_COLOURB, wxEVT_NULL, p_ctrlInfos.m_pfnFunction, NULL, m_pGenerator );
					}
					else
					{
						l_pSpec->m_pControl->Connect( p_ctrlInfos.m_iId, m_arrayTypes[p_ctrlInfos.m_eEventType], p_ctrlInfos.m_pfnFunction, NULL, m_pGenerator );
					}
				}

				if ( ! p_ctrlInfos.m_bVisible )
				{
					l_pSpec->m_pControl->Hide();
				}
			}
		}
		void DoCreateFileButton( ControlInfos & p_ctrlInfos )
		{
			wxString l_strText;
			SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON > * l_pSpec = NULL;

			if ( p_ctrlInfos.m_pSpecific && p_ctrlInfos.m_pSpecific->m_eCtrlType == eCONTROL_TYPE_FILE_BUTTON )
			{
				l_pSpec = static_cast< SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON > * >( p_ctrlInfos.m_pSpecific );
				l_strText = l_pSpec->m_strText;
				l_pSpec->m_pControl = new wxButton( this, p_ctrlInfos.m_iId, l_strText, p_ctrlInfos.m_ptPosition, p_ctrlInfos.m_szSize, p_ctrlInfos.m_iStyle );
				l_pSpec->m_pControl->SetClientData( l_pSpec );

				if ( p_ctrlInfos.m_pfnFunction )
				{
					if ( p_ctrlInfos.m_eEventType == eEVENT_TYPE_LOAD_TEXT_FILEA )
					{
						Connect( p_ctrlInfos.m_iId, m_arrayTypes[eEVENT_TYPE_BUTTON_CLIKED], wxCommandEventHandler( ConfigPanel::OnLoadTextFileA ) );
						m_pGenerator->Connect( eEVENT_TEXTA, wxEVT_NULL, p_ctrlInfos.m_pfnFunction, NULL, m_pGenerator );
					}
					else if ( p_ctrlInfos.m_eEventType == eEVENT_TYPE_LOAD_TEXT_FILEB )
					{
						Connect( p_ctrlInfos.m_iId, m_arrayTypes[eEVENT_TYPE_BUTTON_CLIKED], wxCommandEventHandler( ConfigPanel::OnLoadTextFileB ) );
						m_pGenerator->Connect( eEVENT_TEXTB, wxEVT_NULL, p_ctrlInfos.m_pfnFunction, NULL, m_pGenerator );
					}
					else if ( p_ctrlInfos.m_eEventType == eEVENT_TYPE_LOAD_IMAGEA )
					{
						Connect( p_ctrlInfos.m_iId, m_arrayTypes[eEVENT_TYPE_BUTTON_CLIKED], wxCommandEventHandler( ConfigPanel::OnLoadImageA ) );
						m_pGenerator->Connect( eEVENT_IMAGEA, wxEVT_NULL, p_ctrlInfos.m_pfnFunction, NULL, m_pGenerator );
					}
					else if ( p_ctrlInfos.m_eEventType == eEVENT_TYPE_LOAD_IMAGEB )
					{
						Connect( p_ctrlInfos.m_iId, m_arrayTypes[eEVENT_TYPE_BUTTON_CLIKED], wxCommandEventHandler( ConfigPanel::OnLoadImageB ) );
						m_pGenerator->Connect( eEVENT_IMAGEB, wxEVT_NULL, p_ctrlInfos.m_pfnFunction, NULL, m_pGenerator );
					}
					else if ( p_ctrlInfos.m_eEventType == eEVENT_TYPE_LOAD_BINARY_FILEA )
					{
						Connect( p_ctrlInfos.m_iId, m_arrayTypes[eEVENT_TYPE_BUTTON_CLIKED], wxCommandEventHandler( ConfigPanel::OnLoadBinaryFileA ) );
						m_pGenerator->Connect( eEVENT_BINARYA, wxEVT_NULL, p_ctrlInfos.m_pfnFunction, NULL, m_pGenerator );
					}
					else if ( p_ctrlInfos.m_eEventType == eEVENT_TYPE_LOAD_BINARY_FILEB )
					{
						Connect( p_ctrlInfos.m_iId, m_arrayTypes[eEVENT_TYPE_BUTTON_CLIKED], wxCommandEventHandler( ConfigPanel::OnLoadBinaryFileB ) );
						m_pGenerator->Connect( eEVENT_BINARYB, wxEVT_NULL, p_ctrlInfos.m_pfnFunction, NULL, m_pGenerator );
					}
					else if ( p_ctrlInfos.m_eEventType == eEVENT_TYPE_LOAD_COLOURA )
					{
						Connect( p_ctrlInfos.m_iId, m_arrayTypes[eEVENT_TYPE_BUTTON_CLIKED], wxCommandEventHandler( ConfigPanel::OnLoadColourA ) );
						m_pGenerator->Connect( eEVENT_COLOURA, wxEVT_NULL, p_ctrlInfos.m_pfnFunction, NULL, m_pGenerator );
					}
					else if ( p_ctrlInfos.m_eEventType == eEVENT_TYPE_LOAD_COLOURB )
					{
						Connect( p_ctrlInfos.m_iId, m_arrayTypes[eEVENT_TYPE_BUTTON_CLIKED], wxCommandEventHandler( ConfigPanel::OnLoadColourB ) );
						m_pGenerator->Connect( eEVENT_COLOURB, wxEVT_NULL, p_ctrlInfos.m_pfnFunction, NULL, m_pGenerator );
					}
					else
					{
						l_pSpec->m_pControl->Connect( p_ctrlInfos.m_iId, m_arrayTypes[p_ctrlInfos.m_eEventType], p_ctrlInfos.m_pfnFunction, NULL, m_pGenerator );
					}
				}

				if ( ! p_ctrlInfos.m_bVisible )
				{
					l_pSpec->m_pControl->Hide();
				}
			}
		}
		void OnLoadImageA( wxCommandEvent & p_event )
		{
			wxString l_strWildcard;
			wxString l_strImagePath;
			wxButton * l_pButton = reinterpret_cast< wxButton * >( p_event.GetEventObject() );

			if ( l_pButton->GetClientData() )
			{
				SpecificControlParametersBase * l_pData = reinterpret_cast< SpecificControlParametersBase * >( l_pButton->GetClientData() );

				if ( l_pData->GetControlType() == eCONTROL_TYPE_FILE_BUTTON )
				{
					l_strWildcard = static_cast< SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON > * >( l_pData )->m_strWildcard;
				}
			}

			if ( l_strWildcard.empty() )
			{
				l_strWildcard = _( "All supported files (*.bmp;*.gif;*.png;*.jpg )|*.bmp;*.gif;*.png;*.jpg|BITMAP files (*.bmp )|*.bmp|GIF files(*.gif )|*.gif|JPEG files (*.jpg )|*.jpg|PNG files (*.png )|*.png" );
			}

			wxFileDialog l_dialog( this, _( "Choose a picture" ), wxT( "./data" ), wxEmptyString, l_strWildcard );

			if ( l_dialog.ShowModal() == wxID_OK )
			{
				l_strImagePath = l_dialog.GetPath();
				wxCommandEvent l_event( wxEVT_NULL, eEVENT_IMAGEA );
				l_event.SetString( l_strImagePath );
				m_pGenerator->AddPendingEvent( l_event );
			}
		}
		void OnLoadImageB( wxCommandEvent & p_event )
		{
			wxString l_strWildcard;
			wxString l_strImagePath;
			wxButton * l_pButton = reinterpret_cast< wxButton * >( p_event.GetEventObject() );

			if ( l_pButton->GetClientData() )
			{
				SpecificControlParametersBase * l_pData = reinterpret_cast< SpecificControlParametersBase * >( l_pButton->GetClientData() );

				if ( l_pData->GetControlType() == eCONTROL_TYPE_FILE_BUTTON )
				{
					l_strWildcard = static_cast< SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON > * >( l_pData )->m_strWildcard;
				}
			}

			if ( l_strWildcard.empty() )
			{
				l_strWildcard = _( "All supported files (*.bmp;*.gif;*.png;*.jpg )|*.bmp;*.gif;*.png;*.jpg|BITMAP files (*.bmp )|*.bmp|GIF files(*.gif )|*.gif|JPEG files (*.jpg )|*.jpg|PNG files (*.png )|*.png" );
			}

			wxFileDialog l_dialog( this, _( "Choose a picture" ), wxT( "./data" ), wxEmptyString, l_strWildcard );

			if ( l_dialog.ShowModal() == wxID_OK )
			{
				l_strImagePath = l_dialog.GetPath();
				wxCommandEvent l_event( wxEVT_NULL, eEVENT_IMAGEB );
				l_event.SetString( l_strImagePath );
				m_pGenerator->AddPendingEvent( l_event );
			}
		}
		void OnLoadTextFileA( wxCommandEvent & p_event )
		{
			wxString l_strWildcard;
			wxString l_strFile;
			wxButton * l_pButton = reinterpret_cast< wxButton * >( p_event.GetEventObject() );

			if ( l_pButton->GetClientData() )
			{
				SpecificControlParametersBase * l_pData = reinterpret_cast< SpecificControlParametersBase * >( l_pButton->GetClientData() );

				if ( l_pData->GetControlType() == eCONTROL_TYPE_FILE_BUTTON )
				{
					l_strWildcard = static_cast< SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON > * >( l_pData )->m_strWildcard;
				}
			}

			if ( l_strWildcard.empty() )
			{
				l_strWildcard = _( "All files (*.* )|*.*" );
			}

			wxFileDialog l_dialog( this, _( "Choose a file" ), wxT( "./data" ), wxEmptyString, l_strWildcard );

			if ( l_dialog.ShowModal() == wxID_OK )
			{
				l_strFile = l_dialog.GetPath();
				wxCommandEvent l_event( wxEVT_NULL, eEVENT_TEXTA );
				l_event.SetString( l_strFile );
				m_pGenerator->AddPendingEvent( l_event );
			}
		}
		void OnLoadTextFileB( wxCommandEvent & p_event )
		{
			wxString l_strWildcard;
			wxString l_strFile;
			wxButton * l_pButton = reinterpret_cast< wxButton * >( p_event.GetEventObject() );

			if ( l_pButton->GetClientData() )
			{
				SpecificControlParametersBase * l_pData = reinterpret_cast< SpecificControlParametersBase * >( l_pButton->GetClientData() );

				if ( l_pData->GetControlType() == eCONTROL_TYPE_FILE_BUTTON )
				{
					l_strWildcard = static_cast< SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON > * >( l_pData )->m_strWildcard;
				}
			}

			if ( l_strWildcard.empty() )
			{
				l_strWildcard = _( "All files (*.* )|*.*" );
			}

			wxFileDialog l_dialog( this, _( "Choose a file" ), wxT( "./data" ), wxEmptyString, l_strWildcard );

			if ( l_dialog.ShowModal() == wxID_OK )
			{
				l_strFile = l_dialog.GetPath();
				wxCommandEvent l_event( wxEVT_NULL, eEVENT_TEXTB );
				l_event.SetString( l_strFile );
				m_pGenerator->AddPendingEvent( l_event );
			}
		}
		void OnLoadBinaryFileA( wxCommandEvent & p_event )
		{
			wxString l_strWildcard;
			wxString l_strFile;
			wxButton * l_pButton = reinterpret_cast< wxButton * >( p_event.GetEventObject() );

			if ( l_pButton->GetClientData() )
			{
				SpecificControlParametersBase * l_pData = reinterpret_cast< SpecificControlParametersBase * >( l_pButton->GetClientData() );

				if ( l_pData->GetControlType() == eCONTROL_TYPE_FILE_BUTTON )
				{
					l_strWildcard = static_cast< SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON > * >( l_pData )->m_strWildcard;
				}
			}

			if ( l_strWildcard.empty() )
			{
				l_strWildcard = _( "All files (*.* )|*.*" );
			}

			wxFileDialog l_dialog( this, _( "Choose a file" ), wxT( "./data" ), wxEmptyString, l_strWildcard );

			if ( l_dialog.ShowModal() == wxID_OK )
			{
				l_strFile = l_dialog.GetPath();
				wxCommandEvent l_event( wxEVT_NULL, eEVENT_BINARYA );
				l_event.SetString( l_strFile );
				m_pGenerator->AddPendingEvent( l_event );
			}
		}
		void OnLoadBinaryFileB( wxCommandEvent & p_event )
		{
			wxString l_strWildcard;
			wxString l_strFile;
			wxButton * l_pButton = reinterpret_cast< wxButton * >( p_event.GetEventObject() );

			if ( l_pButton->GetClientData() )
			{
				SpecificControlParametersBase * l_pData = reinterpret_cast< SpecificControlParametersBase * >( l_pButton->GetClientData() );

				if ( l_pData->GetControlType() == eCONTROL_TYPE_FILE_BUTTON )
				{
					l_strWildcard = static_cast< SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON > * >( l_pData )->m_strWildcard;
				}
			}

			if ( l_strWildcard.empty() )
			{
				l_strWildcard = _( "All files (*.* )|*.*" );
			}

			wxFileDialog l_dialog( this, _( "Choose a file" ), wxT( "./data" ), wxEmptyString, l_strWildcard );

			if ( l_dialog.ShowModal() == wxID_OK )
			{
				l_strFile = l_dialog.GetPath();
				wxCommandEvent l_event( wxEVT_NULL, eEVENT_BINARYB );
				l_event.SetString( l_strFile );
				m_pGenerator->AddPendingEvent( l_event );
			}
		}
		void OnLoadColourA( wxCommandEvent & WXUNUSED( p_event ) )
		{
			wxColourDialog l_dialog( NULL );

			if ( l_dialog.ShowModal() == wxID_OK )
			{
				wxColour l_colour = l_dialog.GetColourData().GetColour();
				wxCommandEvent l_event( wxEVT_NULL, eEVENT_COLOURA );
				l_event.SetInt( l_colour.GetPixel() );
				m_pGenerator->AddPendingEvent( l_event );
			}
		}
		void OnLoadColourB( wxCommandEvent & WXUNUSED( p_event ) )
		{
			wxColourDialog l_dialog( NULL );

			if ( l_dialog.ShowModal() == wxID_OK )
			{
				wxColour l_colour = l_dialog.GetColourData().GetColour();
				wxCommandEvent l_event( wxEVT_NULL, eEVENT_COLOURB );
				l_event.SetInt( l_colour.GetPixel() );
				m_pGenerator->AddPendingEvent( l_event );
			}
		}

	private:
		//! The generator
		ProceduralGenerator * m_pGenerator;
		//! Array connecting wxEventType and eEVENT_TYPE
		wxEventType m_arrayTypes[eEVENT_TYPE_COUNT];
	};
}

#endif
