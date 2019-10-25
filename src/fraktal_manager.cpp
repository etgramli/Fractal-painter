/* Header of my little OpenCL library
 * Copyright (C) 2014  Etienne Gramlich
 * See main.cpp for more information.
 */

#include "fraktal_manager.h"
#include "fracfuncclass.h"

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

long Fraktal_Manager::getNumCores() {
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
    return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

Fraktal_Manager::Fraktal_Manager(size_t xRes, size_t yRes) {
    this->xRes = xRes;
    this->yRes = yRes;
    midPointX = 0.0f;
    midPointY = 0.0f;
    range = 2;
    numCores = getNumCores();
    numFract = Julia;
    fractChanged = true;
    juliaC = std::complex<float>(0,0);
    juliaCchanged = false;
    useCPU = false;
    rangeChanged = false;
    buffer = new unsigned char[xRes * yRes * 4];
    globalWorkSize[0] = xRes;   // Set the work sizes so that each kernel
    globalWorkSize[1] = yRes;   // can query its pixel position
    localWorkSize[0] = 0;       // Work size for work groups; May set them
    localWorkSize[1] = 0;       // to 0 for performance improvement by Implementation
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
            myCLHandler->createMemObj(sizeof(cl_float2), &clJuliaC, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR);
                                              //Kernel index, Argument index, Memory object index
            myCLHandler->setKernelArgWithMemObj(0           , 0             , 2);
            myCLHandler->setKernelArgWithMemObj(0           , 1             , 0);
            if(myCLHandler->validateKernelArgs(0))
                printf("Finished: Setting kernel args for juliaPoint: successfully\n");
            else
                printf("Finished: Setting kernel args for juliaPoint: NOT successfully\n");
        }
        errNum = myCLHandler->compileKernelFromFile(clKernelNames[1]);
        if(errNum == CL_SUCCESS){
            myCLHandler->setKernelArgWithMemObj(1, 0, 0);
            if(myCLHandler->validateKernelArgs(1))
                printf("Finished: Setting kernel args for mandelbrotPoint: successfully\n");
            else
                printf("Finished: Setting kernel args for mandelbrotPoint: NOT successfully\n");
        }
        errNum = myCLHandler->compileKernelFromFile(clKernelNames[2]);
        if(errNum == CL_SUCCESS){
            myCLHandler->setKernelArgWithMemObj(2, 0, 0);
            if(myCLHandler->validateKernelArgs(2))
                printf("Finished: Setting kernel args for burningShipPoint: successfully\n");
            else
                printf("Finished: Setting kernel args for burningShipPoint: NOT successfully\n");
        }
        errNum = myCLHandler->compileKernelFromFile(clKernelNames[3]);
        if(errNum == CL_SUCCESS){
            myCLHandler->setKernelArgWithMemObj(3, 0, 0);
            if(myCLHandler->validateKernelArgs(3))
                printf("Finished: Setting kernel args for tricornPoint: successfully\n");
            else{
                printf("Finished: Setting kernel args for tricornPoint: NOT successfully\n");
                throw "Can not execute kernel for color space conversion";
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
                throw "Can not execute kernel for color space conversion";
            }
        }
        else {
            throw "Setting color space conversion kernel failed";
        }
    } catch(...) {
        myCLHandler = nullptr;
    }
}
Fraktal_Manager::~Fraktal_Manager(){
    delete myCLHandler;
    delete buffer;
}

void Fraktal_Manager::setJuliaCimag(double imag) {
    if(imag <= 5.0f && imag >= -5.0f){
        juliaC.imag(imag);
    } else {
        juliaC.imag(0.0f);
    }
    if (myCLHandler != nullptr) {   // Also update Value in OpenCL
        cl_float2 clJuliaC; // Create constant for Julia Set to pass to OCL
        clJuliaC.x = juliaC.real();
        clJuliaC.y = juliaC.imag();
        myCLHandler->overwriteMemObj(2, sizeof(cl_float2), &clJuliaC, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR);
        myCLHandler->setKernelArgWithMemObj(0, 0, 2);
    }
    juliaCchanged = true;
}
void Fraktal_Manager::setJuliaCreal(double real){
    if(real <= 5.0f && real >= -5.0f){
        juliaC.real(real);
    } else {
        juliaC.real(0.0f);
    }
    if(myCLHandler != nullptr) {   // Also update Value in OpenCL
        cl_float2 clJuliaC; // Create constant for Julia Set to pass to OCL
        clJuliaC.x = juliaC.real();
        clJuliaC.y = juliaC.imag();
        myCLHandler->overwriteMemObj(2, sizeof(cl_float2), &clJuliaC, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR);
        myCLHandler->setKernelArgWithMemObj(0, 0, 2);
    }
    juliaCchanged = true;
}
void Fraktal_Manager::setJuliaC(std::complex<float> c) {
    setJuliaCimag(c.imag());
    setJuliaCreal(c.real());
}
void Fraktal_Manager::setRange(int delta){
    // Delta is positive if the wheel is scrolled away from the user
    // and negative if the wheel is scrolled towards the user
    int mouseWheelSteps = delta / 120;
    range = range - (range / 16.0f) * mouseWheelSteps;
    rangeChanged = true;
}
void Fraktal_Manager::setMidPoint(int x, int y){
    // Change midpoint of the image
    midPointX += ((float) x/xRes - 0.5f) * range / 2.0f;
    midPointY += ((float) y/yRes - 0.5f) * range / 2.0f;
}

void Fraktal_Manager::setFractal(int numFract){
    if(numFract == this->numFract)
        return;
    else if (numFract > Tricorn)
        return;
    else {
        fractChanged = true;
        this->numFract = numFract;
        // If another fractal chosen reset range and midpoint
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
        switch (numFract){
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

        std::vector<Thread> threads(numCores);
        for(long l = 0; l < numCores; l++) {
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
        switch (numFract){
        case Julia:
            myCLHandler->setKernelArg(Julia, 2, sizeof(cl_float), &range);
            myCLHandler->setKernelArg(Julia, 3, sizeof(cl_float), &midPointX);
            myCLHandler->setKernelArg(Julia, 4, sizeof(cl_float), &midPointY);
            break;
        default:    // For every other function the parameters have those indices
            myCLHandler->setKernelArg(numFract, 1, sizeof(cl_float), &range);
            myCLHandler->setKernelArg(numFract, 2, sizeof(cl_float), &midPointX);
            myCLHandler->setKernelArg(numFract, 3, sizeof(cl_float), &midPointY);
        }

        // Enqueue the kernel to the respective fractal
        myCLHandler->enqueueKernel(numFract, 2, globalWorkSize, localWorkSize);
        // Convert from HSV to RGB color space
        myCLHandler->enqueueKernel( 4, 2, globalWorkSize, localWorkSize);
        // Copy back image from OpenCL device
        cl::size_t<3> region;
        region[0] = xRes;
        region[1] = yRes;
        region[2] = 1;
        errNum = myCLHandler->getImageFromDevice(1, region, buffer);
        if(errNum == CL_SUCCESS) {
            // Copy from OpenCL Image to Qt Image
            renderedImage = QImage(buffer, xRes, yRes, QImage::Format_ARGB32);
        }
    }
    image = renderedImage;
    return renderedImage;
}

void Fraktal_Manager::setCPUrender(bool useCPU){
    this->useCPU = useCPU;
}
