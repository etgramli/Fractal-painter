/* Header of the function classes for the fractal calculation.
 * Copyright (C) 2014  Etienne Gramlich
 * See main.cpp for more information.
 */
#ifndef FRACFUNCCLASS_H
#define FRACFUNCCLASS_H

#include "fraktal_funcs.h"
#include <complex>

class FracFuncClass
{
public:
    FracFuncClass(int xRes, int yRes) : xRes(xRes), yRes(yRes) {}
    virtual ~FracFuncClass() {}
    //virtual float operator()(float x, float y) = 0;
    virtual float calcFract(float x, float y) = 0;

protected:
    int xRes, yRes;
};

class JuliaFuncClass : public FracFuncClass
{
public:
    JuliaFuncClass(int xRes, int yRes, std::complex<float> c) : FracFuncClass(xRes, yRes), c(c) {}
    float calcFract(float x, float y);

private:
    std::complex<float> c;
};

class MandelbrotFuncClass : public FracFuncClass
{
public:
    MandelbrotFuncClass(int xRes, int yRes) : FracFuncClass(xRes, yRes) {}
    float calcFract(float x, float y);
};

class BurningShipFuncClass : public FracFuncClass
{
public:
    BurningShipFuncClass(int xRes, int yRes) : FracFuncClass(xRes, yRes) {}
    float calcFract(float x, float y);
};

class TricornFuncClass : public FracFuncClass
{
public:
    TricornFuncClass(int xRes, int yRes) : FracFuncClass(xRes, yRes) {}
    float calcFract(float x, float y);
};

#endif // FRACFUNCCLASS_H
