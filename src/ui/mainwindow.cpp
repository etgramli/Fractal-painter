/* Implementation of the main window written with Qt 4.
 * Copyright (C) 2014  Etienne Gramlich
 * See main.cpp for more information.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../opencl/openclhandler.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Fractal painter"));
    fraktalRenderArea = new RenderArea(this);
    setCentralWidget(fraktalRenderArea);
    setWindowState(Qt::WindowMaximized);
    numFract = Mandelbrot;
    useCPU = true;
    numColorMode = 0;

    // Set menu
    // file menu
    auto *fileMenu = new QMenu(tr("&File"), this);
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
    auto *juliaAction = new QAction(tr("&Julia-Set"), this);
    juliaAction->setCheckable(true);
    juliaAction->setShortcut(tr("Ctrl+J"));
    juliaAction->setStatusTip(tr("Paint a Julia-Set"));
    auto *mandelbrotAction = new QAction(tr("&Mandelbrot-Set"), this);
    mandelbrotAction->setCheckable(true);
    mandelbrotAction->setShortcut(tr("Ctrl+M"));
    mandelbrotAction->setStatusTip(tr("Paint the Mabdelbrot-Set"));
    auto *burningShipAction = new QAction(tr("&Burnung Ship"), this);
    burningShipAction->setCheckable(true);
    burningShipAction->setShortcut(tr("Ctrl+B"));
    burningShipAction->setStatusTip(tr("Paint the burning ship fractal"));
    auto *mandelbrotSineAction = new QAction(tr("&Tricorn"), this);
    mandelbrotSineAction->setCheckable(true);
    mandelbrotSineAction->setShortcut(tr("Ctrl+T"));
    mandelbrotSineAction->setStatusTip(tr("Paint the Tricorn"));
    auto *fractalList = new QActionGroup(this);
    fractalList->addAction(juliaAction);
    fractalList->addAction(mandelbrotAction);
    fractalList->addAction(burningShipAction);
    fractalList->addAction(mandelbrotSineAction);
    fracList = fractalList->actions();
    mandelbrotAction->setChecked(true);

    auto *selectMenu = new QMenu(tr("&Choose fractal"), this);
    menuBar()->addMenu(selectMenu);
    selectMenu->addActions(fractalList->actions());

    // options menu
    auto *optionMenu = new QMenu(tr("&Options"), this);
    menuBar()->addMenu(optionMenu);
    optionMenu->addAction(QIcon("images/page_white.png"),
                        tr("&Repaint image"),
                        this, SLOT(renderImage()));
    optionMenu->addSeparator();
    auto *useCpuAction = new QAction(tr("&Use CPU"), this);
    useCpuAction->setCheckable(true);
    useCpuAction->setShortcut(tr("Ctrl+C"));
    useCpuAction->setStatusTip(tr("Use CPU for rendering"));
    auto *useGPU = new QAction(tr("Use &GPU"), this);
    useGPU->setCheckable(true);
    useGPU->setShortcut(tr("Ctrl+G"));
    useGPU->setStatusTip(tr("Use GPU for rendering"));
    auto *deviceGroup = new QActionGroup(this);
    deviceGroup->addAction(useCpuAction);
    deviceGroup->addAction(useGPU);
    useCpuAction->setChecked(true);
    optionMenu->addActions(deviceGroup->actions());
    devList = deviceGroup->actions();
    OpenCLHandler *myCLHandler;
    try { myCLHandler = new OpenCLHandler(); }
    catch(...){ myCLHandler = NULL; }
    if(myCLHandler == NULL ){
        useGPU->setDisabled(true);
    } else {
        useCpuAction->setChecked(false);
        this->useCPU = false;
        useGPU->setChecked(true);
    }

    // About Message Box
    auto *aboutMessage = new QAction(tr("&About"), this);
    menuBar()->addAction(aboutMessage);

    juliaDialog = new JuliaInputDialog(fraktalRenderArea);

    connect(fractalList, SIGNAL(triggered(QAction*)),
            this,        SLOT(changeFractal(QAction*)) );
    connect(deviceGroup, SIGNAL(triggered(QAction*)),
            this,        SLOT(changeDevice(QAction*))  );
    connect(aboutMessage, SIGNAL(triggered()),
            this,         SLOT(showAboutBox()) );
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
    if((useCPU && deviceNum == 0) ||
       (!useCPU && deviceNum == 1) ||
       (deviceNum > 1) )
        return;
    useCPU = deviceNum == 0;
    fraktalRenderArea->setRenderDevice(useCPU);
}


void MainWindow::showAboutBox(){
    QMessageBox::information(this, tr("About"),
                             tr("This program uses the Qt framework and OpenCL.\n Licensed under GPLv3. http://www.gnu.org/licenses/gpl-3.0\nAuthor(s): Etienne Gramlich"));
}

void MainWindow::closeEvent(QCloseEvent*)
{
    qApp->quit();
}
