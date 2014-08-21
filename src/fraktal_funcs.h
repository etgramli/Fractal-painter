/* Header of the function for calculating the fractals.
 * Copyright (C) 2014  Etienne Gramlich
 * See main.cpp for more information.
 */
#ifndef FRAKTAL_FUNCS_H
#define FRAKTAL_FUNCS_H

#include <complex>

const double PI = 3.1415926535897324;
inline float quadAbs(std::complex<float> a);

// Functions for calculating points of fractals
float juliaPoint(int xRes, int yRes, float x, float y, std::complex<float> c);
float mandelbrotPoint(int xRes, int yRes, float x, float y);

// DOES NOT WORK CURRENTLY
float mandelbrotSinePoint(int xRes, int yRes, float x, float y);

float tricornPoint(int xRes, int yRes, float x, float y);

float burningShipPoint(int xRes, int yRes, float x, float y);

// Function(s) for colors
struct colorRGB {
    unsigned char red, green, blue;
};

//Hue = [0;6]; Saturation, Value = [0;1]
struct colorRGB HSVtoRGB(float h, float s, float v);


#endif // FRAKTAL_FUNCS_H
