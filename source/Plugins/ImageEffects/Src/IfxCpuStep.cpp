#include "IfxCpuStep.h"

#include "AnisotropicNoiseReductionFunctor.h"
#include "DummyFunctor.h"
#include "ExplosionFunctor.h"
#include "GaussianBlurFunctor.h"
#include "GaussianBlurSharpenFunctor.h"
#include "GrayScaleFunctor.h"
#include "ImplosionFunctor.h"
#include "LinearNoiseReductionFunctor.h"
#include "RecursiveAnisotropicNoiseReductionFunctor.h"
#include "RecursiveGaussianBlurFunctor.h"
#include "RecursiveGaussianBlurSharpenFunctor.h"
#include "RecursiveLinearNoiseReductionFunctor.h"
#include "RecursiveSharpenFunctor.h"
#include "SepiaFunctor.h"
#include "SharpenFunctor.h"
#include "WatershedFunctor.h"
#include "WaveFunctor.h"

using namespace ProceduralTextures;

namespace ImageEffects
{
	Thread::Thread( std::shared_ptr< ProceduralTextures::CpuStepBase > p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight )
		: ProceduralTextures::Thread( p_pParent, p_uiIndex, iWidth, iTop, iBottom, iTotalHeight )
		, m_firstInBuffer( std::make_shared< PixelBuffer >( Size( iWidth, iTotalHeight ) ) )
		, m_secondInBuffer( std::make_shared< PixelBuffer >( Size( iWidth, iTotalHeight ) ) )
		, m_pParent( std::static_pointer_cast< CpuStep >( p_pParent ) )
	{
		std::shared_ptr< CpuStep > l_generator = m_pParent.lock();
		m_pFirstFunctionBufferIn = m_firstInBuffer;
		m_pFirstFunction = l_generator->GetFunctor( eEFFECT_TYPE_NONE );
		m_pNextFirstFunction = l_generator->GetFunctor( eEFFECT_TYPE_NONE );
		m_pSecondFunction = l_generator->GetFunctor( eEFFECT_TYPE_NONE );
		m_pNextSecondFunction = l_generator->GetFunctor( eEFFECT_TYPE_NONE );
	}

	Thread::~Thread()
	{
	}

	void Thread::SetImage( PixelBuffer const & p_image )
	{
		m_size = p_image.GetDimensions();
		m_firstInBuffer->Initialise( m_size );
		m_secondInBuffer->Initialise( m_size );
		m_firstInBuffer->Set( *m_backBuffer.lock() );
	}

	void Thread::SetFirstFunction( eEFFECT_TYPE p_eType )
	{
		std::shared_ptr< CpuStep > l_generator = m_pParent.lock();
		m_pNextFirstFunction = l_generator->GetFunctor( p_eType );

		if ( !m_pNextFirstFunction->AllowsSecondEffect() )
		{
			m_pNextSecondFunction = l_generator->GetFunctor( eEFFECT_TYPE_NONE );
			m_pNextFirstFunctionBufferIn = m_backBuffer;
		}
		else
		{
			m_pNextFirstFunctionBufferIn = m_firstInBuffer;
		}
	}

	void Thread::SetSecondFunction( eEFFECT_TYPE p_eType )
	{
		std::shared_ptr< CpuStep > l_generator = m_pParent.lock();

		if ( m_pFirstFunction->AllowsSecondEffect() && l_generator->GetFunctor( p_eType )->CanBeSecondEffect() )
		{
			m_pNextSecondFunction = l_generator->GetFunctor( p_eType );
		}
	}

	void Thread::DoStep()
	{
		if ( m_pNextFirstFunction != m_pFirstFunction )
		{
			m_backBuffer.lock()->Set( *m_firstInBuffer );
			m_pFirstFunction = m_pNextFirstFunction;
			m_pFirstFunctionBufferIn = m_pNextFirstFunctionBufferIn;
		}

		if ( m_pNextSecondFunction != m_pSecondFunction )
		{
			m_backBuffer.lock()->Set( *m_firstInBuffer );
			m_pSecondFunction = m_pNextSecondFunction;
		}

		if ( !m_pFirstFunctionBufferIn.expired() )
		{
			( *m_pFirstFunction )( *m_pFirstFunctionBufferIn.lock(), *m_secondInBuffer );
			( *m_pSecondFunction )( *m_secondInBuffer, *m_backBuffer.lock() );
		}
	}

	//*************************************************************************************************

