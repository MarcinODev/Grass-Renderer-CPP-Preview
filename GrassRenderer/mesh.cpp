#include "mesh.h"



Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext) : device(device), deviceContext(deviceContext)
{
}



Mesh::~Mesh()
{
	releaseBuffers();
}

void Mesh::setVertices(const std::vector<QVector3D>& vertices)
{
	this->vertices = vertices;
}

void Mesh::setIndices(const std::vector<unsigned int>& indices)
{
	this->indices = indices;
}

void Mesh::reload()
{
	releaseBuffers();

	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(QVector3D) * vertices.size();
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.StructureByteStride = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subResData;
	subResData.pSysMem = &vertices[0];
	subResData.SysMemPitch = 0;
	subResData.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateBuffer(&desc, &subResData, &vertexBuffer);
	assert(!FAILED(hr));

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(unsigned int) * indices.size();
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.StructureByteStride = 0;
	desc.MiscFlags = 0;

	subResData.pSysMem = &indices[0];
	subResData.SysMemPitch = 0;
	subResData.SysMemSlicePitch = 0;

	hr = device->CreateBuffer(&desc, &subResData, &indexBuffer);
	assert(!FAILED(hr));
}

bool Mesh::setBuffersForRender(bool isTesselated)
{
	if(vertexBuffer == nullptr || indexBuffer == nullptr)
	{
		return false;
	}

	unsigned int stride = sizeof(float) * 3;
	unsigned int offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(isTesselated ? D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST : D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

void Mesh::draw()
{
	if(vertexBuffer == nullptr || indexBuffer == nullptr)
	{
		return;
	}

	deviceContext->DrawIndexed(indices.size(), 0, 0);
}

void Mesh::releaseBuffers()
{
	if(vertexBuffer != nullptr)
	{
		vertexBuffer->Release();
	}

	if(indexBuffer != nullptr)
	{
		indexBuffer->Release();
	}
}
