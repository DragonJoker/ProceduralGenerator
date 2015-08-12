/*
This source file is part of Castor3D (http://castor3d.developpez.com/castor3d.htm)

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

#ifndef ___GENERATOR_STRING_H___
#define ___GENERATOR_STRING_H___

#include <string>
#include <vector>
#include <locale>
#include <cstdint>
#include <sstream>

namespace ProceduralTextures
{
	typedef std::basic_string< char > String;
	typedef std::basic_stringstream< char > StringStream;
	typedef std::basic_ostream< char > OutputStream;
	typedef std::basic_istream< char > InputStream;
	typedef std::vector< String > StringArray;

	namespace StringUtils
	{
		namespace Utf8
		{
			static const unsigned char FirstBitMask = 0x80; // 1000000
			static const unsigned char SecondBitMask = 0x40; // 0100000
			static const unsigned char ThirdBitMask = 0x20; // 0010000
			static const unsigned char FourthBitMask = 0x10; // 0001000
			static const unsigned char FifthBitMask = 0x08; // 0000100
		}

		namespace details
		{
			/*!
			@author
				Sylvain Doremus
			@version
				2.0.0
			@date
				09/03/2015
			@brief
				Structure used to convert from a string type to another string type
			*/
			template< typename InChar, typename OutChar > struct StringConverter;

			/*!
			@author
				Sylvain Doremus
			@version
				2.0.0
			@date
				09/03/2015
			@brief
				Structure used to convert from a string type to another string type
			@remarks
				Specialization to convert from char strings to wchar_t strings
			*/
			template<> struct StringConverter< char, wchar_t >
			{
				/** Conversion function
				@param[in] p_strIn
					The input string
				@param[out] p_strOut
					The output string
				@param[in] p_locale
					The locale
				 */
				static void Convert( std::basic_string< char > const & p_strIn, std::basic_string< wchar_t > & p_strOut, std::locale const & p_locale = std::locale() )
				{
					if ( !p_strIn.empty() )
					{
						typedef std::codecvt< wchar_t, char, std::mbstate_t > facet_type;
						typedef facet_type::result result_type;
						const facet_type & facet = std::use_facet< facet_type >( p_locale );
						std::mbstate_t state = std::mbstate_t();
						std::vector< wchar_t > dst( p_strIn.size() * facet.max_length(), 0 );
						const char * endSrc = NULL;
						wchar_t * endDst = NULL;
						facet.in( state,
								  p_strIn.data(), p_strIn.data() + p_strIn.size(), endSrc,
								  &dst[0], &dst[0] + dst.size(), endDst
								);
						p_strOut = std::wstring( &dst.front(), endDst );
					}
				}
			};

			/*!
			@author
				Sylvain Doremus
			@version
				2.0.0
			@date
				09/03/2015
			@brief
				Structure used to convert from a string type to another string type
			@remarks
				Specialization to convert from wchar_t strings to char strings
			*/
			template<> struct StringConverter< wchar_t, char >
			{
				/** Conversion function
				@param[in] p_strIn
					The input string
				@param[out] p_strOut
					The output string
				@param[in] p_locale
					The locale
				 */
				static void Convert( std::basic_string< wchar_t > const & p_strIn, std::basic_string< char > & p_strOut, std::locale const & p_locale = std::locale() )
				{
					if ( !p_strIn.empty() )
					{
						typedef std::codecvt< wchar_t, char, std::mbstate_t > facet_type;
						typedef facet_type::result result_type;
						const facet_type & facet = std::use_facet< facet_type >( p_locale );
						std::mbstate_t state = std::mbstate_t();
						std::vector< char > dst( p_strIn.size() * facet.max_length(), 0 );
						const wchar_t * endSrc = NULL;
						char * endDst = NULL;
						facet.out( state,
								   p_strIn.data(), p_strIn.data() + p_strIn.size(), endSrc,
								   &dst[0], &dst[0] + dst.size(), endDst
								 );
						p_strOut = std::string( &dst.front(), endDst );
					}
				}
			};

			/*!
			@author
				Sylvain Doremus
			@version
				2.0.0
			@date
				09/03/2015
			@brief
				Structure used to convert from a string type to another string type
			@remarks
				Specialization used when no conversion is needed
			*/
			template< typename InChar >
			struct StringConverter< InChar, InChar >
			{
				/** Conversion function
				@param[in] p_strIn
					The input string
				@param[out] p_strOut
					The output string
				@param[in] p_locale
					The locale
				 */
				static void Convert( std::basic_string< InChar > const & p_strIn, std::basic_string< InChar > & p_strOut, std::locale const & p_locale = std::locale() )
				{
					p_strOut = p_strIn;
				}
			};
		}

		/** Puts a value into a String
		@param[in] p_in
			The value
		@return
			The String
		 */
		template< typename T >
		String ToString( T const & p_in )
		{
			StringStream l_stream;
			l_stream << p_in;
			return l_stream.str();
		}

		/** Converts a string from one type to another type
		@param[in] p_in
			The input string
		@return
			The converted string
		 */
		template< typename OChar, typename IChar >
		std::basic_string< OChar > StringCast( std::basic_string< IChar > const & p_in )
		{
			std::basic_string< OChar > l_return;
			details::StringConverter< IChar, OChar >::Convert( std::basic_string< IChar >( p_in ), l_return );
			return l_return;
		}

		/** Converts a string from one type to another type
		@param[in] p_in
			The input string
		@return
			The converted string
		 */
		template< typename OChar, typename IChar >
		std::basic_string< OChar > StringCast( IChar const * p_in )
		{
			std::basic_string< OChar > l_return;
			details::StringConverter< IChar, OChar >::Convert( std::basic_string< IChar >( p_in ), l_return );
			return l_return;
		}

		/** Retrieves a String from a std::string
		@param[in] p_str
			The std::string
		@return
			The String
		 */
		inline String FromStdString( std::string const & p_str )
		{
			return StringCast< char >( p_str );
		}

		/** Retrieves a String from a std::wstring
		@param[in] p_str
			The std::wstring
		@return
			The String
		 */
		inline String FromStdWString( std::wstring const & p_str )
		{
			return StringCast< char >( p_str );
		}

		/** Retrieves a std::string from a std::wstring
		@param[in] p_str
			The String
		@return
			The std::string
		 */
		inline std::string ToStdString( std::wstring const & p_str )
		{
			return StringCast< char >( p_str );
		}

		/** Retrieves a std::string from a std::string
		@param[in] p_str
			The String
		@return
			The std::string
		 */
		inline std::string ToStdString( std::string const & p_str )
		{
			std::string::iterator l_it;
			return p_str;
		}

		/** Retrieves a std::wstring from a std::string
		@param[in] p_str
			The String
		@return
			The std::wstring
		 */
		inline std::wstring ToStdWString( std::string const & p_str )
		{
			return StringCast< wchar_t >( p_str );
		}

		/** Retrieves a std::wstring from a std::wstring
		@param[in] p_str
			The String
		@return
			The std::wstring
		 */
		inline std::wstring ToStdWString( std::wstring const & p_str )
		{
			return p_str;
		}

		/** Retrieves an UTF-8 char32_t from a char array
		@param[in] p_value
			The integer char
		@return
			The UTF-8 char32_t character
		 */
		template< typename IteratorType >
		inline char32_t ToUtf8( IteratorType p_value )
		{
			char32_t l_return;
			char l_firstByte = *p_value;

			if ( l_firstByte & Utf8::FirstBitMask ) // This means the first byte has a value greater than 127, and so is beyond the ASCII range.
			{
				if ( l_firstByte & Utf8::ThirdBitMask ) // This means that the first byte has a value greater than 191, and so it must be at least a three-octet code point.
				{
					if ( l_firstByte & Utf8::FourthBitMask ) // This means that the first byte has a value greater than 224, and so it must be a four-octet code point.
					{
						l_return = ( l_firstByte & 0x07 ) << 18;
						char l_secondByte = *( p_value + 1 );
						l_return += ( l_secondByte & 0x3f ) << 12;
						char l_thirdByte = *( p_value + 2 );
						l_return += ( l_thirdByte & 0x3f ) << 6;;
						char l_fourthByte = *( p_value + 3 );
						l_return += ( l_fourthByte & 0x3f );
					}
					else
					{
						l_return = ( l_firstByte & 0x0f ) << 12;
						char l_secondByte = *( p_value + 1 );
						l_return += ( l_secondByte & 0x3f ) << 6;
						char l_thirdByte = *( p_value + 2 );
						l_return += ( l_thirdByte & 0x3f );
					}
				}
				else
				{
					l_return = ( l_firstByte & 0x1f ) << 6;
					char l_secondByte = *( p_value + 1 );
					l_return += ( l_secondByte & 0x3f );
				}
			}
			else
			{
				l_return = l_firstByte;
			}

			return l_return;
		}

		/*!
		@author
			Sylvain Doremus
		@version
			2.0.0
		@date
			09/03/2015
		@brief
			An iterator, to an UTF-8 string
		*/
		class Utf8Iterator
			: public std::iterator< std::bidirectional_iterator_tag, char32_t, std::string::difference_type, const char32_t *, const char32_t & >
		{
		public:
			/** Constructor
			@param[in] p_it
				The string iterator
			 */
			Utf8Iterator( String::const_iterator const & p_it )
				: m_it( p_it )
				, m_lastCodePoint( 0 )
				, m_dirty( true )
			{
			}

			/** Copy constructor
			@param[in] p_it
				The other object
			 */
			Utf8Iterator( Utf8Iterator const & p_it )
				: m_it( p_it.m_it )
				, m_lastCodePoint( p_it.m_lastCodePoint )
				, m_dirty( p_it.m_dirty )
			{
			}

			/** Destructor
			 */
			~Utf8Iterator()
			{
			}

			/** Assignment operator
			@param[in] p_it
				The string iterator
			@return
				A reference to this object
			 */
			Utf8Iterator & operator=( String::const_iterator const & p_it )
			{
				m_it = p_it;
				m_lastCodePoint = 0;
				m_dirty = true;
				return *this;
			}

			/** Assignment operator
			@param[in] p_it
				The string iterator
			@return
				A reference to this object
			 */
			Utf8Iterator & operator=( String::iterator const & p_it )
			{
				m_it = p_it;
				m_lastCodePoint = 0;
				m_dirty = true;
				return *this;
			}

			/** Copy assignment operator
			@param[in] p_it
				The other object
			@return
				A reference to this object
			 */
			Utf8Iterator & operator=( Utf8Iterator const & p_it )
			{
				m_it = p_it.m_it;
				m_lastCodePoint = p_it.m_lastCodePoint;
				m_dirty = p_it.m_dirty;
				return *this;
			}

			/** Assigment addition operator
			@param[in] p_offset
				The offset to add
			@return
				A reference to this object
			 */
			Utf8Iterator & operator+=( size_t p_offset )
			{
				while ( p_offset-- )
				{
					++( *this );
				}

				return *this;
			}

			/** Assignment subtraction operator
			@param[in] p_offset
				The offset to subtract
			@return
				A reference to this object
			 */
			Utf8Iterator & operator-=( size_t p_offset )
			{
				while ( p_offset-- )
				{
					--( *this );
				}

				return *this;
			}

			/** Pre-increment operator
			@return
				A reference to this object
			 */
			Utf8Iterator & operator++()
			{
				char l_firstByte = *m_it;
				++m_it;

				if ( l_firstByte & Utf8::FirstBitMask ) // This means the first byte has a value greater than 127, and so is beyond the ASCII range.
				{
					++m_it;

					if ( l_firstByte & Utf8::ThirdBitMask ) // This means that the first byte has a value greater than 224, and so it must be at least a three-octet code point.
					{
						++m_it;

						if ( l_firstByte & Utf8::FourthBitMask ) // This means that the first byte has a value greater than 240, and so it must be a four-octet code point.
						{
							++m_it;
						}
					}
				}

				m_dirty = true;
				return *this;
			}

			/** Post-increment operator
			@return
				A copy of this iterator, befor increment
			 */
			Utf8Iterator operator++( int )
			{
				Utf8Iterator temp = *this;
				++( *this );
				return temp;
			}

			/** Pre-decrement operator
			@return
				A reference to this object
			 */
			Utf8Iterator & operator--()
			{
				--m_it;

				if ( *m_it & Utf8::FirstBitMask ) // This means that the previous byte is not an ASCII character.
				{
					--m_it;

					if ( ( *m_it & Utf8::SecondBitMask ) == 0 )
					{
						--m_it;

						if ( ( *m_it & Utf8::SecondBitMask ) == 0 )
						{
							--m_it;
						}
					}
				}

				m_dirty = true;
				return *this;
			}

			/** Post-decrement operator
			@return
				A copy of this iterator, befor dencrement
			 */
			Utf8Iterator operator--( int )
			{
				Utf8Iterator temp = *this;
				--( *this );
				return temp;
			}

			/** Access to the current codepoint value
			@return
				The current codepoint value
			 */
			char32_t operator*()const
			{
				DoCalculateCurrentCodePoint();
				return m_lastCodePoint;
			}

			/** Equality operator
			 */
			bool operator==( const Utf8Iterator & p_it )const
			{
				return m_it == p_it.m_it;
			}

			/** Equality operator
			 */
			bool operator==( const String::const_iterator & p_it )const
			{
				return m_it == p_it;
			}

			/** Equality operator
			 */
			bool operator==( const String::iterator & p_it )const
			{
				return m_it == p_it;
			}

			/** Difference operator
			 */
			bool operator!=( const Utf8Iterator & p_it )const
			{
				return m_it != p_it.m_it;
			}

			/** Difference operator
			 */
			bool operator!=( const String::const_iterator & p_it )const
			{
				return m_it != p_it;
			}

			/** Difference operator
			 */
			bool operator!=( const String::iterator & p_it )const
			{
				return m_it != p_it;
			}

			/** Retrieves the internal iterator
			 */
			String::const_iterator internal()const
			{
				return m_it;
			}

		private:
			/** Computes the cached codepoint
			 */
			void DoCalculateCurrentCodePoint()const
			{
				if ( m_dirty )
				{
					m_lastCodePoint = ToUtf8( m_it );
					m_dirty = false;
				}
			}

			//! The internal iterator
			String::const_iterator m_it;
			//! The last computed codepoint
			mutable char32_t m_lastCodePoint;
			//! Tells the codepoint needs recomputing
			mutable bool m_dirty;
		};

		/** Addition operator
		@param[in] p_it
			The iterator
		@param[in] p_offset
			The offset to add
		@return
			A reference to this object
		 */
		inline Utf8Iterator operator+( Utf8Iterator p_it, size_t p_offset )
		{
			Utf8Iterator l_it( p_it );
			l_it += p_offset;
			return l_it;
		}

		/** Subtraction operator
		@param[in] p_it
			The iterator
		@param[in] p_offset
			The offset to subtract
		@return
			A reference to this object
		 */
		inline Utf8Iterator operator-( Utf8Iterator p_it, size_t p_offset )
		{
			Utf8Iterator l_it( p_it );
			l_it -= p_offset;
			return l_it;
		}
	}
}

#undef _T
#define _T( x ) x

#endif
