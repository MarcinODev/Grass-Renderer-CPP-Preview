#include "transform.h"
#include "renderer.h"



Renderer::Renderer(GameObject* owner) : Component(owner)
{
}


Renderer::~Renderer()
{
}

const QMatrix4x4& Renderer::getModelMatrix()
{
	auto transform = gameObject->getComponent<Transform>();
	if (transform.expired())
	{
		return identityMatrix;
	}

	return transform.lock()->getTransformMatrix();
}
