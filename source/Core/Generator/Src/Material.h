/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/ )

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (At your option ) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___GENERATOR_MATERIAL_H___
#define ___GENERATOR_MATERIAL_H___

#include "Pixel.h"

namespace ProceduralTextures
{
	/*!
	@author
		Sylvain DOREMUS
	@date
		02/03/2015
	@version
			2.0.0
	@brief
		Simmple structure holding material data
	*/
	class Material
	{
	public:
		/** Constructor
		 */
		Material();

		/** Constructor, for colour materials
		@param[in] p_colour
			The wanted colour
		 */
		Material( Colour const & p_colour );

		/** Constructor, for texture materials
		@param[in] p_texture
			The wanted texture
		 */
		Material( std::shared_ptr< gl::Texture > p_texture );

		/** Activates the material
		 */
		void Activate();

		/** Deactivates the material
		 */
		void Deactivate();

		/** Retrieves the material colour
		@return
			The value
		 */
		void SetType( eMATERIAL_TYPE p_type, std::shared_ptr< gl::ShaderProgram > p_program );

		/** Sets the material colour
		@param[in] p_colour
			The new value
		 */
		void SetColour( Colour const & p_colour );

		/** Sets the material texture
		@param[in] p_texture
			The new value
		 */
		void SetTexture( std::shared_ptr< gl::Texture > p_texture );

		/** Retrieves the material colour
		@return
			The value
		 */
		inline eMATERIAL_TYPE GetType()const
		{
			return m_type;
		}

		/** Retrieves the material colour
		@return
			The value
		 */
		inline Colour const & GetColour()const
		{
			return m_colour;
		}

		/** Retrieves the material texture
		@return
			The value
		 */
		inline std::shared_ptr< gl::Texture > GetTexture()const
		{
			return m_texture;
		}

		/** Retrieves the material vertex attribute
		@return
			The value
		 */
		inline uint32_t GetVertexAttribute()const
		{
			return m_vertexAttribute;
		}

		/** Retrieves the material texture attribute
		@return
			The value
		 */
		inline uint32_t GetTextureAttribute()const
		{
			return m_textureAttribute;
		}

	private:
		//! The material type
		eMATERIAL_TYPE m_type;
		//! The shader program
		std::weak_ptr< gl::ShaderProgram > m_program;
		//! The vertex attribute "vertex" location in the program
		uint32_t m_vertexAttribute;
		//! The vertex attribute "texture" location in the program
		uint32_t m_textureAttribute;
		//! The texture, for texture material
		std::shared_ptr< gl::Texture > m_texture;
		//! The texture uniform variable
		std::weak_ptr< gl::FrameVariable< int > > m_uniformTexture;
		//! The colour, for colour material
		Colour m_colour;
		//! The colour uniform variable
		std::weak_ptr< gl::Vec4FrameVariable< float > > m_uniformColour;
	};
}

#endif
