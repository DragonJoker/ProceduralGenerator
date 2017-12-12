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
#ifndef ___PG_Application___
#define ___PG_Application___

#include "MainFrame.h"

#pragma warning( push )
#pragma warning( disable:4996 )
#include <wx/app.h>
#pragma warning( pop )

namespace ProceduralGenerator
{
	class Application
		:	public wxApp
	{
	private:
		MainFrame * m_mainFrame;
		std::unique_ptr< wxLocale > m_pLocale;

	public:
		Application();
		~Application();

		virtual bool OnInit();
		virtual int OnExit();

	private:
		std::streambuf * m_cout;
		std::streambuf * m_cerr;
		std::streambuf * m_clog;
	};
}

DECLARE_APP( ProceduralGenerator::Application );

#endif
