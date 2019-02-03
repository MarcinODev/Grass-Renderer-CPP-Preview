#include <QVector3D>
#include <qtimer.h>
#include <functional>
#include "preview.h"
#include "camera.h"
#include "transform.h"
#include "meshRenderer.h"

Preview::Preview(Engine* engine)
{
	this->engine = engine;
	initCamera();
	initGrass();
}

Preview::~Preview()
{
}

QColor Preview::getGrassColor() const
{
	QColor color = QColor(grassColorVec.x() * 255, grassColorVec.y() * 255, grassColorVec.z() * 255);
	return color;
}

QColor Preview::getLightColor() const
{
	QColor color = QColor(lightColorVec.x() * 255, lightColorVec.y() * 255, lightColorVec.z() * 255);
	return color;
}

float Preview::getGrassDensity() const
{
	return grassDensity;
}

float Preview::getWindPower() const
{
	return windParams.w();
}

float Preview::getWindFrequency() const
{
	return windParams.z();
}

void Preview::setGrassColor(const QColor& color)
{
	grassColorVec.setX(color.red() / 255.0f);
	grassColorVec.setY(color.green() / 255.0f);
	grassColorVec.setZ(color.blue() / 255.0f);
	grassMaterial->setVector("_GrassColor", grassColorVec);
}

void Preview::setLightColor(const QColor& color)
{
	lightColorVec.setX(color.red() / 255.0f);
	lightColorVec.setY(color.green() / 255.0f);
	lightColorVec.setZ(color.blue() / 255.0f);
	//TODO rethink of import alpha too
	grassMaterial->setVector("_LightColor", lightColorVec);
}

void Preview::setGrassDensity(float grassDensity)
{
	this->grassDensity = grassDensity;
	grassMaterial->setFloat("_TessInside", grassDensity);
	grassMaterial->setFloat("_TessOutside", grassDensity);
}

void Preview::setWindPower(float windPower)
{
	windParams.setW(windPower);
	grassMaterial->setVector("_WindParams", windParams);
}

void Preview::setWindFrequency(float windFrequency)
{
	windParams.setZ(windFrequency);
	grassMaterial->setVector("_WindParams", windParams);
}

void Preview::initCamera()
{
	auto camGo = engine->addGameObject("Camera").lock();
	camera = camGo->addComponent<Camera>();
	auto cam = camera.lock();
	//cam->lookAt(QVector3D(0, grassSize * 0.5f, grassSize * -0.25f), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
	float aspect = (float)engine->getRendererWidget()->width() / (float)engine->getRendererWidget()->height();
	cam->setFrustrum(60.0f, aspect, 0.1f, 1000);
	cam->setClearColor(QColor(0,0,0,255));
}

void Preview::initGrass()
{
	auto grassGo = engine->addGameObject("Grass").lock();
	grassTrans = grassGo->addComponent<Transform>();

	auto renderer = grassGo->addComponent<MeshRenderer>().lock();
	initGrassMaterial();	
	auto mesh = createGrassMesh();
	renderer->setMaterial(grassMaterial);
	renderer->setMesh(mesh);

	onGrassUpdate();
}

void Preview::initGrassMaterial()
{
	auto device = engine->getRendererWidget()->getDevice();
	auto deviceContext = engine->getRendererWidget()->getDeviceContext();

	auto shader = Shader::findOrCreate(device, deviceContext, "grass");

	grassMaterial = std::make_shared<Material>(device, deviceContext, shader);

	grassMaterial->setFloat("_TessInside", grassDensity);
	grassMaterial->setFloat("_TessOutside", grassDensity);
	grassMaterial->setFloat("_GrassHeight", grassHeight);
	grassMaterial->setFloat("_GrassWidth", grassWidth);
	grassMaterial->setFloat("_GrassShrinkLinear", grassShrinkLinear);
	grassMaterial->setFloat("_GrassShrinkPow", grassShrinkPow);
	grassMaterial->setVector("_GrassBendParams", grassBendParams);
	grassMaterial->setVector("_WindParams", windParams);
	grassMaterial->setVector("_WindDir", QVector4D(1,-0.5,0,0).normalized());
	grassMaterial->setFloat("_GrassDarknessPow", 1.5f);
	grassMaterial->setVector("_GrassColor", grassColorVec);
	grassMaterial->setVector("_LightColor", lightColorVec);
	grassMaterial->setVector("_LightDir", QVector4D(-1,-0.1,-1,0).normalized());
}

std::shared_ptr<Mesh> Preview::createGrassMesh()
{
	auto device = engine->getRendererWidget()->getDevice();
	auto deviceContext = engine->getRendererWidget()->getDeviceContext();

	auto mesh = std::make_shared<Mesh>(device, deviceContext);

	std::vector<QVector3D> vertices;
	std::vector<unsigned int> indices;
	int quadsCount = 2;
	for(int i = 0; i < quadsCount; i++)
	{
		float offset = 0.1f * i;
		vertices.push_back(QVector3D(-grassSize + offset, 0, -grassSize + offset));
		vertices.push_back(QVector3D(-grassSize + offset, 0, grassSize + offset));
		vertices.push_back(QVector3D(grassSize + offset, 0, grassSize + offset));
		vertices.push_back(QVector3D(grassSize + offset, 0, -grassSize + offset));
				
		int indexOffset = i * 4;
		indices.push_back(indexOffset);
		indices.push_back(1 + indexOffset);
		indices.push_back(2 + indexOffset);
		indices.push_back(indexOffset);
		indices.push_back(2 + indexOffset);
		indices.push_back(3 + indexOffset);
	}

	mesh->setVertices(vertices);
	mesh->setIndices(indices);
	mesh->reload();
	return mesh;
}

void Preview::onGrassUpdate()
{
	if (grassTrans.expired())
	{
		return;
	}
	
	grassMaterial->setFloat("_Time", engine->getTime());

	QVector3D cameraPos;
	cameraPos.setY(50.0f);
	cameraPos.setX(sin(engine->getTime() * rotationOverTime) * 25.0f);
	cameraPos.setZ(-cos(engine->getTime() * rotationOverTime) * 25.0f);
	camera.lock()->lookAt(cameraPos, QVector3D(0, 0, 0), QVector3D(0, 1, 0));
	grassMaterial->setVector("_CameraPos", cameraPos.toVector4D());

	engine->getTaskTimer().lock()->queueTask(0, std::bind(&Preview::onGrassUpdate, this));
}


