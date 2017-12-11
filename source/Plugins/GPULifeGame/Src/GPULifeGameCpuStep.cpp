#include "GpuLifeGameCpuStep.h"

#include <GlFrameBuffer.h>
#include <GlTexture.h>

namespace GpuLifeGame
{
	CpuStep::CpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, ProceduralTextures::Size const & p_size )
		: ProceduralTextures::DefaultCpuStep( p_generator, p_size )
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
}
