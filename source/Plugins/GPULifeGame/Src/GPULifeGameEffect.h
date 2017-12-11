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
#ifndef ___GpuLifeGameEffect_H___
#define ___GpuLifeGameEffect_H___

#include <GlHolder.h>

namespace GpuLifeGame
{
	class Effect
		: public ProceduralTextures::gl::Holder
	{
	public:
		Effect( ProceduralTextures::gl::OpenGl & p_openGl, int p_iWidth, int p_iHeight );
		virtual ~Effect();

		void Compile();

		void Initialise();
		void Cleanup();

		bool Activate( ProceduralTextures::Position const & p_ptMousePosition );
		void Deactivate();

		ProceduralTextures::String GetCompilerLog( ProceduralTextures::gl::eSHADER_OBJECT_TYPE p_eType );
		ProceduralTextures::String GetLinkerLog();
		bool IsInitialised()const;

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
		std::shared_ptr< ProceduralTextures::gl::ShaderProgram > m_pShaderProgram;
		bool m_bNewShader;
		ProceduralTextures::Clock::time_point m_llStartTime;
		int m_iWidth;
		int m_iHeight;
		std::weak_ptr< ProceduralTextures::gl::OpenGl > m_pOpenGl;
		std::weak_ptr< ProceduralTextures::gl::FrameVariable< int > > m_uniformWidth;
		std::weak_ptr< ProceduralTextures::gl::FrameVariable< int > > m_uniformHeight;
		std::weak_ptr< ProceduralTextures::gl::FrameVariable< int > > m_uniformTime;
		//! The vertex attribute "vertex" location in the program
		uint32_t m_vertexAttribLocation;
		//! The vertex attribute "texture" location in the program
		uint32_t m_textureAttribLocation;
	};
}

#endif
