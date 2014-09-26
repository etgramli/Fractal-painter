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
    FracFuncClass(int xRes, int yRes, float range, float midX, float midY)
        : xRes(xRes), yRes(yRes), range(range), midX(midX), midY(midY) {}
    virtual ~FracFuncClass() {}
    //virtual float operator()(float x, float y) = 0;
    virtual float calcFract(float x, float y) = 0;

protected:
    int xRes, yRes;
    float range;
    float midX, midY;
};

class JuliaFuncClass : public FracFuncClass
{
public:
    JuliaFuncClass(int xRes, int yRes, std::complex<float> c, float range, float midX, float midY)
        : FracFuncClass(xRes, yRes, range, midX, midY), c(c) {}
    float calcFract(float x, float y);

private:
    std::complex<float> c;
};

class MandelbrotFuncClass : public FracFuncClass
{
public:
    MandelbrotFuncClass(int xRes, int yRes, float range, float midX, float midY)
        : FracFuncClass(xRes, yRes, range, midX, midY) {}
    float calcFract(float x, float y);
};

class BurningShipFuncClass : public FracFuncClass
{
public:
    BurningShipFuncClass(int xRes, int yRes, float range, float midX, float midY)
        : FracFuncClass(xRes, yRes, range, midX, midY) {}
    float calcFract(float x, float y);
};

class TricornFuncClass : public FracFuncClass
{
public:
    TricornFuncClass(int xRes, int yRes, float range, float midX, float midY)
        : FracFuncClass(xRes, yRes, range, midX, midY) {}
    float calcFract(float x, float y);
};

#endif // FRACFUNCCLASS_H
