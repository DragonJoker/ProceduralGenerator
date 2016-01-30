#include "GeneratorUtils.h"
#include "Size.h"

#include <locale>
#include <cstdarg>

#if defined( _WIN32 )
#	if defined( _MSC_VER )
#		pragma warning( push )
#		pragma warning( disable:4311 )
#		pragma warning( disable:4312 )
#	endif
#	include <Windows.h>
#	if defined( _MSC_VER )
#		pragma warning( pop )
#	endif
#endif

#if defined( __GNUG__ )
#	include <sys/time.h>
#	include <errno.h>
#	include <iostream>
#	include <unistd.h>
#	include <cerrno>
#	if !defined( _WIN32 )
#		include <X11/Xlib.h>
#	endif
#endif

namespace ProceduralTextures
{
	namespace System
	{
#if defined( _WIN32 )
		namespace
		{
			struct stSCREEN
			{
				uint32_t m_wanted;
				uint32_t m_current;
				Size & m_size;
			};

			BOOL CALLBACK MonitorEnum( HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData )
			{
				stSCREEN * l_screen = reinterpret_cast< stSCREEN * >( dwData );

				if ( l_screen && l_screen->m_current++ == l_screen->m_wanted )
				{
					l_screen->m_size = Size( lprcMonitor->right - lprcMonitor->left, lprcMonitor->bottom - lprcMonitor->top );
				}

				return FALSE;
			}
		}

		bool GetScreenSize( uint32_t p_screen, Size & p_size )
		{
			stSCREEN l_screen = { p_screen, 0, p_size };
			BOOL bRet = ::EnumDisplayMonitors( NULL, NULL, MonitorEnum, WPARAM( &l_screen ) );
			return true;
		}

		int GetCPUCount()
		{
			int l_iReturn = 1;
			SYSTEM_INFO sysinfo;
			GetSystemInfo( & sysinfo );
			l_iReturn = sysinfo.dwNumberOfProcessors;
			return l_iReturn;
		}

		String GetExecutableDirectory()
		{
			String l_return;
			char l_path[FILENAME_MAX];
			DWORD dwResult = ::GetModuleFileNameA( NULL, l_path, _countof( l_path ) );

			if ( dwResult != 0 )
			{
				l_return = StringUtils::ToString( l_path );
			}

			l_return = l_return.substr( 0, l_return.find_last_of( FOLDER_SEPARATOR ) );
			return l_return;
		}

#elif defined( __linux__ )

		bool GetScreenSize( uint32_t p_screen, Size & p_size )
		{
			bool l_bReturn = false;
			Display * pdsp = NULL;
			Screen * pscr = NULL;
			pdsp = XOpenDisplay( NULL );

			if ( !pdsp )
			{
			  std::cerr << "Failed to open default display." << std::endl;
			}
			else
			{
				pscr = DefaultScreenOfDisplay( pdsp );

				if ( !pscr )
				{
					std::cerr << "Failed to obtain the default screen of given display." << std::endl;
				}
				else
				{
					p_size = Size( pscr->width, pscr->height );
					l_bReturn = true;
				}

				XCloseDisplay( pdsp );
			}

			return l_bReturn;
		}

		int GetCPUCount()
		{
			int l_iReturn = 1;
			FILE * fp;
			char res[128];
			fp = popen( "/bin/cat /proc/cpuinfo |grep -c '^processor'", "r" );
			fread( res, 1, sizeof( res ) - 1, fp );
			fclose( fp );
			l_iReturn = atoi( res );
			return l_iReturn;
		}

		String GetExecutableDirectory()
		{
			String l_return;
			char l_path[FILENAME_MAX];
			char l_tmp[32];
			sprintf( l_tmp, "/proc/%d/exe", getpid() );
			int l_bytes = std::min< std::size_t >( readlink( l_tmp, l_path, sizeof( l_path ) ), sizeof( l_path ) - 1 );

			if ( l_bytes > 0 )
			{
				l_path[l_bytes] = '\0';
				l_return = StringUtils::ToString( l_path );
			}

			l_return = l_return.substr( 0, l_return.find_last_of( FOLDER_SEPARATOR ) );
			return l_return;
		}

#else
#	error "Unsupported platform"
#endif

		String GetDataDirectory()
		{
			String l_return = GetExecutableDirectory();
			l_return = l_return.substr( 0, l_return.find_last_of( FOLDER_SEPARATOR ) + 1 ) + _T( "share" ) + FOLDER_SEPARATOR + _T( "ProceduralGenerator" );
			return l_return;
		}
	}
}
