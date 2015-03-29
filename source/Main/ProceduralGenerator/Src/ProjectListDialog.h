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
#ifndef ___ProjectListFrame___
#define ___ProjectListFrame___

#pragma warning( push )
#pragma warning( disable:4996 )
#include <wx/combobox.h>
#include <wx/dialog.h>
#include <wx/stattext.h>
#pragma warning( pop )

namespace ProceduralGenerator
{
	class ProjectListDialog
		: public wxDialog
	{
	private:
		enum
		{
			pldOK,
			pldCancel,
			pldProjects,
		};

	public:
		ProjectListDialog( wxWindow * p_parent, wxArrayString const & p_choices );
		~ProjectListDialog();

		wxString GetProject();
		wxSize GetResolution();

	private:
		DECLARE_EVENT_TABLE();
		void _onClose( wxCloseEvent & event );
		void _onOK( wxCommandEvent & event );
		void _onCancel( wxCommandEvent & event );
		void _onProjects( wxCommandEvent & event );

	private:
		wxComboBox * m_pComboProjets;
		wxComboBox * m_pComboWidth;
		wxComboBox * m_pComboHeight;
		wxStaticText * m_pStaticResolution;
	};
}

#endif
