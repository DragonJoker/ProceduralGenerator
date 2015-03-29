#include "TfxCpuStep.h"

namespace TextEffects
{
	CpuStep::CpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, ProceduralTextures::Size const & p_size )
		: ProceduralTextures::CpuStepBase( p_generator, p_size )
	{
	}

	CpuStep::~CpuStep()
	{
	}

	void CpuStep::SetBackgroundImage( ProceduralTextures::PixelBuffer const & p_image )
	{
		m_initialised = false;
		m_finalBuffer->Initialise( p_image.GetDimensions() );
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
