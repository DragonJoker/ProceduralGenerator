#include "Application.h"

IMPLEMENT_APP( ProceduralTextures::ProceduralGeneratorApp );

namespace ProceduralTextures
{
	ProceduralGeneratorApp::ProceduralGeneratorApp()
		:	m_pLocale( NULL )
		,	m_mainFrame( NULL )
	{
	}

	ProceduralGeneratorApp::~ProceduralGeneratorApp()
	{
	}

	bool ProceduralGeneratorApp::OnInit()
	{
		wxStandardPathsBase const & l_stdPaths = wxStandardPaths::Get();
		long 		l_lLanguage		= wxLANGUAGE_DEFAULT;
		wxFileName	l_pathExe		= l_stdPaths.GetExecutablePath();
		wxString	l_strPath		= l_pathExe.GetPath( wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR );
		wxString	l_strSep		= wxFileName::GetPathSeparator();
		std::size_t l_uiIndex = l_strPath.find( l_strSep + wxT( "bin" ) + l_strSep );

		if ( l_uiIndex != wxString::npos )
		{
			l_strPath = l_strPath.substr( 0, l_uiIndex );
		}

		// load language if possible, fall back to english otherwise
		if ( wxLocale::IsAvailable( l_lLanguage ) )
		{
			m_pLocale = new wxLocale( l_lLanguage, wxLOCALE_LOAD_DEFAULT );
			// add locale search paths
			m_pLocale->AddCatalogLookupPathPrefix( l_strPath + l_strSep + wxT( "share" ) + l_strSep + wxT( "ProceduralGenerator" ) );
#if defined( _MSC_VER )
#	if defined( NDEBUG )
			m_pLocale->AddCatalogLookupPathPrefix( l_strPath + l_strSep + wxT( "share" ) + l_strSep + wxT( "Release" ) + l_strSep + wxT( "ProceduralGenerator" ) );
#	else
			m_pLocale->AddCatalogLookupPathPrefix( l_strPath + l_strSep + wxT( "share" ) + l_strSep + wxT( "Debug" ) + l_strSep + wxT( "ProceduralGenerator" ) );
#	endif
#endif
			m_pLocale->AddCatalog( wxT( "ProceduralGenerator" ) );

			if ( !m_pLocale->IsOk() )
			{
				std::cerr << "Selected language is wrong" << std::endl;
				delete m_pLocale;
				m_pLocale = new wxLocale( wxLANGUAGE_ENGLISH );
				l_lLanguage = wxLANGUAGE_ENGLISH;
			}
		}
		else
		{
			std::cout << "The selected language is not supported by your system."
					  << "Try installing support for this language." << std::endl;
			m_pLocale = new wxLocale( wxLANGUAGE_ENGLISH );
			l_lLanguage = wxLANGUAGE_ENGLISH;
		}

		wxAppConsole::SetAppName( wxT( "procedural_generator" ) );
		wxAppConsole::SetVendorName( wxT( "dragonjoker" ) );
//		wxAppConsole::SetAppDisplayName( wxT( "Procedural Generator" ) );
//		wxAppConsole::SetVendorDisplayName( wxT( "DragonJoker" ) );
		m_mainFrame = new MainFrame();
		SetTopWindow( m_mainFrame );
		m_mainFrame->Initialise();
		return true;
	}

	int ProceduralGeneratorApp::OnExit()
	{
		delete m_pLocale;
		m_pLocale = NULL;
		wxImage::CleanUpHandlers();
		return wxApp::OnExit();
	}
}
