#include "OclCpuStep.h"

namespace OCLProcessing
{
	CpuStep::CpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, ProceduralTextures::Size const & p_size )
		: ProceduralTextures::CpuStepBase( p_generator, p_size )
	{
	}

	CpuStep::~CpuStep()
	{
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
