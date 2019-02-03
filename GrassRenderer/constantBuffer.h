#pragma once
#include <d3d11.h>

class ConstantBuffer
{
public:
	ConstantBuffer(unsigned int sizeInBytes, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~ConstantBuffer();

	/// <param name="bufferId">Index of buffer in shader's order</param>
	void setForVertexShader(unsigned int bufferId);
	
	/// <param name="bufferId">Index of buffer in shader's order</param>
	void setForPixelShader(unsigned int bufferId);
	
	/// <param name="bufferId">Index of buffer in shader's order</param>
	void setForGeometryShader(unsigned int bufferId);
	
	/// <param name="bufferId">Index of buffer in shader's order</param>
	void setForHullShader(unsigned int bufferId);
	
	/// <param name="bufferId">Index of buffer in shader's order</param>
	void setForDomainShader(unsigned int bufferId);

	/// <summary>
	/// Maps DX11 buffer into void* array
	/// </summary>
	void* mapBuffer();
	
	/// <summary>
	/// Sends mapped array to GPU
	/// </summary>
	void unmapBuffer();

private:
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ID3D11Buffer* buffer = nullptr;
	ID3D11DeviceContext* deviceContext;

};