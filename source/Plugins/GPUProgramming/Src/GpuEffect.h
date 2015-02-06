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
#ifndef ___GPU_EFFECT_H___
#define ___GPU_EFFECT_H___

#include <GeneratorPrerequisites.h>

namespace ProceduralTextures
{
	class Effect
	{
	public:
		Effect( OpenGl * p_pOpenGl, int p_iWidth, int p_iHeight );
		virtual ~Effect();

		void SetVertexFile( const wxString & p_strPath );
		void SetFragmentFile( const wxString & p_strPath );
		void Compile();

		void Initialise();
		void Cleanup();

		bool Activate( wxPoint const & p_ptMousePosition );
		void Deactivate();

		wxString GetCompilerLog( eSHADER_OBJECT_TYPE p_eType );
		wxString GetLinkerLog();

		inline void ResetTime()
		{
			m_llStartTime = wxGetLocalTimeMillis();
		}
		inline bool IsInitialised()const
		{
			return !m_bNewShader;
		}

	private:
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
		GlFrameVariable< int > * m_uniformMouseX;
		GlFrameVariable< int > * m_uniformMouseY;
	};
}

#endif
