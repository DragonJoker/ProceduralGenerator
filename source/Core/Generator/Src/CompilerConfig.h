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
#ifndef ___GENERATOR_COMPILER_CONFIG_H___
#define ___GENERATOR_COMPILER_CONFIG_H___

#undef HAS_VARIADIC_TEMPLATES
#undef HAS_DELEGATED_CONSTRUCTORS
#undef HAS_MAKE_UNIQUE
#undef HAS_INITIALIZER_LISTS

#if defined( _MSC_VER )
#	if _MSC_VER >= 1800
#		define HAS_VARIADIC_TEMPLATES		1
#		define HAS_DELEGATING_CONSTRUCTORS	1
#		define HAS_MAKE_UNIQUE				1
#		define HAS_INITIALIZER_LISTS		1
#	else
#		define HAS_VARIADIC_TEMPLATES		0
#		define HAS_DELEGATING_CONSTRUCTORS	0
#		define HAS_MAKE_UNIQUE				0
#		define HAS_INITIALIZER_LISTS		0
#	endif
#elif defined( __clang__)
#	define HAS_VARIADIC_TEMPLATES			__has_feature(cxx_variadic_templates)
#	define HAS_DELEGATING_CONSTRUCTORS		__has_feature(cxx_delegating_constructors)
#	define HAS_MAKE_UNIQUE					__has_feature(cxx_variadic_templates)
#	define HAS_INITIALIZER_LISTS			__has_feature(cxx_generalized_initializers)
#elif defined( __GNUG__)
#	define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#	if GCC_VERSION >= 40700
#		define HAS_VARIADIC_TEMPLATES		1
#		define HAS_DELEGATING_CONSTRUCTORS	1
#		define HAS_MAKE_UNIQUE				1
#		define HAS_INITIALIZER_LISTS		1
#	elif GCC_VERSION >= 40300
#		define HAS_VARIADIC_TEMPLATES		1
#		define HAS_DELEGATING_CONSTRUCTORS	0
#		define HAS_MAKE_UNIQUE				1
#		define HAS_INITIALIZER_LISTS		0
#	else
#		define HAS_VARIADIC_TEMPLATES		0
#		define HAS_DELEGATING_CONSTRUCTORS	0
#		define HAS_MAKE_UNIQUE				0
#	endif
#elif defined( __BORLANDC__ )
#	warning "Theorically supported compiler, but untested yet"
#	if __BORLANDC__ >= 0x621
#		define HAS_VARIADIC_TEMPLATES		0
#		define HAS_DELEGATING_CONSTRUCTORS	0
#		define HAS_MAKE_UNIQUE				0
#		define HAS_INITIALIZER_LISTS		0
#	endif
#else
#	error "Yet unsupported compiler"
#endif

#endif
