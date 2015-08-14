#include "Application.h"

#include <wx/sysopt.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>

#include <GeneratorUtils.h>

IMPLEMENT_APP( ProceduralGenerator::Application );

using namespace ProceduralTextures;

namespace ProceduralGenerator
{
	Application::Application()
		:	m_mainFrame( NULL )
	{
		wxSystemOptions::SetOption( wxT( "msw.remap" ), 0 );
		wxInitAllImageHandlers();
	}

	Application::~Application()
	{
		wxImage::CleanUpHandlers();
	}

	bool Application::OnInit()
	{
		Translator::Initialise( System::GetExecutableDirectory(), _T( "ProceduralGenerator" ), _T( "ProceduralGenerator" ) );
		wxAppConsole::SetAppName( wxT( "procedural_generator" ) );
		wxAppConsole::SetVendorName( wxT( "dragonjoker" ) );
#if wxCHECK_VERSION( 2, 9, 0 )
		wxAppConsole::SetAppDisplayName( wxT( "Procedural Generator" ) );
		wxAppConsole::SetVendorDisplayName( wxT( "DragonJoker" ) );
#endif
		m_mainFrame = new MainFrame;
		SetTopWindow( m_mainFrame );
		m_mainFrame->Initialise();
		return true;
	}

	int Application::OnExit()
	{
		Translator::Cleanup( _T( "ProceduralGenerator" ) );
		return wxApp::OnExit();
	}
}
