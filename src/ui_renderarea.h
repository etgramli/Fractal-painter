/********************************************************************************
** Form generated from reading UI file 'renderarea.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RENDERAREA_H
#define UI_RENDERAREA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RenderArea
{
public:

    void setupUi(QWidget *RenderArea)
    {
        if (RenderArea->objectName().isEmpty())
            RenderArea->setObjectName("RenderArea");
        RenderArea->resize(240, 320);

        retranslateUi(RenderArea);

        QMetaObject::connectSlotsByName(RenderArea);
    } // setupUi

    void retranslateUi(QWidget *RenderArea)
    {
        RenderArea->setWindowTitle(QApplication::translate("RenderArea", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class RenderArea: public Ui_RenderArea {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENDERAREA_H
