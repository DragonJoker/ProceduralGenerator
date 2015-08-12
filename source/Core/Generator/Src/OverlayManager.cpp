#include "OverlayManager.h"

#include "Font.h"
#include "GeneratorUtils.h"
#include "GlMat4FrameVariable.h"
#include "GlShaderProgram.h"
#include "Overlay.h"
#include "PanelOverlay.h"
#include "BorderPanelOverlay.h"
#include "TextOverlay.h"

#if defined( DrawText )
#	undef DrawText
#endif

namespace ProceduralTextures
{
	const String VertexShader =
		_T( "attribute vec2 vertex;\n" )
		_T( "attribute vec2 texture;\n" )
		_T( "uniform mat4 mvp;\n" )
		_T( "varying vec2 pxl_texture;\n" )
		_T( "void main()\n" )
		_T( "{\n" )
		_T( "    pxl_texture = texture;\n" )
		_T( "    gl_Position = mvp * vec4( vertex.xy, 0.0, 1.0 );\n" )
		_T( "}\n" );

	const String PixelShaderColour =
		_T( "uniform vec4 pxl_colour;\n" )
		_T( "void main()\n" )
		_T( "{\n" )
		_T( "    gl_FragColor = pxl_colour;\n" )
		_T( "}\n" );

	const String PixelShaderTexture =
		_T( "varying vec2 pxl_texture;\n" )
		_T( "uniform sampler2D pxl_mapTexture;\n" )
		_T( "void main()\n" )
		_T( "{\n" )
		_T( "    gl_FragColor = texture2D( pxl_mapTexture, pxl_texture );\n" )
		_T( "}\n" );

	const String PixelShaderTextColour =
		_T( "varying vec2 pxl_texture;\n" )
		_T( "uniform vec4 pxl_colour;\n" )
		_T( "uniform sampler2D pxl_mapText;\n" )
		_T( "void main()\n" )
		_T( "{\n" )
		_T( "    float l_alpha = texture2D( pxl_mapText, pxl_texture ).r;\n" )
		_T( "    gl_FragColor = vec4( pxl_colour.rgb, l_alpha );\n" )
		_T( "}\n" );

	const String PixelShaderTextTexture =
		_T( "varying vec2 pxl_texture;\n" )
		_T( "uniform sampler2D pxl_mapTexture;\n" )
		_T( "uniform sampler2D pxl_mapText;\n" )
		_T( "void main()\n" )
		_T( "{\n" )
		_T( "    vec4 l_colour = texture2D( pxl_mapTexture, pxl_texture );\n" )
		_T( "    float l_alpha = l_colour.a * texture2D( pxl_mapText, pxl_texture ).r;\n" )
		_T( "    gl_FragColor = vec4( l_colour.rgb, l_alpha );\n" )
		_T( "}\n" );

