#pragma once
#include "gameObject.h"

class GameObject;
class Engine;

/// <summary>
/// Components can be attached to GameObjects to be part of it.
/// If Component isUpdatable() than Engine>>GameObject::update() will call it's update() method.
/// </summary>
class Component
{
	friend class GameObject;

public:
	virtual ~Component();

	/// <summary>
	/// If override returns true, than update() will be called every frame
	/// </summary>
	virtual bool isUpdatable() const {return false;} 
	
protected:
	GameObject* gameObject;
	Engine* engine;
	
	Component(GameObject* owner);
	virtual void update() {}//called only if isUpdatable == true;
};

