/********************************************************************************
** Form generated from reading UI file 'grassRenderer.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRASSRENDERER_H
#define UI_GRASSRENDERER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GrassRendererClass
{
public:
    QWidget *centralWidget;
    QDockWidget *propertiesDock;
    QWidget *dockWidgetContents;
    QWidget *layoutWidget;
    QFormLayout *layout;
    QLabel *label;
    QLabel *label_2;
    QPushButton *grassColorPicker;
    QSlider *grassDensitySlider;
    QLabel *label_3;
    QSlider *windPowerSlider;
    QLabel *label_4;
    QPushButton *lightColorPicker;
    QLabel *label_5;
    QSlider *windFreqSlider;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GrassRendererClass)
    {
        if (GrassRendererClass->objectName().isEmpty())
            GrassRendererClass->setObjectName(QString::fromUtf8("GrassRendererClass"));
        GrassRendererClass->resize(1280, 1024);
        centralWidget = new QWidget(GrassRendererClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        propertiesDock = new QDockWidget(centralWidget);
        propertiesDock->setObjectName(QString::fromUtf8("propertiesDock"));
        propertiesDock->setGeometry(QRect(980, 10, 291, 171));
        propertiesDock->setMaximumSize(QSize(500, 500));
        propertiesDock->setFloating(true);
        propertiesDock->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable);
        propertiesDock->setAllowedAreas(Qt::NoDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        layoutWidget = new QWidget(dockWidgetContents);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(2, 3, 281, 138));
        layout = new QFormLayout(layoutWidget);
        layout->setSpacing(6);
        layout->setContentsMargins(11, 11, 11, 11);
        layout->setObjectName(QString::fromUtf8("layout"));
        layout->setHorizontalSpacing(20);
        layout->setContentsMargins(1, 1, 1, 1);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        layout->setWidget(0, QFormLayout::LabelRole, label);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        layout->setWidget(1, QFormLayout::LabelRole, label_2);

        grassColorPicker = new QPushButton(layoutWidget);
        grassColorPicker->setObjectName(QString::fromUtf8("grassColorPicker"));
        grassColorPicker->setAutoFillBackground(false);

        layout->setWidget(0, QFormLayout::FieldRole, grassColorPicker);

        grassDensitySlider = new QSlider(layoutWidget);
        grassDensitySlider->setObjectName(QString::fromUtf8("grassDensitySlider"));
        grassDensitySlider->setMaximum(800);
        grassDensitySlider->setValue(800);
        grassDensitySlider->setOrientation(Qt::Horizontal);

        layout->setWidget(1, QFormLayout::FieldRole, grassDensitySlider);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        layout->setWidget(2, QFormLayout::LabelRole, label_3);

        windPowerSlider = new QSlider(layoutWidget);
        windPowerSlider->setObjectName(QString::fromUtf8("windPowerSlider"));
        windPowerSlider->setMaximum(10);
        windPowerSlider->setPageStep(1);
        windPowerSlider->setValue(2);
        windPowerSlider->setOrientation(Qt::Horizontal);

        layout->setWidget(2, QFormLayout::FieldRole, windPowerSlider);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        layout->setWidget(4, QFormLayout::LabelRole, label_4);

        lightColorPicker = new QPushButton(layoutWidget);
        lightColorPicker->setObjectName(QString::fromUtf8("lightColorPicker"));
        lightColorPicker->setAutoFillBackground(false);

        layout->setWidget(4, QFormLayout::FieldRole, lightColorPicker);

        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        layout->setWidget(3, QFormLayout::LabelRole, label_5);

        windFreqSlider = new QSlider(layoutWidget);
        windFreqSlider->setObjectName(QString::fromUtf8("windFreqSlider"));
        windFreqSlider->setMaximum(30);
        windFreqSlider->setValue(15);
        windFreqSlider->setOrientation(Qt::Horizontal);

        layout->setWidget(3, QFormLayout::FieldRole, windFreqSlider);

        propertiesDock->setWidget(dockWidgetContents);
        GrassRendererClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(GrassRendererClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1280, 21));
        GrassRendererClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(GrassRendererClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        GrassRendererClass->setStatusBar(statusBar);

        retranslateUi(GrassRendererClass);

        QMetaObject::connectSlotsByName(GrassRendererClass);
    } // setupUi

    void retranslateUi(QMainWindow *GrassRendererClass)
    {
        GrassRendererClass->setWindowTitle(QApplication::translate("GrassRendererClass", "GrassRenderer", nullptr));
        propertiesDock->setWindowTitle(QString());
        label->setText(QApplication::translate("GrassRendererClass", "Grass color", nullptr));
        label_2->setText(QApplication::translate("GrassRendererClass", "Grass density", nullptr));
        grassColorPicker->setText(QString());
        label_3->setText(QApplication::translate("GrassRendererClass", "Wind power", nullptr));
        label_4->setText(QApplication::translate("GrassRendererClass", "Light color", nullptr));
        lightColorPicker->setText(QString());
        label_5->setText(QApplication::translate("GrassRendererClass", "Wind frequency", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GrassRendererClass: public Ui_GrassRendererClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRASSRENDERER_H
