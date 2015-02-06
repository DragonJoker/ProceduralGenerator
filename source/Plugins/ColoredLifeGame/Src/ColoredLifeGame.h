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
#ifndef ___CLG_COLORED_LIFE_GAME_H___
#define ___CLG_COLORED_LIFE_GAME_H___

#include "ClgCell.h"

#include <ConfigPanel.h>
#include <Buffer.h>

namespace ProceduralTextures
{
	typedef Buffer< ClgCell > ClgCellBuffer;

	class ColoredLifeGame
		: public ProceduralGenerator
	{
	private:
		typedef enum
		{
			eRedValue	= 50,
			eGreenValue	= 51,
			eBlueValue	= 52,
		}
		eIDs;

		class Thread
			: public ProceduralGenerator::Thread
		{
		public:
			Thread( ProceduralGenerator * p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight, const UbPixel & p_pxColour );
			virtual ~Thread();

			virtual void Step();

			inline void SetCells( ClgCellBuffer * p_cells, ClgCellBuffer * p_oldCells )
			{
				m_cells = p_cells;
				m_oldCells = p_oldCells;
			}

		private:
			int _getAliveNeightbours( int p_x, int p_y );

		private:
			ClgCellBuffer * m_cells;
			ClgCellBuffer * m_oldCells;
		};

	public:
		ColoredLifeGame( int p_width, int p_height, int p_iWndId, wxFrame * p_pFrame );
		virtual ~ColoredLifeGame();

		virtual void SwapBuffers();

		virtual void DoSetRed( uint8_t val );
		virtual void DoSetGreen( uint8_t val );
		virtual void DoSetBlue( uint8_t val );

	private:
		virtual void DoGlInitialise() {}
		virtual void DoGlPreRender() {}
		virtual void DoGlRender( bool & WXUNUSED( p_bChanged ) ) {}
		virtual void DoGlPostRender() {}
		virtual void DoGlCleanup() {}
		virtual void DoGeneratePanel();

		void OnSliderRed( wxCommandEvent & p_event );
		void OnSliderGreen( wxCommandEvent & p_event );
		void OnSliderBlue( wxCommandEvent & p_event );

	private:
		ClgCellBuffer m_cells;
		ClgCellBuffer m_oldCells;
		UbPixel m_deadPixel;
		UbPixel m_alivePixel;
		UiPixel m_stepPixel;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStatR;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStatG;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStatB;
		SpecificControlParameters< eCONTROL_TYPE_SLIDER > m_specSlideR;
		SpecificControlParameters< eCONTROL_TYPE_SLIDER > m_specSlideG;
		SpecificControlParameters< eCONTROL_TYPE_SLIDER > m_specSlideB;
	};
}

#endif
