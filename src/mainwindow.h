/* Header of the main window written with Qt 4.
 * Copyright (C) 2014  Etienne Gramlich
 * See main.cpp for more information.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "renderarea.h"
#include "juliainputdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void saveImage();
    void renderImage();
    void changeFractal(QAction *action);
    void changeDevice(QAction *action);
    //void changeColorMode(QAction *action);

protected:
void  closeEvent(QCloseEvent*);

private:
    Ui::MainWindow *ui;
    RenderArea *fraktalRenderArea;
    JuliaInputDialog *juliaDialog;

    QList<QAction *> fracList;
    int numFract;
    QList<QAction *> devList;
    bool useCPU;

    QList<QAction *> colorList;
    int numColorMode;

};

#endif // MAINWINDOW_H
