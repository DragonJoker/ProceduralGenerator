#include "GeneratorPch.h"

#include "Pixel.h"

namespace ProceduralTextures
{
	namespace System
	{
		int GetCPUCount()
		{
			int l_iReturn = 1;
#if defined( __linux__ )
			FILE * fp;
			char res[128];
			fp = popen( "/bin/cat /proc/cpuinfo |grep -c '^processor'", "r" );
			fread( res, 1, sizeof( res ) - 1, fp );
			fclose( fp );
			l_iReturn = atoi( res );
#elif defined( _WIN32 )
			SYSTEM_INFO sysinfo;
			GetSystemInfo( & sysinfo );
			l_iReturn = sysinfo.dwNumberOfProcessors;
#endif
			return l_iReturn;
		}
	}
}
