#pragma once
#include <QVector3D>
#include <QMatrix4x4>
#include <qcolor.h>
#include <d3d11.h>

/// <summary>
/// Container for mesh buffers (vertex, index, etc.) ready for rendering 
/// </summary>
class Mesh
{
public:
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~Mesh();
	
	/// <summary>
	/// Set vertices for mesh.
	/// </summary>
	/// <remarks>When setup is ready use reload() to "save" state</remarks>
	void setVertices(const std::vector<QVector3D>& vertices);
	
	/// <summary>
	/// Set indices for mesh.
	/// </summary>
	/// <remarks>When setup is ready use reload() to "save" state</remarks>
	void setIndices(const std::vector<unsigned int>& indices);
	
	/// <summary>
	/// Reloads buffers into GPU
	/// </summary>
	void reload();

	/// <summary>
	/// Enabled buffers to be ready for render
	/// </summary>
	/// <returns>Returns false if buffer is not ready</returns>
	bool setBuffersForRender(bool isTesselated);

	/// <summary>
	/// Draws mesh
	/// </summary>
	void draw();

private:
	ID3D11DeviceContext* deviceContext;
	ID3D11Device* device;
	std::vector<QVector3D> vertices;
	std::vector<unsigned int> indices;
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;

	void releaseBuffers();
};

