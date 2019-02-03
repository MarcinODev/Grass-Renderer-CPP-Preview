#include <QtWidgets/QApplication>
#include <QtWidgets/qdockwidget.h>
#include <functional>
#include "grassRenderer.h"
#include "engine.h"
#include "main.h"
#include "preview.h"
#include "previewProperties.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	GrassRenderer widget;
	app.processEvents();

	Engine engine(&widget);
	QDockWidget* propertiesDock = widget.findChild<QDockWidget*>("propertiesDock");
	widget.setCentralWidget(engine.getRendererWidget().get());
	widget.show();

	Preview preview(&engine);
	PreviewProperties properties(propertiesDock, &preview);
	
	std::function<void(void)> processEvents = [&](){app.processEvents();};
	engine.play(processEvents);

	app.exit();
	return 0;
}
