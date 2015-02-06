/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___IFS_ITERATED_FUNCTION_SYSTEM_H___
#define ___IFS_ITERATED_FUNCTION_SYSTEM_H___

#include "IfsCell.h"
#include "IfsGenerator.h"

#include <ConfigPanel.h>

#include <map>
#include <list>
#include <vector>

namespace ProceduralTextures
{
	class IteratedFunctionSystem
		: public ProceduralGenerator
	{
		class Thread
			: public ProceduralGenerator::Thread
		{
		public:
			Thread( ProceduralGenerator * p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight, const UbPixel & p_pxColour );
			virtual ~Thread();

			virtual void Step();

			void Initialise( IfsCellMap * p_pMapCells, IfsCellLst * p_pLstCells, IfsCellVct * p_pVctCells, PixelBuffer * p_pBackBuffer, IfsGenerator ** p_pGenerator );

		private:
			void StepMap();
			void StepLst();
			void StepVct();

		private:

			IfsCellMap * m_pMapCells;
			IfsCellMap m_mapGeneratedCells;
			IfsCellMap::iterator m_itMapCells1;

			IfsCellLst * m_pLstCells;
			IfsCellLst m_lstGeneratedCells;
			IfsCellLst::iterator m_itLstCells1;

			IfsCellVct * m_pVctCells;
			size_t m_uiCells1;
			size_t m_uiCellsSize;

			PixelBuffer * m_backBuffer;
			IfsGenerator ** m_pGenerator;
			bool m_bResult;
		};

	public:
		static wxString ArrayNames[eIFS_GENERATOR_COUNT];

	public:
		IteratedFunctionSystem( int p_width, int p_height, int p_iWndId, wxFrame * p_pFrame );
		virtual ~IteratedFunctionSystem();

		int	GetGeneratorShift()const
		{
			return m_pGenerator->GetShift();
		}
		int	GetGeneratorAngle()const
		{
			return m_pGenerator->GetAngle();
		}
		int	GetGeneratorSize()const
		{
			return m_pGenerator->GetSize();
		}
		void SetGeneratorSize( int val )
		{
			m_pGenerator->SetSize( val );
		}
		void SetGeneratorAngle( int val )
		{
			m_pGenerator->SetAngle( val );
		}
		void SetGeneratorShift( int val )
		{
			m_pGenerator->SetShift( val );
		}

		void SetGenerator( eIFS_GENERATOR p_eGenerator );

	private:
		virtual void DoGlInitialise() {}
		virtual void DoGlPreRender() {}
		virtual void DoGlRender( bool & WXUNUSED( p_bChanged ) ) {}
		virtual void DoGlPostRender() {}
		virtual void DoGlCleanup() {}
		virtual void DoGeneratePanel();

		void _initialise();

		typedef enum
		{
			eID_ANGLE		= 50,
			eID_SHIFT		= 51,
			eID_SIZE		= 52,
			eID_GENERATOR	= 53,
		}	eID;

		void OnAngle( wxCommandEvent & p_event );
		void OnShift( wxCommandEvent & p_event );
		void OnSizeIfs( wxCommandEvent & p_event );
		void OnGenerator( wxCommandEvent & p_event );

	private:
		IfsCellMap m_mapCells;
		IfsCellLst m_lstCells;
		IfsCellVct m_VctCells;
		IfsGenerator * m_pGenerator;
		IfsGenerator * m_arrayGenerators[eIFS_GENERATOR_COUNT];
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStaticAngle;
		SpecificControlParameters< eCONTROL_TYPE_EDIT > m_specEditAngle;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStaticShift;
		SpecificControlParameters< eCONTROL_TYPE_EDIT > m_specEditShift;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStaticSize;
		SpecificControlParameters< eCONTROL_TYPE_EDIT > m_specEditSize;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStaticGenerator;
		SpecificControlParameters< eCONTROL_TYPE_COMBO > m_specComboGenerator;
	};
}

#endif
