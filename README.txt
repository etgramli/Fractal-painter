Fractal painter
*******************************************************************************

This program paints the fractals listed below and allows you to save them as
an image (windows bitmap).
You can zoom in and out via the mouse wheel.
It is also possible to render the fractals on your GPU via OpenCL. Look at the
list below for supported fractals.
The default language is English, but I made a German localization.

*******************************************************************************

Supproted fractals (CPU and OpenCL):
- Julia-Set
- Mandelbrot-Set
- Burning Ship
- Tricorn

*******************************************************************************

Execute on Windows:
-------------------
There is a .zip file which contains the executable (.exe) and other files that
are requred for execution.

Content of the .zip archive:
Fraktals.exe            The executable; libraries are statically linked
README.txt              This README   
fraktal_de.qm           German translation
gpl-3.0.txt             Text of the GNU GPL - the license of this program

burningShipPoint.cl     The OpenCL code to generate the burning ship fractal
HSVtoRGB.cl             The OpenCL code to convert from HSV color space to RGB
juliaPoint.cl           The OpenCL code to generate the julia set
mandelbrotPoint.cl      The OpenCL code to generate the mandelbrot set
tricornPoint            The OpenCL code to generate the tricorn

*******************************************************************************

How to compile with CMake:
--------------------------
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

How to compile with Qt Creator:
-------------------------------
Requred:
--------
-Qt4 development files and Qt Creator
-OpenCL development files

In Fraktal.pro, replace
"C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v6.0/lib/Win32/"
and
"C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v6.0/include"
with the path of your OpenCL-Implementation.
Then you can open the file with Qt Creator and compile it yourself.

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

Version 1.5 - (2014-09-26)
-Zooming is now possible with both CPU and OpenCL (mouse wheel)
