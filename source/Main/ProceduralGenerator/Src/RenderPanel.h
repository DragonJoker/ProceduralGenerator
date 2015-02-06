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
#ifndef ___Pg_RenderPanel___
#define ___Pg_RenderPanel___

#include <wx/timer.h>
#include <wx/glcanvas.h>

#if defined( PGEN_FFMPEG )
#	define USE_STREAMS 0
extern "C"
{
#	include <libavutil/opt.h>
#	include <libavcodec/avcodec.h>
#	include <libavformat/avformat.h>
#	include <libavutil/common.h>
#	include <libavutil/imgutils.h>
#	include <libavutil/mathematics.h>
#	include <libavutil/samplefmt.h>
#	include <libswscale/swscale.h>
#	include <libswresample/swresample.h>
}
#elif defined( PGEN_OCV )
#	include <opencv2/opencv.hpp >
#endif

namespace ProceduralTextures
{
	class ProceduralGenerator;
	class wxStatusUpdateEvent;
	class MainFrame;

	class RenderPanel
		: public wxGLCanvas
	{
	public:
		RenderPanel( MainFrame * m_pFrame, int * p_attribList, const wxPoint & p_position, const wxSize & p_size );
		~RenderPanel();

		void Resize( const wxSize & p_size );
		void SetGenerator( ProceduralGenerator * p_pGenerator );
		void SaveFrame();
		void Render();
#if defined( PGEN_RECORDS )
		bool StartRecord();
		bool IsRecording();
		void StopRecord();
#endif

	private:
		void DoRender();

		DECLARE_EVENT_TABLE();
		void OnPaint( wxPaintEvent & p_event );
		void OnSize( wxSizeEvent & p_event );
		void OnTimer( wxTimerEvent & p_event );
		void OnClose( wxCloseEvent & p_event );
		void OnKeyUp( wxKeyEvent & p_event );
		void OnMouseMove( wxMouseEvent & p_event );
		void OnLeftDClick( wxMouseEvent & p_event );

	private:
		static const int m_iTimerId = 27;
		static wxString m_strGlErrors[6];
		ProceduralGenerator * m_pGenerator;
		wxGLContext * m_pContext;
		bool m_bInitialised;
		bool m_bFullScreen;
		MainFrame * m_pFrame;
#if defined( PGEN_RECORDS )
		wxMilliClock_t m_msSaved;
		uint32_t m_uiRecordedCount;
		uint64_t m_ui64RecordedTime;
		std::vector< uint8_t > m_bufferNV12;
#	if defined( PGEN_FFMPEG )
		AVCodec * m_pAvCodec;
		AVFrame * m_pAvFrame;
		AVPicture m_avEncodedPicture;
#		if USE_STREAMS
		AVFormatContext * m_pAvFormatContext;
		AVStream * m_pAvStream;
#		else
		AVCodecContext * m_pAvCodecContext;
		FILE * m_pFile;
#		endif
#	elif defined( PGEN_OCV )
		cv::VideoWriter m_writer;
#	endif
#endif
	};
}

#endif
