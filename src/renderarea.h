/* Header of the renderarea, that holds information about the image to paint.
 * Copyright (C) 2014  Etienne Gramlich
 * See main.cpp for more information.
 */
#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QPainter>
#include "fraktal_List.h"
#include "fraktal_manager.h"
namespace Ui {
    class RenderArea;
}

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(QWidget *parent = 0, int numCores = 1, size_t xRes = 1280, size_t yRes = 720);
    ~RenderArea();
    void setResolution(int xRes, int yRes);
    void setRange(double xRange, double yRange);
    void setRenderDevice(bool useCPU);
    void saveBmp();
    void paintImage(QPainter &painter);
    void calculateImage(int numFractal);
    void redrawImage();
    void setJuliaC(std::complex<float> c);

public slots:
    void setJuliaCimag(double imag);
    void setJuliaCreal(double real);

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::RenderArea *ui;
    QImage img;
    Fraktal_Manager *fraktal;
    int currentFraktal;
    size_t xRes, yRes;
    double xRange, yRange;
};

#endif // RENDERAREA_H
