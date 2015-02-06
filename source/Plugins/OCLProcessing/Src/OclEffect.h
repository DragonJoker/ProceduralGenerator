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
#ifndef ___OCL_EFFECT_H___
#define ___OCL_EFFECT_H___

#include <GeneratorPrerequisites.h>
#include "cl.hpp"

#include <vector>
#include <map>
#include <string>

namespace ProceduralTextures
{
	namespace ocl
	{
		bool CheckErr( cl_int p_iErr, const char * p_szName );
	}

	typedef enum
	{
		eSEPARATION_NONE,
		eSEPARATION_HORIZ,
		eSEPARATION_VERT,
	}	eSEPARATION;

	struct stKERNEL
	{
		std::string strName;
		std::map< std::string, int > mapParams;
	};

	class Effect
	{
	public:
		Effect( cl::Context const & p_clContext, cl::Device const & p_clDevice, cl::CommandQueue const & p_clQueue, wxImage * p_pImage, PixelBuffer * p_pBuffer );
		virtual ~Effect();

		bool LoadFile( wxString const & p_strPath );
		bool LoadKernel( wxString const & p_strName );
		void Compile();
		void SetImagePath( size_t p_iImage, wxString const & p_strImagePath );

		void Initialise();
		void Cleanup();

		bool Apply( eSEPARATION p_eType, int p_iOffset );

		wxString GetCompilerLog();
		wxString GetLinkerLog();

		void UpdateImages();

		inline void ResetTime()
		{
			m_llStartTime = wxGetLocalTimeMillis();
		}
		inline bool IsInitialised()const
		{
			return !m_bNewProgram;
		}
		inline size_t GetKernelCount()const
		{
			return m_arrayKernels.size();
		}
		inline std::string GetKernelName( size_t i )const
		{
			return m_arrayKernels[i].strName;
		}

	private:
		void DoInitialiseImages();
		void DoCleanupImages();

	private:
		wxImage * m_pImage;
		std::vector< wxImage * > m_arrayImages;
		std::vector< wxString > m_arrayImagesFiles;
		cl::Context m_clContext;
		cl::Device m_clDevice;
		cl::CommandQueue m_clQueue;
		cl::Program m_clProgram;
		cl::Kernel m_clKernel;
		wxString m_strFile;
		wxString m_strKernel;
		bool m_bNewProgram;
		wxMilliClock_t m_llStartTime;
		int m_iWidth;
		int m_iHeight;
		bool m_bLoaded;
		cl::Image2D m_inputImage2DBuffer;
		cl::Image2D m_outputImage2DBuffer;
		wxImage * m_outputImage;
		PixelBuffer * m_pInputBuffer;
		int m_iArgsCount;
		std::vector< stKERNEL > m_arrayKernels;
		bool m_bProgramLoaded;
		bool m_bKernelLoaded;
		std::vector< stKERNEL >::iterator m_itKernel;
	};
}

#endif
