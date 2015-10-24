/*
This source file is part of Castor3D (http://castor3d.developpez.com/castor3d.htm)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (At your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___GENERATOR_PLATFORM_CONFIG_H___
#define ___GENERATOR_PLATFORM_CONFIG_H___

#if defined( _WIN32 )
#	if !defined( Generator_SHARED )
#		define GeneratorAPI
#	else
#		if defined( Generator_EXPORTS )
#			define GeneratorAPI __declspec( dllexport )
#		else
#			define GeneratorAPI __declspec( dllimport )
#		endif
#	endif
#else
#	define GeneratorAPI
#endif

#include <string>
#include <cstdint>

#include "GeneratorString.h"

#if defined( _WIN32 )
#	if defined( _MSC_VER )
#		define DLL_PREFIX _T( "" )
#	else
#		define DLL_PREFIX _T( "lib" )
#	endif
#	define DLL_EXT _T( "dll" )
#	define dlerror() ::GetLastError()
#	define PGEN_CALLBACK __stdcall
#else
#	define DLL_EXT _T( "so" )
#	define DLL_PREFIX _T( "lib" )
#	define PGEN_CALLBACK
#endif

#if !defined( __linux__ ) && !defined( _WIN32 )
#	error "Yet unsupported OS"
#endif

#endif
