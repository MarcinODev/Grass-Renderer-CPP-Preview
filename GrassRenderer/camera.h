#pragma once
#include <QVector3D>
#include <QMatrix4x4>
#include <QColor>
#include <d3d11.h>
#include <DirectXMath.h>
#include "gameObject.h"
#include "component.h"
#include "renderer.h"
#include "constantBuffer.h"

/// <summary>
/// Camera rendering component, attach it into GameObject to add new rendering layer.
/// </summary>
class Camera : public Component
{
public:
	Camera(GameObject* owner);
	virtual ~Camera();

	/// <summary>
	/// Looks with Camera at certain pos and setups Camera's positioning
	/// </summary>
	/// <param name="pos">Camera position</param>
	/// <param name="targetPos">Look at position</param>
	/// <param name="up">Up vector</param>
	void lookAt(const QVector3D& pos, const QVector3D& targetPos, const QVector3D& up);

	/// <summary>
	/// Setups camera view range
	/// </summary>
	/// <param name="angle">Camera view angle</param>
	/// <param name="aspect">Aspect ratio</param>
	/// <param name="zNear">Min dist from camera</param>
	/// <param name="zFar">Max dist from camera</param>
	void setFrustrum(float angle, float aspect, float zNear, float zFar);

	/// <summary>
	/// Renders all renderers passed to camera
	/// </summary>
	void render(const std::vector<std::weak_ptr<Renderer>>& renderers);

	/// <summary>
	/// Sets background color
	/// </summary>
	void setClearColor(const QColor& color);

	const QMatrix4x4& getViewProjMatrix() const;
	const D3D11_VIEWPORT& getViewport() const;
	const QVector3D& getPosition() const;
	
protected:
	float clearColor[4];
	ID3D11DeviceContext* deviceContext;
	QMatrix4x4 viewMatrix;
	QMatrix4x4 projectionMatrix;
	QMatrix4x4 viewProjMatrix;
	QVector3D position;
	D3D11_VIEWPORT viewport;
	std::shared_ptr<ConstantBuffer> constantMatrixBuffer;
	ID3D11RenderTargetView* renderTarget;
	ID3D11DepthStencilView* depthStencilTarget;

	void initializeViewport();
	void startupRender();
	void finishRender();
	void refreshViewProjectionMatrix();
};

