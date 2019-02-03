#pragma once
#include <QMatrix4x4>
#include <QVector3D>
#include <qquaternion.h>
#include "gameObject.h"
#include "component.h"

/// <summary>
/// Component for managing objet positions, scales, rotations
/// </summary>
class Transform : public Component
{
public:
	Transform(GameObject* owner);
	virtual ~Transform();
	
	const QVector3D& getPosition() const;
	const QVector3D& getScale() const;
	const QQuaternion& getRotation() const;
	const QMatrix4x4& getTransformMatrix();

	void setPosition(const QVector3D& val);
	void setScale(const QVector3D& val);
	void setRotation(const QQuaternion& val);
	void rotate(const QVector3D& eulerAngles);

protected:
	QMatrix4x4 transformMatrix;
	QVector3D position;
	QVector3D scale;
	QQuaternion rotation;
	bool isDirty;//if true than transform matrix needs to be updated

};

