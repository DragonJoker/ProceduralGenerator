#include "MainFrame.h"

#include <Generator.h>

#include <emmintrin.h>

#include "RenderPanel.h"

#pragma warning( push )
#pragma warning( disable:4996 )
#include <wx/dcclient.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#pragma warning( pop )

using namespace ProceduralTextures;

namespace ProceduralGenerator
{
	namespace
	{
		eKEYBOARD_KEY ConvertKeyCode( int p_code )
		{
			eKEYBOARD_KEY l_return = eKEYBOARD_KEY_NONE;

			if ( p_code < 0x20 )
			{
				switch ( p_code )
				{
				case WXK_BACK:
				case WXK_TAB:
				case WXK_RETURN:
				case WXK_ESCAPE:
					l_return = eKEYBOARD_KEY( p_code );
					break;
				}
			}
			else if ( p_code == 0x7F )
			{
				l_return = eKEY_DELETE;
			}
			else if ( p_code > 0xFF )
			{
				l_return = eKEYBOARD_KEY( p_code + eKEY_START - WXK_START );
			}
			else
			{
				// ASCII or extended ASCII character
				l_return = eKEYBOARD_KEY( p_code );
			}

			return l_return;
		}
	}

	static const int g_iWantedFPS = 25;

	//*************************************************************************************************

	RenderPanel::RenderPanel( MainFrame * p_parent, int * p_attribList, wxPoint const & p_position, wxSize const & p_size )
		: wxGLCanvas( p_parent, wxID_ANY, p_attribList, p_position, p_size )
		, m_pContext( NULL )
		, m_bInitialised( false )
		, m_pGenerator( NULL )
		, m_bFullScreen( false )
		, m_pFrame( p_parent )
		, m_mouseMoveIndex( 0 )
		, m_timer( NULL )
		, m_count( 0 )
		, m_recorder()
	{
		m_timer = new wxTimer( this, eID_TIMER );
		SetBackgroundColour( p_parent->GetBackgroundColour() );
		SetForegroundColour( p_parent->GetForegroundColour() );
		Show();
	}

	RenderPanel::~RenderPanel()
	{
		delete m_timer;
	}

	void RenderPanel::SetGenerator( std::shared_ptr< GeneratorBase > p_pGenerator )
	{
		m_timer->Stop();
		m_bFullScreen = false;
		DoStopGenerator();
		m_pGenerator = p_pGenerator;

		if ( m_pGenerator )
		{
			SetSize( wxSize( p_pGenerator->GetDisplaySize().x(), p_pGenerator->GetDisplaySize().y() ) );
			m_pGenerator->Run();
			m_timer->Start( int( m_pGenerator->GetFrameTime().count() ) );
		}

		m_bInitialised = false;
	}

	std::shared_ptr< GeneratorBase > RenderPanel::GetGenerator()const
	{
		return m_pGenerator;
	}

	void RenderPanel::SaveFrame()
	{
		if ( m_pGenerator )
		{
			m_pGenerator->SaveFrame();
			DoRender();
			PixelBuffer const & l_buffer = m_pGenerator->GetSavedFrame();
			Size l_size = m_pGenerator->GetImageSize();
			wxImage l_image( l_size.x(), l_size.y(), true );
			int x = 0;

			for ( uint32_t i = 0; i < l_size.x(); i++ )
			{
				int y = l_size.y() - 1;

				for ( uint32_t j = 0; j < l_size.y(); j++ )
				{
					UbPixel const & l_pixel = l_buffer[j][i];
					l_image.SetRGB( x, y, l_pixel.r, l_pixel.g, l_pixel.b );
					y--;
				}

				x++;
			}

			wxString l_strWildcard = _( "All supported files" );
			l_strWildcard += wxT( " (*.bmp;*.gif;*.png;*.jpg)|*.bmp;*.gif;*.png;*.jpg|" );
			l_strWildcard += _( "BITMAP files" );
			l_strWildcard += wxT( " (*.bmp)|*.bmp|" );
			l_strWildcard += _( "GIF files" );
			l_strWildcard += wxT( " (*.gif)|*.gif|" );
			l_strWildcard += _( "JPEG files" );
			l_strWildcard += wxT( " (*.jpg)|*.jpg|" );
			l_strWildcard += _( "PNG files" );
			l_strWildcard += wxT( " (*.png)|*.png" );
			wxFileDialog l_dialog( this, _( "Please choose an image file name" ), wxEmptyString, wxEmptyString, l_strWildcard, wxFD_SAVE | wxFD_OVERWRITE_PROMPT );

			if ( l_dialog.ShowModal() == wxID_OK )
			{
				l_image.SaveFile( l_dialog.GetPath() );
			}
		}
	}

