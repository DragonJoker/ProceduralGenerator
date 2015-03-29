#include "Overlay.h"

#include "OverlayManager.h"
#include "GeometryBuffers.h"
#include "OpenGl.h"

namespace ProceduralTextures
{
	Overlay::Overlay( std::shared_ptr< gl::OpenGl > p_openGl, Material const & p_material, eOVERLAY_TYPE p_type, std::shared_ptr< Overlay > p_parent )
		: gl::Holder( p_openGl )
		, m_parent( p_parent )
		, m_overlayType( p_type )
		, m_visible( true )
		, m_zIndex( 1 )
		, m_changed( true )
		, m_material( p_material )
		, m_uv( 0, 1, 1, 0 )
		, m_pixelPositioned( false )
		, m_level( 0 )
	{
		if ( p_parent )
		{
			m_level = p_parent->m_level + 1;
		}
	}

	Overlay::~Overlay()
	{
	}

	void Overlay::Render( Size const & p_size )
	{
		UpdatePositionAndSize( p_size );

		if ( IsVisible() )
		{
			if ( m_changed )
			{
				DoUpdate();
				m_changed = false;
			}

			Point2d l_position = GetAbsolutePosition();
			Point2d l_size = GetAbsoluteSize();

			if ( std::abs( l_size[0] ) > std::numeric_limits< double >::epsilon() // Non null width
					&& std::abs( l_size[1] ) > std::numeric_limits< double >::epsilon() // Non null height
					&& l_position[0] < 1.0 && l_position[0] + l_size[0] > 0.0 // Not beyond screen width
					&& l_position[1] < 1.0 && l_position[1] + l_size[1] > 0.0 // Not beyond screen height
			   )
			{
				DoRender();
			}
		}
	}

	bool Overlay::AddChild( std::shared_ptr< Overlay > p_pOverlay, uint64_t p_iZIndex )
	{
		bool l_bReturn = false;

		if ( p_iZIndex == 0 )
		{
			m_overlaysByZIndex.insert( std::make_pair( GetZIndex(), p_pOverlay ) );
			m_arrayOverlays.push_back( p_pOverlay );
			SetZIndex( GetZIndex() + 100 );
			l_bReturn = true;
		}
		else if ( m_overlaysByZIndex.find( p_iZIndex ) == m_overlaysByZIndex.end() )
		{
			m_overlaysByZIndex.insert( std::make_pair( p_iZIndex, p_pOverlay ) );
			m_arrayOverlays.push_back( p_pOverlay );
			l_bReturn = true;
		}

		p_pOverlay->SetZIndex( p_iZIndex );
		p_pOverlay->m_parent = shared_from_this();
		return l_bReturn;
	}

	void Overlay::Initialise()
	{
		m_geometryBuffers = std::make_shared< GeometryBuffersI >( GetOpenGl(), GL_DYNAMIC_DRAW, false );
		m_geometryBuffers->Initialise();
		DoInitialise();
	}

	void Overlay::Cleanup()
	{
		DoCleanup();
		m_geometryBuffers->Cleanup();
		m_geometryBuffers.reset();
	}

