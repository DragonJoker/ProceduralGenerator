/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/ )

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (At your option ) any later
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

#include <algorithm>
#include <atomic>
#include <array>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <deque>
#include <functional>
#include <iostream>
#include <limits>
#include <locale>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <sstream>
#include <thread>
#include <vector>

#if defined( _MSC_VER )
#	pragma warning( disable:4251 )	// ... needs to have dll-interface to be used by clients ...
#	pragma warning( disable:4275 )	// ... non dll-interface class '' used as base for dll-interface class ...
#	pragma warning( disable:4996 )	// ... This function or variable may be unsafe ...
#	pragma warning( push )
#	pragma warning( disable:4311 )	// 'cast de type' : troncation de pointeur de 'PVOID' à 'LONG'
#	pragma warning( disable:4312 )	// 'cast de type' : la conversion de 'LONG' en 'PVOID' d'une taille supérieure
#endif

#if defined( _MSC_VER )
#	pragma warning( pop )
#endif

#include "PlatformConfig.h"
#include "CompilerConfig.h"

#if HAS_INITIALIZER_LISTS
#	define CONSTRUCT_NAMED( type, name, ... ) type name = { __VA_ARGS__ }
#	define CONSTRUCT_ANONYMOUS( type, ... ) { __VA_ARGS__ }
#else
#	define CONSTRUCT_NAMED( type, name, ... ) type name( __VA_ARGS__ )
#	define CONSTRUCT_ANONYMOUS( type, ... ) type( __VA_ARGS__ )
#endif

namespace ProceduralTextures
{
	typedef std::chrono::high_resolution_clock Clock;

#if defined( _WIN32 )
	const String FOLDER_SEPARATOR = _T( "\\" );
#else
	const String FOLDER_SEPARATOR = _T( "/" );
#endif
	const int CONFIG_PANEL_WIDTH = 150;

