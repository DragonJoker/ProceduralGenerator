#include "MainFrame.h"
#include "Application.h"
#include "ProjectListDialog.h"

#include "exit.xpm"
#include "print_screen.xpm"
#include "select.xpm"
#include "pg.xpm"
#include "button.xpm"
#if defined( PGEN_RECORDS )
#	include "record.xpm"
#	include "stop.xpm"
#endif

#pragma warning( push )
#pragma warning( disable:4996 )
#include <wx/button.h>
#include <wx/colordlg.h>
#include <wx/dir.h>
#include <wx/display.h>
#include <wx/dynlib.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <wx/msgdlg.h>
#include <wx/mstream.h>
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/stdpaths.h>
#include <wx/toolbar.h>
#pragma warning( pop )

#include <Plugin.h>

#include "RenderPanel.h"

#define PGEN_CHECK_TIMES 0

using namespace ProceduralTextures;

namespace ProceduralGenerator
{
#ifndef NDEBUG
	const int OPTIONS_PANEL_HEIGHT = 256;
#else
	const int OPTIONS_PANEL_HEIGHT = 512;
#endif
	namespace detail
	{
		bool OpenFile( wxWindow * p_parent, String & p_path, String const & p_prompt, String const & p_wildcard )
		{
			bool l_return = false;
			wxString l_wildcard = wxString( p_wildcard.c_str(), wxConvUTF8 );
			wxString l_prompt = wxString( p_prompt.c_str(), wxConvUTF8 );
			wxFileDialog l_dialog( p_parent, l_prompt, wxT( "./data" ), wxEmptyString, l_wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST );

			if ( l_dialog.ShowModal() == wxID_OK )
			{
				p_path = String( l_dialog.GetPath().mb_str( wxConvUTF8 ) );
				l_return = true;
			}

			return l_return;
		}

		bool SaveFile( wxWindow * p_parent, String & p_path, String const & p_prompt, String const & p_wildcard )
		{
			bool l_return = false;
			wxString l_wildcard = wxString( p_wildcard.c_str(), wxConvUTF8 );
			wxString l_prompt = wxString( p_prompt.c_str(), wxConvUTF8 );
			wxFileDialog l_dialog( p_parent, l_prompt, wxT( "./data" ), wxEmptyString, l_wildcard, wxFD_SAVE | wxFD_OVERWRITE_PROMPT );

			if ( l_dialog.ShowModal() == wxID_OK )
			{
				p_path = String( l_dialog.GetPath().mb_str( wxConvUTF8 ) );
				l_return = true;
			}

			return l_return;
		}

		void LoadImageBuffer( wxImage & p_image, ProceduralTextures::PixelBuffer & p_buffer )
		{
			Size l_sizeReturn( p_image.GetWidth(), p_image.GetHeight() );
			p_buffer.Initialise( l_sizeReturn );

			if ( !p_image.HasAlpha() )
			{
				p_image.InitAlpha();
			}

			if ( p_image.HasAlpha() )
			{
				p_buffer.Set< PG_UINT8_RGBA >( p_image.GetData(), p_image.GetAlpha() );
			}
			else
			{
				p_buffer.Set< PG_UINT8_RGB >( p_image.GetData(), NULL );
			}
		}

		bool SelectImage( wxWindow * p_parent, ProceduralTextures::PixelBuffer & p_buffer, String const & p_prompt )
		{
			bool l_return = false;
			wxString l_wildcard = _( "All supported files " );
			l_wildcard += wxT( "(*.bmp;*.gif;*.png;*.jpg)|*.bmp;*.gif;*.png;*.jpg|" );
			l_wildcard += _( "BITMAP files" );
			l_wildcard += wxT( " (*.bmp)|*.bmp|" );
			l_wildcard += _( "GIF files" );
			l_wildcard += wxT( " (*.gif)|*.gif|" );
			l_wildcard += _( "JPEG files" );
			l_wildcard += wxT( " (*.jpg)|*.jpg|" );
			l_wildcard += _( "PNG files" );
			l_wildcard += wxT( " (*.png)|*.png" );
			wxString l_prompt = _( "Choose a picture" );
			wxFileDialog l_dialog( p_parent, l_prompt, wxT( "./data" ), wxEmptyString, l_wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST );

			if ( l_dialog.ShowModal() == wxID_OK )
			{
				wxImage l_image;

				if ( l_image.LoadFile( l_dialog.GetPath() ) )
				{
					l_image = l_image.Mirror( false );
					LoadImageBuffer( l_image, p_buffer );
					l_return = true;
				}
				else
				{
					wxMessageBox( _( "Image loading error" ), _( "No handler found for this image format" ), wxOK | wxCENTRE | wxICON_INFORMATION );
				}
			}

			return l_return;
		}

