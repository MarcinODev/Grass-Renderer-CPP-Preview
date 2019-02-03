#include <memory>
#include "gameObject.h"



GameObject::GameObject(Engine* engine, std::string name) : engine(engine), name(name)
{
}


GameObject::~GameObject()
{
}

Engine* GameObject::getEngine()
{
	return engine;
}

const std::string & GameObject::getName()
{
	return name;
}

void GameObject::update()
{
	for (int i = 0; i < componentsToUpdate.size(); i++)
	{
		componentsToUpdate[i]->update();
	}
}
