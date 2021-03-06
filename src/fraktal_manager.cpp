/* Header of my little OpenCL library
 * Copyright (C) 2014  Etienne Gramlich
 * See main.cpp for more information.
 */

#include "fraktal_manager.h"
#include "fracfuncclass.h"
#include "exceptions/KernelException.h"

#define CL_TARGET_OPENCL_VERSION 120

#include <complex>
#include <cstdio>

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#ifdef _WIN32
#include <windows.h>
#elif MACOS
#include <sys/param.h>
#include <sys/sysctl.h>
#else
#include <unistd.h>
#endif

int Fraktal_Manager::getNumCores() {
#ifdef WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
#elif MACOS
    int nm[2];
    size_t len = 4;
    uint32_t count;

    nm[0] = CTL_HW;
    nm[1] = HW_AVAILCPU;
    sysctl(nm, 2, &count, &len, NULL, 0);

    if(count < 1) {
        nm[1] = HW_NCPU;
        sysctl(nm, 2, &count, &len, NULL, 0);
        if(count < 1) {
            count = 1;
        }
    }
    return count;
#else
    return (int) sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

Fraktal_Manager::Fraktal_Manager(size_t xRes, size_t yRes) {
    this->xRes = xRes;
    this->yRes = yRes;
    midPointX = 0.0f;
    midPointY = 0.0f;
    range = 2;
    numCores = getNumCores();
    numFractal = Julia;
    fractChanged = true;
    juliaC = std::complex<float>(0,0);
    juliaCchanged = false;
    useCPU = false;
    rangeChanged = false;
    globalWorkSize[0] = xRes;   // Set the work sizes so that each kernel
    globalWorkSize[1] = yRes;   // can query its pixel position
    localWorkSize[0] = 0;       // Work size for work groups; May set them
    localWorkSize[1] = 0;       // to 0 for performance improvement by Implementation
    try {
        buffer = new unsigned char[xRes * yRes * 4];
    } catch (...) {
        // ToDo: Better error handling
        buffer = nullptr;
    }

    try {
        myCLHandler = new OpenCLHandler();

        std::vector<std::string> clKernelNames = {
                "juliaPoint.cl",
                "mandelbrotPoint.cl",
                "burningShipPoint.cl",
                "tricornPoint.cl",
                "HSVtoRGB.cl"};


        myCLHandler->create2DImageA(xRes, yRes, nullptr);       // Write to by OCL to store the fractal values as float
        myCLHandler->create2DImageARGB(xRes, yRes, nullptr);    // Holds the RGB valus after converting from HSV

        errNum = myCLHandler->compileKernelFromFile(clKernelNames[0]);
        if(errNum == CL_SUCCESS){
            cl_float2 clJuliaC; // Create constant for Julia Set to pass to OCL
            clJuliaC.x = juliaC.real();
            clJuliaC.y = juliaC.imag();
            printf("Julia constant with OpenCL float2: (%f,%f)", clJuliaC.x, clJuliaC.y);
            myCLHandler->createMemObj(sizeof(cl_float2), &clJuliaC, clFlagsReadOnlyCopyHostPtr);
                                              //Kernel index, Argument index, Memory object index
            myCLHandler->setKernelArgWithMemObj(0           , 0             , 2);
            myCLHandler->setKernelArgWithMemObj(0           , 1             , 0);
            if(myCLHandler->validateKernelArgs(0))
                printf("Finished: Setting kernel args for juliaPoint: successfully\n");
            else
                printf("Finished: Setting kernel args for juliaPoint: NOT successfully\n");
        }
        for (int i = 1; i <= 3; ++i) {
            errNum = myCLHandler->compileKernelFromFile(clKernelNames[i]);
            if (errNum == CL_SUCCESS) {
                myCLHandler->setKernelArgWithMemObj(i, 0, 0);
                printf("Finished: Setting kernel args for ");
                printf("%s", clKernelNames[i].c_str());
                if (myCLHandler->validateKernelArgs(i)) {
                     printf(": successfully\n");
                } else {
                    printf(": NOT successfully");
                }
            }
        }
        errNum = myCLHandler->compileKernelFromFile(clKernelNames[4]);
        if(errNum == CL_SUCCESS){
            myCLHandler->setKernelArgWithMemObj(4, 0, 0);
            myCLHandler->setKernelArgWithMemObj(4, 1, 1);
            if(myCLHandler->validateKernelArgs(4))
                printf("Finished: Setting kernel args for HSVtoRGB: successfully\n");
            else{
                printf("Finished: Setting kernel args for HSVtoRGB: NOT successfully\n");
                throw KernelException("Can not execute kernel for color space conversion");
            }
        }
        else {
            throw KernelException("Setting color space conversion kernel failed");
        }
    } catch(...) {
        delete myCLHandler;
        myCLHandler = nullptr;
    }
}
Fraktal_Manager::~Fraktal_Manager(){
    delete myCLHandler;
    delete[] buffer;
}

void Fraktal_Manager::setJuliaCimag(float imag) {
    juliaC.imag(imag > 5 || imag < -5 ? 0.0f : imag);
    if (myCLHandler != nullptr) {   // Also update Value in OpenCL
        cl_float2 clJuliaC{{juliaC.real(), juliaC.imag()}}; // Create constant for Julia Set to pass to OCL
        myCLHandler->overwriteMemObj(2, sizeof(cl_float2), &clJuliaC, clFlagsReadOnlyCopyHostPtr);
        myCLHandler->setKernelArgWithMemObj(0, 0, 2);
    }
    juliaCchanged = true;
}
void Fraktal_Manager::setJuliaCreal(float real){
    juliaC.real(real > 5 || real < -5 ? 0.0f : real);
    if(myCLHandler != nullptr) {   // Also update Value in OpenCL
        cl_float2 clJuliaC{{juliaC.real(), juliaC.imag()}}; // Create constant for Julia Set to pass to OCL
        myCLHandler->overwriteMemObj(2, sizeof(cl_float2), &clJuliaC, clFlagsReadOnlyCopyHostPtr);
        myCLHandler->setKernelArgWithMemObj(0, 0, 2);
    }
    juliaCchanged = true;
}
void Fraktal_Manager::setJuliaC(std::complex<float> c) {
    setJuliaCimag(c.imag());
    setJuliaCreal(c.real());
}
void Fraktal_Manager::setRange(int delta) {
    // Delta is positive if the wheel is scrolled away from the user
    // and negative if the wheel is scrolled towards the user
    const float mouseWheelSteps = (float) delta / 120.0f;
    range = range - (range / 16.0f) * mouseWheelSteps;
    rangeChanged = true;
}
void Fraktal_Manager::setMidPoint(int x, int y) {
    // Change midpoint of the image
    const float xRatio = (float) x / (float)xRes;
    const float yRatio = (float) y / (float)yRes;
    midPointX += (xRatio - 0.5f) * range / 2.0f;
    midPointY += (yRatio - 0.5f) * range / 2.0f;
}

void Fraktal_Manager::setFractal(int newNumFractal){
    if(newNumFractal != numFractal && newNumFractal <= Tricorn) {
        fractChanged = true;
        numFractal = newNumFractal;
        // Reset range and midpoint on fractal change
        range = 2;
        midPointX = 0.0f;
        midPointY = 0.0f;
    }
}

QImage Fraktal_Manager::paint(std::complex<float> centerPoint){
    if (!fractChanged && !juliaCchanged && !rangeChanged)
        return image;   // If fractal or range not changed do nothing

    juliaCchanged = false;
    QImage renderedImage = QImage(xRes, yRes, QImage::Format_RGB32);

    FracFuncClass *funcObject;

    // Use CPU if OpenCL-Handler creation or kernel creation failed
    if(myCLHandler == nullptr || useCPU){
        // Initialize CPU to calculate fractals
        switch (numFractal){
        case Julia:
            funcObject = new JuliaFuncClass(renderedImage.width(),
                                            renderedImage.height(),
                                            juliaC,
                                            range,
                                            midPointX, midPointY);
            break;
        case Mandelbrot:
            funcObject = new MandelbrotFuncClass(renderedImage.width(),
                                                 renderedImage.height(),
                                                 range,
                                                 midPointX, midPointY);
            break;
        case BurningShip:
            funcObject = new BurningShipFuncClass(renderedImage.width(),
                                                  renderedImage.height(),
                                                  range,
                                                  midPointX, midPointY);
            break;
        case Tricorn:
            funcObject = new TricornFuncClass(renderedImage.width(),
                                              renderedImage.height(),
                                              range,
                                              midPointX, midPointY);
            break;
        default:    // Action if GUI passes wrong index: return black image
            return renderedImage;
        }

        printf("Num Cores: %d\n", numCores);
        std::vector<Thread> threads(numCores);
        for(int l = 0; l < numCores; l++) {
            threads[l].init(funcObject, &renderedImage, centerPoint, numCores, l);
            threads[l].start();
        }
        bool allThreadsFinished = false;
        while( !allThreadsFinished ) {      //Wait for all threads to finish
            allThreadsFinished = threads[0].isFinished();
            for(int l = 1; l < numCores; l++){
                allThreadsFinished = allThreadsFinished && threads[l].isFinished();
            }
        }
        delete funcObject;

    } else {
        // Initialize CPU to calculate fractals
        // Set variable kernel arguments: range, midpoint X, Y
        switch (numFractal){
        case Julia:
            myCLHandler->setKernelArg(Julia, 2, sizeof(cl_float), &range);
            myCLHandler->setKernelArg(Julia, 3, sizeof(cl_float), &midPointX);
            myCLHandler->setKernelArg(Julia, 4, sizeof(cl_float), &midPointY);
            break;
        default:    // For every other function the parameters have those indices
            myCLHandler->setKernelArg(numFractal, 1, sizeof(cl_float), &range);
            myCLHandler->setKernelArg(numFractal, 2, sizeof(cl_float), &midPointX);
            myCLHandler->setKernelArg(numFractal, 3, sizeof(cl_float), &midPointY);
        }

        myCLHandler->enqueueKernel(numFractal, 2, globalWorkSize, localWorkSize); // Enqueue respective fractal kernel
        myCLHandler->enqueueKernel(         4, 2, globalWorkSize, localWorkSize); // Convert from HSV to RGB color space
        // Copy back image from OpenCL device
        cl::size_t<3> region;
        region[0] = xRes;
        region[1] = yRes;
        region[2] = 1;
        errNum = myCLHandler->getImageFromDevice(1, region, buffer);
        if(errNum == CL_SUCCESS) {
            renderedImage = QImage(buffer, xRes, yRes, QImage::Format_ARGB32);  // Copy from OpenCL Image to Qt Image
        }
    }
    image = renderedImage;
    return renderedImage;
}

void Fraktal_Manager::setCPUrender(bool useNowCPU){
    this->useCPU = useNowCPU;
}
