#pragma once
#include <QtWidgets/qdockwidget.h>
#include <QPushButton>
#include <QObject>
#include <QColorDialog>
#include <QColor>
#include <memory>
#include "preview.h"

/// <summary>
/// Class for QT window manimulating properies in preview scene
/// </summary>
class PreviewProperties
{
public:
	PreviewProperties(QDockWidget * propertiesDock, Preview* preview);
	~PreviewProperties();

private:
	enum ColorSelectionMode {Grass, Light};

	Preview* preview;
	QPushButton* grassColorPicker;
	QPushButton* lightColorPicker;
	std::unique_ptr<QColorDialog> colorDialog;
	ColorSelectionMode colorSelectionMode;

	void setColorForButton(QPushButton* button, const QColor& color);
	void onGrassColorPickClick();
	void onLightColorPickClick();
	void onGrassDensityChanged(int value);
	void onWindPowerChanged(int value);
	void onWindFrequencyChanged(int value);
	void onColorSelectedInDialog(const QColor& color);
};