	OverlayManager::OverlayManager( std::shared_ptr< gl::OpenGl > p_openGl )
		: gl::Holder( p_openGl )
		, m_currentZIndex( 0 )
		, m_programColour( std::make_shared< gl::ShaderProgram >( GetOpenGl() ) )
		, m_programTexture( std::make_shared< gl::ShaderProgram >( GetOpenGl() ) )
		, m_programTextColour( std::make_shared< gl::ShaderProgram >( GetOpenGl() ) )
		, m_programTextTexture( std::make_shared< gl::ShaderProgram >( GetOpenGl() ) )
		, m_changed( false )
	{
		m_programColour->CreateObject( gl::eSHADER_OBJECT_TYPE_VERTEX );
		m_programColour->CreateObject( gl::eSHADER_OBJECT_TYPE_PIXEL );
		m_programColour->SetProgramText( VertexShader, gl::eSHADER_OBJECT_TYPE_VERTEX );
		m_programColour->SetProgramText( PixelShaderColour, gl::eSHADER_OBJECT_TYPE_PIXEL );
		m_programColour->CreateVec4FloatFrameVariable( _T( "pxl_colour" ) );
		m_programColour->CreateMat4FloatFrameVariable( _T( "mvp" ) );

		m_programTexture->CreateObject( gl::eSHADER_OBJECT_TYPE_VERTEX );
		m_programTexture->CreateObject( gl::eSHADER_OBJECT_TYPE_PIXEL );
		m_programTexture->SetProgramText( VertexShader, gl::eSHADER_OBJECT_TYPE_VERTEX );
		m_programTexture->SetProgramText( PixelShaderTexture, gl::eSHADER_OBJECT_TYPE_PIXEL );
		m_programTexture->CreateIntFrameVariable( _T( "pxl_mapTexture" ) );
		m_programTexture->CreateMat4FloatFrameVariable( _T( "mvp" ) );

		m_programTextColour->CreateObject( gl::eSHADER_OBJECT_TYPE_VERTEX );
		m_programTextColour->CreateObject( gl::eSHADER_OBJECT_TYPE_PIXEL );
		m_programTextColour->SetProgramText( VertexShader, gl::eSHADER_OBJECT_TYPE_VERTEX );
		m_programTextColour->SetProgramText( PixelShaderTextColour, gl::eSHADER_OBJECT_TYPE_PIXEL );
		m_programTextColour->CreateVec4FloatFrameVariable( _T( "pxl_colour" ) );
		m_programTextColour->CreateIntFrameVariable( _T( "pxl_mapText" ) );
		m_programTextColour->CreateMat4FloatFrameVariable( _T( "mvp" ) );

		m_programTextTexture->CreateObject( gl::eSHADER_OBJECT_TYPE_VERTEX );
		m_programTextTexture->CreateObject( gl::eSHADER_OBJECT_TYPE_PIXEL );
		m_programTextTexture->SetProgramText( VertexShader, gl::eSHADER_OBJECT_TYPE_VERTEX );
		m_programTextTexture->SetProgramText( PixelShaderTextTexture, gl::eSHADER_OBJECT_TYPE_PIXEL );
		m_programTextTexture->CreateIntFrameVariable( _T( "pxl_mapTexture" ) );
		m_programTextTexture->CreateIntFrameVariable( _T( "pxl_mapText" ) );
		m_programTextTexture->CreateMat4FloatFrameVariable( _T( "mvp" ) );

		LoadFont( DEFAULT_FONT_NAME, 20, System::GetDataDirectory() + FOLDER_SEPARATOR + _T( "arial.ttf" ) );
		LoadFont( DEFAULT_FONT_NAME, DEFAULT_FONT_HEIGHT, System::GetDataDirectory() + FOLDER_SEPARATOR + _T( "arial.ttf" ) );
	}

	OverlayManager::~OverlayManager()
	{
		m_loadedFonts.clear();
	}

	void OverlayManager::Initialise()
	{
		m_programColour->Create();
		m_programColour->Initialise();

		m_programTexture->Create();
		m_programTexture->Initialise();

		m_programTextColour->Create();
		m_programTextColour->Initialise();

		m_programTextTexture->Create();
		m_programTextTexture->Initialise();
	}

	void OverlayManager::Cleanup()
	{
		std::unique_lock< std::mutex > l_lock( m_mutex );

		for ( auto l_overlay : m_overlays )
		{
			l_overlay->Cleanup();
		}

		m_initialisable.clear();
		m_mapOverlaysByZIndex.clear();
		m_overlays.clear();
		m_currentZIndex = 1;

		m_programColour->Cleanup();
		m_programColour->Destroy();

		m_programTexture->Cleanup();
		m_programTexture->Destroy();

		m_programTextColour->Cleanup();
		m_programTextColour->Destroy();

		m_programTextTexture->Cleanup();
		m_programTextTexture->Destroy();
	}

	void OverlayManager::Render( Size const & p_size )
	{
		try
		{
			if ( m_changed )
			{
				DoUpdate();
			}

			std::vector< std::weak_ptr< Overlay > > l_initialisable;
			std::vector< std::shared_ptr< Overlay > > l_overlays;
			{
				std::unique_lock< std::mutex > l_lock( m_mutex );
				std::swap( l_initialisable, m_initialisable );

				for ( auto && l_ov : m_renderableOverlays )
				{
					l_overlays.push_back( l_ov.lock() );
				}
			}

			for ( auto && l_overlay : l_initialisable )
			{
				l_overlay.lock()->Initialise();
			}

			DoUpdateSize( p_size );
			std::shared_ptr< gl::OpenGl > l_gl = GetOpenGl();
			l_gl->Enable( GL_BLEND );
			l_gl->BlendFuncSeparate( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA );

			for ( auto && l_overlay : l_overlays )
			{
				l_overlay->Render( p_size );
			}

			l_gl->BlendFunc( GL_ONE, GL_ZERO );
			l_gl->Disable( GL_BLEND );
		}
		catch ( std::exception & p_exc )
		{
			std::cerr << "%s\n" << p_exc.what() << std::endl;
		}
	}

