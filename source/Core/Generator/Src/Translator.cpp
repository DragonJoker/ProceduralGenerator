#include "Translator.h"

#include "GeneratorUtils.h"

#include <locale>
#include <cstdarg>
#include <fstream>

namespace ProceduralTextures
{
	namespace
	{
		inline unsigned long hashpjw( const char * str_param )
		{
			const uint32_t HASHWORDBITS = 32;
			uint32_t hval = 0;
			uint32_t g;
			const char * s = str_param;

			while ( *s )
			{
				hval <<= 4;
				hval += uint8_t( *s++ );
				g = hval & ( uint32_t( 0xF ) << ( HASHWORDBITS - 4 ) );

				if ( g )
				{
					hval ^= g >> ( HASHWORDBITS - 8 );
					hval ^= g;
				}
			}

			return hval;
		}
	}

	//************************************************************************************************

	LocalisedText::LocalisedText()
		: m_numStrings( 0 )
		, m_originalTableOffset( 0 )
		, m_translatedTableOffset( 0 )
		, m_hashNumEntries( 0 )
		, m_hashOffset( 0 )
		, m_reversed( -1 )
	{
	}

	LocalisedText::~LocalisedText()
	{
	}

	void LocalisedText::Initialise( String const & p_path )
	{
		DoDumpFile( p_path );

		// There has to be at least this much in the header...
		if ( m_moData.size() >= 24 )
		{
			const uint32_t TARGET_MAGIC = 0x950412DE;
			const uint32_t TARGET_MAGIC_REVERSED = 0xDE120495;

			uint32_t * l_data = reinterpret_cast< uint32_t * >( m_moData.data() );
			uint32_t l_magic = l_data[0];

			if ( l_magic == TARGET_MAGIC )
			{
				m_reversed = 0;
			}
			else if ( l_magic == TARGET_MAGIC_REVERSED )
			{
				m_reversed = 1;
			}

			if ( m_reversed >= 0 )
			{
				m_numStrings = DoRead4( 8 );
				m_originalTableOffset = DoRead4( 12 );
				m_translatedTableOffset = DoRead4( 16 );
				m_hashNumEntries = DoRead4( 20 );
				m_hashOffset = DoRead4( 24 );
			}
		}
		else
		{
			TRACE( _T( "Couldn't load the translation file " ) + p_path + _T( "\n" ) );
		}
	}

	void LocalisedText::Cleanup()
	{
		m_moData.clear();
		m_numStrings = 0;
		m_originalTableOffset = 0;
		m_translatedTableOffset = 0;
		m_hashNumEntries = 0;
		m_hashOffset = 0;
		m_reversed = -1;
	}

	String LocalisedText::GetSourceString( int p_index )
	{
		int l_addrOffset = m_originalTableOffset + 8 * p_index + 4;
		int l_stringOffset = DoRead4( l_addrOffset );
		return String( reinterpret_cast< char * >( &m_moData[l_stringOffset] ) );
	}

	String LocalisedText::GetTranslatedString( int p_index )
	{
		int l_addrOffset = m_translatedTableOffset + 8 * p_index + 4;
		int l_stringOffset = DoRead4( l_addrOffset );
		return String( reinterpret_cast< char * >( &m_moData[l_stringOffset] ) );
	}

	int LocalisedText::GetTargetIndex( String const & p_string )
	{
		int l_return = -1;

		if ( m_hashNumEntries )
		{
			unsigned long V = hashpjw( StringUtils::ToStdString( p_string ).c_str() );
			int S = m_hashNumEntries;

			int l_hashCursor = V % S;
			int l_origHashCursor = l_hashCursor;
			int l_increment = 1 + ( V % ( S - 2 ) );
			bool l_continue = true;

			while ( l_continue )
			{
				uint32_t l_index = DoRead4( m_hashOffset + 4 * l_hashCursor );

				if ( l_index )
				{
					l_index--;// Because entries in the table are stored +1 so that 0 means empty.

					if ( DoLabelMatches( p_string, l_index ) )
					{
						l_continue = false;
						l_return = l_index;
					}
					else
					{
						l_hashCursor += l_increment;
						l_hashCursor %= S;

						if ( l_hashCursor == l_origHashCursor )
						{
							l_continue = false;
						}
					}
				}
				else
				{
					l_continue = false;
				}
			}
		}

		return l_return;
	}

	void LocalisedText::DoDumpFile( String const & p_file )
	{
		std::ifstream l_fin;
		l_fin.open( StringUtils::ToStdString( p_file ).c_str(), std::ios::binary );

		if ( l_fin.is_open() )
		{
			l_fin.seekg( 0, std::ios::end );
			std::streampos l_length = l_fin.tellg();
			l_fin.seekg( 0, std::ios::beg );

			if ( l_length )
			{
				m_moData.resize( size_t( l_length ) );
				l_fin.read( reinterpret_cast< char * >( m_moData.data() ), l_length );
			}
		}
	}

	int LocalisedText::DoSwap4( int p_value )
	{
		unsigned long l_c0 = ( p_value >>  0 ) & 0xff;
		unsigned long l_c1 = ( p_value >>  8 ) & 0xff;
		unsigned long l_c2 = ( p_value >> 16 ) & 0xff;
		unsigned long l_c3 = ( p_value >> 24 ) & 0xff;
		return ( l_c0 << 24 ) | ( l_c1 << 16 ) | ( l_c2 << 8 ) | l_c3;
	}

	int LocalisedText::DoRead4( int p_offset )
	{
		uint32_t l_ret = *reinterpret_cast< uint32_t * >( &m_moData[p_offset] );

		if ( m_reversed )
		{
			l_ret = DoSwap4( l_ret );
		}

		return l_ret;
	}

	bool LocalisedText::DoLabelMatches( String const & p_string, int p_index )
	{
		return GetSourceString( p_index ) == p_string;
	}

	//************************************************************************************************

	std::vector< LocalisedText > Translator::m_translations;

	void Translator::Initialise( String const & p_path, String const & p_parent, String const & p_name )
	{
		setlocale( LC_ALL, "" );
		String l_locale = StringUtils::ToString( setlocale( LC_ALL, NULL ) );

		if ( l_locale.find( _T( "French" ) ) != String::npos )
		{
			l_locale = _T( "fr" );
		}
		else if ( l_locale.find( _T( "fr_FR" ) ) != String::npos )
		{
			l_locale = _T( "fr" );
		}

		String l_path = p_path;
		std::size_t l_index = l_path.rfind( FOLDER_SEPARATOR + _T( "bin" ) );

		if ( l_index != String::npos )
		{
			l_path = l_path.substr( 0, l_index );
		}

		LocalisedText l_translation;
		l_translation.Initialise( l_path + FOLDER_SEPARATOR + _T( "share" ) + FOLDER_SEPARATOR + p_parent + FOLDER_SEPARATOR + l_locale + FOLDER_SEPARATOR + p_name + _T( ".mo" ) );
		m_translations.push_back( l_translation );
	}

	void Translator::Cleanup()
	{
		for ( auto && l_translation : m_translations )
		{
			l_translation.Cleanup();
		}

		m_translations.clear();
	}

	String Translator::TranslateString( char const * p_txt )
	{
		String l_return = p_txt;
		auto && l_it = m_translations.begin();
		int l_index = -1;

		while ( l_index == -1 && l_it != m_translations.end() )
		{
			l_index = l_it->GetTargetIndex( l_return );

			if ( l_index != -1 )
			{
				l_return = l_it->GetTranslatedString( l_index );
			}

			++l_it;
		}

		return l_return;
	}
}
