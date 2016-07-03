# Fractal painter

This program paints the fractals listed below and allows you to save them as an image (windows bitmap).
You can zoom in and out via the mouse wheel.
It is also possible to render the fractals on your GPU via OpenCL. Look at the
list below for supported fractals.
The default language is English, but I made a German localization.

## Supproted fractals (CPU and OpenCL):
- Julia-Set
- Mandelbrot-Set
- Burning Ship
- Tricorn


## Compiling

### How to compile with CMake (Linux):

Required packages:
- Qt4 development files
- OpenCL development files
- CMake
- CCMake

I recommend you to make an "out-of-source" build with CMake in the parent directory because you need the *.cl files for OpenCL to work.
Additionally you need the fraktal_de.qm for the German translation in the executable's directory.
You can do a "out-of-source" build with the following commands for example:
```
ccmake src/
Press 'c', type in Release as CMake_Build_Type, again 'c' then 'g'.
make
```
Or just run cmake in the src directory and copy the executable in the parent directory.

Maybe you need to add Qt4.x\bin to the Path for find_package to be successful.


### How to compile with Qt Creator:
Requred Packages:
- Qt4 development files
- Qt Creator
- OpenCL development files

In Fraktal.pro, replace
```
"C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v6.0/lib/Win32/"
```
and
```
"C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v6.0/include"
```
with the path of your OpenCL-Implementation.
Then you can open the file with Qt Creator and compile it yourself.
