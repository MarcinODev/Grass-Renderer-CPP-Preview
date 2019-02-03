#pragma once
#include <QObject>
#include <vector>
#include <string>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include "component.h"

class Component;

/// <summary>
/// Entity which contains and updates Components
/// </summary>
class GameObject
{
	friend class Engine;

public:
	GameObject(Engine* engine, std::string name);
	virtual ~GameObject();

	Engine* getEngine();
	const std::string& getName();

	/// <summary>
	/// Creates and adds Component. T must inherit from Component
	/// </summary>
	template<typename T>
	std::weak_ptr<T> addComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
		std::shared_ptr<T> component = std::make_shared<T>(this);
		components[typeid(T).hash_code()] = component;
		if(component->isUpdatable())
		{
			componentsToUpdate.push_back(component);
		}
		std::shared_ptr<Component> trueComponent = std::dynamic_pointer_cast<Component>(component);
		engine->registerComponent<T>(trueComponent);
		return component;
	}
	
	/// <summary>
	/// Gets component. T must inherit from Component class
	/// </summary>
	template<typename T>
	std::weak_ptr<T> getComponent()
	{
		auto find = components.find(typeid(T).hash_code());
		if (find == components.end())
		{
			return std::weak_ptr<T>();
		}
		else
		{
			return std::dynamic_pointer_cast<T>(find->second);
		}
	}
	
private:
	std::unordered_map< size_t, std::shared_ptr<Component> > components;
	std::vector<std::shared_ptr<Component>> componentsToUpdate;
	Engine* engine;
	std::string name;

	void update();
};

