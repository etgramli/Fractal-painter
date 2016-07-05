/* Header of the dialog for getting the complex number for the julia-set
 * from the user.
 * Copyright (C) 2014  Etienne Gramlich
 * See main.cpp for more information.
 */
#ifndef JULIAINPUTDIALOG_H
#define JULIAINPUTDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDoubleSpinBox>
#include <QLabel>

#include "ui/renderarea.h"

class JuliaInputDialog : public QDialog {
    Q_OBJECT
public:
    JuliaInputDialog(RenderArea *parent);

    QDoubleSpinBox *imagBox, *realBox;

private:
    RenderArea *parent;
    QVBoxLayout *layout;

};

#endif // JULIAINPUTDIALOG_H
