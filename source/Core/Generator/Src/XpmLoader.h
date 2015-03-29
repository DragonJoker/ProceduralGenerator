/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/ )

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___GENERATOR_XPM_IMAGE_H___
#define ___GENERATOR_XPM_IMAGE_H___

#include "Buffer.h"

namespace ProceduralTextures
{
	/*!
	\author		Sylvain DOREMUS
	\version	2.0.0
	\date		23/02/2015
	\brief		Font resource
	\remark		Representation of a font : face, precision, and others
				<br />The generated font will be put in an image
	*/
	class GeneratorAPI XpmLoader
	{
	public:
		/**
		 *\brief		Constructor
		 */
		XpmLoader();
		/**
		 *\brief		Destructor
		 */
		~XpmLoader();
		/**
		 *\brief		Loads the image data
		 *\param[in]	p_path	The file data
		 *\param[in]	p_size	The data size
		 *\return		The image buffer
		 */
		PixelBuffer LoadImage( char const ** p_data, size_t p_size );
	};
}

#endif
