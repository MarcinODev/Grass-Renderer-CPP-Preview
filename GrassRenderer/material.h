#pragma once
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <qcolor.h>
#include <d3d11.h>
#include "shader.h"
#include "constantBuffer.h"

/// <summary>
/// Material contains Shader and properies which should be passed to it to render certain Mesh
/// </summary>
class Material
{
public:
	Material(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::shared_ptr<Shader> shader);
	~Material();

	std::shared_ptr<Shader> getShader();
	/// <summary>
	/// Enables Material (and Shader in it) to be ready for render call.
	/// </summary>
	void setShadersForRender(std::shared_ptr<ConstantBuffer> matrixConstantBuffer);

	/// <summary>
	/// Sets float value
	/// </summary>
	void setFloat(const std::string& propertyName, float value);
	
	/// <summary>
	/// Sets float4 value
	/// </summary>
	void setVector(const std::string& propertyName, const QVector4D& value);

private:
	ID3D11DeviceContext* deviceContext;
	std::shared_ptr<Shader> shader;
	std::shared_ptr<ConstantBuffer> propertiesBuffer;
	char* propertiesData = nullptr;
	unsigned int propertiesBufferSize = 0;
	bool bufferNeedsRefresh = false;
};

