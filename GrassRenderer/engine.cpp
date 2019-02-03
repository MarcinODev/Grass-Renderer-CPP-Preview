#include <functional>
#include <qdatetime.h>
#include <typeindex>
#include <typeinfo>
#include "engine.h"
#include "camera.h"
#include "renderer.h"
#include "logger.h"


Engine::Engine(QWidget* parentOfRenderer)
{
	rendererWidget = std::make_shared<D3DRendererWidget>(parentOfRenderer);

	using namespace std::placeholders;
	rendererWidget->setOnResizeHandler(std::bind(&Engine::onResize, this, _1, _2));
	rendererWidget->setOnPaintHandler(std::bind(&Engine::onPaint, this));
	rendererWidget->setOnMouseClickHandler(std::bind(&Engine::onMouseClick, this, _1, _2));
	rendererWidget->setOnMouseMoveHandler(std::bind(&Engine::onMouseMove, this, _1));
	rendererWidget->setOnCloseHandler(std::bind(&Engine::onWindowClose, this));

	taskTimer = std::make_shared<TaskTimer>(this);
}

Engine::~Engine()
{
}

void Engine::play(std::function<void(void)> onEveryUpdate)
{
	startQTime = QTime::currentTime();
	float lastTime = 0.0f;
	isRunning = true;
	while (isRunning)
	{
		if (onEveryUpdate)
		{
			onEveryUpdate();
		}

		float time = startQTime.msecsTo(QTime::currentTime()) * 0.001f;
		lastDeltaTime = time - lastTime;
		lastTime = time;

		updateComponents();
		updateRendering();
	}
}

std::shared_ptr<D3DRendererWidget> Engine::getRendererWidget()
{
	return rendererWidget;
}

std::weak_ptr<TaskTimer> Engine::getTaskTimer()
{
	return taskTimer;
}

std::weak_ptr<GameObject> Engine::addGameObject(std::string name)
{
	std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(this, name);
	gameObjects.push_back(gameObject);
	return gameObject;
}

float Engine::getLastDeltaTime()
{
	return lastDeltaTime;
}

float Engine::getTime()
{
	return startQTime.msecsTo(QTime::currentTime()) * 0.001f;
}

HWND Engine::getWindowID()
{
	return (HWND)rendererWidget->winId();
}

void Engine::updateComponents()
{
	taskTimer->update();
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->update();
	}
}

void Engine::updateRendering()
{
	rendererWidget->setRenderTargetForRendering();
	size_t camType = typeid(Camera).hash_code();
	auto& cameras = components[camType];
	for (int i = 0; i < cameras.size(); i++)
	{
		std::dynamic_pointer_cast<Camera>(cameras[i].lock())->render(renderers);
	}
	rendererWidget->finishRendering();

}

void Engine::onResize(int width, int height)
{
	//TODO react on widget resize, refresh buffers etc.
}

void Engine::onPaint()
{
}

void Engine::onMouseClick(QMouseEvent * evt, bool press)
{
}

void Engine::onMouseMove(QMouseEvent * evt)
{
}

void Engine::onWindowClose()
{
	isRunning = false;
	LogDebug("Closing engine...");
}

