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

RenderArea::RenderArea(QWidget *parent)
            : QWidget(parent), ui(new Ui::RenderArea)
{
    ui->setupUi(this);
    QDesktopWidget desktop; // Get the resolution od the main screen
    this->xRes = desktop.availableGeometry(desktop.primaryScreen()).width();
    this->yRes = desktop.availableGeometry(desktop.primaryScreen()).height();
    img = QImage(this->xRes, this->yRes, QImage::Format_ARGB32);
    fraktal = new Fraktal_Manager(xRes, yRes);
    currentFraktal = 1;
    setEnabled(true);   // Must be enabled to recieve mouse wheel event
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

    img = fraktal->paint(currentFraktal, std::complex<float>(0,0));
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
void RenderArea::setJuliaCimag(double imag) {
    fraktal->setJuliaCimag(imag);
    redrawImage();
}
void RenderArea::setJuliaCreal(double real) {
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
