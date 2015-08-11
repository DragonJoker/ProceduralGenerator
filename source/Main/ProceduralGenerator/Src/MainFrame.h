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
#ifndef ___PROCEDURAL_GENERATOR_MAIN_FRAME_H___
#define ___PROCEDURAL_GENERATOR_MAIN_FRAME_H___

#include <chrono>
#include <functional>
#include <map>
#include <vector>

#pragma warning( push )
#pragma warning( disable:4996 )
#include <wx/dynlib.h>
#include <wx/frame.h>
#include <wx/object.h>
#include <wx/panel.h>
#pragma warning( pop )

#include <GeneratorPrerequisites.h>

namespace ProceduralGenerator
{
	class RenderPanel;
	typedef std::chrono::high_resolution_clock clock;

	class MainFrame
		:	public wxFrame
	{
	private:
		typedef std::map< wxString, std::pair< ProceduralTextures::PluginBase *, wxDynamicLibrary * > > PluginStrMap;

	public:
		enum eID
		{
			eID_MAIN_FRAME = 1,
			eID_SELECT_GENERATOR,
			eID_PRINT_SCREEN,
			eID_RECORD,
			eID_STOP,
			eID_EXIT,
		}	eID;

		MainFrame();
		~MainFrame();

		void Initialise();
		void StopRecord();

	private:
		DECLARE_CLASS( MainFrame );
		void DoBuildMenuBar();
		void DoSelectGenerator();
		std::shared_ptr< ProceduralTextures::GeneratorBase > DoCreateGenerator();
		void DoDestroyGenerator( std::shared_ptr< ProceduralTextures::GeneratorBase > & p_generator );
		void DoLoadPlugins();
		void DoClose();
		void DoLoadPlugins( wxString const & p_strPath );
		void FireShowMessageBox( ProceduralTextures::String const p_title, ProceduralTextures::String const & p_message );
		void FireResize( ProceduralTextures::Size const & p_size );

		DECLARE_EVENT_TABLE();
		void OnPaint( wxPaintEvent & p_event );
		void OnSize( wxSizeEvent & p_event );
		void OnClose( wxCloseEvent & p_event );
		void OnStatusUpdate( wxNotifyEvent & p_event );
		void OnUpdateFps( wxCommandEvent & p_event );
		void OnKeyUp( wxKeyEvent & p_event );
		void OnExit( wxCommandEvent & p_event );
		void OnSelectGenerator( wxCommandEvent & p_event );
		void OnPrintScreen( wxCommandEvent & p_event );
		void OnShowMessageBox( wxCommandEvent & p_event );
		void OnResize( wxCommandEvent & p_event );
		void OnRecord( wxCommandEvent & p_event );
		void OnStop( wxCommandEvent & p_event );

	private:
		RenderPanel * m_pRenderPanel;
		PluginStrMap m_mapPluginsByName;
		uint32_t m_indexCpuStepEnd;
	};
}

#endif
