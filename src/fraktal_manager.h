/* Header of the fractal manager class.
 * It handles the calculation, chooses CPU or OpenCL
 * and returns the image.
 * Copyright (C) 2014  Etienne Gramlich
 * See main.cpp for more information.
 */
#ifndef FRAKTAL_MANAGER_H
#define FRAKTAL_MANAGER_H

#include <QImage>
#include "fraktal_List.h"
#include <complex>
#include "thread.h"
#include "opencl/openclhandler.h"

class Fraktal_Manager
{
public:
    Fraktal_Manager(int xRes = 1920, int yRes = 1080);
    ~Fraktal_Manager();
    void setJuliaCimag(float imag);
    void setJuliaCreal(float real);
    void setJuliaC(std::complex<float> c);
    void setFractal(int numFract);
    void setRange(int delta);
    void setMidPoint(int x, int y);
    QImage paint(std::complex<float> centerPoint);
    void setCPUrender(bool useCPU);

private:
    static int getNumCores();
    int numCores;
    int numFract;
    bool fractChanged;
    int xRes, yRes;
    float range;
    float midPointX, midPointY;
    QImage image;
    std::complex<float> juliaC;
    bool juliaCchanged;
    bool useCPU;
    bool rangeChanged;
    OpenCLHandler *myCLHandler;
    cl_int errNum;
    size_t globalWorkSize[2]{};
    size_t localWorkSize[2]{};
    unsigned char *buffer;
};

#endif // FRAKTAL_MANAGER_H
