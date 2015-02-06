/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/ )

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option ) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___GENERATOR_PLUGIN_H___
#define ___GENERATOR_PLUGIN_H___

#include "GeneratorPrerequisites.h"

#include "ProceduralGenerator.h"

//! Macro to use in the header of a class that derives from Plugin
#	define DECLARE_PLUGIN() virtual void Destroy();
#if defined( __WXMSW__ )
#	include < process.h>
//! Macro to use in the source of a class that derives from plugin
#	define IMPLEMENT_PLUGIN( PluginClass, GeneratorClass )	extern "C"\
															{\
																__declspec( dllexport ) PluginBase * CreatePlugin( wxDynamicLibrary * p_pDll, wxApp * p_pApp ){ return Plugin< GeneratorClass, PluginClass >::Creator( p_pDll, p_pApp ); }\
																__declspec( dllexport ) void DestroyPlugin() { Plugin< GeneratorClass, PluginClass >::Destroyer(); }\
															}\
															IMPLEMENT_APP_NO_MAIN( PluginClass )\
															void PluginClass::Destroy()\
															{\
																DestroyPlugin();\
															}
#else
//! Macro to use in the source of a class that derives from plugin
#	define IMPLEMENT_PLUGIN( PluginClass, GeneratorClass )	extern "C" PluginBase * CreatePlugin( wxDynamicLibrary * p_pDll, wxApp * p_pApp ) { return new PluginClass; }\
															void PluginClass::Destroy()\
															{\
																delete this;\
															}
#endif

namespace ProceduralTextures
{
	/*!
	\author 	Sylvain DOREMUS
	\brief		Base class for a plugin
	\remark		Exposes the functions needed to implement a plugin
	*/
	class GeneratorAPI PluginBase
#if defined( __WXMSW__ )
		:   public wxApp
#endif
	{
	protected:
		wxLocale  * m_pLocale;
		wxString	m_strName;

	public:
		//! Typedef to the exported function used to create a plugin
		typedef PluginBase * ( CreatePluginFunction )( wxDynamicLibrary * p_pDll, wxApp * p_pApp );
		typedef CreatePluginFunction * PCreatePluginFunction;

	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_strName	Plugin internal name
		 */
		PluginBase( wxString const & p_strName )
			:	m_pLocale( NULL )
			,	m_strName( p_strName )
		{
			DoInitialiseLanguage();
		}
		/**
		 *\brief		Destructor
		 */
		virtual ~PluginBase()
		{
			DoCleanupLanguage();
		}
		/**
		 *\brief		Destroys this plugin
		 *\remark		Implemented in \p IMPLEMENT_PLUGIN macro
		 */
		virtual void Destroy() = 0;
		/**
		*\brief		Creates the ProceduralGenerator
		*\remark		Implemented in Plugin
		*/
		virtual ProceduralGenerator * CreateGenerator( int p_iWidth, int p_iHeight, int p_iWndId, wxFrame * p_pFrame ) = 0;
		/**
		 *\brief		Gives the plugin displayable name
		 *\remark		Derived classes must implement it
		 */
		virtual wxString GetName() = 0;
		/**
		 *\brief		Loads plugin language files, if any
		 */
		void DoInitialiseLanguage()
		{
			if ( !m_pLocale )
			{
				wxStandardPathsBase const & l_stdPaths = wxStandardPaths::Get();
				long 		l_lLanguage		= wxLANGUAGE_DEFAULT;
				wxFileName	l_pathExe		= l_stdPaths.GetExecutablePath();
				wxString	l_strPath		= l_pathExe.GetPath( wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR );
				wxString	l_strSep		= wxFileName::GetPathSeparator();
				std::size_t l_uiIndex = l_strPath.find( l_strSep + wxT( "bin" ) + l_strSep );

				if ( l_uiIndex != wxString::npos )
				{
					l_strPath = l_strPath.substr( 0, l_uiIndex );
				}

				// load language if possible, fall back to english otherwise
				if ( wxLocale::IsAvailable( l_lLanguage ) )
				{
					m_pLocale = new wxLocale( l_lLanguage, wxLOCALE_LOAD_DEFAULT );
					// add locale search paths
					m_pLocale->AddCatalogLookupPathPrefix( l_strPath + l_strSep + wxT( "share" ) + l_strSep + wxT( "ProceduralGenerator" ) );
#if defined( _MSC_VER )
#	if defined( NDEBUG )
					m_pLocale->AddCatalogLookupPathPrefix( l_strPath + l_strSep + wxT( "share" ) + l_strSep + wxT( "Release" ) + l_strSep + wxT( "ProceduralGenerator" ) );
#	else
					m_pLocale->AddCatalogLookupPathPrefix( l_strPath + l_strSep + wxT( "share" ) + l_strSep + wxT( "Debug" ) + l_strSep + wxT( "ProceduralGenerator" ) );
#	endif
#endif
					m_pLocale->AddCatalog( m_strName );

					if ( !m_pLocale->IsOk() )
					{
						std::cerr << "Selected language is wrong" << std::endl;
						delete m_pLocale;
						m_pLocale = new wxLocale( wxLANGUAGE_ENGLISH );
						l_lLanguage = wxLANGUAGE_ENGLISH;
					}
				}
				else
				{
					std::cout << "The selected language is not supported by your system."
							  << "Try installing support for this language." << std::endl;
					m_pLocale = new wxLocale( wxLANGUAGE_ENGLISH );
					l_lLanguage = wxLANGUAGE_ENGLISH;
				}
			}
		}
		/**
		 *\brief		Cleans up plugin language
		 */
		void DoCleanupLanguage()
		{
			delete m_pLocale;
			m_pLocale = NULL;
		}
	};
	/*!
	\author 	Sylvain DOREMUS
	\brief		Temlate class for a plugin
	\remark		Implements the CreateGenerator function
				< br / >On MS-Windows, it also creates the dll main application
	*/
	template< class GeneratorClass, class PluginClass >
	class Plugin : public PluginBase
	{
#if defined( __WXMSW__ )
	private:
		static wxCriticalSection	m_critStartup;
		static HANDLE				m_hThreadId;
		static wxDynamicLibrary	*	m_pDll;

