#include "meshRenderer.h"
#include "logger.h"


MeshRenderer::MeshRenderer(GameObject* owner) : Renderer(owner)
{
}


MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::render(std::shared_ptr<ConstantBuffer> matrixConstantBuffer)
{
	if(mesh->setBuffersForRender(material->getShader()->isTesselationShaderCompiled()))
	{
		material->setShadersForRender(matrixConstantBuffer);
		mesh->draw();
	}
	else
	{
		LogDebug("Fail to render " + gameObject->getName());
	}
}

void MeshRenderer::setMaterial(std::shared_ptr<Material> material)
{
	this->material = material;
}

void MeshRenderer::setMesh(std::shared_ptr<Mesh> mesh)
{
	this->mesh = mesh;
}
