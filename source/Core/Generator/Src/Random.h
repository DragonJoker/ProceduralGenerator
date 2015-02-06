/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/ )

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option ) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___GENERATOR_RANDOM_H___
#define ___GENERATOR_RANDOM_H___

namespace ProceduralTextures
{
	/*!
	\author		Sylvain DOREMUS
	\date		14/02/2010
	\brief		Pseudorandom number generator
	*/
	class GeneratorAPI Random
	{
	public:
		/**
		 *\brief		Initialize the generator from a seed
		 */
		static void srand( int seed );
		/**
		 *\brief		Extract a tempered pseudorandom number based on the index-th value, calling generateNumbers() every 624 numbers
		 */
		static int rand();

	private:
		/**
		 *\brief		Generate an array of 624 untempered numbers
		 */
		static void _generateNumbers();

	private:
		//! Create a length 624 array to store the state of the generator
		static int MT[624];
		static int Index;
	};
}

#endif