	const int DEFAULT_HEIGHT = 25;
	const String DEFAULT_FONT_NAME = _T( "Arial" );
	const uint32_t DEFAULT_FONT_HEIGHT = 11;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//	GUI
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Enumeration of supported controls in a OptionsPanel
	*/
	typedef enum eCONTROL_TYPE
		: uint8_t
	{
		eCONTROL_TYPE_NONE,
		//! StaticCtrl
		eCONTROL_TYPE_STATIC,
		//! EditCtrl
		eCONTROL_TYPE_EDIT,
		//! SliderCtrl
		eCONTROL_TYPE_SLIDER,
		//! ComboBoxCtrl
		eCONTROL_TYPE_COMBO,
		//! ListBoxCtrl
		eCONTROL_TYPE_LIST,
		//! ButtonCtrl
		eCONTROL_TYPE_BUTTON,
	}	eCONTROL_TYPE;
	/*!
	\author		Sylvain DOREMUS
	\date		06/03/2015
	\version 	2.0.0
	\brief		Enumeration of supported combobox events
	*/
	typedef enum eCOMBOBOX_EVENT
		: uint8_t
	{
		//! Item selected
		eCOMBOBOX_EVENT_SELECTED,
		//! The combo box events count
		eCOMBOBOX_EVENT_COUNT,
	}	eCOMBOBOX_EVENT;
	/*!
	\author		Sylvain DOREMUS
	\date		06/03/2015
	\version 	2.0.0
	\brief		Enumeration of supported listbox events
	*/
	typedef enum eLISTBOX_EVENT
		: uint8_t
	{
		//! Item selected
		eLISTBOX_EVENT_SELECTED,
		//! The combobox events count
		eLISTBOX_EVENT_COUNT,
	}	eLISTBOX_EVENT;
	/*!
	\author		Sylvain DOREMUS
	\date		06/03/2015
	\version 	2.0.0
	\brief		Enumeration of supported button events
	*/
	typedef enum eBUTTON_EVENT
		: uint8_t
	{
		//! Button clicked
		eBUTTON_EVENT_CLICKED,
		//! The button events count
		eBUTTON_EVENT_COUNT,
	}	eBUTTON_EVENT;
	/*!
	\author		Sylvain DOREMUS
	\date		06/03/2015
	\version 	2.0.0
	\brief		Enumeration of supported slider events
	*/
	typedef enum eSLIDER_EVENT
		: uint8_t
	{
		//! Slider thumb release
		eSLIDER_EVENT_THUMBRELEASE,
		//! Slider thumb track
		eSLIDER_EVENT_THUMBTRACK,
		//! The slider events count
		eSLIDER_EVENT_COUNT,
	}	eSLIDER_EVENT;
	/*!
	\author		Sylvain DOREMUS
	\date		06/03/2015
	\version 	2.0.0
	\brief		Enumeration of supported edit events
	*/
	typedef enum eEDIT_EVENT
		: uint8_t
	{
		//! The edit text has changed
		eEDIT_EVENT_UPDATED,
		//! The return key has been hit (the edit must have eEDIT_STYLE_PROCESS_ENTER style)
		eEDIT_EVENT_RETURN,
		//! The edit events count
		eEDIT_EVENT_COUNT,
	}	eEDIT_EVENT;
	/*!
	\author		Sylvain DOREMUS
	\date		11/02/2015
	\version	2.0.0
	\brief		Comb control supported styles
	*/
	typedef enum eCOMBO_STYLE
		: uint32_t
	{
		//! Read only combo box
		eCOMBO_STYLE_READONLY = 0x00000001,
	}	eCOMBO_STYLE;
	/*!
	\author		Sylvain DOREMUS
	\date		11/02/2015
	\version	2.0.0
	\brief		Edit control supported styles
	*/
	typedef enum eEDIT_STYLE
		: uint32_t
	{
		//! The edit control process 'return' hit as an event and not as a newline
		eEDIT_STYLE_PROCESS_ENTER = 0x00000100,
		//! The edit control is a multiline edit control
		eEDIT_STYLE_MULTILINE = 0x00000200,
		//! The edit control process 'tab' hit as the tab character and not as an event
		eEDIT_STYLE_PROCESS_TAB = 0x00000400,
		//! Words are not cut
		eEDIT_STYLE_WORDWRAP = 0x00000800,
	}	eEDIT_STYLE;
	/*!
	\author		Sylvain DOREMUS
	\date		11/02/2015
	\version	2.0.0
	\brief		Slider control supported styles
	*/
	typedef enum eSLIDER_STYLE
		: uint32_t
	{
		// The slider is displaye horizontally (default mode)
		eSLIDER_STYLE_HORIZONTAL = 0x00010000,
		// The slider is displaye vertically
		eSLIDER_STYLE_VERTICAL = 0x000200000,
	}	eSLIDER_STYLE;
	/*!
	\author		Sylvain DOREMUS
	\date		11/02/2015
	\version	2.0.0
	\brief		Static control supported styles
	*/
	typedef enum eSTATIC_STYLE
		: uint32_t
	{
		//! The static's text is left aligned
		eSTATIC_STYLE_HALIGN_LEFT	= 0x01000000,
		//! The static's text is horizontally centered
		eSTATIC_STYLE_HALIGN_CENTER	= 0x02000000,
		//! The static's text is right aligned
		eSTATIC_STYLE_HALIGN_RIGHT	= 0x04000000,
		//! The static's text is aligned on top
		eSTATIC_STYLE_VALIGN_TOP	= 0x10000000,
		//! The static's text is vertically centered
		eSTATIC_STYLE_VALIGN_CENTER	= 0x20000000,
		//! The static's text is aligned on bottom
		eSTATIC_STYLE_VALIGN_BOTTOM	= 0x40000000,
	}	eSTATIC_STYLE;
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\version 	1.3.0.0
	\brief		Enumeration of supported GUI events in ControlInfos
	*/
	typedef enum eGUI_EVENT
		: uint8_t
	{
		//! Event to ask the main window to show a message box
		eGUI_EVENT_SHOW_MESSAGE_BOX,
		//! Event to ask the main window to resize to the given size
		eGUI_EVENT_RESIZE_PARENT,
	}	eGUI_EVENT;
	/*!
	\author		Sylvain DOREMUS
	\date		23/02/2015
	\version 	2.0.0
	\brief		Enumeration of supported overlay types
	*/
	typedef enum eOVERLAY_TYPE
		: uint8_t
	{
		//! Panel overlay
		eOVERLAY_TYPE_PANEL,
		//! Border panel overlay
		eOVERLAY_TYPE_BORDER_PANEL,
		//! Text overlay
		eOVERLAY_TYPE_TEXT,
	}	eOVERLAY_TYPE;
	/*!
	\author 	Sylvain DOREMUS
	\date		23/02/2015
	\version 	2.0.0
	\brief		Text wrapping modes, handles the way text is cut when it overflows the overlay width
	*/
	typedef enum eBORDER_POSITION
	{
		//! The border is outside the overlay
		eBORDER_POSITION_INTERNAL,
		//! The border is middle outside and middle inside the overlay
		eBORDER_POSITION_MIDDLE,
		//! The border is outside the overlay
		eBORDER_POSITION_EXTERNAL,
	}	eBORDER_POSITION;
	/*!
	\author 	Sylvain DOREMUS
	\date		23/02/2015
	\version 	2.0.0
	\brief		Text wrapping modes, handles the way text is cut when it overflows the overlay width
	*/
	typedef enum eTEXT_WRAPPING_MODE
	{
		//! The text is cut
		eTEXT_WRAPPING_MODE_NONE,
		//! The text jumps to next line
		eTEXT_WRAPPING_MODE_BREAK,
		//! The text jumps to next line without cutting words
		eTEXT_WRAPPING_MODE_BREAK_WORDS,
	}	eTEXT_WRAPPING_MODE;
	/*!
	\author 	Sylvain DOREMUS
	\date 		02/03/2015
	\version	2.0.0
	\brief		Mouse buttons enumeration
	*/
	typedef enum eMOUSE_BUTTON
	{
		//! The left button
		eMOUSE_BUTTON_LEFT,
		//! The middle button
		eMOUSE_BUTTON_MIDDLE,
		//! The right button
		eMOUSE_BUTTON_RIGHT,
		//! The buttons count
		eMOUSE_BUTTON_COUNT,
	}	eMOUSE_BUTTON;
	/*!
	\author 	Sylvain DOREMUS
	\date 		02/03/2015
	\version	2.0.0
	\brief		Keyboard keys enumeration
	*/
	typedef enum eKEYBOARD_KEY
	{
		eKEYBOARD_KEY_NONE,
		eKEY_BACKSPACE = 0x08,
		eKEY_TAB = 0x0A,
		eKEY_RETURN = 0x0D,
		eKEY_ESCAPE = 0x1B,
		eKEY_SPACE = 0x20,

		// values from 0x21 to 0x7E are reserved for the standard ASCII characters

		eKEY_DELETE = 0x7F,

		// values from 0x80 to 0xFF are reserved for ASCII extended characters

		eKEY_START   = 0x100,
		eKEY_LBUTTON,
		eKEY_RBUTTON,
		eKEY_CANCEL,
		eKEY_MBUTTON,
		eKEY_CLEAR,
		eKEY_SHIFT,
		eKEY_ALT,
		eKEY_CONTROL,
		eKEY_MENU,
		eKEY_PAUSE,
		eKEY_CAPITAL,
		eKEY_END,
		eKEY_HOME,
		eKEY_LEFT,
		eKEY_UP,
		eKEY_RIGHT,
		eKEY_DOWN,
		eKEY_SELECT,
		eKEY_PRINT,
		eKEY_EXECUTE,
		eKEY_SNAPSHOT,
		eKEY_INSERT,
		eKEY_HELP,
		eKEY_NUMPAD0,
		eKEY_NUMPAD1,
		eKEY_NUMPAD2,
		eKEY_NUMPAD3,
		eKEY_NUMPAD4,
		eKEY_NUMPAD5,
		eKEY_NUMPAD6,
		eKEY_NUMPAD7,
		eKEY_NUMPAD8,
		eKEY_NUMPAD9,
		eKEY_MULTIPLY,
		eKEY_ADD,
		eKEY_SEPARATOR,
		eKEY_SUBTRACT,
		eKEY_DECIMAL,
		eKEY_DIVIDE,
		eKEY_F1,
		eKEY_F2,
		eKEY_F3,
		eKEY_F4,
		eKEY_F5,
		eKEY_F6,
		eKEY_F7,
		eKEY_F8,
		eKEY_F9,
		eKEY_F10,
		eKEY_F11,
		eKEY_F12,
		eKEY_F13,
		eKEY_F14,
		eKEY_F15,
		eKEY_F16,
		eKEY_F17,
		eKEY_F18,
		eKEY_F19,
		eKEY_F20,
		eKEY_F21,
		eKEY_F22,
		eKEY_F23,
		eKEY_F24,
		eKEY_NUMLOCK,
		eKEY_SCROLL,
		eKEY_PAGEUP,
		eKEY_PAGEDOWN,
		eKEY_NUMPAD_SPACE,
		eKEY_NUMPAD_TAB,
		eKEY_NUMPAD_ENTER,
		eKEY_NUMPAD_F1,
		eKEY_NUMPAD_F2,
		eKEY_NUMPAD_F3,
		eKEY_NUMPAD_F4,
		eKEY_NUMPAD_HOME,
		eKEY_NUMPAD_LEFT,
		eKEY_NUMPAD_UP,
		eKEY_NUMPAD_RIGHT,
		eKEY_NUMPAD_DOWN,
		eKEY_NUMPAD_PAGEUP,
		eKEY_NUMPAD_PAGEDOWN,
		eKEY_NUMPAD_END,
		eKEY_NUMPAD_BEGIN,
		eKEY_NUMPAD_INSERT,
		eKEY_NUMPAD_DELETE,
		eKEY_NUMPAD_EQUAL,
		eKEY_NUMPAD_MULTIPLY,
		eKEY_NUMPAD_ADD,
		eKEY_NUMPAD_SEPARATOR,
		eKEY_NUMPAD_SUBTRACT,
		eKEY_NUMPAD_DECIMAL,
		eKEY_NUMPAD_DIVIDE,
	}	eKEYBOARD_KEY;
	/*!
	\author 	Sylvain DOREMUS
	\date 		10/03/2015
	\version	2.0.0
	\brief		Mouse cursors enumeration
	*/
	typedef enum eMOUSE_CURSOR
	{
		//! The usual arrow cursor
		eMOUSE_CURSOR_ARROW,
		//! The hand cursor
		eMOUSE_CURSOR_HAND,
		//! The text cursor
		eMOUSE_CURSOR_TEXT,
		//! The cursors count
		eMOUSE_CURSOR_COUNT,
	}	eMOUSE_CURSOR;
	/*!
	\author		Sylvain DOREMUS
	\date		12/03/2015
	\version 	2.0.0
	\brief		Enumeration of supported control events
	*/
	typedef enum eCONTROL_EVENT
		: uint8_t
	{
		//! Control activated
		eCONTROL_EVENT_ACTIVATE,
		//! Control deactivated
		eCONTROL_EVENT_DEACTIVATE,
		//! The control events count
		eCONTROL_EVENT_COUNT,
	}	eCONTROL_EVENT;
	/*!
	\author 	Sylvain DOREMUS
	\date 		02/03/2015
	\version	2.0.0
	\brief		Supported mouse events enumeration
	*/
	typedef enum eMOUSE_EVENT
	{
		//! The mouse enters into the control client area
		eMOUSE_EVENT_MOUSE_ENTER,
		//! The mouse leaves the control client area
		eMOUSE_EVENT_MOUSE_LEAVE,
		//! The mouse moves into the control client area
		eMOUSE_EVENT_MOUSE_MOVE,
		//! The mouse remains into the client area during hover time
		eMOUSE_EVENT_MOUSE_HOVER,
		//! One mouse button is pushed
		eMOUSE_EVENT_MOUSE_BUTTON_PUSHED,
		//! One mouse button is released
		eMOUSE_EVENT_MOUSE_BUTTON_RELEASED,
		//! One mouse wheel is rolled
		eMOUSE_EVENT_MOUSE_WHEEL,
		//! The mouse events count
		eMOUSE_EVENT_COUNT,
	}	eMOUSE_EVENT;
	/*!
	\author 	Sylvain DOREMUS
	\date 		02/03/2015
	\version	2.0.0
	\brief		Supported keyboard events enumeration
	*/
	typedef enum eKEYBOARD_EVENT
	{
		//! A key is pushed
		eKEYBOARD_EVENT_KEY_PUSHED,
		//! A key is released
		eKEYBOARD_EVENT_KEY_RELEASED,
		//! A printable key is pressed, the KeyboardEvent will hold the character to display
		eKEYBOARD_EVENT_CHAR,
		//! The keyboard events count
		eKEYBOARD_EVENT_COUNT,
	}	eKEYBOARD_EVENT;

