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
#ifndef ___GENERATOR_GENERATOR_H___
#define ___GENERATOR_GENERATOR_H___

#include "GeneratorUtils.h"
#include "Buffer.h"
#include "GeneratorSignal.h"
#include "CtrlStatic.h"
#include "CtrlEdit.h"
#include "CtrlComboBox.h"
#include "CtrlListBox.h"
#include "CtrlSlider.h"
#include "CtrlButton.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <chrono>

namespace ProceduralTextures
{
	const uint32_t DEFAULT_FRAME_TIME = 20;

	/*!
	@author
		Sylvain DOREMUS
	@date
		12/03/2015
	@version
		2.0.0
	@brief
		Base class for every generator
	*/
	class GeneratorAPI GeneratorBase
	{
	public:
		/** Constructor
		@param[in] p_needsConfigPanel
			Tells if the generator needs a configuration panel
		@param[in] p_frameTime
			The time in milliseconds to wait between each frame
		*/
		GeneratorBase( bool p_needsConfigPanel, int p_frameTime );

		/** Destructor
		*/
		virtual ~GeneratorBase();

		/** Creates the CPU and GPU steps
		@param[in] p_size
			The computing surface dimensions
		@param[in] p_bordersSize
			The windows' borders size
		*/
		void Create( Size const & p_size, Size const & p_bordersSize );

		/** Destroys the CPU and GPU steps
		*/
		void Destroy();

		/** Initialises the generator
		*/
		void Initialise();

		/** Cleans up the generator
		*/
		void Cleanup();

		/** Launches the thread
		*/
		void Run();

		/** Stops the thread
		*/
		void Stop();

		/** Renders the frame : parallel execution of CPU and GPU steps
		@param[in] p_swapFunction
			The function used to swap OpenGL buffers
		*/
		void Render( std::function< void() > p_swapFunction );

		/** Resize the generator computation dimensions
		@param[in] p_iWidth, p_iHeight
			The new dimensions
		*/
		void Resize( int p_iWidth, int p_iHeight );

		/** Resize the generator computation dimensions
		@param[in] p_size
			The new dimensions
		*/
		void Resize( Size const & p_size );

		/** Tells the next frame must be saved
		*/
		void SaveFrame();

		/** Shows a message box
		@remarks
			Uses the 'show message box' callback, hence does nothing if this one is not set.
		@param[in] p_title
			The box title
		@param[in] p_message
			The box message
		*/
		void ShowMessageBox( String const & p_title, String const & p_message );

		/** Sets displayed image size, respecting the working image ratio
		@param[in] p_size
			The new value
		*/
		void UpdateDisplaySize( Size const & p_size );

		/** Opens a file dialog, in write mode
		@remarks
			Uses the 'open file' callback, hence does nothing if this one is not set.
		@param[out] p_path
			Receives the file path
		@param[in] p_prompt
			The dialog message
		@param[in] p_wildcard
			The file wildcard
		@return
			true if a file has been selected
		*/
		bool OpenFile( String & p_path, String const & p_prompt = _T( "Select a file" ), String const & p_wildcard = _T( "All files (*.*)|*.*" ) );

		/** Opens a file dialog, in read mode
		@remarks
			Uses the 'save file' callback, hence does nothing if this one is not set.
		@param[out] p_path
			Receives the file path
		@param[in] p_prompt
			The dialog message
		@param[in] p_wildcard
			The file wildcard
		@return
			true if a file has been selected
		*/
		bool SaveFile( String & p_path, String const & p_prompt = _T( "Select a file" ), String const & p_wildcard = _T( "All files (*.*)|*.*" ) );

		/** Opens an image selection dialog and loads the image
		@remarks
			Uses the 'select image' callback, hence does nothing if this one is not set.
		@param[out] p_buffer
			Receives the image buffer
		@param[in] p_prompt
			The dialog message
		@return
			true if an image has been selected
		*/
		bool SelectImage( PixelBuffer & p_buffer, String const & p_prompt = _( "Choose an image" ) );

		/** Opens a colour selection dialog
		@remarks
			Uses the 'select colour' callback, hence does nothing if this one is not set.
		@param[out] p_colour
			Receives the colour
		@param[in] p_prompt
			The dialog message
		@return
			true if a colour has been selected
		*/
		bool SelectColour( uint32_t & p_colour, String const & p_prompt = _( "Choose a colour" ) );

		/** Changes the mouse cursor
		@remarks
			Uses the 'set cursor' callback, hence does nothing if this one is not set.
		@param[out] p_cursor
			The cursor
		*/
		void SetCursor( eMOUSE_CURSOR p_cursor );

		/** Posts a mouse event to the controls manager
		@param[in] p_type
			The mouse event type
		@param[in] p_position
			The position, of wheel or mouse, depending on the event type
		@param[in] p_button
			The mouse button
		*/
		void PostMouseEvent( eMOUSE_EVENT p_type, Position const & p_position, eMOUSE_BUTTON p_button = eMOUSE_BUTTON_COUNT );

