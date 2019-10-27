# Creates the application "Fraktalzeichner" from source
cmake_minimum_required( VERSION 2.8.11 )

project( fraktal ) 
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_STANDARD 11)

# Add path to module FindOpenCL.cmake
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

# Find OpenCL
find_package( OpenCL REQUIRED )
include_directories( ${OPENCL_INCLUDE_DIRS} )

# Find the Qt4 library
find_package(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})
# Run moc
set( fraktal_HEADERSqt fraktal_manager.h ui/juliainputdialog.h ui/mainwindow.h ui/renderarea.h thread.h )
QT4_WRAP_CPP(fraktal_HEADERS_MOC ${fraktal_HEADERSqt})

set( fraktal_SRC fracfuncclass.cpp fraktal_funcs.cpp fraktal_manager.cpp ui/juliainputdialog.cpp main.cpp ui/mainwindow.cpp ui/renderarea.cpp thread.cpp opencl/openclhandler.cpp )

add_executable( fraktal ${fraktal_SRC} ${fraktal_HEADERS_MOC} )

target_link_libraries(fraktal ${QT_LIBRARIES})
target_link_libraries(fraktal OpenCL)
