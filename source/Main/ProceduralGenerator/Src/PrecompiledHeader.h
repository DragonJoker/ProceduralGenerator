/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (At your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___PROCEDURAL_GENERATOR_PCH_H___
#define	___PROCEDURAL_GENERATOR_PCH_H___

#include <chrono>
#include <functional>
#include <map>
#include <vector>

#include <emmintrin.h>

#pragma warning( push )
#pragma warning( disable:4996 )

#include <wx/app.h>
#include <wx/button.h>
#include <wx/colordlg.h>
#include <wx/combobox.h>
#include <wx/dcclient.h>
#include <wx/dialog.h>
#include <wx/dir.h>
#include <wx/display.h>
#include <wx/dynlib.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/frame.h>
#include <wx/image.h>
#include <wx/msgdlg.h>
#include <wx/mstream.h>
#include <wx/object.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>
#include <wx/stdpaths.h>
#include <wx/sysopt.h>
#include <wx/timer.h>
#include <wx/toolbar.h>

#pragma warning( pop )

#if defined( PGEN_FFMPEG )
#	define USE_STREAMS 0
extern "C"
{
#	include <libavutil/opt.h>
#	include <libavcodec/avcodec.h>
#	include <libavformat/avformat.h>
#	include <libavutil/common.h>
#	include <libavutil/imgutils.h>
#	include <libavutil/mathematics.h>
#	include <libavutil/samplefmt.h>
#	include <libswscale/swscale.h>
//#	include <libswresample/swresample.h>
}
#elif defined( PGEN_OCV )
#	include <opencv2/opencv.hpp>
#endif

#include <GeneratorSignal.h>
#include <Plugin.h>
#include <Generator.h>

#endif
