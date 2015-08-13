#include "BorderPanelOverlay.h"

#include "GeometryBuffers.h"
#include "GlFrameVariable.h"
#include "GlShaderProgram.h"
#include "GlVec4FrameVariable.h"
#include "GlVertexBuffer.h"

namespace ProceduralTextures
{
	BorderPanelOverlay::BorderPanelOverlay( gl::OpenGl & p_openGl, Material const & p_material, Material const & p_bordersMaterial, std::shared_ptr< Overlay > p_parent )
		: Overlay( p_openGl, p_material, eOVERLAY_TYPE_BORDER_PANEL, p_parent )
		, m_borderOuterUv( 0, 0, 1, 1 )
		, m_borderInnerUv( 0.33, 0.33, 0.66, 0.66 )
		, m_borderPosition( eBORDER_POSITION_MIDDLE )
		, m_bordersMaterial( p_bordersMaterial )
		, m_bordersGeometryBuffers( p_openGl, GL_DYNAMIC_DRAW, false )
	{
		m_uv = Point4d( 0.33, 0.66, 0.66, 0.33 );
	}

	BorderPanelOverlay::~BorderPanelOverlay()
	{
	}

	void BorderPanelOverlay::SetBordersProgram( eMATERIAL_TYPE p_type, std::shared_ptr< gl::ShaderProgram > p_program )
	{
		m_bordersMaterial.SetType( p_type, p_program );
	}

	void BorderPanelOverlay::SetBordersColour( Colour const & p_colour )
	{
		m_bordersMaterial.SetColour( p_colour );
	}

	void BorderPanelOverlay::SetBordersTexture( std::shared_ptr< gl::Texture > p_texture )
	{
		m_bordersMaterial.SetTexture( p_texture );
	}

	void BorderPanelOverlay::DoInitialise()
	{
		m_bordersGeometryBuffers.Initialise();
	}

	void BorderPanelOverlay::DoCleanup()
	{
		m_bordersGeometryBuffers.Cleanup();
	}

	void BorderPanelOverlay::DoRender()
	{
		m_material.Activate();
		m_geometryBuffers.Draw( m_material.GetVertexAttribute(), m_material.GetTextureAttribute() );
		m_material.Deactivate();

		m_bordersMaterial.Activate();
		m_bordersGeometryBuffers.Draw( m_bordersMaterial.GetVertexAttribute(), m_bordersMaterial.GetTextureAttribute() );
		m_bordersMaterial.Deactivate();
	}