	template< class Derived > class EventHandler;
	class ControlsManager;
	class Control;
	class ButtonCtrl;
	class ComboBoxCtrl;
	class EditCtrl;
	class ListBoxCtrl;
	class SliderCtrl;
	class StaticCtrl;
	class OverlayManager;
	class Overlay;
	class PanelOverlay;
	class BorderPanelOverlay;
	class TextOverlay;
	class Font;
	class Glyph;
	class Material;
	typedef std::vector< std::shared_ptr< Control > > ControlArray;

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
		//! Event launched by main thread when computation ends
		eEVENT_CPU_STEP_END = 100001,
		//! Event sent to main thread to retrieve mouse position
		eEVENT_MOUSEMOVE = 100011,
	}	eEVENT;
	/*!
	\author 	Sylvain DOREMUS
	\date		23/02/2015
	\version 	2.0.0
	\brief		The supported materials types
	*/
	typedef enum eMATERIAL_TYPE
		: uint8_t
	{
		//! Colour material
		eMATERIAL_TYPE_COLOUR,
		//! Texture material
		eMATERIAL_TYPE_TEXTURE,
	}	eMATERIAL_TYPE;

	class PointBase;
	template< typename T > class DynPoint;
	template < typename T, size_t Count > class Point;
	template< typename T > class Pixel;
	template< typename T > class Buffer;
	class PluginBase;
	template< class GeneratorClass, class PluginClass > class Plugin;
	class Random;
	class CpuStepBase;
	template< class TThread > class CpuStep;
	class GpuStep;
	class GeneratorBase;
	template< class TCpuStep, class TGpuStep > class Generator;
	typedef Point< float, 1 > Point1f;
	typedef Point< float, 2 > Point2f;
	typedef Point< float, 3 > Point3f;
	typedef Point< float, 4 > Point4f;
	typedef Point< double, 1 > Point1d;
	typedef Point< double, 2 > Point2d;
	typedef Point< double, 3 > Point3d;
	typedef Point< double, 4 > Point4d;
	typedef Point< int32_t, 1 > Point1i;
	typedef Point< int32_t, 2 > Point2i;
	typedef Point< int32_t, 3 > Point3i;
	typedef Point< int32_t, 4 > Point4i;
	typedef Point< uint32_t, 1 > Point1ui;
	typedef Point< uint32_t, 2 > Point2ui;
	typedef Point< uint32_t, 3 > Point3ui;
	typedef Point< uint32_t, 4 > Point4ui;
	typedef Point< uint8_t, 1 > Point1ub;
	typedef Point< uint8_t, 2 > Point2ub;
	typedef Point< uint8_t, 3 > Point3ub;
	typedef Point< uint8_t, 4 > Point4ub;
	class Size;
	class Position;
	typedef Pixel< uint8_t > UbPixel;
	typedef Pixel< uint32_t > UiPixel;
	typedef Pixel< float > Colour;
	typedef Buffer<  UbPixel > PixelBuffer;
	template< typename PosType > class TVertex;
	typedef TVertex< float > Vertex;
	typedef TVertex< int32_t > VertexI;
	template< typename PosType > class TGeometryBuffers;
	typedef TGeometryBuffers< float > GeometryBuffers;
	typedef TGeometryBuffers< int32_t > GeometryBuffersI;
	typedef std::map< uint64_t, std::weak_ptr< Overlay > > OverlayPtrIntMap;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//	GPU
	////////////////////////////////////////////////////////////////////////////////////////////////////
	namespace gl
	{
		/*!
		\author 	Sylvain DOREMUS
		\date		01/07/2011
		\version 	1.2.0.0
		\brief		Shader object supported types enumerator
		*/
		typedef enum
		{
			eSHADER_OBJECT_TYPE_NONE = -1,
			//! Vertex shader
			eSHADER_OBJECT_TYPE_VERTEX = 0,
			//! Fragment shader
			eSHADER_OBJECT_TYPE_PIXEL = 1,
			eSHADER_OBJECT_TYPE_COUNT
		}	eSHADER_OBJECT_TYPE;

		class Holder;
		template< typename CreateFunction, typename DestroyFunction > class Object;
		class OpenGl;
		class Buffer;
		template< typename PosType > class TVertexBuffer;
		typedef TVertexBuffer< float > VertexBuffer;
		typedef TVertexBuffer< int32_t > VertexBufferI;
		class IndexBuffer;
		class GpuIOBuffer;
		class DownloadPixelBuffer;
		class UploadPixelBuffer;
		class Texture;
		class FrameBuffer;
		class FrameVariableBase;
		template < typename T, uint32_t Count  > class TFrameVariable;
		template< typename T, uint32_t Count  > struct TVariableApplier;
		template< typename T > class FrameVariable;
		template< typename T > class Vec2FrameVariable;
		template< typename T > class Vec3FrameVariable;
		template< typename T > class Vec4FrameVariable;
		template< typename T > class Mat4FrameVariable;
		class ShaderObject;
		class ShaderProgram;
		class OverlayRenderer;
		typedef std::list< std::shared_ptr< FrameVariableBase > > FrameVariablePtrList;
		typedef std::map< String, std::shared_ptr< FrameVariableBase > > FrameVariablePtrStrMap;
	}
}

