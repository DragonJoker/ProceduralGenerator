#include "MainFrame.h"
#include "Application.h"
#include "ProjectListDialog.h"
#include "RenderPanel.h"
#include "StatusBar.h"
#include "exit.xpm"
#include "print_screen.xpm"
#include "select.xpm"
#include "pg.xpm"
#if defined( PGEN_RECORDS )
#	include "record.xpm"
#	include "stop.xpm"
#endif

namespace ProceduralTextures
{
	IMPLEMENT_CLASS( MainFrame, wxFrame );

	MainFrame::MainFrame()
		:	wxFrame( NULL, eID_MAIN_FRAME, wxT( "Procedural Generator" ), wxPoint( 0, 0 ), wxDefaultSize, wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN )
		,	m_pRenderPanel( NULL )
		,	m_pConfigPanel( NULL )
		,	m_pPlugin( NULL )
		,	m_pStatusBar( NULL )
	{
		SetBackgroundColour( * wxBLACK );
		SetForegroundColour( * wxWHITE );
		m_pStatusBar = new StatusBar( this );
		SetStatusBar( m_pStatusBar );
		Connect( eEVENT_CPU_STEP_END,			wxEVT_NULL,	wxCommandEventHandler( MainFrame::OnCpuStepEnd ) );
		Connect( eGUI_EVENT_SHOW_MESSAGE_BOX,	wxEVT_NULL,	wxCommandEventHandler( MainFrame::OnShowMessageBox ), NULL, this );
		Connect( eGUI_EVENT_RESIZE_PARENT,		wxEVT_NULL,	wxCommandEventHandler( MainFrame::OnResize ), NULL, this );
	}

	MainFrame::~MainFrame()
	{
	}

	void MainFrame::Initialise()
	{
		wxSize l_size( 256, 256 );
		wxIcon l_icon = wxIcon( pg_xpm );
		SetIcon( l_icon );
		SetClientSize( l_size );
		DoLoadPlugins();
		DoBuildMenuBar();
		int l_attribList[] = { WX_GL_RGBA, WX_GL_LEVEL, 0, WX_GL_DOUBLEBUFFER };
		m_pRenderPanel = new RenderPanel( this, l_attribList, wxPoint( 0, 0 ), l_size );
		m_pRenderPanel->SetBackgroundColour( GetBackgroundColour() );
		m_pRenderPanel->SetForegroundColour( GetForegroundColour() );
		Show();
		DoSelectGenerator();
	}

	void MainFrame::DoBuildMenuBar()
	{
		wxPNGHandler * l_pHandler = new wxPNGHandler;
		wxImage::AddHandler( l_pHandler );
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
		l_pTool = l_pToolbar->AddTool( eID_SELECT_GENERATOR,	_( "Generator" ),	l_imgSelect.Rescale(	32, 32, wxIMAGE_QUALITY_HIGH ),	_( "Choose a generator" ) );
		l_pTool = l_pToolbar->AddTool( eID_PRINT_SCREEN,		_( "Snapshot" ),	l_imgPrint.Rescale(	32, 32, wxIMAGE_QUALITY_HIGH ),	_( "Take a snapshot" ) );
#if defined( PGEN_RECORDS )
		wxImage l_imgRecord;
		l_imgRecord.Create( record_xpm );
		wxImage l_imgStop;
		l_imgStop.Create( stop_xpm );
		wxImage l_imgRecordDis = l_imgRecord.ConvertToGreyscale();
		wxImage l_imgStopDis = l_imgStop.ConvertToGreyscale();
		l_pTool = l_pToolbar->AddTool( eID_RECORD,				_( "Record" ),	l_imgRecord.Rescale(	32, 32, wxIMAGE_QUALITY_HIGH ),	_( "Record a video" ) );
		l_pTool->SetDisabledBitmap( l_imgRecordDis.Rescale( 32, 32, wxIMAGE_QUALITY_HIGH ) );
		l_pTool = l_pToolbar->AddTool( eID_STOP,				_( "Stop" ),	l_imgStop.Rescale(	32, 32, wxIMAGE_QUALITY_HIGH ),	_( "Stop recording" ) );
		l_pTool->Enable( false );
		l_pTool->SetDisabledBitmap( l_imgStopDis.Rescale( 32, 32, wxIMAGE_QUALITY_HIGH ) );
#endif
		l_pTool = l_pToolbar->AddTool( eID_EXIT,				_( "Quit" ),	l_imgExit.Rescale(	32, 32, wxIMAGE_QUALITY_HIGH ),	_( "Quit ProceduralGenerator" ) );
		l_pToolbar->Realize();
	}

