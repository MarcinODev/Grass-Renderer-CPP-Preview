#pragma once
#include "engine.h"
#include "transform.h"
#include "camera.h"
#include "material.h"
#include "mesh.h"

/// <summary>
/// Project preview class, just setups scene (light, camera, grass)
/// </summary>
class Preview
{
public:
	Preview(Engine* engine);
	~Preview();

	QColor getGrassColor() const;
	QColor getLightColor() const;
	float getGrassDensity() const;
	float getWindPower() const;
	float getWindFrequency() const;
	void setGrassColor(const QColor& color);
	void setLightColor(const QColor& color);
	void setGrassDensity(float grassDensity);
	void setWindPower(float windPower);
	void setWindFrequency(float windFrequency);


private:
	const float rotationOverTime = 0.10f;
	const float grassSize = 70.0f; 
	const float grassHeight = 9.0f;
	const float grassWidth = 0.4f;
	const float grassShrinkLinear = 0.5f;
	const float grassShrinkPow = 6.5f;
	float grassDensity = 1000.0f;
	QVector4D lightColorVec = QVector4D(1,1,1,1.5);//rgb, a - fake ao intensity
	QVector4D grassColorVec = QVector4D(0.13, 0.56, 0.13, 0.5);//rgb, a - variation difference
	QVector4D grassBendParams = QVector4D(10, 10, 0.45, 1);//xy - randomisation over [x,z], z - bendLinear, w - bendPow
	QVector4D windParams = QVector4D(0.1, 0.01, 1.5, 2);//xy - direction of waves (unnormalized as it is used also for waves length), z - frequency, w - wind max power

	Engine * engine;
	std::weak_ptr<Camera> camera;
	std::weak_ptr<Transform> grassTrans;
	std::shared_ptr<Material> grassMaterial;
	
	void initCamera();
	void initGrass();
	void initGrassMaterial();
	std::shared_ptr<Mesh> createGrassMesh();
	void onGrassUpdate();
};