namespace std
{
#if _MSC_VER < 1800
	template< class T >
	unique_ptr< T > make_unique()
	{
		return unique_ptr< T >( new T );
	}
	template< class T, typename Arg1 >
	unique_ptr< T > make_unique( Arg1 && arg1 )
	{
		return unique_ptr< T >( new T(
									std::forward< Arg1 >( arg1 )
								) );
	}
	template< class T, typename Arg1, typename Arg2 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2 )
	{
		return unique_ptr< T >( new T(
									std::forward< Arg1 >( arg1 ),
									std::forward< Arg2 >( arg2 )
								) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3 )
	{
		return unique_ptr< T >( new T(
									std::forward< Arg1 >( arg1 ),
									std::forward< Arg2 >( arg2 ),
									std::forward< Arg3 >( arg3 )
								) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4 )
	{
		return unique_ptr< T >( new T(
									std::forward< Arg1 >( arg1 ),
									std::forward< Arg2 >( arg2 ),
									std::forward< Arg3 >( arg3 ),
									std::forward< Arg4 >( arg4 )
								) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5 )
	{
		return unique_ptr< T >( new T(
									std::forward< Arg1 >( arg1 ),
									std::forward< Arg2 >( arg2 ),
									std::forward< Arg3 >( arg3 ),
									std::forward< Arg4 >( arg4 ),
									std::forward< Arg5 >( arg5 )
								) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5, Arg6 && arg6 )
	{
		return unique_ptr< T >( new T(
									std::forward< Arg1 >( arg1 ),
									std::forward< Arg2 >( arg2 ),
									std::forward< Arg3 >( arg3 ),
									std::forward< Arg4 >( arg4 ),
									std::forward< Arg5 >( arg5 ),
									std::forward< Arg6 >( arg6 )
								) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5, Arg6 && arg6, Arg7 && arg7 )
	{
		return unique_ptr< T >( new T(
									std::forward< Arg1 >( arg1 ),
									std::forward< Arg2 >( arg2 ),
									std::forward< Arg3 >( arg3 ),
									std::forward< Arg4 >( arg4 ),
									std::forward< Arg5 >( arg5 ),
									std::forward< Arg6 >( arg6 ),
									std::forward< Arg7 >( arg7 )
								) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5, Arg6 && arg6, Arg7 && arg7, Arg8 && arg8 )
	{
		return unique_ptr< T >( new T(
									std::forward< Arg1 >( arg1 ),
									std::forward< Arg2 >( arg2 ),
									std::forward< Arg3 >( arg3 ),
									std::forward< Arg4 >( arg4 ),
									std::forward< Arg5 >( arg5 ),
									std::forward< Arg6 >( arg6 ),
									std::forward< Arg7 >( arg7 ),
									std::forward< Arg8 >( arg8 )
								) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5, Arg6 && arg6, Arg7 && arg7, Arg8 && arg8, Arg9 && arg9 )
	{
		return unique_ptr< T >( new T(
									std::forward< Arg1 >( arg1 ),
									std::forward< Arg2 >( arg2 ),
									std::forward< Arg3 >( arg3 ),
									std::forward< Arg4 >( arg4 ),
									std::forward< Arg5 >( arg5 ),
									std::forward< Arg6 >( arg6 ),
									std::forward< Arg7 >( arg7 ),
									std::forward< Arg8 >( arg8 ),
									std::forward< Arg9 >( arg9 )
								) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6 >
	shared_ptr< T > make_shared( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5, Arg6 && arg6 )
	{
		return shared_ptr< T >( new T(
									std::forward< Arg1 >( arg1 ),
									std::forward< Arg2 >( arg2 ),
									std::forward< Arg3 >( arg3 ),
									std::forward< Arg4 >( arg4 ),
									std::forward< Arg5 >( arg5 ),
									std::forward< Arg6 >( arg6 )
								) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7 >
	shared_ptr< T > make_shared( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5, Arg6 && arg6, Arg7 && arg7 )
	{
		return shared_ptr< T >( new T(
									std::forward< Arg1 >( arg1 ),
									std::forward< Arg2 >( arg2 ),
									std::forward< Arg3 >( arg3 ),
									std::forward< Arg4 >( arg4 ),
									std::forward< Arg5 >( arg5 ),
									std::forward< Arg6 >( arg6 ),
									std::forward< Arg7 >( arg7 )
								) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8 >
	shared_ptr< T > make_shared( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5, Arg6 && arg6, Arg7 && arg7, Arg8 && arg8 )
	{
		return shared_ptr< T >( new T(
									std::forward< Arg1 >( arg1 ),
									std::forward< Arg2 >( arg2 ),
									std::forward< Arg3 >( arg3 ),
									std::forward< Arg4 >( arg4 ),
									std::forward< Arg5 >( arg5 ),
									std::forward< Arg6 >( arg6 ),
									std::forward< Arg7 >( arg7 ),
									std::forward< Arg8 >( arg8 )
								) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9 >
	shared_ptr< T > make_shared( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5, Arg6 && arg6, Arg7 && arg7, Arg8 && arg8, Arg9 && arg9 )
	{
		return shared_ptr< T >( new T(
									std::forward< Arg1 >( arg1 ),
									std::forward< Arg2 >( arg2 ),
									std::forward< Arg3 >( arg3 ),
									std::forward< Arg4 >( arg4 ),
									std::forward< Arg5 >( arg5 ),
									std::forward< Arg6 >( arg6 ),
									std::forward< Arg7 >( arg7 ),
									std::forward< Arg8 >( arg8 ),
									std::forward< Arg9 >( arg9 )
								) );
	}
#endif
}

#include "Translator.h"

#endif