		typedef enum eIDs
		{
			eTerminate
		}	eIDs;

		static unsigned int wxSTDCALL Launcher( void * p_event )
		{
#	if wxVERSION_NUMBER < 2900
			const HINSTANCE l_hInstance = m_pDll->GetLibHandle();
#	else
			const HINSTANCE l_hInstance = wxDynamicLibrary::MSWGetModuleHandle( "", & Plugin< GeneratorClass, PluginClass >::m_hThreadId );
#	endif

			if ( !l_hInstance )
			{
				return 0;
			}

#	if wxVERSION_NUMBER >= 2900
			wxDISABLE_DEBUG_SUPPORT();
#	endif
			wxInitializer l_wxinit;

			if ( !l_wxinit.IsOk() )
			{
				return 0;
			}

			HANDLE l_hEvent = *( static_cast< HANDLE * >( p_event ) );

			if ( !::SetEvent( l_hEvent ) )
			{
				return 0;
			}

			wxEntry( l_hInstance );
			return 1;
		}

	public:
		/**
		 *\brief		THe function that effectively creates the plugin and launches the dll main
		 */
		static PluginBase * Creator( wxDynamicLibrary * p_pDll, wxApp * WXUNUSED( p_pApp ) )
		{
			PluginBase * pReturn = NULL;
			wxCriticalSectionLocker lock( m_critStartup );

			if ( ! m_hThreadId )
			{
				m_pDll = p_pDll;
				HANDLE l_hEvent =::CreateEvent( NULL, FALSE, FALSE, NULL );

				if ( ! l_hEvent )
				{
					return NULL;
				}

				m_hThreadId = ( HANDLE )_beginthreadex( NULL, 0, & Launcher, & l_hEvent, 0, NULL );

				if ( ! m_hThreadId )
				{
					::CloseHandle( l_hEvent );
					return NULL;
				}

				::WaitForSingleObject( l_hEvent, INFINITE );
				::CloseHandle( l_hEvent );
				pReturn = ( PluginBase * )wxApp::GetInstance();
			}

			return pReturn;
		}
		/**
		 *\brief		The function that effectively destroys the plugin data
		 */
		static void Destroyer()
		{
			wxCriticalSectionLocker lock( m_critStartup );

			if ( ! m_hThreadId )
			{
				return;
			}

			m_pDll->Detach();
			delete m_pDll;
			wxThreadEvent * l_pEvent = new wxThreadEvent( wxEVT_THREAD, eTerminate );
			wxTheApp->AddPendingEvent( * l_pEvent );

			// We must then wait for the thread to actually terminate.
			if ( ::WaitForSingleObject( m_hThreadId, 10000 ) == WAIT_TIMEOUT )
			{
				::TerminateThread( m_hThreadId, 0xFFFFFFFF );
			}

			CloseHandle( m_hThreadId );
			m_hThreadId = NULL;
			delete l_pEvent;
		}
		void OnTerminate( wxThreadEvent & WXUNUSED( p_event ) )
		{
			ExitMainLoop();
		}
#endif
	public:
		/**
		 *\brief		Constructor
		 *\remark		On MS-Windows, it connects the wxApp to the terminate event, in order to be able to exit the dll main
		 */
		Plugin( wxString const & p_strName )
			:	PluginBase( p_strName )
		{
#if defined( __WXMSW__ )
			Connect( eTerminate, wxEVT_THREAD, wxThreadEventHandler( Plugin::OnTerminate ) );
#else
			DoInitialiseLanguage();
#endif
		}
		/**
		 *\brief		Destructor
		 */
		virtual ~Plugin()
		{
#if !defined( __WXMSW__ )
			DoCleanupLanguage();
#endif
		}
		/**
		*\brief		Creates the ProceduralGenerator
		*\param[in]	p_iWidth	The computing surface width
		*\param[in]	p_iHeight	The computing surface height
		*\param[in]	p_iWndId	(unused )
		*\param[in]	p_pFrame	The frame holding the generator result
		*/
		ProceduralGenerator * CreateGenerator( int p_iWidth, int p_iHeight, int p_iWndId, wxFrame * p_pFrame )
		{
			return new GeneratorClass( p_iWidth, p_iHeight, p_iWndId, p_pFrame );;
		}
#if defined( __WXMSW__ )
		/**
		 *\brief		Needed by wxApp inheritance
		 */
		bool OnInit()
		{
			DoInitialiseLanguage();
			return true;
		}
		/**
		 *\brief		Needed by wxApp inheritance
		 */
		int OnExit()
		{
			DoCleanupLanguage();
			return 0;
		}
#endif
	};

#if defined( __WXMSW__ )
	template< class GeneratorClass, class PluginClass > wxCriticalSection	Plugin< GeneratorClass, PluginClass >::m_critStartup;
	template< class GeneratorClass, class PluginClass > HANDLE				Plugin< GeneratorClass, PluginClass >::m_hThreadId		= NULL;
	template< class GeneratorClass, class PluginClass > wxDynamicLibrary *	Plugin< GeneratorClass, PluginClass >::m_pDll			= NULL;
#endif
}

#endif
