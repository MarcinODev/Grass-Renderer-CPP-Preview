#include "material.h"
#include "logger.h"


Material::Material(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::shared_ptr<Shader> shader)
				: deviceContext(deviceContext), shader(shader)
{
	if(!shader)
	{
		LogError("Passed null shader into material");
		return;
	}

	propertiesBufferSize = shader->getPropertiesSizeInBytes();
	if(propertiesBufferSize > 0)
	{
		propertiesBuffer = std::make_shared<ConstantBuffer>(propertiesBufferSize, device, deviceContext);
		propertiesData = new char[propertiesBufferSize];	
	}
}


Material::~Material()
{
	if(propertiesData != nullptr)
	{
		delete[] propertiesData;
	}
}

std::shared_ptr<Shader> Material::getShader()
{
	return shader;
}

void Material::setShadersForRender(std::shared_ptr<ConstantBuffer> matrixConstantBuffer)
{
	if(!shader)
	{
		return;
	}
	
	//TODO optimize set shader (reuse from previous steps, proper sorting by material, set only used constant buffers etc)
	shader->setForRender();
	matrixConstantBuffer->setForVertexShader(0);
	matrixConstantBuffer->setForPixelShader(0);
	if(shader->isTesselationShaderCompiled())
	{
		matrixConstantBuffer->setForHullShader(0);
		matrixConstantBuffer->setForDomainShader(0);
	}
	if(shader->isGeometryShaderCompiled())
	{
		matrixConstantBuffer->setForGeometryShader(0);
	}

	if(propertiesBuffer)
	{
		if(bufferNeedsRefresh)
		{
			void* propsBufferData = propertiesBuffer->mapBuffer();
			memcpy(propsBufferData, propertiesData, propertiesBufferSize);
			propertiesBuffer->unmapBuffer();
			bufferNeedsRefresh = false;
		}

		propertiesBuffer->setForVertexShader(1);
		propertiesBuffer->setForPixelShader(1);
		if(shader->isTesselationShaderCompiled())
		{
			propertiesBuffer->setForHullShader(1);
			propertiesBuffer->setForDomainShader(1);
		}
		if(shader->isGeometryShaderCompiled())
		{
			propertiesBuffer->setForGeometryShader(1);
		}
	}
}

void Material::setFloat(const std::string& propertyName, float value)
{
	int offset = shader->getPropertyOffsetInShader(propertyName);
	if(offset == -1)
	{
		LogWarning("Unknown property " + shader->getName() + "::" +  propertyName);
		return;
	}

	*((float*)&propertiesData[offset]) = value;
	bufferNeedsRefresh = true;
}

void Material::setVector(const std::string& propertyName, const QVector4D& value)
{
	int offset = shader->getPropertyOffsetInShader(propertyName);
	if(offset == -1)
	{
		LogWarning("Unknown property " + shader->getName() + "::" +  propertyName);
		return;
	}
	
	float* floatTab = (float*)&propertiesData[offset];
	floatTab[0] = value.x();
	floatTab[1] = value.y();
	floatTab[2] = value.z();
	floatTab[3] = value.w();
	bufferNeedsRefresh = true;
}
