#pragma once
#include "gameObject.h"
#include "renderer.h"
#include "mesh.h"
#include "material.h"

/// <summary>
/// Renderer of Mesh and provided Material
/// </summary>
class MeshRenderer : public Renderer
{
public:
	MeshRenderer(GameObject* owner);
	virtual ~MeshRenderer();

	/// <summary>
	/// Renders Mesh with Material
	/// </summary>
	virtual void render(std::shared_ptr<ConstantBuffer> matrixConstantBuffer);

	void setMaterial(std::shared_ptr<Material> material);
	void setMesh(std::shared_ptr<Mesh> mesh);

protected:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;
};

