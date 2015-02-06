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
#ifndef ___SimiliLifeGame___
#define ___SimiliLifeGame___

#include <LibGenerator/ProceduralGenerator.h>
#include <vector>
#include <map>
#include <list>

namespace ProceduralTextures
{
	class LSystem : public ProceduralGenerator
	{
	private:
		class Variable
		{
		public:
			Variable();
			virtual ~Variable();

			void Draw();
		};

		class Constant
		{
		public:
			Constant();
			virtual ~Constant();
		};

		class Start
		{
		public:
			Start();
			virtual ~Start();
		};

		class Rule
		{
		public:
			Rule();
			virtual ~Rule();
		};

		typedef std::map <Variable *, Rule *>	RuleVariableMap;
		typedef std::vector <Constant *>		ConstantArray;
		typedef std::list <Variable *>			VariableList;

		class LSystemDefinition
		{
		public:
			RuleVariableMap m_mapRules;
			ConstantArray m_arrayConstants;
			Start * m_pStart;

			LSystemDefinition();
			virtual ~LSystemDefinition();
		};

		class Thread : public ProceduralGenerator::Thread
		{
		private:
			RuleVariableMap * m_pMapRules;
			ConstantArray  * m_pArrayConstants;
			Start ** m_ppStart;
			VariableList * m_pListResult;

		public:
			Thread( ProceduralGenerator * p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight, const UbPixel & p_pxColour);
			virtual ~Thread();

			virtual void Step();

			void SetSystem( RuleVariableMap * p_pRules, ConstantArray * p_pConstants, Start ** p_ppStart, VariableList * p_pListResult);
		};

	private:
		RuleVariableMap m_mapRules;
		ConstantArray m_arrayConstants;
		Start * m_pStart;
		VariableList m_listResult;

	public:
		LSystem( int p_width, int p_height, int p_iWndId, wxFrame * p_pFrame);
		virtual ~LSystem();
	};

	class LsPanel : public ConfigPanel
	{
	private:
		typedef enum
		{	eRedValue	= 50
		,	eGreenValue	= 51
		,	eBlueValue	= 52
		}
		eIDs;

		LSystem			*	m_pGenerator;
		wxSlider		*	m_pRedValue;
		wxStaticText	*	m_pRedText;
		wxSlider		*	m_pGreenValue;
		wxStaticText	*	m_pGreenText;
		wxSlider		*	m_pBlueValue;
		wxStaticText	*	m_pBlueText;

	public:
		LsPanel( wxWindow * p_pWindow, wxWindowID p_id, const wxPoint & p_ptPosition=wxDefaultPosition, const wxSize & p_size=wxDefaultSize);
		virtual ~LsPanel();

		void Create( LSystem * p_pGenerator, wxWindow * p_pWindow, const wxPoint & p_ptPosition, const wxSize & p_size);

	private:
		DECLARE_EVENT_TABLE()
		void _onSliderRed		( wxCommandEvent	&	p_event);
		void _onSliderGreen		( wxCommandEvent	&	p_event);
		void _onSliderBlue		( wxCommandEvent	&	p_event);
	};

	class LsPlugin : public Plugin<LsPanel, LSystem, LsPlugin>
	{
	public:
		LsPlugin();
		virtual ~LsPlugin();

		virtual wxString GetName();

		DECLARE_PLUGIN()
	};
}

#endif