	void Overlay::UpdatePositionAndSize( Size const & p_size )
	{
		m_changed |= p_size != m_displaySize;

		if ( IsChanged() )
		{
			std::shared_ptr< Overlay > l_parent = GetParent();
			Point2d l_totalSize( p_size.x(), p_size.y() );
			m_displaySize = p_size;

			if ( l_parent )
			{
				Point2d l_parentSize = l_parent->GetAbsoluteSize();
				l_totalSize[0] = l_parentSize[0] * l_totalSize[0];
				l_totalSize[1] = l_parentSize[1] * l_totalSize[1];
			}

			Position l_pixelPosition = GetPixelPosition();
			Point2d l_position = GetPosition();
			bool l_changed = m_changed;

			if ( l_pixelPosition.x() || m_pixelPositioned )
			{
				double l_value = l_pixelPosition.x() / l_totalSize[0];
				l_changed |= std::abs( l_position[0] - l_value ) > std::numeric_limits< double >::epsilon();
				l_position[0] = l_value;
			}

			if ( l_pixelPosition.y() || m_pixelPositioned )
			{
				double l_value = l_pixelPosition.y() / l_totalSize[1];
				l_changed |= std::abs( l_position[1] - l_value ) > std::numeric_limits< double >::epsilon();
				l_position[1] = l_value;
			}

			if ( l_changed )
			{
				m_position = l_position;
				m_changed = true;
			}

			Size l_pixelSize = GetPixelSize();
			Point2d l_size = GetSize();
			l_changed = m_changed;

			if ( l_pixelSize.x() )
			{
				double l_value = std::min( 1.0, l_pixelSize.x() / l_totalSize[0] );
				l_changed |= std::abs( l_size[0] - l_value ) > std::numeric_limits< double >::epsilon();
				l_size[0] = l_value;
			}

			if ( l_pixelSize.y() )
			{
				double l_value = std::min( 1.0, l_pixelSize.y() / l_totalSize[1] );
				l_changed |= std::abs( l_size[1] - l_value ) > std::numeric_limits< double >::epsilon();
				l_size[1] = l_value;
			}

			if ( l_changed )
			{
				m_size = l_size;
				m_changed = true;
			}

			DoUpdatePositionAndSize( p_size );
			m_displaySize = p_size;
		}
	}

	Point2d Overlay::GetAbsolutePosition()const
	{
		Point2d l_position( GetPosition() );
		std::shared_ptr< Overlay > l_parent = GetParent();

		if ( l_parent )
		{
			Point2d l_size( l_parent->GetAbsoluteSize() );
			l_size = Point2d( std::min( 1.0, l_size[0] ), std::min( 1.0, l_size[1] ) );
			l_position *= l_size;
			l_position += l_parent->GetAbsolutePosition();
		}

		return l_position;
	}

	Point2d Overlay::GetAbsoluteSize()const
	{
		Point2d l_size( GetSize() );
		std::shared_ptr< Overlay > l_parent = GetParent();

		if ( l_parent )
		{
			Point2d l_parentSize( l_parent->GetAbsoluteSize() );
			l_parentSize = Point2d( std::min( 1.0, l_parentSize[0] ), std::min( 1.0, l_parentSize[1] ) );
			l_size *= l_parentSize;
		}

		return l_size;
	}

	Position Overlay::GetAbsolutePixelPosition()const
	{
		Point2d l_position = GetAbsolutePosition();
		return Position( int32_t( l_position[0] * m_displaySize.x() ), int32_t( l_position[1] * m_displaySize.y() ) );
	}

	Size Overlay::GetAbsolutePixelSize()const
	{
		Point2d l_size = GetAbsoluteSize();
		return Size( uint32_t( l_size[0] * m_displaySize.x() ), uint32_t( l_size[1] * m_displaySize.y() ) );
	}

	void Overlay::SetProgram( eMATERIAL_TYPE p_type, std::shared_ptr< gl::ShaderProgram > p_program )
	{
		m_material.SetType( p_type, p_program );
	}

	void Overlay::SetColour( Colour const & p_colour )
	{
		m_material.SetColour( p_colour );
	}

	void Overlay::SetTexture( std::shared_ptr< gl::Texture > p_texture )
	{
		m_material.SetTexture( p_texture );
	}

	bool Overlay::IsVisible()const
	{
		return m_visible && ( GetParent() ? GetParent()->IsVisible() : true );
	}

	void Overlay::SetVisible( bool p_visible )
	{
		m_visible = p_visible;
	}

	bool Overlay::IsChanged()const
	{
		bool l_return = m_changed;
		std::shared_ptr< Overlay > l_parent = GetParent();

		if ( !m_changed && l_parent )
		{
			l_return = l_parent->IsChanged();
		}

		return l_return;
	}
}
