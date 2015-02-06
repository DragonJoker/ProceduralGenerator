#include "ImageEffects.h"

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

#include <cmath>

namespace ProceduralTextures
{
	EffectFunctor * ImageEffects::Thread::m_pFunctors[eEFFECT_TYPE_COUNT];

	ImageEffects::Thread::Thread( ProceduralGenerator * p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight, const UbPixel & p_pxColour )
		:	ProceduralGenerator::Thread( p_pParent, p_uiIndex, iWidth, iTop, iBottom, iTotalHeight, p_pxColour )
		,	m_firstInBuffer( Size( iWidth, iTotalHeight ) )
		,	m_secondInBuffer( Size( iWidth, iTotalHeight ) )
		,	m_pFirstFunction( m_pFunctors[eEFFECT_TYPE_NONE] )
		,	m_pNextFirstFunction( m_pFunctors[eEFFECT_TYPE_NONE] )
		,	m_pSecondFunction( m_pFunctors[eEFFECT_TYPE_NONE] )
		,	m_pNextSecondFunction( m_pFunctors[eEFFECT_TYPE_NONE] )
		,	m_pFirstFunctionBufferIn( & m_firstInBuffer )
	{
	}

	ImageEffects::Thread::~Thread()
	{
	}

	void ImageEffects::Thread::Step()
	{
		if ( m_pNextFirstFunction != m_pFirstFunction )
		{
			( * m_backBuffer ) = m_firstInBuffer;
			m_pFirstFunction = m_pNextFirstFunction;
			m_pFirstFunctionBufferIn = m_pNextFirstFunctionBufferIn;
		}

		if ( m_pNextSecondFunction != m_pSecondFunction )
		{
			( * m_backBuffer ) = m_firstInBuffer;
			m_pSecondFunction = m_pNextSecondFunction;
		}

		( *m_pFirstFunction )( * m_pFirstFunctionBufferIn, m_secondInBuffer );
		( *m_pSecondFunction )( m_secondInBuffer, * m_backBuffer );
	}

	void ImageEffects::Thread::SetImage( const wxImage & p_image )
	{
		m_iImgWidth = p_image.GetWidth();
		m_iImgHeight = p_image.GetHeight();
		m_firstInBuffer.init( Size( m_iImgWidth, m_iImgHeight ) );
		m_secondInBuffer.init( Size( m_iImgWidth, m_iImgHeight ) );
		m_firstInBuffer = * m_backBuffer;

		for ( int i = eEFFECT_TYPE_NONE ; i < eEFFECT_TYPE_COUNT ; i++ )
		{
			m_pFunctors[i]->SetImage( p_image );
		}
	}

	void ImageEffects::Thread::SetFirstFunction( eEFFECT_TYPE p_eType )
	{
		m_pNextFirstFunction = m_pFunctors[p_eType];

		if ( ! m_pFunctors[p_eType]->AllowsSecondEffect() )
		{
			m_pNextSecondFunction = m_pFunctors[eEFFECT_TYPE_NONE];
			m_pNextFirstFunctionBufferIn = m_backBuffer;
		}
		else
		{
			m_pNextFirstFunctionBufferIn = & m_firstInBuffer;
		}
	}

	void ImageEffects::Thread::SetSecondFunction( eEFFECT_TYPE p_eType )
	{
		if ( m_pFirstFunction->AllowsSecondEffect() && m_pFunctors[p_eType]->CanBeSecondEffect() )
		{
			m_pNextSecondFunction = m_pFunctors[p_eType];
		}
	}

