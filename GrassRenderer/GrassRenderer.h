#pragma once
#include <QCloseEvent>
#include <QtWidgets/QMainWindow>
#include "ui_GrassRenderer.h"

/// <summary>
/// Just QT window in which Engine renders
/// </summary>
class GrassRenderer : public QMainWindow
{
	Q_OBJECT

public:
	GrassRenderer(QWidget *parent = Q_NULLPTR);


private:
	Ui::GrassRendererClass ui;
	void closeEvent(QCloseEvent *event);
};
