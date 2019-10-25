/* Header of the renderarea, that holds information about the image to paint.
 * Copyright (C) 2014  Etienne Gramlich
 * See main.cpp for more information.
 */
#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QPainter>

#include "../fraktal_List.h"
#include "../fraktal_manager.h"

namespace Ui {
    class RenderArea;
}

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(QWidget *parent = nullptr);
    ~RenderArea();
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
    void wheelEvent(QWheelEvent *event);

private:
    Ui::RenderArea *ui;
    QImage img;
    Fraktal_Manager *fraktal;
    int currentFraktal;
    size_t xRes, yRes;
};

#endif // RENDERAREA_H
