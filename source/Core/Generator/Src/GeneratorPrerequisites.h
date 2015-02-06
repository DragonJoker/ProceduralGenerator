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
#ifndef ___GENERATOR_PREREQUISITES_H___
#define ___GENERATOR_PREREQUISITES_H___

#if defined( _WIN32 )
#	if !defined( Generator_SHARED )
#		define GeneratorAPI
#	else
#		if defined( Generator_EXPORTS )
#			define GeneratorAPI __declspec( dllexport )
#		else
#			define GeneratorAPI __declspec( dllimport )
#		endif
#	endif
#else
#	define GeneratorAPI
#endif

#include <set>
#include <map>
#include <list>
#include <vector>
#include <limits>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <functional>
#include <cmath>
#include <cstring>
#include <cstdint>

#include "glew.h"

#if defined( _MSC_VER )
#	pragma warning( disable:4251 )	// ... needs to have dll-interface to be used by clients ...
#	pragma warning( disable:4275 )	// ... non dll-interface class 'wxPanel' used as base for dll-interface class ...
#	pragma warning( disable:4996 )	// ... This function or variable may be unsafe ...
#	pragma warning( push )
#	pragma warning( disable:4311 )	// 'cast de type' : troncation de pointeur de 'PVOID' à 'LONG'
#	pragma warning( disable:4312 )	// 'cast de type' : la conversion de 'LONG' en 'PVOID' d'une taille supérieure
#endif

#include <wx/app.h>
#include <wx/button.h>
#include <wx/choicdlg.h>
#include <wx/colordlg.h>
#include <wx/combobox.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/dialog.h>
#include <wx/display.h>
#include <wx/dynlib.h>
#include <wx/event.h>
#include <wx/filedlg.h>
#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/fontdlg.h>
#include <wx/frame.h>
#include <wx/glcanvas.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <wx/notebook.h>
#include <wx/numdlg.h>
#include <wx/panel.h>
#include <wx/rawbmp.h>
#include <wx/slider.h>
#include <wx/stattext.h>
#include <wx/statline.h>
#include <wx/statusbr.h>
#include <wx/stdpaths.h>
#include <wx/sysopt.h>
#include <wx/textfile.h>
#include <wx/thread.h>
#include <wx/tglbtn.h>
#include <wx/timer.h>
#include <wx/utils.h>
#include <wx/wx.h>

#ifdef __WXMSW__
#	include< wx/msw/msvcrt.h>
#endif

#if defined( _MSC_VER )
#	pragma warning( pop )
#endif

