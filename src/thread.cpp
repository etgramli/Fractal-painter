/* Implementation of the thread class for splitting the work on multiple
 * CPU cores.
 * Copyright (C) 2014  Etienne Gramlich
 * See main.cpp for more information.
 */
#include "thread.h"

Thread::Thread() {}
Thread::~Thread() {}

void Thread::init(FracFuncClass *funcClass, QImage *img,
                  std::complex<float> centerPoint,
                  int numThreads, int offsetThread) {
    this->fracFunc = funcClass;
    this->image = img;
    this->width = image->width();
    this->height = image->height();
    this->centerPoint = centerPoint;
    this->numThreads = numThreads;
    this->offset = offsetThread;
}

void Thread::setLine(int line, QRgb* PixelLine){
    this->line = line;
    Pixel = PixelLine;
}

void Thread::run(){
    if (Pixel == NULL) {
        exit(1);
    }

    float fracValue;
    struct colorRGB point;

    for(int line = offset; line < height; line += numThreads) {
        Pixel = reinterpret_cast<QRgb*>(image->scanLine(line));  //Reading line of pixels
        for(int pos = 0; pos < width; pos++){
            fracValue = fracFunc->calcFract((float) pos + centerPoint.real(),
                                            (float) line + centerPoint.imag());

            point = HSVtoRGB( fracValue * 6.0, 1, fracValue == 0 ? 0 : 1);

            Pixel[pos] = qRgb(point.red, point.green, point.blue);
        }
    }
    return;
}