	CpuStep::CpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, ProceduralTextures::Size const & p_size )
		: ProceduralTextures::CpuStep< Thread >( p_generator, p_size )
	{
		for ( int i = eEFFECT_TYPE_NONE; i < eEFFECT_TYPE_COUNT; i++ )
		{
			switch ( i )
			{
			case eEFFECT_TYPE_NONE:
				m_functors[i] = std::make_shared< DummyFunctor >();
				break;

			case eEFFECT_TYPE_GAUSSIAN_BLUR:
				m_functors[i] = std::make_shared< GaussianBlurFunctor >();
				break;

			case eEFFECT_TYPE_SHARPEN:
				m_functors[i] = std::make_shared< SharpenFunctor >();
				break;

			case eEFFECT_TYPE_GAUSSIAN_BLUR_SHARPEN:
				m_functors[i] = std::make_shared< GaussianBlurSharpenFunctor >();
				break;

			case eEFFECT_TYPE_LINEAR_NR:
				m_functors[i] = std::make_shared< LinearNoiseReductionFunctor >();
				break;

			case eEFFECT_TYPE_ANISOTROPIC_NR:
				m_functors[i] = std::make_shared< AnisotropicNoiseReductionFunctor >();
				break;

			case eEFFECT_TYPE_WAVE:
				m_functors[i] = std::make_shared< WaveFunctor >();
				break;

			case eEFFECT_TYPE_GRAYSCALE:
				m_functors[i] = std::make_shared< GrayScaleFunctor >();
				break;

			case eEFFECT_TYPE_SEPIA:
				m_functors[i] = std::make_shared< SepiaFunctor >();
				break;

			case eEFFECT_TYPE_IMPLOSION:
				m_functors[i] = std::make_shared< ImplosionFunctor >();
				break;

			case eEFFECT_TYPE_EXPLOSION:
				m_functors[i] = std::make_shared< ExplosionFunctor >();
				break;

			case eEFFECT_TYPE_WATERSHED:
				m_functors[i] = std::make_shared< WatershedFunctor >();
				break;

			case eEFFECT_TYPE_REC_GAUSSIAN_BLUR:
				m_functors[i] = std::make_shared< RecursiveGaussianBlurFunctor >();
				break;

			case eEFFECT_TYPE_REC_SHARPEN:
				m_functors[i] = std::make_shared< RecursiveSharpenFunctor >();
				break;

			case eEFFECT_TYPE_REC_GAUSSIAN_BLUR_SHARPEN:
				m_functors[i] = std::make_shared< RecursiveGaussianBlurSharpenFunctor >();
				break;

			case eEFFECT_TYPE_REC_LINEAR_NR:
				m_functors[i] = std::make_shared< RecursiveLinearNoiseReductionFunctor >();
				break;

			case eEFFECT_TYPE_REC_ANISOTROPIC_NR:
				m_functors[i] = std::make_shared< RecursiveAnisotropicNoiseReductionFunctor >();
				break;
			}

			m_functors[i]->SetGenerator( this );
		}
	}

	CpuStep::~CpuStep()
	{
		for ( auto && l_functor : m_functors )
		{
			l_functor.reset();
		}
	}

	void CpuStep::DoInitialiseStep()
	{
	}

	void CpuStep::DoInitialise()
	{
		DoCreateThread( 0, m_sizeImage.x(), m_sizeImage.y() );

		DoForEachThread( [this]( Thread & l_thread )
		{
			l_thread.m_backBuffer = m_finalBuffer;
			l_thread.Run();
		} );
	}

	void CpuStep::DoCleanup()
	{
	}

	void CpuStep::DoSwapBuffers()
	{
	}

	std::shared_ptr< EffectFunctor > CpuStep::GetFunctor( eEFFECT_TYPE p_eType )
	{
		return m_functors[p_eType];
	}

	void CpuStep::SetFirstFunction( uint32_t p_choice )
	{
		DoForEachThread( [&p_choice]( Thread & l_thread )
		{
			l_thread.SetFirstFunction( eEFFECT_TYPE( p_choice ) );
		} );
	}

	void CpuStep::SetSecondFunction( uint32_t p_choice )
	{
		DoForEachThread( [&p_choice]( Thread & l_thread )
		{
			l_thread.SetSecondFunction( eEFFECT_TYPE( p_choice ) );
		} );
	}

	void CpuStep::SetImage( PixelBuffer const & p_buffer )
	{
		m_initialised = false;
		*m_finalBuffer = p_buffer;

		for ( auto && l_functor : m_functors )
		{
			l_functor->SetImage( *m_finalBuffer );
		}

		DoForEachThread( [this]( Thread & l_thread )
		{
			l_thread.SetImage( *m_finalBuffer );
		} );

		m_initialised = true;
	}
}