	std::shared_ptr< PanelOverlay > OverlayManager::CreatePanel( String const & p_name, Point2d const & p_position, Point2d const & p_size, Material const & p_material, std::shared_ptr< Overlay > p_parent )
	{
		std::shared_ptr< PanelOverlay > l_return = std::make_shared< PanelOverlay >( GetOpenGl(), p_material, p_parent );
		l_return->SetName( p_name );
		l_return->SetPosition( p_position );
		l_return->SetSize( p_size );

		if ( p_material.GetType() == eMATERIAL_TYPE_COLOUR )
		{
			l_return->SetProgram( p_material.GetType(), GetProgramColour() );
		}
		else
		{
			l_return->SetProgram( p_material.GetType(), GetProgramTexture() );
		}

		AddOverlay( l_return, p_parent );
		return l_return;
	}

	std::shared_ptr< PanelOverlay > OverlayManager::CreatePanel( String const & p_name, Position const & p_position, Size const & p_size, Material const & p_material, std::shared_ptr< Overlay > p_parent )
	{
		std::shared_ptr< PanelOverlay > l_return = std::make_shared< PanelOverlay >( GetOpenGl(), p_material, p_parent );
		l_return->SetName( p_name );
		l_return->SetPixelPosition( p_position );
		l_return->SetPixelSize( p_size );

		if ( p_material.GetType() == eMATERIAL_TYPE_COLOUR )
		{
			l_return->SetProgram( p_material.GetType(), GetProgramColour() );
		}
		else
		{
			l_return->SetProgram( p_material.GetType(), GetProgramTexture() );
		}

		AddOverlay( l_return, p_parent );
		return l_return;
	}

	std::shared_ptr< BorderPanelOverlay > OverlayManager::CreateBorderPanel( String const & p_name, Point2d const & p_position, Point2d const & p_size, Material const & p_material, Point4d const & p_bordersSize, Material const & p_bordersMaterial, std::shared_ptr< Overlay > p_parent )
	{
		std::shared_ptr< BorderPanelOverlay > l_return = std::make_shared< BorderPanelOverlay >( GetOpenGl(), p_material, p_bordersMaterial, p_parent );
		l_return->SetName( p_name );
		l_return->SetPosition( p_position );
		l_return->SetSize( p_size );

		if ( p_material.GetType() == eMATERIAL_TYPE_COLOUR )
		{
			l_return->SetProgram( p_material.GetType(), GetProgramColour() );
		}
		else
		{
			l_return->SetProgram( p_material.GetType(), GetProgramTexture() );
		}

		if ( p_bordersMaterial.GetType() == eMATERIAL_TYPE_COLOUR )
		{
			l_return->SetBordersProgram( p_bordersMaterial.GetType(), GetProgramColour() );
		}
		else
		{
			l_return->SetBordersProgram( p_bordersMaterial.GetType(), GetProgramTexture() );
		}

		l_return->SetBordersSize( p_bordersSize );
		AddOverlay( l_return, p_parent );
		return l_return;
	}

	std::shared_ptr< BorderPanelOverlay > OverlayManager::CreateBorderPanel( String const & p_name, Position const & p_position, Size const & p_size, Material const & p_material, Point4i const & p_bordersSize, Material const & p_bordersMaterial, std::shared_ptr< Overlay > p_parent )
	{
		std::shared_ptr< BorderPanelOverlay > l_return = std::make_shared< BorderPanelOverlay >( GetOpenGl(), p_material, p_bordersMaterial, p_parent );
		l_return->SetName( p_name );
		l_return->SetPixelPosition( p_position );
		l_return->SetPixelSize( p_size );

		if ( p_material.GetType() == eMATERIAL_TYPE_COLOUR )
		{
			l_return->SetProgram( p_material.GetType(), GetProgramColour() );
		}
		else
		{
			l_return->SetProgram( p_material.GetType(), GetProgramTexture() );
		}

		if ( p_bordersMaterial.GetType() == eMATERIAL_TYPE_COLOUR )
		{
			l_return->SetBordersProgram( p_bordersMaterial.GetType(), GetProgramColour() );
		}
		else
		{
			l_return->SetBordersProgram( p_bordersMaterial.GetType(), GetProgramTexture() );
		}

		l_return->SetBordersPixelSize( p_bordersSize );
		AddOverlay( l_return, p_parent );
		return l_return;
	}

