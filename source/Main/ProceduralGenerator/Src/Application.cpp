#include "Application.h"

#include <GeneratorUtils.h>

#include <wx/sysopt.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>

#include <fstream>

IMPLEMENT_APP( ProceduralGenerator::Application );

using namespace ProceduralTextures;

namespace ProceduralGenerator
{
	namespace
	{
		template< typename LogStreambufTraits >
		class LogStreambuf
			: public std::streambuf
		{
		public:
			using CharType = char;
			using string_type = std::basic_string< CharType >;
			using ostream_type = std::basic_ostream< CharType >;
			using streambuf_type = std::basic_streambuf< CharType >;
			using int_type = typename std::basic_streambuf< CharType >::int_type;
			using traits_type = typename std::basic_streambuf< CharType >::traits_type;

			LogStreambuf( string_type const & appName, ostream_type & stream )
				: m_stream( stream )
				, m_appName{ appName }
			{
				m_old = m_stream.rdbuf( this );
			}

			~LogStreambuf()
			{
				m_stream.rdbuf( m_old );
			}

		private:
			int_type overflow( int_type c = traits_type::eof() )override
			{
				if ( traits_type::eq_int_type( c, traits_type::eof() ) )
				{
					doSync();
				}
				else if ( c == '\n' )
				{
					doSync();
				}
				else if ( c == '\r' )
				{
					m_buffer += '\r';
					doSyncNoNl();
				}
				else
				{
					m_buffer += traits_type::to_char_type( c );
				}

				return c;
			}

			int doSync()
			{
				if ( !m_buffer.empty() )
				{
					LogStreambufTraits::Log( m_appName, m_buffer );
					m_buffer.clear();
				}

				return 0;
			}

			int doSyncNoNl()
			{
				if ( !m_buffer.empty() )
				{
					LogStreambufTraits::Log( m_appName, m_buffer );
				}

				return 0;
			}

		private:
			string_type m_buffer;
			ostream_type & m_stream;
			streambuf_type * m_old;
			string_type m_appName;
		};

		enum class LogType
		{
			eDebug,
			eInfo,
			eWarning,
			eError,
		};

		void logDebugString( std::string const & log );

		template< LogType Type >
		struct TLogStreambufTraits
		{
			using CharType = char;
			using string_type = std::basic_string< CharType >;

			static void Log( string_type const & appName
				, string_type const & text )
			{
				static std::string const LogName[]
				{
					"Debug",
					"Info",
					"Warning",
					"Error",
				};
				auto log = appName + " - " + LogName[size_t( Type )] + ": " + text + "\n";
				printf( "%s", log.c_str() );
				logDebugString( log );
			}
		};

		using DebugLogStreambufTraits = TLogStreambufTraits< LogType::eDebug >;
		using InfoLogStreambufTraits = TLogStreambufTraits< LogType::eInfo >;
		using WarningLogStreambufTraits = TLogStreambufTraits< LogType::eWarning >;
		using ErrorLogStreambufTraits = TLogStreambufTraits< LogType::eError >;

		std::string const LogFileName = "Debug.log";

		void logDebugString( std::string const & log )
		{
#if _MSC_VER
			::OutputDebugStringA( log.c_str() );
#endif
			std::tm today = { 0 };
			time_t ttime;
			time( &ttime );
#if _MSC_VER
			localtime_s( &today, &ttime );
#else
			today = *localtime( &ttime );
#endif			
			char buffer[33] = { 0 };
			strftime( buffer, 32, "%Y-%m-%d %H:%M:%S", &today );
			std::string timeStamp = buffer;

			std::ofstream file{ LogFileName, std::ios::app };

			if ( file )
			{
				file << timeStamp << " - " << log;
			}
		}

		void flushLogFile()
		{
			if ( wxFileExists( LogFileName ) )
			{
				wxRemoveFile( LogFileName );
			}
		}
	}

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
		flushLogFile();
		m_cout = new LogStreambuf< InfoLogStreambufTraits >( "Procedural Generator"
			, std::cout );
		m_cerr = new LogStreambuf< ErrorLogStreambufTraits >( "Procedural Generator"
			, std::cerr );
		m_clog = new LogStreambuf< DebugLogStreambufTraits >( "Procedural Generator"
			, std::clog );
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
		delete m_cout;
		delete m_cerr;
		delete m_clog;
		return wxApp::OnExit();
	}
}