	void BorderPanelOverlay::DoUpdate()
	{
		Position l_pos = GetAbsolutePixelPosition();
		Size l_size = GetAbsolutePixelSize();
		Point4i l_sizes = GetBordersPixelSize();

		int32_t l_centerL = l_pos.x();
		int32_t l_centerT = l_pos.y();
		int32_t l_centerR = l_pos.x() + l_size.x();
		int32_t l_centerB = l_pos.y() + l_size.y();

		if ( GetPixelSize().x() == -1 )
		{
			l_centerR = std::min( l_centerR, int32_t( m_displaySize.x() ) );
		}

		if ( GetPixelSize().y() == -1 )
		{
			l_centerB = std::min( l_centerB, int32_t( m_displaySize.y() ) );
		}

		if ( m_borderPosition == eBORDER_POSITION_INTERNAL )
		{
			l_centerL += l_sizes[0];
			l_centerT += l_sizes[1];
			l_centerR -= l_sizes[2];
			l_centerB -= l_sizes[3];
		}
		else if ( m_borderPosition == eBORDER_POSITION_MIDDLE )
		{
			l_centerL += l_sizes[0] / 2;
			l_centerT += l_sizes[1] / 2;
			l_centerR -= l_sizes[2] / 2;
			l_centerB -= l_sizes[3] / 2;
		}

		int32_t l_borderL = l_centerL - l_sizes[0];
		int32_t l_borderT = l_centerT - l_sizes[1];
		int32_t l_borderR = l_centerR + l_sizes[2];
		int32_t l_borderB = l_centerB + l_sizes[3];

		double l_borderUvLL = double( m_borderOuterUv[0] );
		double l_borderUvTT = double( m_borderOuterUv[1] );
		double l_borderUvML = double( m_borderInnerUv[0] );
		double l_borderUvMT = double( m_borderInnerUv[1] );
		double l_borderUvMR = double( m_borderInnerUv[2] );
		double l_borderUvMB = double( m_borderInnerUv[3] );
		double l_borderUvRR = double( m_borderOuterUv[2] );
		double l_borderUvBB = double( m_borderOuterUv[3] );

		// Center
		std::vector< VertexI > l_vtx;
		l_vtx.reserve( 6 );
		l_vtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerL, l_centerT ), CONSTRUCT_ANONYMOUS( Point2d, m_uv[0], m_uv[3] ) ) );
		l_vtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerL, l_centerB ), CONSTRUCT_ANONYMOUS( Point2d, m_uv[0], m_uv[1] ) ) );
		l_vtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerR, l_centerB ), CONSTRUCT_ANONYMOUS( Point2d, m_uv[2], m_uv[1] ) ) );
		l_vtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerL, l_centerT ), CONSTRUCT_ANONYMOUS( Point2d, m_uv[0], m_uv[3] ) ) );
		l_vtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerR, l_centerB ), CONSTRUCT_ANONYMOUS( Point2d, m_uv[2], m_uv[1] ) ) );
		l_vtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerR, l_centerT ), CONSTRUCT_ANONYMOUS( Point2d, m_uv[2], m_uv[3] ) ) );

		gl::VertexBufferI & l_vertexBuffer = m_geometryBuffers.GetVertexBuffer();
		l_vertexBuffer.SetBuffer( l_vtx );
		l_vertexBuffer.Initialise();

		std::vector< VertexI > l_brdvtx;
		l_brdvtx.reserve( 48 );
		// Corner Top Left
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_borderL, l_borderT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvLL, l_borderUvTT ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_borderL, l_centerT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvLL, l_borderUvMT ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerL, l_centerT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvML, l_borderUvMT ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_borderL, l_borderT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvLL, l_borderUvTT ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerL, l_centerT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvML, l_borderUvMT ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerL, l_borderT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvML, l_borderUvTT ) ) );

		// Border Top
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerL, l_borderT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvML, l_borderUvTT ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerL, l_centerT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvML, l_borderUvMT ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerR, l_centerT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvMR, l_borderUvMT ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerL, l_borderT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvML, l_borderUvTT ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerR, l_centerT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvMR, l_borderUvMT ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerR, l_borderT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvMR, l_borderUvTT ) ) );

		// Corner Top Right
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerR, l_borderT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvMR, l_borderUvTT ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerR, l_centerT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvMR, l_borderUvMT ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_borderR, l_centerT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvRR, l_borderUvMT ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerR, l_borderT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvMR, l_borderUvTT ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_borderR, l_centerT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvRR, l_borderUvMT ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_borderR, l_borderT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvRR, l_borderUvTT ) ) );

		// Border Left
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_borderL, l_centerT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvLL, l_borderUvMT ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_borderL, l_centerB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvLL, l_borderUvMB ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerL, l_centerB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvML, l_borderUvMB ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_borderL, l_centerT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvLL, l_borderUvMT ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerL, l_centerB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvML, l_borderUvMB ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerL, l_centerT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvML, l_borderUvMT ) ) );

		// Border Right
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerR, l_centerT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvMR, l_borderUvMT ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerR, l_centerB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvMR, l_borderUvMB ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_borderR, l_centerB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvRR, l_borderUvMB ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerR, l_centerT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvMR, l_borderUvMT ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_borderR, l_centerB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvRR, l_borderUvMB ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_borderR, l_centerT ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvRR, l_borderUvMT ) ) );

		// Corner Bottom Left
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_borderL, l_centerB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvLL, l_borderUvMB ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_borderL, l_borderB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvLL, l_borderUvBB ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerL, l_borderB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvML, l_borderUvBB ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_borderL, l_centerB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvLL, l_borderUvMB ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerL, l_borderB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvML, l_borderUvBB ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerL, l_centerB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvML, l_borderUvMB ) ) );

		// Border Bottom
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerL, l_centerB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvML, l_borderUvMB ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerL, l_borderB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvML, l_borderUvBB ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerR, l_borderB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvMR, l_borderUvBB ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerL, l_centerB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvML, l_borderUvMB ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerR, l_borderB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvMR, l_borderUvBB ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerR, l_centerB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvMR, l_borderUvMB ) ) );

		// Corner Bottom Right
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerR, l_centerB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvMR, l_borderUvMB ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerR, l_borderB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvMR, l_borderUvBB ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_borderR, l_borderB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvRR, l_borderUvBB ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerR, l_centerB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvMR, l_borderUvMB ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_borderR, l_borderB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvRR, l_borderUvBB ) ) );
		l_brdvtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_borderR, l_centerB ), CONSTRUCT_ANONYMOUS( Point2f, l_borderUvRR, l_borderUvMB ) ) );

		gl::VertexBufferI & l_bordersVertexBuffer = m_bordersGeometryBuffers.GetVertexBuffer();
		l_bordersVertexBuffer.SetBuffer( l_brdvtx );
		l_bordersVertexBuffer.Initialise();
	}

	void BorderPanelOverlay::DoUpdatePositionAndSize( Size const & p_size )
	{
		std::shared_ptr< Overlay > l_parent = GetParent();
		Point2d l_totalSize( p_size.x(), p_size.y() );

		if ( l_parent )
		{
			Point2d l_parentSize = l_parent->GetAbsoluteSize();
			l_totalSize[0] = l_parentSize[0] * l_totalSize[0];
			l_totalSize[1] = l_parentSize[1] * l_totalSize[1];
		}

		Point4i l_sizes = GetBorderPixelSize();
		Point4d l_ptSizes = GetBordersSize();
		bool l_changed = m_changed;

		if ( l_sizes[0] )
		{
			double l_value = std::min( 1.0, l_sizes[0] / l_totalSize[0] );
			l_changed |= std::abs( l_ptSizes[0] - l_value ) > std::numeric_limits< double >::epsilon();
			l_ptSizes[0] = l_value;
		}

		if ( l_sizes[1] )
		{
			double l_value = std::min( 1.0, l_sizes[1] / l_totalSize[1] );
			l_changed |= std::abs( l_ptSizes[1] - l_value ) > std::numeric_limits< double >::epsilon();
			l_ptSizes[1] = l_value;
		}

		if ( l_sizes[2] )
		{
			double l_value = std::min( 1.0, l_sizes[2] / l_totalSize[0] );
			l_changed |= std::abs( l_ptSizes[2] - l_value ) > std::numeric_limits< double >::epsilon();
			l_ptSizes[2] = l_value;
		}

		if ( l_sizes[3] )
		{
			double l_value = std::min( 1.0, l_sizes[3] / l_totalSize[1] );
			l_changed |= std::abs( l_ptSizes[3] - l_value ) > std::numeric_limits< double >::epsilon();
			l_ptSizes[3] = l_value;
		}

		if ( l_changed )
		{
			SetBordersSize( l_ptSizes );
		}
	}
}
