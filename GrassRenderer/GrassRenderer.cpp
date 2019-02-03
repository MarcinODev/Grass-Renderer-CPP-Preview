#include <qevent.h>
#include <QCloseEvent>
#include <cstdlib>
#include <memory>
#include "grassRenderer.h"
#include "d3dRendererWidget.h"

GrassRenderer::GrassRenderer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	showMaximized();
}

void GrassRenderer::closeEvent(QCloseEvent * event)
{
	centralWidget()->close();
	QMainWindow::closeEvent(event);
}
