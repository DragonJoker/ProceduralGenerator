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
#ifndef ___GENERATOR_SPECIFIC_CONTROL_PARAMETERS_H___
#define ___GENERATOR_SPECIFIC_CONTROL_PARAMETERS_H___

#include "SpecificControlParametersBase.h"

namespace ProceduralTextures
{
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\brief		Specialisation of Specifics for wxStaticText
	*/
	template<> class GeneratorAPI SpecificControlParameters< eCONTROL_TYPE_STATIC >
		: public SpecificControlParametersBase
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_strText	The control label
		 */
		SpecificControlParameters( wxString const & p_strText )
			:	SpecificControlParametersBase( eCONTROL_TYPE_STATIC )
			,	m_strText( p_strText )
			,	m_pControl( NULL )
		{
		}

	public:
		//! The label
		wxString m_strText;
		//! The static control
		wxStaticText * m_pControl;
	};
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\brief		Specialisation of Specifics for wxTextCtrl
	*/
	template<> class GeneratorAPI SpecificControlParameters< eCONTROL_TYPE_EDIT >
		: public SpecificControlParametersBase
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_strText	The control initial caption
		 */
		SpecificControlParameters( wxString const & p_strText )
			:	SpecificControlParametersBase( eCONTROL_TYPE_EDIT )
			,	m_strText( p_strText )
			,	m_pControl( NULL )
		{
		}

	public:
		//! The initial caption
		wxString m_strText;
		//! The text control
		wxTextCtrl * m_pControl;
	};
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\brief		Specialisation of Specifics for wxSlider
	*/
	template<> class GeneratorAPI SpecificControlParameters< eCONTROL_TYPE_SLIDER >
		: public SpecificControlParametersBase
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_iMin		The minimal value
		 *\param[in]	p_iMax		The maximal value
		 *\param[in]	p_iValue	The initial value
		 */
		SpecificControlParameters( int p_iMin, int p_iMax, int p_iValue )
			:	SpecificControlParametersBase( eCONTROL_TYPE_SLIDER )
			,	m_iMin( p_iMin )
			,	m_iMax( p_iMax )
			,	m_iValue( p_iValue )
			,	m_pControl( NULL )
		{
		}

	public:
		//! The minimal value for the slider
		int m_iMin;
		//! The maximal value for the slider
		int m_iMax;
		//! The initial value for the slider
		int m_iValue;
		//! The slider control
		wxSlider * m_pControl;
	};
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\brief		Specialisation of Specifics for wxComboBox
	*/
	template<> class GeneratorAPI SpecificControlParameters< eCONTROL_TYPE_COMBO >
		: public SpecificControlParametersBase
	{
	public:
		/**
		 *\brief		Constructor
		 */
		SpecificControlParameters()
			:	SpecificControlParametersBase( eCONTROL_TYPE_COMBO )
			,	m_pControl( NULL )
		{
		}
		/**
		 *\brief		Constructor
		 *\param[in]	p_strValue		The initial value
		 *\param[in]	p_arrayValues	The possible values array
		 */
		SpecificControlParameters( wxString const & p_strValue, wxArrayString const & p_arrayValues )
			:	SpecificControlParametersBase( eCONTROL_TYPE_COMBO )
			,	m_pControl( NULL )
		{
			if ( !p_arrayValues.empty() )
			{
				Initialise( p_strValue, &p_arrayValues[0], p_arrayValues.size() );
			}
			else
			{
				Initialise( p_strValue, NULL, 0 );
			}
		}
		/**
		 *\brief		Constructor
		 *\param[in]	p_strValue		The initial value
		 *\param[in]	p_arrayValues	The possible values array
		 */
		template< size_t N >
		SpecificControlParameters( wxString const & p_strValue, wxString const( & p_arrayValues )[N] )
			:	SpecificControlParametersBase( eCONTROL_TYPE_COMBO )
			,	m_pControl( NULL )
		{
			Initialise( p_strValue, p_arrayValues, N );
		}
		/**
		 *\brief		Constructor
		 *\param[in]	p_strValue		The initial value
		 *\param[in]	p_arrayValues	The possible values array
		 *\param[in]	p_uiCount		The possible values count
		 */
		SpecificControlParameters( wxString const & p_strValue, wxString const * p_arrayValues, size_t p_uiCount )
			:	SpecificControlParametersBase( eCONTROL_TYPE_COMBO )
			,	m_pControl( NULL )
		{
			Initialise( p_strValue, p_arrayValues, p_uiCount );
		}
		/**
		 *\brief		Initialises the combo strings
		 *\param[in]	p_strValue		The initial value
		 *\param[in]	p_arrayValues	The possible values array
		 */
		void Initialise( wxString const & p_strValue, wxArrayString const & p_arrayValues )
		{
			if ( !p_arrayValues.empty() )
			{
				Initialise( p_strValue, &p_arrayValues[0], p_arrayValues.size() );
			}
			else
			{
				Initialise( p_strValue, NULL, 0 );
			}
		}
		/**
		 *\brief		Initialises the combo strings
		 *\param[in]	p_strValue		The initial value
		 *\param[in]	p_arrayValues	The possible values array
		 */
		template< size_t N >
		void Initialise( wxString const & p_strValue, wxString const( & p_arrayValues )[N] )
		{
			Initialise( p_strValue, p_arrayValues, N );
		}
		/**
		 *\brief		Initialises the combo strings
		 *\param[in]	p_strValue		The initial value
		 *\param[in]	p_arrayValues	The possible values array
		 *\param[in]	p_uiCount		The possible values count
		 */
		void Initialise( wxString const & p_strValue, wxString const * p_arrayValues, size_t p_uiCount )
		{
			m_arrayValues.clear();
			m_strValue = p_strValue;

			if ( p_uiCount && p_arrayValues )
			{
				m_arrayValues.insert( m_arrayValues.end(), &p_arrayValues[0], &p_arrayValues[p_uiCount] );
			}
		}

	public:
		//! The initial value of the combo box
		wxString m_strValue;
		//! The possible values array
		wxArrayString m_arrayValues;
		// The combo box control
		wxComboBox * m_pControl;
	};
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\brief		Specialisation of Specifics for wxButton
	*/
	template<> class GeneratorAPI SpecificControlParameters< eCONTROL_TYPE_BUTTON >
		: public SpecificControlParametersBase
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_strText		The button caption
		 */
		SpecificControlParameters( wxString const & p_strText )
			:	SpecificControlParametersBase( eCONTROL_TYPE_BUTTON )
			,	m_strText( p_strText )
			,	m_pControl( NULL )
		{
		}

	public:
		//! The button caption
		wxString m_strText;
		//! The button control
		wxButton * m_pControl;
	};
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\brief		Specialisation of Specifics for wxButton
	*/
	template<> class GeneratorAPI SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON >
		: public SpecificControlParametersBase
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_strText		The button caption
		 */
		SpecificControlParameters( wxString const & p_strText )
			:	SpecificControlParametersBase( eCONTROL_TYPE_FILE_BUTTON )
			,	m_strText( p_strText )
			,	m_pControl( NULL )
		{
		}

	public:
		//! The button caption
		wxString m_strText;
		//! The file selector extension wildcard
		wxString m_strWildcard;
		//! The button control
		wxButton * m_pControl;
	};
}

#endif
