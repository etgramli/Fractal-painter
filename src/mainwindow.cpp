/* Implementation of the main window written with Qt 4.
 * Copyright (C) 2014  Etienne Gramlich
 * See main.cpp for more information.
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "openclhandler.h"

#include <stdio.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Fractal painter"));
    fraktalRenderArea = new RenderArea(this);
    setCentralWidget(fraktalRenderArea);
    setWindowState(Qt::WindowMaximized);
    numFract = Mandelbrot;
    useCPU = true;
    //numColorMode = 0;

    // Set menu
    // file menu
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);
    fileMenu->addAction(QIcon("images/page_white.png"),
                        tr("&Save"),
                        this, SLOT(saveImage()),
                        QKeySequence(tr("Ctrl+S", "File|Save")));
    fileMenu->addAction(QIcon("images/page_white.png"),
                        tr("&Quit"),
                        qApp, SLOT(quit()),
                        QKeySequence(tr("Ctrl+Q", "File|Quit")));
    // FRAKTAL  MENU
    QAction *juliaAction = new QAction(tr("&Julia-Set"), this);
    juliaAction->setCheckable(true);
    juliaAction->setShortcut(tr("Ctrl+J"));
    juliaAction->setStatusTip(tr("Paint a Julia-Set"));
    QAction *mandelbrotAction = new QAction(tr("&Mandelbrot-Set"), this);
    mandelbrotAction->setCheckable(true);
    mandelbrotAction->setShortcut(tr("Ctrl+M"));
    mandelbrotAction->setStatusTip(tr("Paint the Mabdelbrot-Set"));
    QAction *burningShipAction = new QAction(tr("&Burnung Ship"), this);
    burningShipAction->setCheckable(true);
    burningShipAction->setShortcut(tr("Ctrl+B"));
    burningShipAction->setStatusTip(tr("Paint the burning ship fractal"));
    QAction *mandelbrotSineAction = new QAction(tr("&Tricorn"), this);
    mandelbrotSineAction->setCheckable(true);
    mandelbrotSineAction->setShortcut(tr("Ctrl+T"));
    mandelbrotSineAction->setStatusTip(tr("Paint the Tricorn"));
    QActionGroup *fractalList = new QActionGroup(this);
    fractalList->addAction(juliaAction);
    fractalList->addAction(mandelbrotAction);
    fractalList->addAction(burningShipAction);
    fractalList->addAction(mandelbrotSineAction);
    fracList = fractalList->actions();
    mandelbrotAction->setChecked(true);

    QMenu *selectMenu = new QMenu(tr("&Choose fractal"), this);
    menuBar()->addMenu(selectMenu);
    selectMenu->addActions(fractalList->actions());

    // options menu
    QMenu *optionMenu = new QMenu(tr("&Options"), this);
    menuBar()->addMenu(optionMenu);
    optionMenu->addAction(QIcon("images/page_white.png"),
                        tr("&Repaint image"),
                        this, SLOT(renderImage()));
    optionMenu->addSeparator();
    QAction *useCPU = new QAction(tr("&Use CPU"), this);
    useCPU->setCheckable(true);
    useCPU->setShortcut(tr("Ctrl+C"));
    useCPU->setStatusTip(tr("Use CPU for rendering"));
    QAction *useGPU = new QAction(tr("Use &GPU"), this);
    useGPU->setCheckable(true);
    useGPU->setShortcut(tr("Ctrl+G"));
    useGPU->setStatusTip(tr("Use GPU for rendering"));
    QActionGroup *deviceGroup = new QActionGroup(this);
    deviceGroup->addAction(useCPU);
    deviceGroup->addAction(useGPU);
    useCPU->setChecked(true);
    optionMenu->addActions(deviceGroup->actions());
    devList = deviceGroup->actions();
    OpenCLHandler *myCLHandler;
    try { myCLHandler = new OpenCLHandler(); }
    catch(...){ myCLHandler = NULL; }
    if(myCLHandler == NULL ){
        useGPU->setDisabled(true);
    } else {
        useCPU->setChecked(false);
        this->useCPU = false;
        useGPU->setChecked(true);
    }
/*
    QAction *colorAction = new QAction(tr("&Farbe"), this);
    colorAction->setCheckable(true);
    colorAction->setShortcut(tr("Ctrl+F"));
    colorAction->setStatusTip("Benutze Farben");
    QAction *grayscaleAction = new QAction(tr("&Graustufen"), this);
    grayscaleAction->setCheckable(true);
    grayscaleAction->setShortcut(tr("Ctrl+G"));
    grayscaleAction->setStatusTip("Benutze Graustufen");
    QActionGroup *colorModeList = new QActionGroup(this);
    colorModeList->addAction(colorAction);
    colorModeList->addAction(grayscaleAction);
    colorList = colorModeList->actions();
    colorAction->setChecked(true);
    optionMenu->addActions(colorModeList->actions());
*/

    juliaDialog = new JuliaInputDialog(fraktalRenderArea);

    connect(fractalList, SIGNAL(triggered(QAction*)),
            this,        SLOT(changeFractal(QAction*)) );
    connect(deviceGroup, SIGNAL(triggered(QAction*)),
            this,        SLOT(changeDevice(QAction*))  );
    /*
    connect(colorModeList, SIGNAL(triggered(QAction*)),
            this,        SLOT(changeColorMode(QAction*)) );*/
}

MainWindow::~MainWindow()
{
    delete fraktalRenderArea;
    delete ui;
}

void MainWindow::renderImage(){
    fraktalRenderArea->calculateImage(numFract);
}

void MainWindow::saveImage(){
    fraktalRenderArea->saveBmp();
}

void MainWindow::changeFractal(QAction *action) {
    int numNew = fracList.indexOf(action);
    if (numFract == numNew)
        return;
    numFract = numNew;

    switch (numFract) {
    case Julia:
        juliaDialog->show();
        break;
    default:
        juliaDialog->hide();
        break;
    }
    fraktalRenderArea->calculateImage(numFract);
}

void MainWindow::changeDevice(QAction *action){
    int deviceNum = devList.indexOf(action);
    if( (useCPU == true && deviceNum == 0) ||
            (useCPU == false && deviceNum == 1) ||
            (deviceNum > 1) )
        return;
    useCPU = (deviceNum == 0) ? true : false;
    fraktalRenderArea->setRenderDevice(useCPU);
}

/*
void MainWindow::changeColorMode(QAction *action){
    int numNew = colorList.indexOf(action);
    if (numFract == numNew)
        return;
    numColorMode = numNew;

    switch (numFract) {
    case 0:
        //Set render mode to color
        break;
    case 1:
        //Set render mode to grayscale
        break;
    }
}*/

void MainWindow::closeEvent(QCloseEvent*)
{
    qApp->quit();
}
