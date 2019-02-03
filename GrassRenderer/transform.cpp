#include "transform.h"



Transform::Transform(GameObject* owner) : Component(owner)
{
	setScale(QVector3D(1, 1, 1));
}


Transform::~Transform()
{
}

const QVector3D& Transform::getPosition() const
{
	return position;
}

const QVector3D& Transform::getScale() const
{
	return scale;
}

const QQuaternion& Transform::getRotation() const
{
	return rotation;
}

const QMatrix4x4& Transform::getTransformMatrix()
{
	if (isDirty)
	{
		transformMatrix.setToIdentity();
		transformMatrix.scale(scale);
		transformMatrix.rotate(rotation);
		transformMatrix.translate(position);
	}

	return transformMatrix;
}


void Transform::setPosition(const QVector3D& val)
{
	position = val;
	isDirty = true;
}

void Transform::setScale(const QVector3D& val)
{
	scale = val;
	isDirty = true;
}

void Transform::setRotation(const QQuaternion& val)
{
	rotation = val;
	isDirty = true;
}

void Transform::rotate(const QVector3D& eulerAngles)
{
	QVector3D rot = rotation.toEulerAngles();
	rot += eulerAngles;
	setRotation(QQuaternion::fromEulerAngles(rot));
}