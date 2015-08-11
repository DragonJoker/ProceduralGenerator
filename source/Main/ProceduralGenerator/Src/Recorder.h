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
#ifndef ___Pg_Recorder___
#define ___Pg_Recorder___

#include <GeneratorPrerequisites.h>

namespace ProceduralGenerator
{
	class Recorder
	{
	public:
		class IRecorderImpl
		{
		public:
			IRecorderImpl(){}
			virtual ~IRecorderImpl(){}
			virtual bool StartRecord( ProceduralTextures::Size const & p_size ) = 0;
			virtual bool IsRecording() = 0;
			virtual bool UpdateTime() = 0;
			virtual bool RecordFrame( ProceduralTextures::PixelBuffer const & p_buffer, ProceduralTextures::Size const & p_size ) = 0;
			virtual void StopRecord() = 0;
		};

	public:
		Recorder();
		~Recorder();

		inline bool StartRecord( ProceduralTextures::Size const & p_size )
		{
			bool l_result = !IsRecording();

			if ( l_result )
			{
				l_result = m_impl->StartRecord( p_size );
			}

			return l_result;
		}

		inline bool IsRecording()
		{
			return m_impl->IsRecording();
		}

		inline bool UpdateTime()
		{
			return m_impl->UpdateTime();
		}

		inline bool RecordFrame( ProceduralTextures::PixelBuffer const & p_buffer, ProceduralTextures::Size const & p_size )
		{
			return m_impl->RecordFrame( p_buffer, p_size );
		}

		inline void StopRecord()
		{
			if ( IsRecording() )
			{
				m_impl->StopRecord();
			}
		}

	private:
		std::unique_ptr< IRecorderImpl > m_impl;
	};
}

#endif