		bool SelectColour( wxWindow * p_parent, uint32_t & p_colour, String const & p_prompt )
		{
			bool l_return = false;
			wxColourDialog l_dialog( NULL );

			if ( l_dialog.ShowModal() == wxID_OK )
			{
				wxColour l_colour = l_dialog.GetColourData().GetColour();
				p_colour = l_colour.GetPixel();
				l_return = true;
			}

			return l_return;
		}

		void SetCursor( wxWindow * p_parent, eMOUSE_CURSOR p_cursor )
		{
			switch ( p_cursor )
			{
			case eMOUSE_CURSOR_ARROW:
				p_parent->SetCursor( wxCURSOR_ARROW );
				break;

			case eMOUSE_CURSOR_HAND:
				p_parent->SetCursor( wxCURSOR_HAND );
				break;

			case eMOUSE_CURSOR_TEXT:
				p_parent->SetCursor( wxCURSOR_IBEAM );
				break;
			}
		}
	}

	IMPLEMENT_CLASS( MainFrame, wxFrame );

	MainFrame::MainFrame()
		: wxFrame( NULL, eID_MAIN_FRAME, wxT( "Procedural Generator" ), wxPoint( 0, 0 ), wxDefaultSize, wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN )
		, m_pRenderPanel( NULL )
	{
		SetBackgroundColour( *wxBLACK );
		SetForegroundColour( *wxWHITE );
		Connect( eGUI_EVENT_SHOW_MESSAGE_BOX, wxEVT_NULL, wxCommandEventHandler( MainFrame::OnShowMessageBox ), NULL, this );
		Connect( eGUI_EVENT_RESIZE_PARENT, wxEVT_NULL, wxCommandEventHandler( MainFrame::OnResize ), NULL, this );
#if PGEN_CHECK_TIMES
		wxStatusBar * l_status = new wxStatusBar( this );
		l_status->SetFieldsCount();
		SetStatusBar( l_status );
#endif
	}

	MainFrame::~MainFrame()
	{
	}

	void MainFrame::Initialise()
	{
		wxSize l_size( OPTIONS_PANEL_HEIGHT, OPTIONS_PANEL_HEIGHT );
		wxIcon l_icon = wxIcon( pg_xpm );
		SetIcon( l_icon );
		SetClientSize( l_size );
		DoLoadPlugins();
		DoBuildMenuBar();
		int l_attribList[] = { WX_GL_RGBA, WX_GL_LEVEL, 0, WX_GL_DOUBLEBUFFER, 0 };
		m_pRenderPanel = new RenderPanel( this, l_attribList, wxPoint( 0, 0 ), l_size );
		m_pRenderPanel->SetBackgroundColour( GetBackgroundColour() );
		m_pRenderPanel->SetForegroundColour( GetForegroundColour() );
		wxBoxSizer * l_pSizer = new wxBoxSizer( wxHORIZONTAL );
		l_pSizer->Add( m_pRenderPanel, wxSizerFlags( 1 ).Expand().FixedMinSize() );
		SetSizer( l_pSizer, true );
		l_pSizer->SetSizeHints( this );
		Show();
		DoSelectGenerator();
	}

	void MainFrame::StopRecord()
	{
#if defined( PGEN_RECORDS )
		GetToolBar()->EnableTool( eID_STOP, false );
		GetToolBar()->EnableTool( eID_RECORD, true );
#endif
		m_pRenderPanel->StopRecord();
	}

	void MainFrame::UpdateTimes( std::chrono::milliseconds const & p_gpu, std::chrono::milliseconds const & p_cpu )
	{
#if PGEN_CHECK_TIMES
		GetStatusBar()->SetStatusText( wxString() << wxT( "GPU: " ) << p_gpu.count() << wxT( ", CPU: " ) << p_cpu.count() );
#endif
	}