	void RenderPanel::Render()
	{
		if ( IsShown() && GetParent()->IsShown() )
		{
			wxClientDC l_dc( this );

			if ( m_pGenerator )
			{
				if ( !m_pContext )
				{
					m_pContext = new wxGLContext( this, NULL );
				}

				if ( m_recorder.UpdateTime() && m_recorder.IsRecording() )
				{
					m_pGenerator->SaveFrame();
					DoRender();

					try
					{
						m_recorder.RecordFrame( m_pGenerator->GetSavedFrame(), m_pGenerator->GetImageSize() );
					}
					catch( std::exception & p_exc )
					{
						m_pFrame->StopRecord();
						wxMessageBox( wxString( p_exc.what(), wxMBConvLibc() ) );
					}
				}
				else
				{
					DoRender();
				}
			}
			else
			{
				l_dc.SetBrush( *wxBLACK_BRUSH );
				l_dc.DrawRectangle( wxPoint( 0, 0 ), GetClientSize() );
			}
		}
	}

	bool RenderPanel::StartRecord()
	{
		bool l_bReturn = false;

		if ( m_pGenerator )
		{
			try
			{
				l_bReturn = m_recorder.StartRecord( m_pGenerator->GetImageSize() );
			}
			catch ( std::exception p_exc )
			{
				wxMessageBox( wxString( p_exc.what(), wxMBConvLibc() ) );
				l_bReturn= false;
			}
		}

		return l_bReturn;
	}

	void RenderPanel::StopRecord()
	{
		m_recorder.StopRecord();
	}

	void RenderPanel::DoStopGenerator()
	{
		if ( m_pGenerator )
		{
			if ( m_pContext )
			{
				wxClientDC l_dc( this );
				m_pContext->SetCurrent( *this );
			}

			m_pGenerator->Stop();
			m_pGenerator->Cleanup();
			m_pGenerator->Destroy();
		}
	}

	void RenderPanel::DoRender()
	{
		m_pContext->SetCurrent( *this );

		if ( !m_pGenerator->IsInitialised() )
		{
			m_pGenerator->Initialise();
		}

		m_pGenerator->Render( [this]()
		{
			SwapBuffers();
		} );
	}

	BEGIN_EVENT_TABLE( RenderPanel, wxGLCanvas )
		EVT_PAINT( RenderPanel::OnPaint )
		EVT_SIZE( RenderPanel::OnSize )
		EVT_CLOSE( RenderPanel::OnClose )
		EVT_KEY_DOWN( RenderPanel::OnKeyDown )
		EVT_KEY_UP( RenderPanel::OnKeyUp )
		EVT_MOTION( RenderPanel::OnMouseMove )
		EVT_LEFT_DOWN( RenderPanel::OnLeftDown )
		EVT_LEFT_UP( RenderPanel::OnLeftUp )
		EVT_MIDDLE_DOWN( RenderPanel::OnMiddleDown )
		EVT_MIDDLE_UP( RenderPanel::OnMiddleUp )
		EVT_RIGHT_DOWN( RenderPanel::OnRightDown )
		EVT_RIGHT_UP( RenderPanel::OnRightUp )
		EVT_LEFT_DCLICK( RenderPanel::OnLeftDClick )
		EVT_CHAR( RenderPanel::OnChar )
		EVT_TIMER( eID_TIMER, RenderPanel::OnTimer )
	END_EVENT_TABLE()

	void RenderPanel::OnPaint( wxPaintEvent & p_event )
	{
		wxPaintDC l_dc( this );

		if ( !m_pGenerator )
		{
			l_dc.SetBrush( *wxBLACK_BRUSH );
			l_dc.DrawRectangle( wxPoint( 0, 0 ), GetClientSize() );
		}

		p_event.Skip();
	}

	void RenderPanel::OnSize( wxSizeEvent & p_event )
	{
		if ( m_pGenerator )
		{
			wxSize l_size = p_event.GetSize();
			m_pGenerator->UpdateDisplaySize( Size( l_size.x, l_size.y ) );
		}

		StopRecord();

		if ( !m_pGenerator || !m_pGenerator->IsInitialised() )
		{
			wxClientDC l_dc( this );
			l_dc.SetBrush( *wxBLACK_BRUSH );
			l_dc.DrawRectangle( wxPoint( 0, 0 ), GetClientSize() );
		}

		p_event.Skip();
	}

	void RenderPanel::OnTimer( wxTimerEvent & p_event )
	{
		if ( m_pGenerator )
		{
			Render();
			std::chrono::milliseconds l_time = m_pGenerator->GetCpuTime() + m_pGenerator->GetGpuTime();

			if ( l_time.count() >= m_timer->GetInterval() )
			{
				if ( m_count++ >= 100 )
				{
					m_timer->Stop();
					m_timer->Start( m_timer->GetInterval() * 2 );
				}
			}
			else
			{
				m_count = 0;

				if ( l_time < m_pGenerator->GetFrameTime() )
				{
					m_timer->Stop();
					m_timer->Start( int( m_pGenerator->GetFrameTime().count() ) );
				}
			}
		}
	}

