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
#ifndef ___OCL_EFFECT_H___
#define ___OCL_EFFECT_H___

#include <GeneratorPrerequisites.h>
#include <Size.h>

#include <OpenGl.h>

#include "cl.hpp"

#include <vector>
#include <map>
#include <string>

namespace OCLProcessing
{
	namespace ocl
	{
		bool CheckErr( cl_int p_iErr, ProceduralTextures::String const & p_text, bool p_throws = false );
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
		Effect( cl::Context const & p_clContext, cl::Device const & p_clDevice, cl::CommandQueue const & p_clQueue, std::shared_ptr< ProceduralTextures::PixelBuffer > p_pBuffer );
		virtual ~Effect();

		bool LoadFile( ProceduralTextures::String const & p_strPath );
		bool LoadKernel( ProceduralTextures::String const & p_strName );
		void Compile();

		void Initialise();
		void Resize( ProceduralTextures::Size const & p_size );
		void Cleanup();

		bool Apply( eSEPARATION p_eType, int p_iOffset );

		ProceduralTextures::String GetCompilerLog();
		void UpdateImages();

		inline void ResetTime()
		{
			m_llStartTime = ProceduralTextures::Clock::now();
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
		cl::Context m_clContext;
		cl::Device m_clDevice;
		cl::CommandQueue m_clQueue;
		cl::Program m_clProgram;
		cl::Kernel m_clKernel;
		ProceduralTextures::String m_strFile;
		ProceduralTextures::String m_strKernel;
		bool m_bNewProgram;
		ProceduralTextures::Clock::time_point m_llStartTime;
		ProceduralTextures::Size m_size;
		bool m_changed;
		bool m_bLoaded;
		cl::Image2D m_inputImage2DBuffer;
		cl::Image2D m_outputImage2DBuffer;
		std::weak_ptr< ProceduralTextures::PixelBuffer > m_pInputBuffer;
		int m_iArgsCount;
		std::vector< stKERNEL > m_arrayKernels;
		bool m_bProgramLoaded;
		bool m_bKernelLoaded;
		std::vector< stKERNEL >::iterator m_itKernel;
	};
}

#endif
