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
#ifndef ___GENERATOR_SIGNAL_H___
#define ___GENERATOR_SIGNAL_H___

#include "GeneratorPrerequisites.h"

#include <functional>

namespace ProceduralTextures
{
	/*!
	@author
		Sylvain Doremus
	@version
		2.0.0
	@date
		10/02/2015
	@brief
		Basic signal class
	*/
	template< typename Function >
	class Signal
	{
	public:
		/** Connects a new function that will be called if the signal is raised
		@param[in] p_function
			The function
		@return
			The function index
		*/
		uint32_t Connect( Function p_function )
		{
			uint32_t l_return = uint32_t( m_slots.size() ) + 1;
			m_slots.insert( std::make_pair( l_return, p_function ) );
			return l_return;
		}

		/** Disconnects a function
		@param[in] p_index
			The function index
		*/
		void Disconnect( uint32_t p_index )
		{
			auto it = m_slots.find( p_index );

			if ( it != m_slots.end() )
			{
				m_slots.erase( it );
			}
		}

		/** Raise the signal, calls every connected function
		*/
		void operator()()
		{
			for ( auto it = m_slots.begin(); it != m_slots.end(); ++it )
			{
				it->second();
			}
		}

#if ( _MSC_VER && _MSC_VER > 1700 ) || ( !_MSC_VER )

		/** Raise the signal, calls every connected function
		@param[in] p_params
			The function parameters
		*/
		template< typename ... Params >
		void operator()( Params && ... p_params )
		{
			for ( auto it = m_slots.begin(); it != m_slots.end(); ++it )
			{
				it->second( std::move( p_params )... );
			}
		}

#else

		/** Raise the signal, calls every connected function
		@param[in] param1
			The function parameter
		*/
		template< typename Param1 >
		void operator()( Param1 && param1 )
		{
			for ( auto it = m_slots.begin(); it != m_slots.end(); ++it )
			{
				it->second( std::move( param1 ) );
			}
		}

		/** Raise the signal, calls every connected function
		@param[in] param1
			The first function parameter
		@param[in] param2
			The second function parameter
		*/
		template< typename Param1, typename Param2 >
		void operator()( Param1 && param1, Param2 && param2 )
		{
			for ( auto it = m_slots.begin(); it != m_slots.end(); ++it )
			{
				it->second( std::move( param1 ), std::move( param2 ) );
			}
		}

		/** Raise the signal, calls every connected function
		@param[in] param1
			The first function parameter
		@param[in] param2
			The second function parameter
		@param[in] param3
			The third function parameter
		*/
		template< typename Param1, typename Param2, typename Param3 >
		void operator()( Param1 && param1, Param2 && param2, Param3 && param3 )
		{
			for ( auto it = m_slots.begin(); it != m_slots.end(); ++it )
			{
				it->second( std::move( param1 ), std::move( param2 ), std::move( param3 ) );
			}
		}

		/** Raise the signal, calls every connected function
		@param[in] param1
			The first function parameter
		@param[in] param2
			The second function parameter
		@param[in] param3
			The third function parameter
		@param[in] param4
			The fourth function parameter
		*/
		template< typename Param1, typename Param2, typename Param3, typename Param4 >
		void operator()( Param1 && param1, Param2 && param2, Param3 && param3, Param4 && param4 )
		{
			for ( auto it = m_slots.begin(); it != m_slots.end(); ++it )
			{
				it->second( std::move( param1 ), std::move( param2 ), std::move( param3 ), std::move( param4 ) );
			}
		}

		/** Raise the signal, calls every connected function
		@param[in] param1
			The first function parameter
		@param[in] param2
			The second function parameter
		@param[in] param3
			The third function parameter
		@param[in] param4
			The fourth function parameter
		@param[in] param5
			The fifth function parameter
		*/
		template< typename Param1, typename Param2, typename Param3, typename Param4, typename Param5 >
		void operator()( Param1 && param1, Param2 && param2, Param3 && param3, Param4 && param4, Param5 && param5 )
		{
			for ( auto it = m_slots.begin(); it != m_slots.end(); ++it )
			{
				it->second( std::move( param1 ), std::move( param2 ), std::move( param3 ), std::move( param4 ), std::move( param5 ) );
			}
		}

		/** Raise the signal, calls every connected function
		@param[in] param1
			The first function parameter
		@param[in] param2
			The second function parameter
		@param[in] param3
			The third function parameter
		@param[in] param4
			The fourth function parameter
		@param[in] param5
			The fifth function parameter
		@param[in] param6
			The sixth function parameter
		*/
		template< typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6 >
		void operator()( Param1 && param1, Param2 && param2, Param3 && param3, Param4 && param4, Param5 && param5, Param6 && param6 )
		{
			for ( auto it = m_slots.begin(); it != m_slots.end(); ++it )
			{
				it->second( std::move( param1 ), std::move( param2 ), std::move( param3 ), std::move( param4 ), std::move( param5 ), std::move( param6 ) );
			}
		}

#endif

	private:
		std::map< uint32_t, Function > m_slots;
	};
}

#endif
