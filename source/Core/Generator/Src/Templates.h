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
#ifndef ___GENERATOR_VALUE_H___
#define ___GENERATOR_VALUE_H___

#include "GeneratorPrerequisites.h"

// disable: "< type > needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#   pragma warning (disable : 4251 )
// disable: "non dll-interface class used as base for dll-interface class"
// Happens when deriving from Singleton because bug in compiler ignores
// template export
#   pragma warning (disable : 4275 )

namespace ProceduralTextures
{
	namespace details
	{
		/*!
		@author
			Sylvain DOREMUS
		@date
			19/10/2011
		@brief
			Functor used to delete an object with for_each
		*/
		template < class Object >
		struct ObjectDeleter
		{
			/** Deletes an object
			@param[in] p_pPointer
				The object pointer
			*/
			inline void operator()( Object * p_pPointer )
			{
				delete p_pPointer;
			}
		};

		/** Deletes all container's objects
		*/
		template < class Container, class Object >
		inline void ClearContent( Container &, Object )
		{
		}

		/** Deletes all container's objects
		@param[in] p_container
			The container
		*/
		template < class Container, class Object >
		inline void ClearContent( Container & p_container, Object * )
		{
			std::for_each( p_container.begin(), p_container.end(), ObjectDeleter< Object >() );
		}

		/*!
		@author
			Sylvain DOREMUS
		@date
			19/10/2011
		@brief
			Functor used to delete a pair with for_each
		*/
		template< typename KeyType, typename ObjType >
		struct PairDeleter
		{
			/** Deletes a pair's second object
			@param[in] p_pair
				The pair
			*/
			inline void operator()( std::pair< KeyType, ObjType * > p_pair )
			{
				delete p_pair.second;
			}
		};

		/** Deletes all container's pairs
		*/
		template< class CtnrType, typename KeyType, typename ObjType >
		inline void ClearPairContent( CtnrType &, std::pair< KeyType, ObjType > )
		{
		}

		/** Deletes all container's pairs
		@param[in] p_container
			The container
		*/
		template< class CtnrType, typename KeyType, typename ObjType >
		inline void ClearPairContent( CtnrType & p_container, std::pair< KeyType, ObjType * > )
		{
			std::for_each( p_container.begin(), p_container.end(), PairDeleter< KeyType, ObjType >() );
		}
	}

	/** Clears a container
	@remarks
			Deallocates all the content if needed
	@param[in] p_container
		The container to clear
	*/
	template < class Container >
	inline void ClearContainer( Container & p_container )
	{
		typedef typename Container::value_type value_type;
		details::ClearContent( p_container, value_type() );
		Container().swap( p_container );
	}

	/** Clears a pair container (like std::map)
	@remarks
			Deallocates all the content if needed
	@param[in] p_container
		The container to clear
	*/
	template< class CtnrType >
	inline void ClearPairContainer( CtnrType & p_container )
	{
		typedef typename CtnrType::value_type value_type;
		details::ClearPairContent( p_container, value_type() );
		CtnrType().swap( p_container );
	}

	/*!
	@author
		Sylvain DOREMUS
	@date
		19/10/2011
	@brief
		Struct used to select best way to put type in parameter : 'value' or 'const reference'
	*/
	template< typename T >
	struct CallTraits
	{
	private:
		template < typename U, bool Big > struct CallTraitsImpl;
		/*!
		@author
			Sylvain DOREMUS
		@date
			19/10/2011
		@brief
			Struct used to select 'reference' parameter type
		*/
		template< typename U >
		struct CallTraitsImpl< U, true >
		{
			typedef U const & ConstType;
			typedef U & Type;
		};
		/*!
		@author
			Sylvain DOREMUS
		@date
			19/10/2011
		@brief
			Struct used to select 'value' parameter type
		*/
		template< typename U >
		struct CallTraitsImpl< U, false >
		{
			typedef U const ConstType;
			typedef U Type;
		};

	public:
		/*!
		@author
			Sylvain DOREMUS
		@date
			19/10/2011
		@brief
			Typedef over the best way to put type in parameter : 'value' or 'reference'
		*/
		typedef typename CallTraitsImpl < T, ( sizeof( T ) > sizeof( ptrdiff_t ) ) >::Type ParamType;
		/*!
		@author
			Sylvain DOREMUS
		@date
			19/10/2011
		@brief
			Typedef over the best way to put type in parameter : 'const value' or 'const reference'
		*/
		typedef typename CallTraitsImpl < T, ( sizeof( T ) > sizeof( ptrdiff_t ) ) >::ConstType ConstParamType;
	};
}

#endif
