#pragma once
#include <QMatrix4x4>
#include <QVector3D>
#include "gameObject.h"
#include "component.h"
#include "constantBuffer.h"

/// <summary>
/// Base Renderer class. Should be inherited to make Component be called in Camera render pipeline
/// </summary>
class Renderer : public Component
{
public:
	Renderer(GameObject* owner);
	virtual ~Renderer();

	/// <summary>
	/// Calls render meshes or other objects contained by inheriting class
	/// </summary>
	/// <param name="matrixConstantBuffer"></param>
	virtual void render(std::shared_ptr<ConstantBuffer> matrixConstantBuffer) = 0;

	const QMatrix4x4& getModelMatrix();

protected:
	const QMatrix4x4 identityMatrix;
};

