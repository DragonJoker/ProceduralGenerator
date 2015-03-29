#include "PlnGpuStep.h"

namespace PerlinNoise
{
	GpuStep::GpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, ProceduralTextures::Size const & p_size, ProceduralTextures::Size const & p_bordersSize )
		: ProceduralTextures::GpuStep( p_generator, p_size, p_bordersSize )
	{
	}

	GpuStep::~GpuStep()
	{
	}

	void GpuStep::DoInitialise()
	{
	}

	void GpuStep::DoCleanup()
	{
	}

	void GpuStep::DoResize()
	{
	}

	void GpuStep::DoPreRender()
	{
	}

	void GpuStep::DoRender( bool & )
	{
	}

	void GpuStep::DoPostRender()
	{
	}
}
