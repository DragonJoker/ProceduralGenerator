#include "ShaderEffects.h"

#include <GlFrameBuffer.h>
#include <GlTexture.h>

using namespace ProceduralTextures;

namespace ShaderEffects
{
	CpuStep::CpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, Size const & p_size )
		: ProceduralTextures::CpuStepBase( p_generator, p_size )
	{
	}

	CpuStep::~CpuStep()
	{
	}

	void CpuStep::SetImage( ProceduralTextures::PixelBuffer const & p_image )
	{
		m_initialised = false;
		*m_finalBuffer = p_image;
		m_initialised = true;
	}

	void CpuStep::DoInitialiseStep()
	{
	}

	void CpuStep::DoInitialise()
	{
	}

	void CpuStep::DoCleanup()
	{
	}

	void CpuStep::DoSwapBuffers()
	{
	}

	void CpuStep::DoThreadsStart()
	{
	}

	void CpuStep::DoThreadsCleanup()
	{
	}
}
