/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/)

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
#ifndef ___SFX_EFFECT_H___
#define ___SFX_EFFECT_H___

#include <GlHolder.h>
#include <Size.h>

namespace ShaderEffects
{
	typedef enum
	{
		eSEPARATION_NONE,
		eSEPARATION_HORIZ,
		eSEPARATION_VERT,
	}	eSEPARATION;

	class Effect
		: public ProceduralTextures::gl::Holder
	{
	public:
		Effect( ProceduralTextures::gl::OpenGl & p_openGl, std::shared_ptr< ProceduralTextures::PixelBuffer > p_buffer );
		virtual ~Effect();

		void SetVertexFile( ProceduralTextures::String const & p_strPath );
		void SetFragmentFile( ProceduralTextures::String const & p_strPath );
		void Compile();

		void Initialise();
		void Cleanup();

		bool Activate( eSEPARATION p_eSeparationType, int p_iSeparationOffset );
		void Deactivate();

		ProceduralTextures::String GetCompilerLog( ProceduralTextures::gl::eSHADER_OBJECT_TYPE p_eType );
		ProceduralTextures::String GetLinkerLog();

		bool IsInitialised()const;
		void UpdateImages();

		inline void ResetTime()
		{
			m_llStartTime = ProceduralTextures::Clock::now();
		}
		inline uint32_t GetVertex()const
		{
			return m_vertexAttribLocation;
		}
		inline uint32_t GetTexture()const
		{
			return m_textureAttribLocation;
		}

	private:
		std::weak_ptr< ProceduralTextures::PixelBuffer > m_buffer;
		std::shared_ptr< ProceduralTextures::gl::ShaderProgram > m_pShaderProgram;
		ProceduralTextures::String m_strVertexFile;
		ProceduralTextures::String m_strFragmentFile;
		bool m_bNewShader;
		ProceduralTextures::Clock::time_point m_llStartTime;
		ProceduralTextures::Size m_size;
		std::weak_ptr< ProceduralTextures::gl::FrameVariable< int > > m_uniformWidth;
		std::weak_ptr< ProceduralTextures::gl::FrameVariable< int > > m_uniformHeight;
		std::weak_ptr< ProceduralTextures::gl::FrameVariable< int > > m_uniformTime;
		std::weak_ptr< ProceduralTextures::gl::FrameVariable< int > > m_uniformSepType;
		std::weak_ptr< ProceduralTextures::gl::FrameVariable< int > > m_uniformSepOffset;
		//! The vertex attribute "vertex" location in the program
		uint32_t m_vertexAttribLocation;
		//! The vertex attribute "texture" location in the program
		uint32_t m_textureAttribLocation;
	};
}

#endif
