#include "SwmEffect.h"

#include <GlShaderProgram.h>
#include <GlShaderObject.h>
#include <GlFrameVariable.h>

#include <cmath>

namespace ProceduralTextures
{
	static const wxString VertexProgram =
		wxT( "void main()\n" )
		wxT( "{\n" )
		wxT( "	gl_TexCoord[0] = gl_MultiTexCoord0;\n" )
		wxT( "	gl_Position = ftransform();\n" )
		wxT( "}\n" );

	//*************************************************************************************************

	Effect :: Effect( OpenGl * p_pOpenGl, wxImage * p_pImage )
		:	m_pImage( p_pImage )
		,	m_pShaderProgram( NULL )
		,	m_strVertexFile()
		,	m_strFragmentFile()
		,	m_bNewShader( false )
		,	m_llStartTime( 0 )
		,	m_iWidth( 0 )
		,	m_iHeight( 0 )
		,	m_pOpenGl( p_pOpenGl )
	{
		if ( p_pImage && p_pImage->IsOk() )
		{
			m_iWidth = p_pImage->GetWidth();
			m_iHeight = p_pImage->GetHeight();
		}
	}

	Effect :: ~Effect()
	{
		delete m_pShaderProgram;

		for ( size_t i = 0 ; i < m_arrayImages.size() ; i++ )
		{
			delete m_arrayImages[i];
		}

		m_arrayImages.clear();
		m_arrayImagesFiles.clear();
	}

	void Effect :: SetVertexFile( const wxString & p_strPath )
	{
		m_strVertexFile = p_strPath;
	}

	void Effect :: SetFragmentFile( const wxString & p_strPath )
	{
		m_strFragmentFile = p_strPath;
	}

	void Effect :: Compile()
	{
		m_bNewShader = true;
	}

	void Effect :: SetImagePath( size_t p_iImage, const wxString & p_strImagePath )
	{
		wxImage * l_pImage = NULL;

		if ( p_iImage >= m_arrayImages.size() )
		{
			l_pImage = new wxImage();
			m_arrayImages.push_back( l_pImage );
			m_arrayImagesFiles.push_back( p_strImagePath );
		}
		else
		{
			l_pImage = m_arrayImages[p_iImage];
			m_arrayImagesFiles[p_iImage] = p_strImagePath;
		}

		if ( l_pImage->LoadFile( p_strImagePath ) )
		{
			int l_iWidth = l_pImage->GetWidth();
			int l_iHeight = l_pImage->GetHeight();
			std::cout << "Original : " << l_iWidth << "x" << l_iHeight << "\n";

			if ( m_iWidth != l_iWidth || m_iHeight != l_iHeight )
			{
				std::cout << "Computed : " << m_iWidth << "x" << m_iHeight << "\n";
				l_pImage->Rescale( m_iWidth, m_iHeight, wxIMAGE_QUALITY_HIGH );
				std::cout << "Scaled : " << l_pImage->GetWidth() << "x" << l_pImage->GetHeight() << "\n";
			}
		}
		else
		{
			//		wxMessageBox( wxT( "No handler found for this image format"));
		}
	}

	void Effect :: Initialise()
	{
		if ( m_pShaderProgram != NULL )
		{
			m_pShaderProgram->Cleanup();
		}
		else
		{
			m_pShaderProgram = new GlShaderProgram( m_pOpenGl );
		}

		if ( !m_strVertexFile.empty() )
		{
			m_pShaderProgram->CreateObject( eSHADER_OBJECT_TYPE_VERTEX );
			m_pShaderProgram->SetProgramFile( m_strVertexFile, eSHADER_OBJECT_TYPE_VERTEX );
		}
		else
		{
			m_pShaderProgram->CreateObject( eSHADER_OBJECT_TYPE_VERTEX );
			m_pShaderProgram->SetProgramText( VertexProgram, eSHADER_OBJECT_TYPE_VERTEX );
		}

		if ( ! m_strFragmentFile.empty() )
		{
			m_pShaderProgram->CreateObject( eSHADER_OBJECT_TYPE_PIXEL );
			m_pShaderProgram->SetProgramFile( m_strFragmentFile, eSHADER_OBJECT_TYPE_PIXEL );
		}

		m_pShaderProgram->Initialise();
		m_uniformWidth = m_pShaderProgram->CreateIntFrameVariable( wxT( "pg_width" ) );
		m_uniformHeight = m_pShaderProgram->CreateIntFrameVariable( wxT( "pg_height" ) );
		m_uniformTime = m_pShaderProgram->CreateIntFrameVariable( wxT( "pg_time" ) );
		m_uniformSepType = m_pShaderProgram->CreateIntFrameVariable( wxT( "pg_sep_type" ) );
		m_uniformSepOffset = m_pShaderProgram->CreateIntFrameVariable( wxT( "pg_sep_offset" ) );
		m_pShaderProgram->CreateIntFrameVariable( wxT( "pg_texture" ) );
		m_pShaderProgram->CreateIntFrameVariable( wxT( "pg_modif1" ) )->SetValue( 1 );
		m_pShaderProgram->CreateIntFrameVariable( wxT( "pg_modif2" ) )->SetValue( 2 );
		m_pShaderProgram->CreateIntFrameVariable( wxT( "pg_modif3" ) )->SetValue( 3 );
		m_llStartTime = wxGetLocalTimeMillis();
		m_bNewShader = false;
	}

	void Effect :: Cleanup()
	{
		if ( m_pShaderProgram != NULL )
		{
			m_pShaderProgram->Cleanup();
		}
	}

	bool Effect :: Activate( eSEPARATION p_eSeparationType, int p_iSeparationOffset )
	{
		bool l_bReturn = true;

		if ( m_pShaderProgram != NULL )
		{
			m_uniformWidth->SetValue( m_iWidth );
			m_uniformHeight->SetValue( m_iHeight );
			m_uniformSepType->SetValue( int( p_eSeparationType ) );
			m_uniformSepOffset->SetValue( p_iSeparationOffset );
			m_uniformTime->SetValue( int( ( wxGetLocalTimeMillis() - m_llStartTime ).ToLong() ) );
			m_pShaderProgram->Activate();
		}

		return l_bReturn;
	}

	void Effect :: Deactivate()
	{
		if ( m_pShaderProgram != NULL )
		{
			m_pShaderProgram->Deactivate();
		}
	}

	wxString Effect :: GetCompilerLog( eSHADER_OBJECT_TYPE p_eType )
	{
		wxString l_strReturn;

		if ( m_pShaderProgram != NULL )
		{
			GlShaderObject * l_pShader = m_pShaderProgram->GetObject( p_eType );

			if ( l_pShader != NULL )
			{
				l_strReturn = l_pShader->GetCompilerLog();
			}
		}

		return l_strReturn;
	}

	wxString Effect :: GetLinkerLog()
	{
		wxString l_strReturn;

		if ( m_pShaderProgram != NULL )
		{
			l_strReturn = m_pShaderProgram->GetLinkerLog();
		}

		return l_strReturn;
	}

	void Effect :: UpdateImages()
	{
		m_iWidth = m_pImage->GetWidth();
		m_iHeight = m_pImage->GetHeight();

		for ( size_t i = 0 ; i < m_arrayImages.size() ; i++ )
		{
			m_arrayImages[i]->LoadFile( m_arrayImagesFiles[i] );
			m_arrayImages[i]->ResampleBicubic( m_iWidth, m_iHeight );
		}
	}
}
