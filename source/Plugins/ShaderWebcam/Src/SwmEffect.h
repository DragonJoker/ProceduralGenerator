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
#ifndef ___SWM_EFFECT_H___
#define ___SWM_EFFECT_H___

#if defined( _MSC_VER )
#	pragma warning( push )
#	pragma warning( disable:4311 )	// 'cast de type' : troncation de pointeur de 'PVOID' à 'LONG'
#	pragma warning( disable:4312 )	// 'cast de type' : la conversion de 'LONG' en 'PVOID' d'une taille supérieure
#	pragma warning( disable:4267 )	// 'argument' : conversion de 'size_t' en 'unsigned int', perte possible de données
#endif

#include <GlHolder.h>

#if defined( new )
#	undef new
#endif

#if defined( True )
#   undef True
#endif

#if defined( False )
#   undef False
#endif

#include <opencv2/opencv.hpp>

#define CV_INTVERSION	((CV_MAJOR_VERSION * 10000) + (CV_MINOR_VERSION * 100) + CV_SUBMINOR_VERSION)

#if CV_INTVERSION <= 20500
namespace cv
{
	const int CAP_PROP_FPS			= CV_CAP_PROP_FPS;
	const int CAP_PROP_FRAME_WIDTH	= CV_CAP_PROP_FRAME_WIDTH;
	const int CAP_PROP_FRAME_HEIGHT	= CV_CAP_PROP_FRAME_HEIGHT;
}
#endif

namespace ShaderWebcam
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
		Effect( std::shared_ptr< ProceduralTextures::gl::OpenGl > p_pOpenGl, ProceduralTextures::Size const & p_size );
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
		ProceduralTextures::String m_strVertexFile;
		ProceduralTextures::String m_strFragmentFile;
		bool m_bNewShader;
		ProceduralTextures::Clock::time_point m_llStartTime;
		ProceduralTextures::Size m_size;
		std::weak_ptr< ProceduralTextures::gl::OpenGl > m_pOpenGl;
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

#if defined( _MSC_VER )
#	pragma warning( pop )
#endif

#endif
