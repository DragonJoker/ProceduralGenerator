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
#ifndef ___DLA_DIFFUSED_LIMITED_AGGREGATION_H___
#define ___DLA_DIFFUSED_LIMITED_AGGREGATION_H___

#include "DlaCell.h"

#include <ConfigPanel.h>
#include <Plugin.h>

namespace ProceduralTextures
{
	typedef Buffer< DlaCell > DlaCellBuffer;

	class DiffusionLimitedAggregation
		: public ProceduralGenerator
	{
	private:
		typedef enum
		{
			eID_MASS		= 50,
			eID_MINVALUE	= 51,
		}	eID;

		class Thread
			: public ProceduralGenerator::Thread
		{
		public:
			Thread( ProceduralGenerator * p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight, const UbPixel & p_pxColour );
			virtual ~Thread();

			virtual void Step();

			inline void SetBuffer( DlaCellBuffer * p_cells )
			{
				m_cells = p_cells;
			}

			void _getPointOnCircle( Point2i & p_ptPoint, const Point2i & p_ptOrigin, float p_fRadius );
			void _setCellWhite( const Point2i & p_ptPoint );

		public:
			DlaCellBuffer * m_cells;
			std::vector < bool > m_passed;
			int m_iKillCircle;
			int m_iJumpCircle;
			int m_iBoundingCircle;
			Point2i m_ptOrigin;
			int m_iMinValue;
			int * m_iCurrentMass;
			double m_dMaxDistance;
		};

	public:
		DiffusionLimitedAggregation( int p_width, int p_height, int p_iWndId, wxFrame * p_pFrame );
		virtual ~DiffusionLimitedAggregation();

		virtual void SwapBuffers();

		inline int GetMinValue()const
		{
			return m_iMinValue;
		}
		inline int GetMass()const
		{
			return m_iMass;
		}
		inline void SetMass( int val )
		{
			m_iMass = val;
		}
		inline void SetMinValue( int val )
		{
			m_iMinValue = val;
		}

	private:
		virtual void DoGlInitialise() {}
		virtual void DoGlPreRender() {}
		virtual void DoGlRender( bool & WXUNUSED( p_bChanged ) ) {}
		virtual void DoGlPostRender() {}
		virtual void DoGlCleanup() {}
		virtual void DoGeneratePanel();

		void _setCellWhite( const Point2i & p_ptPoint );

		void OnMinValue( wxCommandEvent	&	p_event );
		void OnMass( wxCommandEvent	&	p_event );

	public:
		DlaCellBuffer m_cells;
		float m_fStickCoeff;
		int m_iKillCircle;
		int m_iJumpCircle;
		int m_iBoundingCircle;
		Point2i m_ptOrigin;
		int m_iMinValue;
		int m_iMass;
		int m_iCurrentMass;
		double m_dMaxDistance;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStatMassTitle;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStatMass;
		SpecificControlParameters< eCONTROL_TYPE_SLIDER > m_specSlideMass;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStatMinValueTitle;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStatMinValue;
		SpecificControlParameters< eCONTROL_TYPE_SLIDER > m_specSlideMinValue;
	};
}

#endif
