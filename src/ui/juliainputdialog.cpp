/* Implemtation of the dialog for getting the complex number for the julia-set
 * from the user.
 * Copyright (C) 2014  Etienne Gramlich
 * See main.cpp for more information.
 */
#include "juliainputdialog.h"

JuliaInputDialog::JuliaInputDialog(RenderArea *parent) {
    this->parent = parent;
    // Create Objects
    layout = new QVBoxLayout(this);
    QDoubleSpinBox *kRealBox = new QDoubleSpinBox();
    realBox = kRealBox;
    realBox->setDecimals(3);
    realBox->setRange(-2.0, 2.0);
    realBox->setSingleStep(0.01);
    realBox->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);

    QDoubleSpinBox *kImagBox = new QDoubleSpinBox();
    imagBox = kImagBox;
    imagBox->setDecimals(3);
    imagBox->setRange(-2.0, 2.0);
    imagBox->setSingleStep(0.01);
    imagBox->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);

    QLabel *realLabel = new QLabel("Realteil von C [-2 ; 2]");
    QLabel *imagLabel = new QLabel("Imagin(ae)rteil von C [-2 ; 2]");

    // Create Layout
    layout->addWidget(realLabel);
    layout->addWidget(realBox);
    layout->addWidget(imagLabel);
    layout->addWidget(imagBox);

    // Connect
    connect(realBox, SIGNAL(valueChanged(double)),
            parent, SLOT(setJuliaCreal(double)) );
    connect(imagBox, SIGNAL(valueChanged(double)),
            parent, SLOT(setJuliaCimag(double)) );
}
