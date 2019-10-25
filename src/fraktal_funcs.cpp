/* Implementation of the function for calculating the fractals.
 * They are not optimized.
 * Copyright (C) 2014  Etienne Gramlich
 * See main.cpp for more information.
 */

#include "fraktal_funcs.h"
#include <complex>

// Calculates the absolute value squared for better performance
inline float sqrAbs(std::complex<float> a){
    return a.imag()*a.imag() + a.real()*a.real();
}

float juliaPoint(int xRes, int yRes, float x, float y,
                 std::complex<float> c, float range, float midX, float midY){
    std::complex<float> cz(0.0f, 0.0f);
    cz.real(range * (x / (float)xRes - 0.5f) + midX);
    cz.imag(range * (y / (float)yRes - 0.5f) + midY);

    // Calcuate max. interations depending on the level of zoom
    int maxiter = 360;
    if(range > 0.0036) {
        maxiter = 360 - (float) 140 * range;
    }

    for (int l = 0; l < maxiter; l++){
        cz = std::pow(cz,2);
        cz += c;
        if (sqrAbs(cz) > 400) {
            return (float) l / maxiter;
        }
    }
    return 0.0f;
}

float mandelbrotPoint(int xRes, int yRes, float x, float y, float range,
                      float midX, float midY) {
    std::complex<float> c, z(0.0f, 0.0f);
    c.real(1.5f * range * (x / (float)xRes - 0.5f) + midX);
    c.imag(1.0f * range * (y / (float)yRes - 0.5f) + midY);

    // Calcuate max. interations depending on the level of zoom
    int maxiter = 360;
    if(range > 0.0036)
        maxiter = 360 - (float)140*range;

    for(int l = 0; l < maxiter; l++){
        z = z*z + c;
        if(sqrAbs(z) > 4)
            return (float)l / maxiter;
    }
    return 0.0f;
}

float tricornPoint(int xRes, int yRes, float x, float y, float range,
                   float midX, float midY) {
    std::complex<float> c, z(0.0f, 0.0f);
    c.real(1.5f * range * (x / (float)xRes - 0.5f) + midX);
    c.imag(1.0f * range * (y / (float)yRes - 0.5f) + midY);

    // Calcuate max. interations depending on the level of zoom
    int maxiter = 360;
    if(range > 0.0036)
        maxiter = 360 - (float)140*range;

    for(int l = 0; l < maxiter; l++){
        z = std::conj(z);
        z = z*z + c;
        if(sqrAbs(z) > 4)
            return (float)l / maxiter;
    }
    return 0.0f;
}

float burningShipPoint(int xRes, int yRes, float x, float y, float range,
                       float midX, float midY)
{
    std::complex<float> c, p, p0;
    c.real(2.0f * range * (x / (float)xRes - 0.5f) + midX);
    c.imag(2.0f * range * (y / (float)yRes - 0.5f) + midY);

    // Calcuate max. interations depending on the level of zoom
    int maxiter = 360;
    if(range > 0.0036)
        maxiter = 360 - (float)140*range;

    for(int l = 0; l < maxiter; l++){
        p = std::complex<float>(std::fabs(p0.real()),
                                std::fabs(p0.imag()));
        p = p*p + c;
        p0 = p;
        if (sqrAbs(p) > 10)
            return (float)l / maxiter;
    }
    return 0.0f;
}

// Convert form HSV color space to RGB
struct colorRGB HSVtoRGB(float h, float s, float v)	//Hue = [0;6]; Saturation, Value = [0;1]
{
    struct colorRGB rgb{0,0,0};
    int i;
    float f, p, q, t;
    if( s == 0 ) {  // achromatic (grey)
        rgb.red = rgb.green = rgb.blue = v;
        return rgb;
    }
    i = floor( h );     // sector 0 to 5
    f = h - i;			// factorial part of h
    p = v * (1 - s);
    q = v * (1 - s * f);
    t = v * (1 - s * (1 - f));
    switch( i ) {
        case 0:
            rgb.red = v * 255.0f;
            rgb.green = t * 255.0f;
            rgb.blue = p * 255.0f;
            break;
        case 1:
            rgb.red = q * 255.0f;
            rgb.green = v * 255.0f;
            rgb.blue = p * 255.0f;
            break;
        case 2:
            rgb.red = p * 255.0f;
            rgb.green = v * 255.0f;
            rgb.blue = t * 255.0f;
            break;
        case 3:
            rgb.red = p * 255.0f;
            rgb.green = q * 255.0f;
            rgb.blue = v * 255.0f;
            break;
        case 4:
            rgb.red = t * 255.0f;
            rgb.green = p * 255.0f;
            rgb.blue = v * 255.0f;
            break;
        default:		// case 5:
            rgb.red = v * 255.0f;
            rgb.green = p * 255.0f;
            rgb.blue = q * 255.0f;
            break;
    }
    return rgb;
}
