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
#ifndef ___GENERATOR_TRANSLATOR_H___
#define ___GENERATOR_TRANSLATOR_H___

#include "PlatformConfig.h"

namespace ProceduralTextures
{
	/*!
	@author
		Sylvain Doremus
	@version
		2.0.0
	@date
		15/02/2015
	@brief
		Class used to retrieve translations from a .mo file
	*/
	class LocalisedText
	{
	public:
		/** Constructor
		 */
		LocalisedText();

		/** Destructor
		 */
		~LocalisedText();

		/** Loads the .mo file
		@param[in] p_path
			The translations file name
		 */
		void Initialise( String const & p_path );

		/** Cleans the memory
		 */
		void Cleanup();

		/** Retrieves the source string at given index
		@param[in] p_index
			The string index
		 */
		String GetSourceString( int p_index );

		/** Retrieves the translated string at given index
		@param[in] p_index
			The string index
		 */
		String GetTranslatedString( int p_index );

		/** Retrieves the index for given source string
		@param[in] p_string
			The string
		 */
		int GetTargetIndex( String const & p_string );

	private:
		/** Loads the file in memory
		@param[in] p_file
			The file address
		 */
		void DoDumpFile( String const & p_file );

		/** Swaps endianness for the given int
		@param[in] p_value
			The value
		 */
		int DoSwap4( int p_value );

		/** Reads four bytes from the file and swaps endianness if needed
		@param[in] p_offset
			The file offset
		 */
		int DoRead4( int p_offset );

		/** Checks if the given source text matches the source text at given index
		@param[in] p_string
			The source text
		@param[in] p_index
			The index
		 */
		bool DoLabelMatches( String const & p_string, int p_index );

	private:
		//! The .mo file dump
		std::vector< uint8_t > m_moData;
		//! Tells if the .mo file has another endianness than the one of the current system
		int m_reversed;
		//! The strings count
		int m_numStrings;
		//! The file offset for the source strings table
		int m_originalTableOffset;
		//! The file offset for the translated strings table
		int m_translatedTableOffset;
		//! The hashes count
		int m_hashNumEntries;
		//! The file offset for the hashes
		int m_hashOffset;
	};
	/*!
	@author
		Sylvain Doremus
	@version
		2.0.0
	@date
		15/02/2015
	@brief
		Class used to translate strings
	*/
	class GeneratorAPI Translator
	{
	public:
		/** Initialises the translation manager
		@param[in] p_path
			The translations folder
		@param[in] p_parent
			The translation parent project name
		@param[in] p_name
			The translation name
		 */
		static void Initialise( String const & p_path, String const & p_parent, String const & p_name );

		/** Cleans the translation manager up
		 */
		static void Cleanup();

		/** Translates a string
		@param[in] p_txt
			The string to translate
		@return
			The translated string
		 */
		static String TranslateString( char const * p_txt );

		/** Translates a string
		@param[in] p_txt
			The string to translate
		@return
			The translated string
		 */
		static String TranslateString( String const & p_txt )
		{
			return p_txt.c_str();
		}

	private:
		static std::vector< LocalisedText > m_translations;
	};
}

#undef _
#define _( x ) ProceduralTextures::Translator::TranslateString( _T( x ) )

#endif