namespace ProceduralTextures
{
#if defined( __WXMSW__ )
#	define wxDEFAULT_HEIGHT	25
#else
#	define wxDEFAULT_HEIGHT	30
#endif
	////////////////////////////////////////////////////////////////////////////////////////////////////
	//	GUI
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Enumeration of supported controls in a ConfigPanel
	*/
	typedef enum
	{
		eCONTROL_TYPE_NONE,
		eCONTROL_TYPE_STATIC,		//!< wxStatic
		eCONTROL_TYPE_EDIT,			//!< wxTextCtrl
		eCONTROL_TYPE_SLIDER,		//!< wxSlider
		eCONTROL_TYPE_COMBO,		//!< wxComboBox
		eCONTROL_TYPE_BUTTON,		//!< wxButton
		eCONTROL_TYPE_FILE_BUTTON,	//!< wxButton used for file loading
	}	eCONTROL_TYPE;
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Enumeration of supported events in ControlInfos
	*/
	typedef enum
	{
		eEVENT_TYPE_NONE,
		eEVENT_TYPE_SLIDER_UPDATED,			//!< Slider updated
		eEVENT_TYPE_SCROLL_THUMBRELEASE,	//!< Scroll thumb release
		eEVENT_TYPE_SCROLL_THUMBTRACK,		//!< Scroll thumb track
		eEVENT_TYPE_COMBOBOX_SELECTED,		//!< Combo box element selected
		eEVENT_TYPE_BUTTON_CLIKED,			//!< Button clicked
		eEVENT_TYPE_TEXT_UPDATED,			//!< Keyboard text updated
		eEVENT_TYPE_TEXT_ENTER,				//!< Keyboard text return
		eEVENT_TYPE_LOAD_TEXT_FILEA,		//!< Button load text file clicked
		eEVENT_TYPE_LOAD_TEXT_FILEB,		//!< Button load another text file clicked
		eEVENT_TYPE_LOAD_IMAGEA,			//!< Button load image clicked
		eEVENT_TYPE_LOAD_IMAGEB,			//!< Button load another image clicked
		eEVENT_TYPE_LOAD_BINARY_FILEA,		//!< Button load binary file clicked
		eEVENT_TYPE_LOAD_BINARY_FILEB,		//!< Button load another binary file clicked
		eEVENT_TYPE_LOAD_COLOURA,			//!< Button load colour clicked
		eEVENT_TYPE_LOAD_COLOURB,			//!< Button load another colour clicked
		eEVENT_TYPE_COUNT
	}	eEVENT_TYPE;
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Enumeration of supported GUI events in ControlInfos
	*/
	typedef enum eGUI_EVENT
	{
		eGUI_EVENT_SHOW_MESSAGE_BOX,	//!< Event to ask the main window to show a message box : p_event.GetString() may contain '||' then the left is the caption, the right is the message
		eGUI_EVENT_RESIZE_PARENT,		//!< Event to ask the main window to resize to the given size  : wxSize( p_event.GetInt(), p_event..GetExtraLong() )
	}	eGUI_EVENT;
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Base class for control specific data
	\remark		Objects of this class are uncopyable
	*/
	class SpecificControlParametersBase;
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Template class to implement SpecificControlParametersBase
	*/
	template< eCONTROL_TYPE Type > class SpecificControlParameters;
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Class describing a control, to allow it's creation by ConfigPanel
	*/
	class ControlInfos;
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Panel holding generator tweaks
	*/
	class ConfigPanel;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//	MAIN
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Enumeration of event ids
	*/
	typedef enum
	{
		eEVENT_WORKER_THREAD_END	= 100000,	//!< \~english Event launched on worker thread step end					\~french Evènement lancé par un thread de travail pour dire qu'il a fini son travail
		eEVENT_THREAD_STOP			= 100001,	//!< \~english Event launched to stop main thread 						\~french Evènement lancé pour terminer le thread principal
		eEVENT_CPU_STEP_END			= 100002,	//!< \~english Event launched by main thread when computation ends		\~french Evènement lancé par le thread principal quand les calculs sont terminés
		eEVENT_TEXTA				= 100003,	//!< \~english Event sent to main thread to give it a text file path		\~french Evènement envoyé au thread principal pour lui donner l'adresse d'un fichier texte
		eEVENT_TEXTB				= 100004,	//!< \~english Event sent to main thread to give it a text file path		\~french Evènement envoyé au thread principal pour lui donner l'adresse d'un fichier texte
		eEVENT_IMAGEA				= 100005,	//!< \~english Event sent to main thread to give it an image file path	\~french Evènement envoyé au thread principal pour lui donner l'adresse d'un fichier image
		eEVENT_IMAGEB				= 100006,	//!< \~english Event sent to main thread to give it an image file path	\~french Evènement envoyé au thread principal pour lui donner l'adresse d'un fichier image
		eEVENT_BINARYA				= 100007,	//!< \~english Event sent to main thread to give it a binary file path	\~french Evènement envoyé au thread principal pour lui donner l'adresse d'un fichier binaire
		eEVENT_BINARYB				= 100008,	//!< \~english Event sent to main thread to give it a binary file path	\~french Evènement envoyé au thread principal pour lui donner l'adresse d'un fichier binaire
		eEVENT_COLOURA				= 100009,	//!< \~english Event sent to main thread to give it a colour				\~french Evènement envoyé au thread principal pour lui donner une couleur
		eEVENT_COLOURB				= 100010,	//!< \~english Event sent to main thread to give it a colour				\~french Evènement envoyé au thread principal pour lui donner une couleur
		eEVENT_MOUSEMOVE			= 100011,	//!< \~english Event sent to main thread to retrieve mouse position		\~french Evènement envoyé au thread principal pour lui donner la position de la souris
	}	eEVENT;
	/*!
	\author		Sylvain DOREMUS
	\date		03/01/2011
	\version	1.0.0.0
	\brief		Policy for various types
	\remark		Policy created to use various types in template classes such as Point, Matrix, and others.
				< br / >Holds  operators, initialisation functions, conversion functions, ...
	*/
	template < typename T > class Policy;
	/*!
	\author		Sylvain DOREMUS
	\date		03/01/2011
	\version	1.0.0.0
	\brief		Base class for points, used to share functions without taking care of templates
	*/
	class PointBase;
	/*!
	\author		Sylvain DOREMUS
	\date		03/01/2011
	\version	1.0.0.0
	\brief		Templated type, dynamic dimensions point class
	*/
	template < typename T > class DynPoint;
	/*!
	\author		Sylvain DOREMUS
	\date		03/01/2011
	\version	1.0.0.0
	\brief		Templated type, static dimensions point representation
	\remark		The template type must have a defined Policy
	*/
	template < typename T, size_t Count > class Point;
	/*!
	\author 	Sylvain DOREMUS
	\date 		03/01/2011
	\version 	1.0.0.0
	\brief 		Description of a pixel
	\remark		Four components, Red, Green, Blue and Alpha. Component type is templated
	*/
	template < typename T > class Pixel;
	/*!
	\author 	Sylvain DOREMUS
	\date 		03/01/2011
	\version 	1.0.0.0
	\brief 		Pixel buffer description
	\remark		It has 2 dimensions and is row major.
	*/
	template < typename T > class Buffer;
	/*!
	\author		Sylvain DOREMUS
	\date		01/07/2011
	\version	1.2.0.0
	\brief		Plugin base class
	\remark		Under MS Windows, in order to have access to every dynamic wxWidget class,
				< br / >we must instantiate a winapp for each dll loaded. So plugin extends wxApp and lauches a dllmain.
				< br / >It causes troubles with OpenGL, which functions must be loaded in each main, so OpenGL functions
				< br / >are loaded in each procedural generator, instead of being loaded in the main application.
	*/
	class PluginBase;
	/*!
	\author		Sylvain DOREMUS
	\date		01/07/2011
	\version	1.2.0.0
	\brief		Plugin class, to be instantiated in each generator
	*/
	template< class GeneratorClass, class PluginClass > class Plugin;
	/*!
	\author		Sylvain DOREMUS
	\date		01/07/2011
	\version	1.0.0.0
	\brief		Random number generator class
	*/
	class Random;
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version	1.0.0.0
	\brief		Base class for all generators
	*/
	class ProceduralGenerator;
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Typedef on a Point with 1 float component
	*/
	typedef Point< float, 1 > Point1f;
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Typedef on a Point with 2 float components
	*/
	typedef Point< float, 2 > Point2f;
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Typedef on a Point with 3 float components
	*/
	typedef Point< float, 3 > Point3f;
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Typedef on a Point with 4 float components
	*/
	typedef Point< float, 4 > Point4f;
	/*!
	\author		Sylvain DOREMUS
	\date		09/09/2014
	\version 	1.3.0.3
	\brief		Typedef on a Point with 1 double component
	*/
	typedef Point< double, 1 > Point1d;
	/*!
	\author		Sylvain DOREMUS
	\date		09/09/2014
	\version 	1.3.0.3
	\brief		Typedef on a Point with 2 double components
	*/
	typedef Point< double, 2 > Point2d;
	/*!
	\author		Sylvain DOREMUS
	\date		09/09/2014
	\version 	1.3.0.3
	\brief		Typedef on a Point with 3 double components
	*/
	typedef Point< double, 3 > Point3d;
	/*!
	\author		Sylvain DOREMUS
	\date		09/09/2014
	\version 	1.3.0.3
	\brief		Typedef on a Point with 4 double components
	*/
	typedef Point< double, 4 > Point4d;
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Typedef on a Point with 1 int component
	*/
	typedef Point< int32_t, 1 > Point1i;
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Typedef on a Point with 2 int components
	*/
	typedef Point< int32_t, 2 > Point2i;
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Typedef on a Point with 3 int components
	*/
	typedef Point< int32_t, 3 > Point3i;
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Typedef on a Point with 4 int components
	*/
	typedef Point< int32_t, 4 > Point4i;
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Typedef on a Point with 1 byte component
	*/
	typedef Point< uint8_t, 1 > Point1ub;
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Typedef on a Point with 2 byte components
	*/
	typedef Point< uint8_t, 2 > Point2ub;
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Typedef on a Point with 3 byte components
	*/
	typedef Point< uint8_t, 3 > Point3ub;
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Typedef on a Point with 4 byte components
	*/
	typedef Point< uint8_t, 4 > Point4ub;
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Typedef on a pixel with byte components
	*/
	typedef Pixel< uint8_t > UbPixel;
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Typedef on a pixel with unsigned int components
	*/
	typedef Pixel< uint32_t > UiPixel;
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Typedef on an array of ControlInfos
	*/
	typedef Buffer<  UbPixel > PixelBuffer;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//	GPU
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/*!
	\author 	Sylvain DOREMUS
	\date		01/07/2011
	\version 	1.2.0.0
	\brief		Shader object supported types enumerator
	*/
	typedef enum
	{
		eSHADER_OBJECT_TYPE_NONE		= -1,
		eSHADER_OBJECT_TYPE_VERTEX		= 0,	//!< Vertex shader
		eSHADER_OBJECT_TYPE_PIXEL		= 1,	//!< Fragment shader
		eSHADER_OBJECT_TYPE_COUNT
	}	eSHADER_OBJECT_TYPE;
	/*!
	\author		Sylvain DOREMUS
	\date 		03/01/2011
	\version 	1.0.0.0
	\brief		Representation of a vertex
	\remark		Specialisation of BufferElement, containing vertex's position, normal, tangent and texture coordinates
	*/
	class Vertex;
	/*!
	\author		Sylvain DOREMUS
	\date 		03/01/2011
	\version 	1.0.0.0
	\brief		OpenGL API wrapper
	*/
	class OpenGl;
	/*!
	\author		Sylvain DOREMUS
	\date 		03/01/2011
	\version 	1.0.0.0
	\brief		OpenGL GPU buffer base class
	*/
	class GlBuffer;
	/*!
	\author		Sylvain DOREMUS
	\date 		03/01/2011
	\version 	1.0.0.0
	\brief		OpenGL Vertex Buffer Object
	*/
	class GlVertexBuffer;
	/*!
	\author		Sylvain DOREMUS
	\date 		03/01/2011
	\version 	1.0.0.0
	\brief		OpenGL Index Buffer Object
	*/
	class GlIndexBuffer;
	/*!
	\author		Sylvain DOREMUS
	\date		01/07/2011
	\version 	1.2.0.0
	\~english
	\brief		OpenGL Pixel Buffer Object base class
	*/
	class GlPixelBuffer;
	/*!
	\author		Sylvain DOREMUS
	\date		01/07/2011
	\version 	1.2.0.0
	\brief		Pixel Buffer Object used to send data to GPU
	*/
	class GlDownloadPixelBuffer;
	/*!
	\author		Sylvain DOREMUS
	\date		01/07/2011
	\version 	1.2.0.0
	\brief		Pixel Buffer Object used to retrieve data from GPU
	*/
	class GlUploadPixelBuffer;
	/*!
	\author		Sylvain DOREMUS
	\date 		03/01/2011
	\version 	1.0.0.0
	\brief		OpenGL texture representation
	\remark		Uses 2 Pixel Buffer Objects to transfer data from/to VRAM memory to/from RAM memory
	*/
	class GlTexture;
	/*!
	\author		Sylvain DOREMUS
	\date		01/07/2011
	\version 	1.2.0.0
	\brief		OpenGL Frame Buffer Object
	*/
	class GlFrameBuffer;
	/*!
	\author		Sylvain DOREMUS
	\date		01/07/2011
	\version 	1.2.0.0
	\brief		Frame variable (uniforms ) base class
	*/
	class GlFrameVariableBase;
	/*!
	\author		Sylvain DOREMUS
	\date		01/07/2011
	\version 	1.2.0.0
	\brief		Type templated frame variable class
	*/
	template < typename T > class GlFrameVariable;
	/*!
	\author		Sylvain DOREMUS
	\date		01/07/2011
	\version 	1.2.0.0
	\brief		Structure used to deifne a frame variable value in a shader
	*/
	template< typename T > struct VariableApplier;
	/*!
	\author		Sylvain DOREMUS
	\date		01/07/2011
	\version 	1.2.0.0
	\brief		Shader object class (vertex, fragment, ... )
	\remark
	*/
	class GlShaderObject;
	/*!
	\author		Sylvain DOREMUS
	\date		01/07/2011
	\version 	1.2.0.0
	\brief		Shader program class, holds shader objects
	*/
	class GlShaderProgram;
	/*!
	\author		Sylvain DOREMUS
	\date		01/07/2011
	\version 	1.2.0.0
	\brief		Typedef on a list of GlFrameVariableBase
	*/
	typedef std::list< GlFrameVariableBase * > FrameVariablePtrList;
	/*!
	\author		Sylvain DOREMUS
	\date		01/07/2011
	\version 	1.2.0.0
	\brief		Typedef on a map of GlFrameVariableBase, ordered by name
	*/
	typedef std::map< wxString, GlFrameVariableBase * > FrameVariablePtrStrMap;

	namespace detail
	{
		/**
		 *\brief		Counts available CPUs
		 *\return		The CPU count
		 */
		int GetCPUCount();
	}
}

#endif
