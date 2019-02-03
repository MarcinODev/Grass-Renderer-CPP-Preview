
#include "camera.h"
#include "gameObject.h"
#include "component.h"
#include "engine.h"

struct MatrixConstantBufferData
{
	float modelViewProjectionMatrix[16];
	float modelMatrix[16];
	float modelToMVPMatrix[16];
};

Camera::Camera(GameObject* owner) : Component(owner)
{
	auto device = engine->getRendererWidget()->getDevice();
	deviceContext = engine->getRendererWidget()->getDeviceContext();
	renderTarget = engine->getRendererWidget()->getRenderTarget();
	depthStencilTarget = engine->getRendererWidget()->getDepthStencilTarget();
	constantMatrixBuffer = std::make_shared<ConstantBuffer>(sizeof(MatrixConstantBufferData), device, deviceContext);
	initializeViewport();
}

Camera::~Camera()
{
}

void Camera::lookAt(const QVector3D& pos, const QVector3D& targetPos, const QVector3D& up)
{
	position = pos;
	viewMatrix.setToIdentity();
	viewMatrix.lookAt(pos, targetPos, up);
	refreshViewProjectionMatrix();
}

void Camera::setFrustrum(float angle, float aspect, float zNear, float zFar)
{
	projectionMatrix.setToIdentity();
	projectionMatrix.perspective(angle, aspect, zNear, zFar);
	refreshViewProjectionMatrix();
}

void Camera::render(const std::vector<std::weak_ptr<Renderer>>& renderers)
{
	startupRender();
	
	//TODO add sorting and culling
	for(int i = 0; i < renderers.size(); i++)
	{
		auto renderer = renderers[i].lock();
		if(renderer)
		{
			QMatrix4x4 modelMatrix = renderer->getModelMatrix();
			QMatrix4x4 mvpMatrix = viewProjMatrix * modelMatrix;
			QMatrix4x4 invertedModelMatrix = modelMatrix.inverted();//TODO compute only if shader uses
			QMatrix4x4 modelToMVPMatrix = mvpMatrix * invertedModelMatrix;//TODO compute only if shader uses
			MatrixConstantBufferData* matrixBuffer = (MatrixConstantBufferData*)constantMatrixBuffer->mapBuffer();
			for(int i = 0; i < 16; i++)
			{
				matrixBuffer->modelViewProjectionMatrix[i] = mvpMatrix.data()[i];
			}
			for(int i = 0; i < 16; i++)
			{
				matrixBuffer->modelMatrix[i] = modelMatrix.data()[i];
			}
			for(int i = 0; i < 16; i++)
			{
				matrixBuffer->modelToMVPMatrix[i] = modelToMVPMatrix.data()[i];
			}
			constantMatrixBuffer->unmapBuffer();

			renderer->render(constantMatrixBuffer);
		}
	}

	finishRender();
}

void Camera::setClearColor(const QColor& color)
{
	clearColor[0] = color.red() / 255.0f;
	clearColor[1] = color.green() / 255.0f;
	clearColor[2] = color.blue() / 255.0f;
	clearColor[3] = color.alpha() / 255.0f;
}

const QMatrix4x4& Camera::getViewProjMatrix() const
{
	return viewProjMatrix;
}

const D3D11_VIEWPORT& Camera::getViewport() const
{
	return viewport;
}

const QVector3D& Camera::getPosition() const
{
	return position;
}

void Camera::initializeViewport()
{
	auto rendererWidget = engine->getRendererWidget();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = rendererWidget->width();
	viewport.Height = rendererWidget->height();
}

void Camera::startupRender()
{
	deviceContext->RSSetViewports(1, &viewport);

	if(clearColor[3] > 0)
	{
		deviceContext->ClearRenderTargetView(renderTarget, clearColor);
	}

	deviceContext->ClearDepthStencilView(depthStencilTarget, D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH, 1, 0);
}

void Camera::finishRender()
{
	//TODO handle postprocess, etc.
}

void Camera::refreshViewProjectionMatrix()
{
	viewProjMatrix = projectionMatrix * viewMatrix;
}
