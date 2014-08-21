/* Implemtation of the function classes for the fractal calculation.
 * Copyright (C) 2014  Etienne Gramlich
 * See main.cpp for more information.
 */
#include "fracfuncclass.h"

float JuliaFuncClass::calcFract(float x, float y) {
    return juliaPoint(xRes, yRes, x, y, c);
}

float MandelbrotFuncClass::calcFract(float x, float y) {
    return mandelbrotPoint(xRes, yRes, x, y);
}

float BurningShipFuncClass::calcFract(float x, float y) {
    return burningShipPoint(xRes, yRes, x, y);
}

float TricornFuncClass::calcFract(float x, float y) {
    return tricornPoint(xRes, yRes, x, y);
}