		/** Posts a keyboard key down event
		@param[in] p_type
			The keyboard event type
		@param[in] p_key
			The key
		@param[in] p_ctrl
			Tells if the Ctrl key is down
		@param[in] p_alt
			Tells if the Alt key is down
		@param[in] p_shift
			Tells if the Shift key is down
		*/
		void PostKeyboardEvent( eKEYBOARD_EVENT p_type, eKEYBOARD_KEY p_key, bool p_ctrl, bool p_alt, bool p_shift );

		/** Posts a printable key event
		@param[in] p_key
			The key
		@param[in] p_char
			The character corresponding to the key
		*/
		void PostCharEvent( eKEYBOARD_KEY p_key, String const & p_char );

		/** Loads a font
		@param[in] p_fontName
			The font name
		@param[in] p_height
			The font height
		@param[in] p_fontPath
			The font file path
		@return
			The loaded font
		*/
		std::shared_ptr< Font > LoadFont( String const & p_fontName, uint32_t p_height, String const & p_fontPath );

		/** Retrieves a font
		@param[in] p_fontName
			The font name
		@param[in] p_height
			The font height
		@return
			The font, nullptr if it does not exist
		*/
		std::shared_ptr< Font > GetFont( String const & p_fontName, uint32_t p_height );

		/** Tells if the generator is initialised
		@return
			The value
		*/
		bool IsInitialised()const;

		/** Retrieves the displayed image dimensions
		@return
			The value
		*/
		Size const & GetDisplaySize()const;

		/** Retrieves the computing image dimensions
		@return
			The value
		*/
		Size const & GetImageSize()const;

		/** Retrieves the saved frame buffer
		@return
			The value
		*/
		PixelBuffer const & GetSavedFrame()const;

		/** Retrieves the CPU step duration
		@return
			The value
		*/
		std::chrono::milliseconds const & GetCpuTime()const;

		/** Retrieves the GPU step duration
		@return
			The value
		*/
		std::chrono::milliseconds const & GetGpuTime()const;

		/** Sets the 'show message box' callback
		@param[in] p_function
			The callback function
		*/
		inline void SetShowMessageBoxCallback( std::function< void( String const &, String const & ) > p_function )
		{
			m_callbackShowMessageBox = p_function;
		}

		/** Sets the 'resize' callback
		@param[in] p_function
			The callback function
		*/
		inline void SetResizeCallback( std::function< void( Size const & ) > p_function )
		{
			m_callbackResize = p_function;
		}

		/** Sets the 'open file selector' callback
		@param[in] p_function
			The callback function
		*/
		inline void SetOpenFileCallback( std::function< bool( String &, String const &, String const & ) > p_function )
		{
			m_callbackOpenFile = p_function;
		}

		/** Sets the 'save file selector' callback
		@param[in] p_function
			The callback function
		*/
		inline void SetSaveFileCallback( std::function< bool( String &, String const &, String const & ) > p_function )
		{
			m_callbackSaveFile = p_function;
		}

		/** Sets the 'select image selector' callback
		@param[in] p_function
			The callback function
		*/
		inline void SetSelectImageCallback( std::function< bool( PixelBuffer &, String const & ) > p_function )
		{
			m_callbackSelectImage = p_function;
		}

		/** Sets the 'select colour selector' callback
		@param[in] p_function
			The callback function
		*/
		inline void SetSelectColourCallback( std::function< bool( uint32_t &, String const & ) > p_function )
		{
			m_callbackSelectColour = p_function;
		}

		/** Sets the 'set mouse cursor' callback
		@param[in] p_function
			The callback function
		*/
		inline void SetSetCursorCallback( std::function< void( eMOUSE_CURSOR ) > p_function )
		{
			m_callbackSetCursor = p_function;
		}

		/** Tells if the generator needs a configuration panel
		@return
			The value
		*/
		inline bool NeedsConfigPanel()const
		{
			return m_needsConfigPanel;
		}

		/** Retrieves an iterator to the controls array beginning
		@return
			The value
		*/
		inline ControlArray::iterator BeginCtrls()
		{
			return m_arrayControls.begin();
		}

		/** Retrieves an iterator to the controls array end
		@return
			The value
		*/
		inline ControlArray::iterator EndCtrls()
		{
			return m_arrayControls.end();
		}

		/** Retrieves the frame wanted duration
		@return
			The value
		*/
		inline std::chrono::milliseconds const & GetFrameTime()const
		{
			return m_frameTime;
		}

		/** Retrieves the controls panel
		@return
			The value
		*/
		inline std::shared_ptr< ControlsManager > GetControlsManager()const
		{
			return m_controlsManager;
		}

		/** Retrieves the button image buffer
		@return
			The value
		*/
		inline std::shared_ptr< PixelBuffer > GetButtonImage()const
		{
			return m_buttonImage;
		}

		/** Retrieves the button mouse over image buffer
		@return
			The value
		*/
		inline std::shared_ptr< PixelBuffer > GetButtonMouseOverImage()const
		{
			return m_buttonMouseOverImage;
		}

