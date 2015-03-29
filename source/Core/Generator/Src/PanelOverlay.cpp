#include "PanelOverlay.h"

#include "GeometryBuffers.h"
#include "GlShaderProgram.h"
#include "GlFrameVariable.h"
#include "GlTexture.h"
#include "GlVec4FrameVariable.h"
#include "GlVertexBuffer.h"

namespace ProceduralTextures
{
	PanelOverlay::PanelOverlay( std::shared_ptr< gl::OpenGl > p_openGl, Material const & p_material, std::shared_ptr< Overlay > p_parent )
		: Overlay( p_openGl, p_material, eOVERLAY_TYPE_PANEL, p_parent )
	{
	}

	PanelOverlay::~PanelOverlay()
	{
	}

	void PanelOverlay::DoInitialise()
	{
	}

	void PanelOverlay::DoCleanup()
	{
	}

	void PanelOverlay::DoRender()
	{
		m_material.Activate();
		m_geometryBuffers->Draw( m_material.GetVertexAttribute(), m_material.GetTextureAttribute() );
		m_material.Deactivate();
	}

	void PanelOverlay::DoUpdate()
	{
		Position l_position = GetAbsolutePixelPosition();
		Size l_size = GetAbsolutePixelSize();

		int32_t l_centerL = l_position.x();
		int32_t l_centerT = l_position.y();
		int32_t l_centerR = l_position.x() + l_size.x();
		int32_t l_centerB = l_position.y() + l_size.y();

		if ( GetPixelSize().x() == -1 )
		{
			l_centerR = std::min( l_centerR, int( m_displaySize.x() ) );
		}

		if ( GetPixelSize().y() == -1 )
		{
			l_centerB = std::min( l_centerB, int( m_displaySize.y() ) );
		}

		std::vector< VertexI > l_vtx;
		l_vtx.reserve( 6 );
		l_vtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerL, l_centerT ), CONSTRUCT_ANONYMOUS( Point2f, float( m_uv[0] ), float( m_uv[3] ) ) ) );
		l_vtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerL, l_centerB ), CONSTRUCT_ANONYMOUS( Point2f, float( m_uv[0] ), float( m_uv[1] ) ) ) );
		l_vtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerR, l_centerB ), CONSTRUCT_ANONYMOUS( Point2f, float( m_uv[2] ), float( m_uv[1] ) ) ) );
		l_vtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerL, l_centerT ), CONSTRUCT_ANONYMOUS( Point2f, float( m_uv[0] ), float( m_uv[3] ) ) ) );
		l_vtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerR, l_centerB ), CONSTRUCT_ANONYMOUS( Point2f, float( m_uv[2] ), float( m_uv[1] ) ) ) );
		l_vtx.push_back( VertexI( CONSTRUCT_ANONYMOUS( VertexI::TPoint, l_centerR, l_centerT ), CONSTRUCT_ANONYMOUS( Point2f, float( m_uv[2] ), float( m_uv[3] ) ) ) );

		std::shared_ptr< gl::VertexBufferI > l_vertexBuffer = m_geometryBuffers->GetVertexBuffer();
		l_vertexBuffer->SetBuffer( l_vtx );
		l_vertexBuffer->Initialise();
	}
}