	std::shared_ptr< TextOverlay > OverlayManager::CreateText( String const & p_name, Point2d const & p_position, Point2d const & p_size, Material const & p_material, std::shared_ptr< Font > p_font, std::shared_ptr< Overlay > p_parent )
	{
		std::shared_ptr< TextOverlay > l_return = std::make_shared< TextOverlay >( GetOpenGl(), p_material, p_parent );
		l_return->SetName( p_name );
		l_return->SetPosition( p_position );
		l_return->SetSize( p_size );

		if ( p_material.GetType() == eMATERIAL_TYPE_COLOUR )
		{
			l_return->SetProgram( p_material.GetType(), GetProgramTextColour() );
		}
		else
		{
			l_return->SetProgram( p_material.GetType(), GetProgramTextTexture() );
		}

		l_return->SetFont( p_font );
		AddOverlay( l_return, p_parent );
		return l_return;
	}

	std::shared_ptr< TextOverlay > OverlayManager::CreateText( String const & p_name, Position const & p_position, Size const & p_size, Material const & p_material, std::shared_ptr< Font > p_font, std::shared_ptr< Overlay > p_parent )
	{
		std::shared_ptr< TextOverlay > l_return = std::make_shared< TextOverlay >( GetOpenGl(), p_material, p_parent );
		l_return->SetName( p_name );
		l_return->SetPixelPosition( p_position );
		l_return->SetPixelSize( p_size );

		if ( p_material.GetType() == eMATERIAL_TYPE_COLOUR )
		{
			l_return->SetProgram( p_material.GetType(), GetProgramTextColour() );
		}
		else
		{
			l_return->SetProgram( p_material.GetType(), GetProgramTextTexture() );
		}

		l_return->SetFont( p_font );
		AddOverlay( l_return, p_parent );
		return l_return;
	}

	void OverlayManager::AddOverlay( std::shared_ptr< Overlay > p_overlay, std::shared_ptr< Overlay > p_parent )
	{
		if ( !p_parent )
		{
			AddOverlay( m_currentZIndex++, p_overlay );
		}
		else
		{
			p_parent->AddChild( p_overlay, m_currentZIndex++ );
			std::unique_lock< std::mutex > l_lock( m_mutex );
			DoAddOverlay( p_overlay );
		}

		m_currentZIndex++;
	}

	void OverlayManager::AddOverlay( std::shared_ptr< Overlay > p_overlay )
	{
		AddOverlay( m_currentZIndex++, p_overlay );
	}

	void OverlayManager::AddOverlay( uint64_t p_iZIndex, std::shared_ptr< Overlay > p_overlay )
	{
		p_overlay->SetZIndex( p_iZIndex );
		std::unique_lock< std::mutex > l_lock( m_mutex );
		m_mapOverlaysByZIndex.insert( std::make_pair( p_iZIndex, p_overlay ) );
		DoAddOverlay( p_overlay );
	}

	bool OverlayManager::HasOverlay( uint64_t p_zIndex )const
	{
		return m_mapOverlaysByZIndex.find( p_zIndex ) != m_mapOverlaysByZIndex.end();
	}

	bool OverlayManager::HasOverlay( String const & p_name )const
	{
		return std::find_if( std::begin( m_overlays ), std::end( m_overlays ), [&p_name]( std::shared_ptr< Overlay > p_overlay )
		{
			return p_overlay->GetName() == p_name;
		} ) != std::end( m_overlays );
	}