	void MainFrame::DoBuildMenuBar()
	{
		wxToolBar * l_pToolbar = CreateToolBar( wxTB_FLAT | wxTB_HORIZONTAL );
		wxMemoryInputStream l_isSelect( select_png, sizeof( select_png ) );
		wxImage l_imgSelect( l_isSelect, wxBITMAP_TYPE_PNG );
		wxMemoryInputStream l_isPrint( print_screen_png, sizeof( print_screen_png ) );
		wxImage l_imgPrint( l_isPrint, wxBITMAP_TYPE_PNG );
		wxMemoryInputStream l_isExit( exit_png, sizeof( exit_png ) );
		wxImage l_imgExit( l_isExit, wxBITMAP_TYPE_PNG );
		l_pToolbar->SetBackgroundColour( GetBackgroundColour() );
		l_pToolbar->SetForegroundColour( GetForegroundColour() );
		wxToolBarToolBase * l_pTool;
		l_pTool = l_pToolbar->AddTool( eID_SELECT_GENERATOR, _( "Generator" ), l_imgSelect.Rescale( 32, 32, wxIMAGE_QUALITY_HIGH ), _( "Choose a generator" ) );
		l_pTool = l_pToolbar->AddTool( eID_PRINT_SCREEN, _( "Snapshot" ), l_imgPrint.Rescale( 32, 32, wxIMAGE_QUALITY_HIGH ), _( "Take a snapshot" ) );
#if defined( PGEN_RECORDS )
		wxImage l_imgRecord;
		l_imgRecord.Create( record_xpm );
		wxImage l_imgStop;
		l_imgStop.Create( stop_xpm );
		wxImage l_imgRecordDis = l_imgRecord.ConvertToGreyscale();
		wxImage l_imgStopDis = l_imgStop.ConvertToGreyscale();
		l_pTool = l_pToolbar->AddTool( eID_RECORD, _( "Record" ), l_imgRecord.Rescale( 32, 32, wxIMAGE_QUALITY_HIGH ), _( "Record a video" ) );
		l_pTool->SetDisabledBitmap( l_imgRecordDis.Rescale( 32, 32, wxIMAGE_QUALITY_HIGH ) );
		l_pTool = l_pToolbar->AddTool( eID_STOP, _( "Stop" ), l_imgStop.Rescale( 32, 32, wxIMAGE_QUALITY_HIGH ), _( "Stop recording" ) );
		l_pTool->Enable( false );
		l_pTool->SetDisabledBitmap( l_imgStopDis.Rescale( 32, 32, wxIMAGE_QUALITY_HIGH ) );
#endif
		l_pTool = l_pToolbar->AddTool( eID_EXIT, _( "Quit" ), l_imgExit.Rescale( 32, 32, wxIMAGE_QUALITY_HIGH ), _( "Quit ProceduralGenerator" ) );
		l_pToolbar->Realize();
	}

	void MainFrame::DoSelectGenerator()
	{
		m_pRenderPanel->StopRecord();
		std::shared_ptr< GeneratorBase > l_generator = m_pRenderPanel->GetGenerator();
		m_pRenderPanel->SetGenerator( nullptr );
		DoDestroyGenerator( l_generator );

		l_generator = DoCreateGenerator();
		Maximize( false );
		m_pRenderPanel->SetGenerator( l_generator );

		if ( l_generator )
		{
			m_pRenderPanel->SetInitialSize( wxSize( l_generator->GetDisplaySize().x(), l_generator->GetDisplaySize().y() ) );
			GetSizer()->SetSizeHints( this );
		}
	}

