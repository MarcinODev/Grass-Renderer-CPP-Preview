#include <QtWidgets/qdockwidget.h>
#include <QPushButton>
#include <QSlider>
#include <QObject>
#include <QPalette>
#include "previewProperties.h"

PreviewProperties::PreviewProperties(QDockWidget * propertiesDock, Preview* preview) : preview(preview)
{
	colorDialog = std::make_unique<QColorDialog>(propertiesDock);
	colorDialog->hide();
	QObject::connect(colorDialog.get(), &QColorDialog::currentColorChanged, [&](const QColor& color){onColorSelectedInDialog(color);});

	grassColorPicker = propertiesDock->findChild<QPushButton*>("grassColorPicker");
	QSlider* grassDensitySlider = propertiesDock->findChild<QSlider*>("grassDensitySlider");
	lightColorPicker = propertiesDock->findChild<QPushButton*>("lightColorPicker");
	QSlider* windPowerSlider = propertiesDock->findChild<QSlider*>("windPowerSlider");
	QSlider* windFreqSlider = propertiesDock->findChild<QSlider*>("windFreqSlider");
	
	setColorForButton(grassColorPicker, preview->getGrassColor());
	setColorForButton(lightColorPicker, preview->getLightColor());

	QObject::connect(grassColorPicker, &QPushButton::released, [&](){onGrassColorPickClick();});
	QObject::connect(lightColorPicker, &QPushButton::released, [&](){onLightColorPickClick();});
	
	QObject::connect(grassDensitySlider, &QSlider::valueChanged, [&](int val){onGrassDensityChanged(val);});
	QObject::connect(windPowerSlider, &QSlider::valueChanged, [&](int val){onWindPowerChanged(val);});
	QObject::connect(windFreqSlider, &QSlider::valueChanged, [&](int val){onWindFrequencyChanged(val);});
}

PreviewProperties::~PreviewProperties()
{
}


void PreviewProperties::setColorForButton(QPushButton * button, const QColor & color)
{
	button->setStyleSheet(QString("background-color: rgb(%1, %2, %3); color: rgb(255, 255, 255)").arg(color.red()).arg(color.green()).arg(color.blue()));
}

void PreviewProperties::onGrassColorPickClick()
{
	colorSelectionMode = ColorSelectionMode::Grass;
	QColor color = preview->getGrassColor();
	colorDialog->show();
	colorDialog->setCurrentColor(color);
}

void PreviewProperties::onLightColorPickClick()
{
	colorSelectionMode = ColorSelectionMode::Light;
	QColor color = preview->getLightColor();
	colorDialog->show();
	colorDialog->setCurrentColor(color);
}

void PreviewProperties::onGrassDensityChanged(int value)
{
	preview->setGrassDensity(value * 0.1f);
}

void PreviewProperties::onWindPowerChanged(int value)
{
	preview->setWindPower(value);
}

void PreviewProperties::onWindFrequencyChanged(int value)
{
	preview->setWindFrequency(value * 0.1f);
}

void PreviewProperties::onColorSelectedInDialog(const QColor& color)
{
	switch(colorSelectionMode)
	{
		case ColorSelectionMode::Grass:
			preview->setGrassColor(color);
			setColorForButton(grassColorPicker, color);
			break;
		case ColorSelectionMode::Light:
			preview->setLightColor(color);
			setColorForButton(lightColorPicker, color);
			break;
	}
}
