/* Implementation of the renderarea, that holds information about the image to
 * paint.
 * Copyright (C) 2014  Etienne Gramlich
 * See main.cpp for more information.
 */

#include <QtGui>
#include <QDesktopWidget>
#include <QWheelEvent>

#include "renderarea.h"
#include "ui_renderarea.h"

RenderArea::RenderArea(QWidget *parent) : QWidget(parent), ui(new Ui::RenderArea)
{
    Ui_RenderArea::setupUi(this);
    const QDesktopWidget desktop; // Get the resolution od the main screen
    const QRect desktopGeometry = desktop.availableGeometry(desktop.primaryScreen());
    const int width = desktopGeometry.width() > 0 ? desktopGeometry.width() : 1920;
    const int height = desktopGeometry.height() > 0 ? desktopGeometry.height() : 1080;
    img = QImage(width, height, QImage::Format_ARGB32);
    fraktal = new Fraktal_Manager(width, height);
    currentFraktal = Mandelbrot;
    setEnabled(true);   // Must be enabled to receive mouse wheel event
}

RenderArea::~RenderArea()
{
    delete ui;
    delete fraktal;
}

void RenderArea::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.fillRect(event->rect(), QBrush(Qt::white));

    fraktal->setFractal(currentFraktal);
    img = fraktal->paint(std::complex<float>(0,0));
    painter.drawImage(0, 0, img);
}

void RenderArea::calculateImage(int numFractal){
    this->currentFraktal = numFractal;
    update();
}
void RenderArea::saveBmp(){
    img.save("Bild.bmp", "BMP");
}

void RenderArea::redrawImage() {
    update();
}

void RenderArea::setJuliaC(std::complex<float> c) {
    fraktal->setJuliaC(c);
    redrawImage();
}

void RenderArea::setJuliaCimag(float imag) {
    fraktal->setJuliaCimag(imag);
    redrawImage();
}

void RenderArea::setJuliaCreal(float real) {
    fraktal->setJuliaCreal(real);
    redrawImage();
}

void RenderArea::setRenderDevice(bool useCPU){
    fraktal->setCPUrender(useCPU);
}

void RenderArea::wheelEvent(QWheelEvent *event){
    fraktal->setMidPoint(event->x(), event->y());
    fraktal->setRange(event->delta());
    update();
    event->accept();
}
