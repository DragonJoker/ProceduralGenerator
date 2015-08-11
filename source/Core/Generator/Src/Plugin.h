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
#ifndef ___GENERATOR_PLUGIN_H___
#define ___GENERATOR_PLUGIN_H___

#include "GeneratorPrerequisites.h"

#include "Generator.h"

#if defined( _WIN32 )
#	define PLUGIN_EXPORT __declspec( dllexport )
#else
#	define PLUGIN_EXPORT
#endif

//! Macro to use in the header of a class that derives from Plugin
#define DECLARE_PLUGIN() virtual void Destroy();

//! Macro to use in the source of a class that derives from plugin
#define IMPLEMENT_PLUGIN( PluginClass, GeneratorClass )\
	extern "C" PLUGIN_EXPORT ProceduralTextures::PluginBase * CreatePlugin()\
	{\
		return new PluginClass;\
	}\
	void PluginClass::Destroy()\
	{\
		delete this;\
	}

namespace ProceduralTextures
{
	/*!
	\author 	Sylvain DOREMUS
	\brief		Base class for a plugin
	\remarks		Exposes the functions needed to implement a plugin
	*/
	class GeneratorAPI PluginBase
	{
	protected:
		String m_strName;

	public:
		//! Typedef to the exported function used to create a plugin
		typedef PluginBase * ( CreatePluginFunction )();
		typedef CreatePluginFunction * PCreatePluginFunction;

	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_strName	Plugin internal name
		 */
		PluginBase( String const & p_strName );
		/**
		 *\brief		Destructor
		 */
		virtual ~PluginBase();
		/**
		 *\brief		Destroys this plugin
		 *\remarks		Implemented in \p IMPLEMENT_PLUGIN macro
		 */
		virtual void Destroy() = 0;
		/**
		 *\brief		Creates the ProceduralGenerators
		 *\remarks		Implemented in Plugin
		 *\return		The created generator
		 */
		virtual std::shared_ptr< GeneratorBase > CreateGenerator() = 0;
		/**
		 *\brief		Gives the plugin displayable name
		 *\remarks		Derived classes must implement it
		 *\return		The plugin name
		 */
		virtual String GetName() = 0;
		/**
		 *\brief		Tells if the plugin resolution is customisable
		 *\return		The status
		 */
		virtual bool HasCustomisableResolution() = 0;
	};
	/*!
	\author 	Sylvain DOREMUS
	\brief		Temlate class for a plugin
	\remarks		Implements the CreateGenerator function
				< br / >On MS-Windows, it also creates the dll main application
	*/
	template< class GeneratorClass, class PluginClass >
	class Plugin : public PluginBase
	{
	public:
		/**
		 *\brief		Constructor
		 *\remarks		On MS-Windows, it connects the wxApp to the terminate event, in order to be able to exit the dll main
		 */
		Plugin( String const & p_strName )
			:	PluginBase( p_strName )
		{
			Translator::Initialise( System::GetExecutableDirectory(), _T( "ProceduralGenerator" ), p_strName );
		}
		/**
		 *\brief		Destructor
		 */
		virtual ~Plugin()
		{
			Translator::Cleanup();
		}
		/**
		 *\brief		Creates the ProceduralGenerator
		 *\return		The created generator
		 */
		std::shared_ptr< GeneratorBase > CreateGenerator()
		{
			return std::make_shared< GeneratorClass >();
		}
	};
}

#endif
