/* Implementation of the renderarea, that holds information about the image to
 * paint.
 * Copyright (C) 2014  Etienne Gramlich
 * See main.cpp for more information.
 */
#include <QtGui>
#include "renderarea.h"
#include "ui_renderarea.h"

RenderArea::RenderArea(QWidget *parent, int numCores,
                       size_t xRes, size_t yRes)
            : QWidget(parent), ui(new Ui::RenderArea)
{
    ui->setupUi(this);
    this->xRes = xRes;
    this->yRes = yRes;
    img = QImage(this->xRes, this->yRes, QImage::Format_ARGB32);
    fraktal = new Fraktal_Manager(xRes, yRes, numCores);
    currentFraktal = 1;
}
RenderArea::~RenderArea()
{
    delete ui;
    delete fraktal;
}

void RenderArea::setResolution(int xRes, int yRes){
    this->xRes = xRes;
    this->yRes = yRes;
}

void RenderArea::setRange(double xRange, double yRange){
    this->xRange = xRange;
    this->yRange = yRange;
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
