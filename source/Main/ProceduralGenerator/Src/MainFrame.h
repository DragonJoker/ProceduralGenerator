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
#ifndef ___MainFrame___
#define ___MainFrame___

namespace ProceduralTextures
{
	class RenderPanel;
	class StatusBar;

	class MainFrame
		:	public wxFrame
	{
	private:
		typedef std::map< wxString, PluginBase * > PluginStrMap;

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
#if defined( PGEN_RECORDS )
		void StopRecord();
#endif

	private:
		DECLARE_CLASS( MainFrame );
		void DoBuildMenuBar();
		void DoSelectGenerator();
		ProceduralGenerator * DoCreateGenerator();
		void DoDestroyGenerator();
		void DoLoadPlugins();
		void DoClose();
		void DoLoadPlugins( wxString const & p_strPath );

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
		void OnCpuStepEnd( wxCommandEvent & p_event );
		void OnShowMessageBox( wxCommandEvent & p_event );
		void OnResize( wxCommandEvent & p_event );
#if defined( PGEN_RECORDS )
		void OnRecord( wxCommandEvent & p_event );
		void OnStop( wxCommandEvent & p_event );
#endif

	private:
		RenderPanel * m_pRenderPanel;
		ConfigPanel * m_pConfigPanel;
		PluginStrMap m_mapPluginsByName;
		PluginBase * m_pPlugin;
		StatusBar  * m_pStatusBar;
		wxMilliClock_t m_savedTime;
		wxMilliClock_t m_time;
	};
}

#endif