	void MainFrame::DoSelectGenerator()
	{
#if defined( PGEN_RECORDS )
		m_pRenderPanel->StopRecord();
#endif
		m_pRenderPanel->SetGenerator( NULL );
		DoDestroyGenerator();
		ProceduralGenerator * l_pGenerator = DoCreateGenerator();
		Maximize( false );
		m_pRenderPanel->SetGenerator( l_pGenerator );

		if ( l_pGenerator )
		{
			m_pRenderPanel->SetInitialSize( l_pGenerator->GetDisplaySize() );
			m_savedTime = wxGetLocalTimeMillis();
			wxBoxSizer * l_pSizer = new wxBoxSizer( wxHORIZONTAL );
			l_pSizer->Add(	m_pRenderPanel,	wxSizerFlags( 1 ).Expand().FixedMinSize() );

			if ( m_pConfigPanel )
			{
				l_pSizer->Add(	m_pConfigPanel,	wxSizerFlags( 0 ).Expand() );
			}

			SetSizer( l_pSizer, true );
			l_pSizer->SetSizeHints( this );
		}
	}

	ProceduralGenerator * MainFrame::DoCreateGenerator()
	{
		ProceduralGenerator * l_pReturn = NULL;
		wxArrayString l_choices;

		for ( PluginStrMap::iterator l_it = m_mapPluginsByName.begin() ; l_it != m_mapPluginsByName.end() ; ++l_it )
		{
			l_choices.push_back( l_it->first );
		}

		ProjectListDialog l_dialog( this, l_choices );

		if ( l_dialog.ShowModal() == wxID_OK )
		{
			wxString l_strProjectName = l_dialog.GetProject();
			wxSize l_sizeResolution = l_dialog.GetResolution();
			PluginStrMap::iterator l_it = m_mapPluginsByName.find( l_strProjectName );

			if ( l_it != m_mapPluginsByName.end() )
			{
				m_pPlugin = l_it->second;
				l_pReturn = m_pPlugin->CreateGenerator( l_sizeResolution.x, l_sizeResolution.y, MainFrame::eID_MAIN_FRAME, this );

				if ( l_pReturn->NeedsConfigPanel() )
				{
					wxEventType l_arrayTypes[eEVENT_TYPE_COUNT];
					l_arrayTypes[eEVENT_TYPE_SLIDER_UPDATED			]	= wxEVT_COMMAND_SLIDER_UPDATED;
					l_arrayTypes[eEVENT_TYPE_SCROLL_THUMBRELEASE	]	= wxEVT_SCROLL_THUMBRELEASE;
					l_arrayTypes[eEVENT_TYPE_SCROLL_THUMBTRACK		]	= wxEVT_SCROLL_THUMBTRACK;
					l_arrayTypes[eEVENT_TYPE_COMBOBOX_SELECTED		]	= wxEVT_COMMAND_COMBOBOX_SELECTED;
					l_arrayTypes[eEVENT_TYPE_BUTTON_CLIKED			]	= wxEVT_COMMAND_BUTTON_CLICKED;
					l_arrayTypes[eEVENT_TYPE_TEXT_UPDATED			]	= wxEVT_COMMAND_TEXT_UPDATED;
					l_arrayTypes[eEVENT_TYPE_TEXT_ENTER				]	= wxEVT_COMMAND_TEXT_ENTER;
					m_pConfigPanel = new ConfigPanel( l_arrayTypes, l_pReturn, this, wxID_ANY );
				}

				SetLabel( l_strProjectName );
			}
		}

		return l_pReturn;
	}

	void MainFrame::DoDestroyGenerator()
	{
		if ( m_pConfigPanel )
		{
			m_pConfigPanel->Close();
			m_pConfigPanel->Destroy();
			m_pConfigPanel = NULL;
		}
	}