	void ImageEffects::Thread::InitialiseFunctors( ImageEffects * p_pParent )
	{
		for ( int i = eEFFECT_TYPE_NONE ; i < eEFFECT_TYPE_COUNT ; i++ )
		{
			switch ( i )
			{
			case eEFFECT_TYPE_NONE:
				m_pFunctors[i] = new DummyFunctor;
				break;

			case eEFFECT_TYPE_GAUSSIAN_BLUR:
				m_pFunctors[i] = new GaussianBlurFunctor;
				break;

			case eEFFECT_TYPE_SHARPEN:
				m_pFunctors[i] = new SharpenFunctor;
				break;

			case eEFFECT_TYPE_GAUSSIAN_BLUR_SHARPEN:
				m_pFunctors[i] = new GaussianBlurSharpenFunctor;
				break;

			case eEFFECT_TYPE_LINEAR_NR:
				m_pFunctors[i] = new LinearNoiseReductionFunctor;
				break;

			case eEFFECT_TYPE_ANISOTROPIC_NR:
				m_pFunctors[i] = new AnisotropicNoiseReductionFunctor;
				break;

			case eEFFECT_TYPE_WAVE:
				m_pFunctors[i] = new WaveFunctor;
				break;

			case eEFFECT_TYPE_GRAYSCALE:
				m_pFunctors[i] = new GrayScaleFunctor;
				break;

			case eEFFECT_TYPE_SEPIA:
				m_pFunctors[i] = new SepiaFunctor;
				break;

			case eEFFECT_TYPE_IMPLOSION:
				m_pFunctors[i] = new ImplosionFunctor;
				break;

			case eEFFECT_TYPE_EXPLOSION:
				m_pFunctors[i] = new ExplosionFunctor;
				break;

			case eEFFECT_TYPE_WATERSHED:
				m_pFunctors[i] = new WatershedFunctor;
				break;

			case eEFFECT_TYPE_REC_GAUSSIAN_BLUR:
				m_pFunctors[i] = new RecursiveGaussianBlurFunctor;
				break;

			case eEFFECT_TYPE_REC_SHARPEN:
				m_pFunctors[i] = new RecursiveSharpenFunctor;
				break;

			case eEFFECT_TYPE_REC_GAUSSIAN_BLUR_SHARPEN:
				m_pFunctors[i] = new RecursiveGaussianBlurSharpenFunctor;
				break;

			case eEFFECT_TYPE_REC_LINEAR_NR:
				m_pFunctors[i] = new RecursiveLinearNoiseReductionFunctor;
				break;

			case eEFFECT_TYPE_REC_ANISOTROPIC_NR:
				m_pFunctors[i] = new RecursiveAnisotropicNoiseReductionFunctor;
				break;
			}

			m_pFunctors[i]->SetGenerator( p_pParent );
		}
	}

	void ImageEffects::Thread::CleanupFunctors()
	{
		for ( int i = eEFFECT_TYPE_NONE ; i < eEFFECT_TYPE_COUNT ; i++ )
		{
			delete m_pFunctors[i];
			m_pFunctors[i] = NULL;
		}
	}

	//*************************************************************************************************

