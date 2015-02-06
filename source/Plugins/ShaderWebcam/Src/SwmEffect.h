/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/)

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
#ifndef ___SWM_EFFECT_H___
#define ___SWM_EFFECT_H___

#if defined( _MSC_VER )
#	pragma warning( push )
#	pragma warning( disable:4311 )	// 'cast de type' : troncation de pointeur de 'PVOID' à 'LONG'
#	pragma warning( disable:4312 )	// 'cast de type' : la conversion de 'LONG' en 'PVOID' d'une taille supérieure
#	pragma warning( disable:4267 )	// 'argument' : conversion de 'size_t' en 'unsigned int', perte possible de données
#endif

#include <GeneratorPrerequisites.h>

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

#if CV_INTVERSION <= 20400
namespace cv
{
	static const int CAP_PROP_FPS			= CV_CAP_PROP_FPS;
	static const int CAP_PROP_FRAME_WIDTH	= CV_CAP_PROP_FRAME_WIDTH;
	static const int CAP_PROP_FRAME_HEIGHT	= CV_CAP_PROP_FRAME_HEIGHT;
}
#endif

namespace ProceduralTextures
{
	typedef enum
	{
		eSEPARATION_NONE,
		eSEPARATION_HORIZ,
		eSEPARATION_VERT,
	}	eSEPARATION;


	class Effect
	{
	public:
		Effect( OpenGl * p_pOpenGl, wxImage * p_pImage );
		virtual ~Effect();

		void SetVertexFile( const wxString & p_strPath );
		void SetFragmentFile( const wxString & p_strPath );
		void Compile();
		void SetImagePath( size_t p_iImage, const wxString & p_strImagePath );

		void Initialise();
		void Cleanup();

		bool Activate( eSEPARATION p_eSeparationType, int p_iSeparationOffset );
		void Deactivate();

		wxString GetCompilerLog( eSHADER_OBJECT_TYPE p_eType );
		wxString GetLinkerLog();

		void UpdateImages();

		inline void ResetTime()
		{
			m_llStartTime = wxGetLocalTimeMillis();
		}
		inline bool IsInitialised()const
		{
			return !m_bNewShader;
		}

	private:
		wxImage * m_pImage;
		std::vector< wxImage * > m_arrayImages;
		std::vector< wxString > m_arrayImagesFiles;
		GlShaderProgram * m_pShaderProgram;
		wxString m_strVertexFile;
		wxString m_strFragmentFile;
		bool m_bNewShader;
		wxMilliClock_t m_llStartTime;
		int m_iWidth;
		int m_iHeight;
		OpenGl * m_pOpenGl;
		GlFrameVariable< int > * m_uniformWidth;
		GlFrameVariable< int > * m_uniformHeight;
		GlFrameVariable< int > * m_uniformTime;
		GlFrameVariable< int > * m_uniformSepType;
		GlFrameVariable< int > * m_uniformSepOffset;
	};
}

#if defined( _MSC_VER )
#	pragma warning( pop )
#endif

#endif
