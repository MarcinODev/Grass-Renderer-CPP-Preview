#pragma once
#include <QWidget>
#include <QMouseEvent>
#include <qdatetime.h>
#include <string>
#include <vector>
#include <typeindex>
#include <typeinfo>
#include <memory>
#include <unordered_map>
#include "d3dRendererWidget.h"
#include "gameObject.h"
#include "renderer.h"
#include "taskTimer.h"

class TaskTimer;

/// <summary>
/// Engine root class. Creates D3DRendererWidget and manages game objects and input updates.
/// </summary>
class Engine
{
	friend class GameObject;

public:
	Engine(QWidget* parentOfRenderer);
	virtual ~Engine();
	
	/// <summary>
	/// Starts and plays game loop
	/// </summary>
	/// <param name="onEveryUpdate">Event on every update. Usefull for assigning QT events update call</param>
	void play(std::function<void(void)> onEveryUpdate);
	
	std::weak_ptr<TaskTimer> getTaskTimer();
	/// <summary>
	/// Creates enoty GameObject and adds it to objects list.
	/// </summary>
	/// <param name="name">GameObject name</param>
	std::weak_ptr<GameObject> addGameObject(std::string name);
	
	std::shared_ptr<D3DRendererWidget> getRendererWidget();
	float getLastDeltaTime();
	float getTime();
	HWND getWindowID();

private:
	bool isRunning;
	QTime startQTime;
	float lastDeltaTime;
	std::shared_ptr<TaskTimer> taskTimer;
	std::shared_ptr<D3DRendererWidget> rendererWidget;
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	std::vector<std::weak_ptr<Renderer>> renderers;
	std::unordered_map< size_t, std::vector< std::weak_ptr<Component> > > components;

	void updateComponents();
	void updateRendering();
	void onResize(int width, int height);
	void onPaint();
	void onMouseClick(QMouseEvent* evt, bool press);
	void onMouseMove(QMouseEvent* evt);
	void onWindowClose();
	
	template<typename T>
	void registerComponent(std::shared_ptr<Component> component)
	{
		size_t componentType = typeid(T).hash_code();
		components[componentType].push_back(component);
		auto renderer = std::dynamic_pointer_cast<Renderer>(component);
		if (renderer != nullptr)
		{
			renderers.push_back(renderer);
		}
	}
};