	std::shared_ptr< GeneratorBase > MainFrame::DoCreateGenerator()
	{
		std::shared_ptr< GeneratorBase > l_pReturn;
		std::vector< bool > l_customResolution;
		wxArrayString l_choices;

		for ( auto l_pluginDllPair : m_mapPluginsByName )
		{
			l_choices.push_back( l_pluginDllPair.first );
			l_customResolution.push_back( l_pluginDllPair.second.first->HasCustomisableResolution() );
		}

		ProjectListDialog l_dialog( this, l_choices, l_customResolution );

		if ( l_dialog.ShowModal() == wxID_OK )
		{
			wxString l_strProjectName = l_dialog.GetProject();
			wxSize l_sizeResolution = l_dialog.GetResolution();
			auto l_it = m_mapPluginsByName.find( l_strProjectName );

			if ( l_it != m_mapPluginsByName.end() )
			{
				PluginBase * l_plugin = l_it->second.first;
				Size l_frameSize( GetSize().x, GetSize().y );
				Size l_clientSize( GetClientSize().x, GetClientSize().y );

				try
				{
					l_pReturn = l_plugin->CreateGenerator();
					l_pReturn->SetShowMessageBoxCallback( std::bind( &MainFrame::FireShowMessageBox, this, std::placeholders::_1, std::placeholders::_2 ) );
					l_pReturn->SetResizeCallback( std::bind( &MainFrame::FireResize, this, std::placeholders::_1 ) );
					l_pReturn->SetOpenFileCallback( std::bind( &detail::OpenFile, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );
					l_pReturn->SetSaveFileCallback( std::bind( &detail::SaveFile, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );
					l_pReturn->SetSelectImageCallback( std::bind( &detail::SelectImage, this, std::placeholders::_1, std::placeholders::_2 ) );
					l_pReturn->SetSelectColourCallback( std::bind( &detail::SelectColour, this, std::placeholders::_1, std::placeholders::_2 ) );
					l_pReturn->SetSetCursorCallback( std::bind( &detail::SetCursor, this, std::placeholders::_1 ) );
					l_pReturn->Create( Size( l_sizeResolution.x, l_sizeResolution.y ), l_frameSize - l_clientSize );
					SetLabel( l_strProjectName );
				}
				catch ( std::exception & l_exc )
				{
					l_pReturn.reset();
					wxMessageBox( _( "The generator loading failed due to the following error:" ) + wxT( "\n" ) + wxString( l_exc.what(), wxConvUTF8 ), _( "Generator loading error" ) );
				}
			}
		}

		return l_pReturn;
	}

	void MainFrame::DoDestroyGenerator( std::shared_ptr< GeneratorBase > & p_generator )
	{
		p_generator.reset();
	}

	void MainFrame::DoLoadPlugins()
	{
		wxStandardPathsBase const & l_paths = wxStandardPaths::Get();
		wxFileName l_dataDir = l_paths.GetExecutablePath();
		wxArrayString l_arrayDirs = l_dataDir.GetDirs();
		wxString l_strUsrPath = l_dataDir.GetVolume() + l_dataDir.GetVolumeSeparator();

		for ( size_t i = 0; i < l_arrayDirs.size() && l_arrayDirs[i] != wxT( "bin" ); i++ )
		{
			l_strUsrPath += wxFileName::GetPathSeparator() + l_arrayDirs[i];
		}

		wxString l_strPluginsPath = l_strUsrPath + wxFileName::GetPathSeparator() + wxT( "lib" ) + wxFileName::GetPathSeparator() + wxT( "ProceduralGenerator" ) + wxFileName::GetPathSeparator();

		DoLoadPlugins( l_strPluginsPath );
	}

	void MainFrame::DoClose()
	{
		Hide();

		if ( m_pRenderPanel )
		{
			std::shared_ptr< GeneratorBase > l_generator = m_pRenderPanel->GetGenerator();
			m_pRenderPanel->SetGenerator( NULL );
			DoDestroyGenerator( l_generator );
			m_pRenderPanel->Close();
		}

		std::shared_ptr< GeneratorBase > l_generator;
		DoDestroyGenerator( l_generator );

		for ( auto l_pluginDllPair : m_mapPluginsByName )
		{
			l_pluginDllPair.second.first->Destroy();
			delete l_pluginDllPair.second.second;
		}

		m_mapPluginsByName.clear();
	}

	void MainFrame::DoLoadPlugins( wxString const & p_strPath )
	{
		wxDir l_dir( p_strPath );
		wxString l_strFileName;

		if ( l_dir.IsOpened() )
		{
#if defined __GNUG__
			bool l_bRes = l_dir.GetFirst( &l_strFileName, wxString( wxT( "lib*" ) ) + wxDynamicLibrary::GetDllExt(), wxDIR_FILES );
#elif defined _WIN32
			bool l_bRes = l_dir.GetFirst( &l_strFileName, wxString( wxT( "*" ) ) + wxDynamicLibrary::GetDllExt(), wxDIR_FILES );
#endif

			while ( l_bRes )
			{
				wxDynamicLibrary * l_pDll = new wxDynamicLibrary;

				if ( l_pDll->Load( p_strPath + l_strFileName ) )
				{
					if ( l_pDll->HasSymbol( wxT( "CreatePlugin" ) ) )
					{
						wxDYNLIB_FUNCTION( PluginBase::PCreatePluginFunction, CreatePlugin, *l_pDll );

						if ( pfnCreatePlugin )
						{
							PluginBase * l_pPlugin = pfnCreatePlugin();

							if ( l_pPlugin )
							{
								m_mapPluginsByName.insert( std::make_pair( wxString( l_pPlugin->GetName().c_str(), wxConvUTF8 ), std::make_pair( l_pPlugin, l_pDll ) ) );
							}
						}
					}
					else
					{
						delete l_pDll;
					}
				}
				else
				{
					delete l_pDll;
				}

				l_bRes = l_dir.GetNext( &l_strFileName );
			}
		}
	}

	void MainFrame::FireShowMessageBox( String const p_title, String const & p_message )
	{
		wxCommandEvent l_event( wxEVT_NULL, eGUI_EVENT_SHOW_MESSAGE_BOX );
		l_event.SetString( wxString( p_title.c_str(), wxConvUTF8 ) + wxT( "||" ) + wxString( p_message.c_str(), wxConvUTF8 ) );
		GetEventHandler()->AddPendingEvent( l_event );
	}

	void MainFrame::FireResize( Size const & p_size )
	{
		wxCommandEvent l_event( wxEVT_NULL, eGUI_EVENT_RESIZE_PARENT );
		l_event.SetExtraLong( p_size.x() );
		l_event.SetInt( p_size.y() );
		GetEventHandler()->AddPendingEvent( l_event );
	}

	BEGIN_EVENT_TABLE( MainFrame, wxFrame )
		EVT_PAINT( MainFrame::OnPaint )
		EVT_SIZE( MainFrame::OnSize )
		EVT_CLOSE( MainFrame::OnClose )
		EVT_KEY_UP( MainFrame::OnKeyUp )
		EVT_MENU( eID_EXIT, MainFrame::OnExit )
		EVT_MENU( eID_SELECT_GENERATOR, MainFrame::OnSelectGenerator )
		EVT_MENU( eID_PRINT_SCREEN, MainFrame::OnPrintScreen )
		EVT_MENU( eID_RECORD, MainFrame::OnRecord )
		EVT_MENU( eID_STOP, MainFrame::OnStop )
	END_EVENT_TABLE()

	void MainFrame::OnPaint( wxPaintEvent & p_event )
	{
		wxPaintDC l_dc( this );
		p_event.Skip();
	}

	void MainFrame::OnSize( wxSizeEvent & p_event )
	{
#if defined( PGEN_RECORDS )

		if ( GetToolBar() )
		{
			GetToolBar()->EnableTool( eID_STOP, false );
			GetToolBar()->EnableTool( eID_RECORD, true );
		}

#endif
		p_event.Skip();
	}

	void MainFrame::OnClose( wxCloseEvent & p_event )
	{
		DoClose();
		p_event.Skip();
	}

	void MainFrame::OnKeyUp( wxKeyEvent & p_event )
	{
		if ( m_pRenderPanel && p_event.GetKeyCode() == WXK_F12 )
		{
			m_pRenderPanel->SaveFrame();
		}

		p_event.Skip();
	}

	void MainFrame::OnExit( wxCommandEvent & p_event )
	{
		Close( true );
		p_event.Skip();
	}

	void MainFrame::OnSelectGenerator( wxCommandEvent & p_event )
	{
		DoSelectGenerator();
		p_event.Skip();
	}

	void MainFrame::OnPrintScreen( wxCommandEvent & p_event )
	{
		if ( m_pRenderPanel )
		{
			m_pRenderPanel->SaveFrame();
		}

		p_event.Skip();
	}

	void MainFrame::OnShowMessageBox( wxCommandEvent & p_event )
	{
		wxString l_strTitle = wxMessageBoxCaptionStr;
		wxString l_strMessage = p_event.GetString();
		std::size_t l_uiIndex = l_strMessage.find( wxT( "||" ) );

		if ( l_uiIndex != wxString::npos )
		{
			l_strTitle = l_strMessage.substr( 0, l_uiIndex );
			l_strMessage = l_strMessage.substr( l_uiIndex + 2 );
		}

		wxMessageBox( l_strMessage, l_strTitle, wxOK | wxCENTRE | wxICON_INFORMATION, this );
		p_event.Skip();
	}

	void MainFrame::OnResize( wxCommandEvent & p_event )
	{
		int l_width = p_event.GetExtraLong();
		int l_height = p_event.GetInt();
		SetClientSize( wxSize( l_width, l_height ) );
		p_event.Skip();
	}

	void MainFrame::OnRecord( wxCommandEvent & p_event )
	{
		if ( m_pRenderPanel->StartRecord() )
		{
#if defined( PGEN_RECORDS )
			GetToolBar()->EnableTool( eID_STOP, true );
			GetToolBar()->EnableTool( eID_RECORD, false );
#endif
		}

		p_event.Skip();
	}

	void MainFrame::OnStop( wxCommandEvent & p_event )
	{
		StopRecord();
		p_event.Skip();
	}
}