	void MainFrame::DoLoadPlugins()
	{
		wxStandardPathsBase const & l_paths = wxStandardPaths::Get();
		wxFileName l_dataDir = l_paths.GetExecutablePath();
		wxArrayString l_arrayDirs = l_dataDir.GetDirs();
		wxString l_strUsrPath = l_dataDir.GetVolume() + l_dataDir.GetVolumeSeparator();

		for ( size_t i = 0 ; i < l_arrayDirs.size() && l_arrayDirs[i] != wxT( "bin" ) ; i++ )
		{
			l_strUsrPath += wxFileName::GetPathSeparator() + l_arrayDirs[i];
		}

		wxString l_strPluginsPath = l_strUsrPath + wxFileName::GetPathSeparator() + wxT( "lib" ) + wxFileName::GetPathSeparator() + wxT( "ProceduralGenerator" ) + wxFileName::GetPathSeparator();

		DoLoadPlugins( l_strPluginsPath );
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
#endif // defined

			while ( l_bRes )
			{
				wxDynamicLibrary * l_pDll = new wxDynamicLibrary;

				if ( l_pDll->Load( p_strPath + l_strFileName ) )
				{
					wxMilliSleep( 100 );

					if ( l_pDll->HasSymbol( wxT( "CreatePlugin" ) ) )
					{
						wxDYNLIB_FUNCTION( PluginBase::PCreatePluginFunction, CreatePlugin, *l_pDll );

						if ( pfnCreatePlugin )
						{
							PluginBase * l_pPlugin = pfnCreatePlugin( l_pDll, wxTheApp );

							if ( l_pPlugin )
							{
								m_mapPluginsByName.insert( std::make_pair( l_pPlugin->GetName(), l_pPlugin ) );
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

	void MainFrame::DoClose()
	{
		Hide();

		if ( m_pRenderPanel )
		{
			m_pRenderPanel->SetGenerator( NULL );
			DoDestroyGenerator();
			m_pRenderPanel->Close();
		}

		DoDestroyGenerator();

		for ( PluginStrMap::iterator l_it = m_mapPluginsByName.begin() ; l_it != m_mapPluginsByName.end() ; ++l_it )
		{
			l_it->second->Destroy();
		}

		m_mapPluginsByName.clear();
	}

	BEGIN_EVENT_TABLE( MainFrame, wxFrame )
		EVT_PAINT(	MainFrame::OnPaint )
		EVT_SIZE(	MainFrame::OnSize )
		EVT_CLOSE(	MainFrame::OnClose )
		EVT_KEY_UP(	MainFrame::OnKeyUp )
		EVT_MENU(	eID_EXIT,				MainFrame::OnExit )
		EVT_MENU(	eID_SELECT_GENERATOR,	MainFrame::OnSelectGenerator )
		EVT_MENU(	eID_PRINT_SCREEN,		MainFrame::OnPrintScreen )
#if defined( PGEN_RECORDS )
		EVT_MENU(	eID_RECORD,				MainFrame::OnRecord )
		EVT_MENU(	eID_STOP,				MainFrame::OnStop )
#endif
	END_EVENT_TABLE()

	void MainFrame::OnPaint( wxPaintEvent & p_event )
	{
		wxPaintDC l_dc( this );
		p_event.Skip();
	}

	void MainFrame::OnSize( wxSizeEvent & p_event )
	{
		if ( m_pConfigPanel )
		{
			m_pConfigPanel->Show( !IsFullScreen() );
		}

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

	void MainFrame::OnCpuStepEnd( wxCommandEvent & p_event )
	{
		typedef std::basic_stringstream< wxChar > wxStringStream;
		wxStringStream l_streamMsCpu;
		wxStringStream l_streamFpsCpu;
		wxStringStream l_streamMsGpu;
		wxStringStream l_streamFpsGpu;
		wxMilliClock_t l_diff = 0;
		wxString l_strStatus = wxT( "C : " );
		m_time = wxGetLocalTimeMillis();
		l_diff = m_time - m_savedTime;
		m_savedTime = wxGetLocalTimeMillis();

		if ( l_diff != 0 )
		{
			l_streamMsCpu.width( 5 );
			l_streamMsCpu.fill( wxT( '0' ) );
			l_streamMsCpu << std::right << l_diff.GetValue();
			l_streamFpsCpu.width( 4 );
			l_streamFpsCpu.fill( wxT( '0' ) );
			l_streamFpsCpu << std::right << ( 1000 / l_diff.GetValue() );
			m_pStatusBar->SetStatusText( l_strStatus + l_streamMsCpu.str() + wxT( "ms - " ) + l_streamFpsCpu.str() + wxT( " FPS" ), 0 );
			m_pStatusBar->Refresh();
		}

		if ( m_pRenderPanel )
		{
			m_pRenderPanel->Render();
			m_time = wxGetLocalTimeMillis();
			l_strStatus = wxT( "R : " );
			l_diff = m_time - m_savedTime;
			m_savedTime = wxGetLocalTimeMillis();

			if ( l_diff != 0 )
			{
				l_streamMsGpu.width( 5 );
				l_streamMsGpu.fill( wxT( '0' ) );
				l_streamMsGpu << std::right << l_diff.GetValue();
				l_streamFpsGpu.width( 4 );
				l_streamFpsGpu.fill( wxT( '0' ) );
				l_streamFpsGpu << std::right << ( 1000 / l_diff.GetValue() );
				m_pStatusBar->SetStatusText( l_strStatus + l_streamMsGpu.str() + wxT( "ms - " ) + l_streamFpsGpu.str() + wxT( " FPS" ), 1 );
				m_pStatusBar->Refresh();
			}
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

		wxMessageBox( l_strMessage, l_strTitle, p_event.GetInt(), this );
		p_event.Skip();
	}

	void MainFrame::OnResize( wxCommandEvent & p_event )
	{
		int l_width = p_event.GetExtraLong();
		int l_height = p_event.GetInt();
		SetClientSize( wxSize( l_width, l_height ) );
		p_event.Skip();
	}

#if defined( PGEN_RECORDS )
	void MainFrame::OnRecord( wxCommandEvent & p_event )
	{
		if ( m_pRenderPanel->StartRecord() )
		{
			GetToolBar()->EnableTool( eID_STOP, true );
			GetToolBar()->EnableTool( eID_RECORD, false );
		}

		p_event.Skip();
	}

	void MainFrame::StopRecord()
	{
		GetToolBar()->EnableTool( eID_STOP, false );
		GetToolBar()->EnableTool( eID_RECORD, true );
		m_pRenderPanel->StopRecord();
	}

	void MainFrame::OnStop( wxCommandEvent & p_event )
	{
		StopRecord();
		p_event.Skip();
	}
#endif
}
