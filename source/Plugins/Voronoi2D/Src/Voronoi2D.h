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
#ifndef ___V2D_GENERATOR_H___
#define ___V2D_GENERATOR_H___

#include <Generator.h>

#include "V2dCpuStep.h"
#include <GpuStep.h>

namespace Voronoi2D
{
	/*!
	@author
		Sylvain DOREMUS
	@date
		23/05/2012
	@brief
		Voronoi 2D generator
	*/
	class Generator
		: public ProceduralTextures::Generator< CpuStep, ProceduralTextures::DefaultGpuStep >
	{
	private:
		/*!
		@author
			Sylvain DOREMUS
		@date
			23/05/2012
		@brief
			The controls IDs
		*/
		typedef enum
		{
			eID_ANY				= -1,
			eID_DEPTH			= 50,
			eID_NEIGHBOURS		= 51,
			eID_DISTANCE_FUNC	= 52,
			eID_MINKOWSKI_ORDER	= 53,
			eID_RESET			= 54,
		}	eID;

	public:
		/** Constructor
		 */
		Generator();
		/** Destructor
		 */
		virtual ~Generator();

	private:
		/** @copydoc ProceduralTexture::Generator::DoCreate
		 */
		virtual void DoCreate( ProceduralTextures::Size const & p_size, ProceduralTextures::Size const & p_bordersSize );
		/** @copydoc ProceduralTexture::Generator::DoDestroy
		 */
		virtual void DoDestroy();
		/** @copydoc ProceduralTexture::Generator::DoGeneratePanel
		 */
		virtual void DoGeneratePanel();
		/** Sets the maximum depth
		@remarks
				Called when the slider control is released
		@param[in] p_value
			The new value
		 */
		void OnMaxDepths( int p_value );
		/** Sets the maximum depth
		@remarks
				Called when the slider control is moved
		@param[in] p_value
			The new value
		 */
		void OnMaxDepthsTrack( int p_value );
		/** Sets the maximum neighbours
		@remarks
				Called when the slider control is released
		@param[in] p_value
			The new value
		 */
		void OnMaxNeighbours( int p_value );
		/** Sets the maximum neighbours
		@remarks
				Called when the slider control is moved
		@param[in] p_value
			The new value
		 */
		void OnMaxNeighboursTrack( int p_value );
		/** Sets the Minkowski function order
		@remarks
				Called when the slider control is released
		@param[in] p_value
			The new value
		 */
		void OnMinkowskiOrder( int p_value );
		/** Sets the Minkowski function order
		@remarks
				Called when the slider control is moved
		@param[in] p_value
			The new value
		 */
		void OnMinkowskiOrderTrack( int p_value );
		/** Sets the distance function
		@param[in] p_value
			The new value
		 */
		void OnDistanceType( int p_value );
		/** Resets the generator
		 */
		void OnReset();

	private:
		std::shared_ptr< ProceduralTextures::StaticCtrl > m_statDepthTitle;
		std::shared_ptr< ProceduralTextures::StaticCtrl > m_statDepth;
		std::shared_ptr< ProceduralTextures::SliderCtrl > m_slideDepth;
		std::shared_ptr< ProceduralTextures::StaticCtrl > m_statNeighboursTitle;
		std::shared_ptr< ProceduralTextures::StaticCtrl > m_statNeighbours;
		std::shared_ptr< ProceduralTextures::SliderCtrl > m_slideNeighbours;
		std::shared_ptr< ProceduralTextures::StaticCtrl > m_statDistance;
		std::shared_ptr< ProceduralTextures::ComboBoxCtrl > m_comboDistance;
		std::shared_ptr< ProceduralTextures::StaticCtrl > m_statMinkowski;
		std::shared_ptr< ProceduralTextures::SliderCtrl > m_slideMinkowski;
		std::shared_ptr< ProceduralTextures::ButtonCtrl > m_buttonReset;
	};
}

#endif
