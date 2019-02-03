#include "component.h"



Component::Component(GameObject* owner)
{
	gameObject = owner;
	engine = gameObject->getEngine();
}

Component::~Component()
{
}