	void RenderPanel::OnClose( wxCloseEvent & p_event )
	{
		StopRecord();
		DoStopGenerator();

		if ( m_pContext )
		{
			delete m_pContext;
			m_pContext = NULL;
		}

		p_event.Skip();
	}

	void RenderPanel::OnKeyDown( wxKeyEvent & p_event )
	{
		if ( p_event.GetKeyCode() == WXK_F12 )
		{
			SaveFrame();
		}
		else if ( m_pGenerator )
		{
			m_pGenerator->PostKeyboardEvent( eKEYBOARD_EVENT_KEY_PUSHED, ConvertKeyCode( p_event.GetKeyCode() ), p_event.ControlDown(), p_event.AltDown(), p_event.ShiftDown() );
		}

		p_event.Skip();
	}

	void RenderPanel::OnKeyUp( wxKeyEvent & p_event )
	{
		if ( p_event.GetKeyCode() == WXK_F12 )
		{
			SaveFrame();
		}
		else if ( m_pGenerator )
		{
			m_pGenerator->PostKeyboardEvent( eKEYBOARD_EVENT_KEY_RELEASED, ConvertKeyCode( p_event.GetKeyCode() ), p_event.ControlDown(), p_event.AltDown(), p_event.ShiftDown() );
		}

		p_event.Skip();
	}

	void RenderPanel::OnChar( wxKeyEvent & p_event )
	{
		if ( m_pGenerator )
		{
			wxChar l_key = p_event.GetUnicodeKey();
			wxString l_tmp;
			l_tmp << l_key;
			m_pGenerator->PostCharEvent( ConvertKeyCode( p_event.GetKeyCode() ), String( l_tmp.mb_str( wxConvUTF8 ) ) );
		}

		p_event.Skip();
	}

	void RenderPanel::OnMouseMove( wxMouseEvent & p_event )
	{
		if ( m_pGenerator )
		{
			m_pGenerator->PostMouseEvent( eMOUSE_EVENT_MOUSE_MOVE, Position( p_event.GetX(), p_event.GetY() ) );
		}

		p_event.Skip();
	}

	void RenderPanel::OnLeftDown( wxMouseEvent & p_event )
	{
		if ( m_pGenerator )
		{
			m_pGenerator->PostMouseEvent( eMOUSE_EVENT_MOUSE_BUTTON_PUSHED, Position( p_event.GetX(), p_event.GetY() ), eMOUSE_BUTTON_LEFT );
		}

		p_event.Skip();
	}

	void RenderPanel::OnLeftUp( wxMouseEvent & p_event )
	{
		if ( m_pGenerator )
		{
			m_pGenerator->PostMouseEvent( eMOUSE_EVENT_MOUSE_BUTTON_RELEASED, Position( p_event.GetX(), p_event.GetY() ), eMOUSE_BUTTON_LEFT );
		}

		p_event.Skip();
	}

	void RenderPanel::OnMiddleDown( wxMouseEvent & p_event )
	{
		if ( m_pGenerator )
		{
			m_pGenerator->PostMouseEvent( eMOUSE_EVENT_MOUSE_BUTTON_PUSHED, Position( p_event.GetX(), p_event.GetY() ), eMOUSE_BUTTON_MIDDLE );
		}

		p_event.Skip();
	}

	void RenderPanel::OnMiddleUp( wxMouseEvent & p_event )
	{
		if ( m_pGenerator )
		{
			m_pGenerator->PostMouseEvent( eMOUSE_EVENT_MOUSE_BUTTON_RELEASED, Position( p_event.GetX(), p_event.GetY() ), eMOUSE_BUTTON_MIDDLE );
		}

		p_event.Skip();
	}

	void RenderPanel::OnRightDown( wxMouseEvent & p_event )
	{
		if ( m_pGenerator )
		{
			m_pGenerator->PostMouseEvent( eMOUSE_EVENT_MOUSE_BUTTON_PUSHED, Position( p_event.GetX(), p_event.GetY() ), eMOUSE_BUTTON_RIGHT );
		}

		p_event.Skip();
	}

	void RenderPanel::OnRightUp( wxMouseEvent & p_event )
	{
		if ( m_pGenerator )
		{
			m_pGenerator->PostMouseEvent( eMOUSE_EVENT_MOUSE_BUTTON_RELEASED, Position( p_event.GetX(), p_event.GetY() ), eMOUSE_BUTTON_RIGHT );
		}

		p_event.Skip();
	}

	void RenderPanel::OnLeftDClick( wxMouseEvent & p_event )
	{
		if ( m_pGenerator && m_pGenerator->IsInitialised() )
		{
			m_bFullScreen = !m_bFullScreen;
			m_pFrame->ShowFullScreen( m_bFullScreen, wxFULLSCREEN_ALL );
		}

		p_event.Skip();
	}
}