	ImageEffects::ImageEffects( int p_width, int p_height, int p_iWndId, wxFrame * p_pFrame )
		:	ProceduralGenerator( p_width, p_height, p_iWndId, p_pFrame )
		,	m_specStaticFirstFunction( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "First Function" ) ) )
		,	m_specComboFirstFunction( SpecificControlParameters< eCONTROL_TYPE_COMBO >( wxT( "" ), wxArrayString() ) )
		,	m_specStaticSecondFunction( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "Second Function" ) ) )
		,	m_specComboSecondFunction( SpecificControlParameters< eCONTROL_TYPE_COMBO >( wxT( "" ), wxArrayString() ) )
		,	m_specButtonImage( SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON >( _( "Image" ) ) )
	{
		Thread::InitialiseFunctors( this );
		m_bNeedsConfigPanel = true;
		wxSystemOptions::SetOption( wxT( "msw.remap" ), 0 );
		wxInitAllImageHandlers();
		ImageEffects::Thread * l_pThread;
		l_pThread = CreateThread< ImageEffects::Thread >( p_width, 0, p_height, p_height, m_pxColour );
		l_pThread->m_backBuffer = & m_backBuffer;
		l_pThread->Run();
		DoGeneratePanel();
	}

	ImageEffects::~ImageEffects()
	{
		Thread::CleanupFunctors();
		wxImage::CleanUpHandlers();
		m_initialised = false;
		DoCleanup();
	}

	void ImageEffects::SetFirstFunction( eEFFECT_TYPE p_eType )
	{
		for ( size_t i = 0 ; i < m_arraySlaveThreads.size() ; i++ )
		{
			if ( m_arraySlaveThreads[i] != NULL )
			{
				static_cast< ImageEffects::Thread * >( m_arraySlaveThreads[i] )->SetFirstFunction( p_eType );
			}
		}
	}

	void ImageEffects::SetSecondFunction( eEFFECT_TYPE p_eType )
	{
		for ( size_t i = 0 ; i < m_arraySlaveThreads.size() ; i++ )
		{
			if ( m_arraySlaveThreads[i] != NULL )
			{
				static_cast< ImageEffects::Thread * >( m_arraySlaveThreads[i] )->SetSecondFunction( p_eType );
			}
		}
	}

	void ImageEffects::SetImagePath( const wxString & p_strImagePath )
	{
		m_initialised = false;
		wxSize l_size = DoLoadImage( p_strImagePath, m_image, m_backBuffer );
		Resize( l_size );

		for ( size_t i = 0 ; i < m_arraySlaveThreads.size() ; i++ )
		{
			if ( m_arraySlaveThreads[i] != NULL )
			{
				static_cast< ImageEffects::Thread * >( m_arraySlaveThreads[i] )->SetImage( m_image );
			}
		}

		m_initialised = true;
	}

	EffectFunctor * ImageEffects::GetFunctor( eEFFECT_TYPE p_eType )
	{
		return Thread::m_pFunctors[p_eType];
	}

	void ImageEffects::DoGeneratePanel()
	{
		EffectFunctor * l_pFunctor;
		int iWidth = 150;

		for ( int i = eEFFECT_TYPE_NONE ; i < eEFFECT_TYPE_COUNT ; i++ )
		{
			l_pFunctor = GetFunctor( eEFFECT_TYPE( i ) );
			m_specComboFirstFunction.m_arrayValues.push_back( l_pFunctor->GetName() );

			if ( l_pFunctor->CanBeSecondEffect() )
			{
				m_specComboSecondFunction.m_arrayValues.push_back( l_pFunctor->GetName() );
			}
		}

		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,		wxID_ANY,			wxPoint( 10, wxDEFAULT_HEIGHT * 0 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specStaticFirstFunction ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_COMBO,		eID_FIRSTFUNCTION,	wxPoint( 10, wxDEFAULT_HEIGHT * 1 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specComboFirstFunction,			wxCB_READONLY,		eEVENT_TYPE_COMBOBOX_SELECTED,	wxCommandEventHandler(	ImageEffects::OnFirstFunction ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,		wxID_ANY,			wxPoint( 10, wxDEFAULT_HEIGHT * 2 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specStaticSecondFunction ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_COMBO,		eID_SECONDFUNCTION,	wxPoint( 10, wxDEFAULT_HEIGHT * 3 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specComboSecondFunction,			wxCB_READONLY,		eEVENT_TYPE_COMBOBOX_SELECTED,	wxCommandEventHandler(	ImageEffects::OnSecondFunction ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_FILE_BUTTON,	eID_IMAGEPATH,		wxPoint( 10, wxDEFAULT_HEIGHT * 4 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specButtonImage,					0,					eEVENT_TYPE_LOAD_IMAGEA,		wxCommandEventHandler(	ImageEffects::OnImagePath ) ) );
	}

	void ImageEffects::OnFirstFunction( wxCommandEvent & p_event )
	{
		int l_iChoice = p_event.GetInt();
		SetFirstFunction( eEFFECT_TYPE( l_iChoice ) );

		if ( l_iChoice >= eEFFECT_TYPE_REC_GAUSSIAN_BLUR )
		{
			m_specComboSecondFunction.m_pControl->SetSelection( 0 );
			m_specComboSecondFunction.m_pControl->Hide();
			m_specStaticSecondFunction.m_pControl->Hide();
		}
		else
		{
			m_specComboSecondFunction.m_pControl->Show();
			m_specStaticSecondFunction.m_pControl->Show();
		}
	}

	void ImageEffects::OnSecondFunction( wxCommandEvent & p_event )
	{
		int l_iChoice = p_event.GetInt();
		SetSecondFunction( eEFFECT_TYPE( l_iChoice ) );
	}

	void ImageEffects::OnImagePath( wxCommandEvent & p_event )
	{
		wxString l_strImagePath = p_event.GetString();

		if ( ! l_strImagePath.empty() )
		{
			SetImagePath( l_strImagePath );
		}
	}
}

//*************************************************************************************************