		/** Retrieves the GPU step
		@return
			The step
		*/
		inline std::shared_ptr< GpuStep > GetGpuStep()const
		{
			return DoGetGpuStep();
		}

	protected:
		/** Creates the CPU and GPU steps
		@param[in] p_size
			The computing surface dimensions
		@param[in] p_bordersSize
			The windows' borders size
		*/
		virtual void DoCreate( Size const & p_size, Size const & p_bordersSize ) = 0;

		/** Destroys the CPU and GPU steps
		*/
		virtual void DoDestroy() = 0;

		/** Initialises the configuration panel structure
		*/
		virtual void DoGeneratePanel() = 0;

		/** Retrieves the CPU Step
		*/
		virtual std::shared_ptr< CpuStepBase > DoGetCpuStep()const = 0;

		/** Retrieves the CPU Step
		*/
		virtual std::shared_ptr< GpuStep > DoGetGpuStep()const = 0;

		/** Retreives a shared pointer to this
		@remarks
			Since the enable_shared_from_this is used in the template class below, I use this function in this class
		*/
		virtual std::shared_ptr< GeneratorBase > DoGetThis() = 0;

		/** Switches the panel visibility
		*/
		virtual void DoSwitchVisibility();

	private:
		/** Writes a step time into the appropriate overlay
		@param[in] p_name
			The step name
		@param[in] p_time
			The step time
		@param[in] p_text
			The step overlay
		*/
		void DoWriteTime( String const & p_name, std::chrono::milliseconds const & p_time, std::shared_ptr< StaticCtrl > p_text );

	protected:
		//! The panel holding times and options
		std::shared_ptr< StaticCtrl > m_panel;
		//! The panel options
		std::shared_ptr< StaticCtrl > m_options;
		//! The CPU time text overlay
		std::weak_ptr< StaticCtrl > m_cpuTime;
		//! The GPU time text overlay
		std::weak_ptr< StaticCtrl > m_gpuTime;
		//! The button image buffer
		std::shared_ptr< PixelBuffer > m_buttonImage;
		//! The button mouse over image buffer
		std::shared_ptr< PixelBuffer > m_buttonMouseOverImage;
		//! The buttons image buffer
		std::shared_ptr< PixelBuffer > m_buffer;
		//! The generator
		std::shared_ptr< ButtonCtrl > m_visibility;
		//! The time to wait between each frame
		std::chrono::milliseconds m_frameTime;
		//! Tells the generator needs a configuration panel
		bool m_needsConfigPanel;
		//! The configuration panels controls
		ControlArray m_arrayControls;
		//! The loaded fonts
		std::map< String, std::shared_ptr< Font > > m_loadedFonts;
		//! The GPU step
		std::shared_ptr< ControlsManager > m_controlsManager;
		//! The show message box callback
		std::function< void( String const &, String const & ) > m_callbackShowMessageBox;
		//! The callback called when the generator is resized
		std::function< void( Size const & ) > m_callbackResize;
		//! The callback called to open a file selector, in write mode
		std::function< bool( String &, String const &, String const & ) > m_callbackOpenFile;
		//! The callback called to open a file selector, in read mode
		std::function< bool( String &, String const &, String const & ) > m_callbackSaveFile;
		//! The callback called to select an image
		std::function< bool( PixelBuffer &, String const & p_prompt ) > m_callbackSelectImage;
		//! The callback called to select a colour
		std::function< bool( uint32_t &, String const & p_prompt ) > m_callbackSelectColour;
		//! The callback used to change the mouse cursor
		std::function< void( eMOUSE_CURSOR ) > m_callbackSetCursor;
	};

	/*!
	@author
		Sylvain DOREMUS
	@date
		12/03/2015
	@version
		2.0.0
	@brief
		Base class for every generator
	*/
	template< class TCpuStep, class TGpuStep >
	class Generator
		: public GeneratorBase
		, public std::enable_shared_from_this< Generator< TCpuStep, TGpuStep > >
	{
	public:
		/** Constructor
		@param[in] p_needsConfigPanel
			Tells if the generator needs a configuration panel
		@param[in] p_frameTime
				The time in milliseconds to wait between each frame
		*/
		Generator( bool p_needsConfigPanel, int p_frameTime )
			: GeneratorBase( p_needsConfigPanel, p_frameTime )
		{
		}

		/** Destructor
		*/
		virtual ~Generator()
		{
		}

	private:
		/** Retrieves the CPU Step
		*/
		virtual std::shared_ptr< CpuStepBase > DoGetCpuStep()const
		{
			return m_cpuStep;
		}

		/** Retrieves the CPU Step
		*/
		virtual std::shared_ptr< GpuStep > DoGetGpuStep()const
		{
			return m_gpuStep;
		}

		/** Retreives a shared pointer to this
		*/
		virtual std::shared_ptr< GeneratorBase > DoGetThis()
		{
			return this->shared_from_this();
		}

	protected:
		//! The CPU step
		std::shared_ptr< TCpuStep > m_cpuStep;
		//! The GPU step
		std::shared_ptr< TGpuStep > m_gpuStep;
	};
}

#endif
