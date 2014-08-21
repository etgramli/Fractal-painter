Fractal painter
*******************************************************************************

This program paints the fractals listed below and allows you to save them as
windows bitmap.
You also can see if painting on your GPU via OpenCL is supported for each
fractal.
The default language is English, but I made a German localization.
*******************************************************************************

Supproted fractals:
- Julia-Set (CPU, OpenCL)
- Mandelbrot-Set (CPU, OpenCL)
- Burning Ship (CPU, OpenCL)
- Tricorn (CPU)
*******************************************************************************

How to compile:
---------------
Requred:
--------
-Qt4 development files
-OpenCL development files
-CMake

I recommend you to make an "out-of-source" build with CMake in the parent
directory because you need the *.cl files for OpenCL to work.
Additionally you need the fraktal_de.qm for the German translation in the
executable's directory.
You can do a "out-of-source" build with the following commands for example:
ccmake src/
Press 'c', type in Release as CMake_Build_Type, again 'c' then 'g'.
make

Or just run cmake in the src directory and copy the executable in the parent
directory.

Maybe you need to add Qt4.x\bin to the Path for find_package to be successful.
*******************************************************************************

Log:
----
Version 1.0	- (2013-12-09):
-Painting Mandelbrot- und Julia-Sets (CPU)

Version 1.1 - (2014-08-06)
-Own structure for complex Numbers replaced by those of the standard library
-Began OpenCL-Implementation

Version 1.2 - (2014-08-10)
-Burning Ship works (CPU)

Version 1.3 - (2014-08-12)
-Julia-, Mandelbrot-Set, Burning Ship and Tricorn work mit OpenCL

Version 1.4 - (2014-08-19)
-Changed main language from German to Enlish
-Added Geman as localization
-Option for choosing rendering device
