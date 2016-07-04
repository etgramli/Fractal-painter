/* Header of the thread class for splitting the work on multiple CPU cores.
 * Copyright (C) 2014  Etienne Gramlich
 * See main.cpp for more information.
 */
#ifndef THREAD_H
#define THREAD_H

#include <QThread>
// TODO: Migrate to C++11 Threads
#include <QImage>
#include <complex>
#include "fracfuncclass.h"

class Thread : public QThread
{
    Q_OBJECT
public:
    Thread();
    ~Thread();

    void init(FracFuncClass *funcClass, QImage *img,
              std::complex<float> centerPoint,
              int numThreads, int offsetThread);
    //void init(QImage *img, MyComplex c, MyComplex centerPoint, int numThreads, int offsetThread);
    void setLine(int line, QRgb* PixelLine);

protected:
    void run();

private:
    FracFuncClass *fracFunc;

    QImage *image;
    QRgb *Pixel;
    int width, height;  //Image dimensions
    int line;           //Image line to compute
    std::complex<float> c, centerPoint;
    int numThreads, offset; //Number of threads, offset of this one
};

#endif // THREAD_H
