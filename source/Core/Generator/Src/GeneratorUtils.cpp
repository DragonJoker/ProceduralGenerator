#include "GeneratorUtils.h"

namespace ProceduralTextures
{
	bool utils::Trace( wxString const & msg )
	{
#if defined( _MSC_VER )
		::OutputDebugString( msg.c_str() );
#endif
#if defined( _UNICODE )
		std::wcout << msg.c_str();
#else
		std::cout << msg.c_str();
#endif
		return true;
	}

	bool utils::Trace( char const * p_szFormat, ... )
	{
		char l_szBuffer[1000];
		va_list l_vaList;
		va_start( l_vaList, p_szFormat );
		vsprintf( l_szBuffer, p_szFormat, l_vaList );
		va_end( l_vaList );
#if defined( _MSC_VER )
		::OutputDebugStringA( l_szBuffer );
#endif
		std::cout << l_szBuffer;
		return true;
	}

	bool utils::Trace( wchar_t const * p_szFormat, ... )
	{
		wchar_t l_szBuffer[1000];
		va_list l_vaList;
		va_start( l_vaList, p_szFormat );
#if defined( _MSC_VER )
		wvsprintf( l_szBuffer, p_szFormat, l_vaList );
#else
		wxVsprintf( l_szBuffer, p_szFormat, l_vaList );
#endif
		va_end( l_vaList );
#if defined( _MSC_VER )
		::OutputDebugStringW( l_szBuffer );
#endif
		std::wcout << l_szBuffer;
		return true;
	}
}
