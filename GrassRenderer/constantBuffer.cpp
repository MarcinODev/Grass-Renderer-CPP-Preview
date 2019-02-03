#include <cassert>
#include <D3Dcompiler.h>
#include "constantBuffer.h"
#include "logger.h"

ConstantBuffer::ConstantBuffer(unsigned int sizeInBytes, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	if(sizeInBytes == 0)
	{
		LogError("Constant buffer cannot have size = 0");
		return;
	}

	this->deviceContext = deviceContext;
	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	const unsigned int sizeOfDataChunk = 16;
	desc.ByteWidth = sizeInBytes % sizeOfDataChunk == 0 ? sizeInBytes : ((sizeInBytes / sizeOfDataChunk) + 1) * sizeOfDataChunk;
	HRESULT hr = device->CreateBuffer(&desc, NULL, &buffer);
	if(FAILED(hr))
	{
		LogError("Constant buffer create error: " + Logger::hresultToString(hr));
		return;
	}
}

ConstantBuffer::~ConstantBuffer()
{
	if(buffer != nullptr)
	{
		buffer->Release();
	}
}

void ConstantBuffer::setForVertexShader(unsigned int bufferId)
{
	if(buffer == nullptr)
	{
		return;
	}
	deviceContext->VSSetConstantBuffers(bufferId, 1, &buffer);
}

void ConstantBuffer::setForPixelShader(unsigned int bufferId)
{
	if(buffer == nullptr)
	{
		return;
	}
	deviceContext->PSSetConstantBuffers(bufferId, 1, &buffer);
}

void ConstantBuffer::setForGeometryShader(unsigned int bufferId)
{
	if(buffer == nullptr)
	{
		return;
	}
	deviceContext->GSSetConstantBuffers(bufferId, 1, &buffer);
}

void ConstantBuffer::setForHullShader(unsigned int bufferId)
{
	if(buffer == nullptr)
	{
		return;
	}
	deviceContext->HSSetConstantBuffers(bufferId, 1, &buffer);
}

void ConstantBuffer::setForDomainShader(unsigned int bufferId)
{
	if(buffer == nullptr)
	{
		return;
	}
	deviceContext->DSSetConstantBuffers(bufferId, 1, &buffer);
}

void* ConstantBuffer::mapBuffer()
{
	if(buffer == nullptr)
	{
		return nullptr;
	}
	deviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	return mappedResource.pData;
}

void ConstantBuffer::unmapBuffer()
{
	if(buffer == nullptr)
	{
		return;
	}
	deviceContext->Unmap(buffer, 0);
}
