Procedural Generator
====================

Procedural Generator is an application that can load various image treatment plug-ins.
The main application loads the plug-in, allows plug-in selection, displays computation results.

The plug-ins' job is made in 2 steps : 
- 1st step : CPU step : image computing using CPU computations.
- 2nd step : GPU step : image post-processing using GPU through GLSL shaders.

Some plug-ins only use CPU step, such as Perlin Noise, Life Game, Diffusion Limited aggregation.
Some others use only the GPU step, such as Shader Effects, Text Effects.

The GPU step uses OpenGL and allows the use of GLSL shaders.
The CPU step is multi-threaded in order to fasten the computations.

Building Procedural Generator
-----------------------------

Procedural Generator uses CMake to build project files.
You just need to launch CMake to build your preferential project file.

Tested compilers are :
- Visual Studio 2015
- Visual Studio 2013
- GCC 4.7.0

The Generator shared library depends on OpenGL and FreeType.
The main application is based upon wxWidgets (from 2.8.11 to latest) so on GNU/Linux it also depends on GTK.
ShaderWebcam also depends on OpenCV (at least 2.1.0) but doesn't compile with clang, because of OpenCV (which is not clang compliant).
Documentation can be generated using Doxygen.

Options
-------

On GNU/Linux the default install directory is in /usr/local.
You can change this directory by changing the CMAKE_INSTALL_PREFIX value.
The main application is installed in <install_dir>/bin/
The library LibGenerator (used to build your own plug-ins) is installed in <install_dir>/lib/.
The plug-ins are installed in <install_dir>/lib/ProceduralGenerator/
The translation files are installed in <install_dir>/share/ProceduralGenerator/

Running Procedural Generator
----------------------------

To run Procedural Generator, simply type 'ProceduralGenerator' in your
favourite command-line tool.

General Questions
-----------------

1. I need a feature that ProceduralGenerator doesn't implement, what can I do?
  There are two ways.
  First, Procedural Generator is a LGPL software and library, so you are welcome to improve it yourself ;-)!
  Second, just ask the author :-)! 

2. I implemented a cool feature, can I commit it?
  Obviously you are welcome to send any improvement. But obviously the author is quite careful with readability, correctness and stability, and will read and check any contribution before including it.
  Thus, it can take time... There are few basic rules to write contributions:
  1. Do never change the indentation of any part of the code that is not yours.
  2. Do never send codes without a lot of comments in (bad or good) English.
  3. Do never send codes with obscure and/or non-English variable names.

3. I implemented a great plug-in, can I commit it?
  I will be really proud to be proposed new plug-ins for Procedural Generator.
  But as for any improvement, I will check and test it before integrating it to Procedural Generator. 

4. Is Procedural Generator bug free?
  No, who can? I do not pretend it to be bug free. Nevertheless because it has been tested, I am fairly sure that Procedural Generator results with default options has good chances to be satisfactory.
  If you encounter trouble, send the most exhaustive description of what you did and what happened.
  I will try to explain the result or to fix the problem as soon as possible.

5. How can I ask the author?
  Just send a mail to dragonjoker59@hotmail.com!