	std::shared_ptr< Overlay > OverlayManager::GetOverlay( int p_zIndex )const
	{
		std::unique_lock< std::mutex > l_lock( m_mutex );
		std::shared_ptr< Overlay > l_return;
		auto l_it = m_mapOverlaysByZIndex.find( p_zIndex );

		if ( l_it != m_mapOverlaysByZIndex.end() )
		{
			l_return = l_it->second.lock();
		}

		return l_return;
	}

	std::shared_ptr< Overlay > OverlayManager::GetOverlay( String const & p_name )const
	{
		std::unique_lock< std::mutex > l_lock( m_mutex );
		std::shared_ptr< Overlay > l_return;
		auto l_it = std::find_if( std::begin( m_overlays ), std::end( m_overlays ), [&p_name]( std::shared_ptr< Overlay > p_overlay )
		{
			return p_overlay->GetName() == p_name;
		} );

		if ( l_it != m_overlays.end() )
		{
			l_return = *l_it;
		}

		return l_return;
	}

	std::shared_ptr< Font > OverlayManager::LoadFont( String const & p_fontName, uint32_t p_height, String const & p_fontPath )
	{
		std::shared_ptr< Font > l_return;
		String l_name = p_fontName + _T( "_" ) + StringUtils::ToString( p_height );
		auto it = m_loadedFonts.find( l_name );

		if ( it == m_loadedFonts.end() )
		{
			l_return = std::make_shared< Font >( p_fontPath, p_height );
			m_loadedFonts[l_name] = l_return;
		}
		else
		{
			l_return = it->second;
		}

		return l_return;
	}

	std::shared_ptr< Font > OverlayManager::GetFont( String const & p_fontName, uint32_t p_height )
	{
		std::shared_ptr< Font > l_return;
		auto it = m_loadedFonts.find( p_fontName + _T( "_" ) + StringUtils::ToString( p_height ) );

		if ( it != m_loadedFonts.end() )
		{
			l_return = it->second;
		}

		return l_return;
	}

	void OverlayManager::DoAddOverlay( std::shared_ptr< Overlay > p_overlay )
	{
		m_overlays.push_back( p_overlay );
		m_initialisable.push_back( p_overlay );
		m_changed = true;
	}

	void OverlayManager::DoUpdateSize( Size const & p_size )
	{
		if ( p_size != m_size )
		{
			m_size = p_size;
			float l_left = 0;
			float l_right = float( p_size.x() );
			float l_top = 0;
			float l_bottom = float( p_size.y() );
			float l_near = 0;
			float l_far = 1000;

			// Retrieved orthographic projection factors from glOrtho on MSDN
			float l_00 = 2 / ( l_right - l_left );
			float l_11 = 2 / ( l_top - l_bottom );
			float l_22 = -2 / ( l_far - l_near );
			float l_tx = -( l_right + l_left ) / ( l_right - l_left );
			float l_ty = -( l_top + l_bottom ) / ( l_top - l_bottom );
			float l_tz = -( l_far + l_near ) / ( l_far - l_near );

			float l_mvp[16] =
			{
				l_00, 0, 0, 0,
				0, l_11, 0, 0,
				0, 0, l_22, 0,
				l_tx, l_ty, l_tz, 1,
			};

			m_programColour->CreateMat4FloatFrameVariable( _T( "mvp" ) )->SetValue( l_mvp );
			m_programTexture->CreateMat4FloatFrameVariable( _T( "mvp" ) )->SetValue( l_mvp );
			m_programTextColour->CreateMat4FloatFrameVariable( _T( "mvp" ) )->SetValue( l_mvp );
			m_programTextTexture->CreateMat4FloatFrameVariable( _T( "mvp" ) )->SetValue( l_mvp );
		}
	}

	void OverlayManager::DoUpdate()
	{
		m_renderableOverlays.clear();

		for ( auto && l_overlay : m_overlays )
		{
			m_renderableOverlays.push_back( l_overlay );
		}

		std::sort( m_renderableOverlays.begin(), m_renderableOverlays.end(), []( std::weak_ptr< Overlay > p_a, std::weak_ptr< Overlay > p_b )
		{
			uint64_t l_a = p_a.lock()->GetZIndex() + p_a.lock()->GetLevel() * 1000;
			uint64_t l_b = p_b.lock()->GetZIndex() + p_b.lock()->GetLevel() * 1000;
			return l_a < l_b;
		} );
	}
}
