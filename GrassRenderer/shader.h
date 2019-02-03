#pragma once
#include <d3d11.h>
#include <string>
#include <unordered_map>
#include <tuple>
#include <memory>

class Shader
{
public:
	Shader(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string shaderName);
	~Shader();

	/// <summary>
	/// Finds or creates new shader
	/// </summary>
	/// <param name="device"></param>
	/// <param name="deviceContext"></param>
	/// <param name="name">Shader name</param>
	/// <returns>shared_ptr, it can be empty if shader wasn't possible to load</returns>
	static std::shared_ptr<Shader> findOrCreate(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string name);

	bool isCompiled() const;
	bool isGeometryShaderCompiled() const;
	bool isTesselationShaderCompiled() const;
	const std::string& getName() const;
	size_t getPropertiesSizeInBytes() const;
	int getPropertyOffsetInShader(const std::string& propertyName) const;
	
	/// <summary>
	/// Enables shader for rendering current object
	/// </summary>
	void setForRender();

private:
	static const std::string pathToShaders;
	static const std::string shaderExtension;
	static const std::string propertiesStartString;
	static const std::string propertiesEndString;

	static std::unordered_map<std::string, std::shared_ptr<Shader>> initializedShaders;

	//TODO make names dynamically loaded
	const std::string vertexFunctionName = "VertMain";
	const std::string pixelFunctionName = "PixelMain";
	const std::string geomFunctionName = "GeometryMain";
	const std::string hullFunctionName = "HullMain";
	const std::string domainFunctionName = "DomainMain";

	//TODO make SM dynamically loaded
	const std::string vertexShModel = "vs_5_0";
	const std::string pixelShModel = "ps_5_0";
	const std::string geomShModel = "gs_5_0";
	const std::string domainShModel = "ds_5_0";
	const std::string hullShModel = "hs_5_0";

	const std::unordered_map<std::string, size_t> propertiesSizesMap =
	{
		{"FLOAT", sizeof(float)},
		{"FLOAT4", sizeof(float) * 4},
		{"MATRIX", sizeof(float) * 4 * 4},
	};


	std::string name;
	bool compiled = false;
	ID3D11Device * device;
	ID3D11DeviceContext* deviceContext;
	ID3DBlob* vertexBlob = nullptr;
	ID3DBlob* pixelBlob = nullptr;
	ID3DBlob* geomBlob = nullptr;
	ID3DBlob* hullBlob = nullptr;
	ID3DBlob* domainBlob = nullptr;
	
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11GeometryShader* geomShader = nullptr;
	ID3D11HullShader* hullShader = nullptr;
	ID3D11DomainShader* domainShader = nullptr;

	ID3D11InputLayout* layout = nullptr;
	std::unordered_map<std::string, unsigned int> propertiesOffsetsMap;
	size_t propertiesSizeInBytes = 0;
	
	void compile();
	bool compileBlob(const std::string& text, const std::string& functionName, const std::string& shaderModel, ID3DBlob*& blob);
	void compileLayout(std::string& text);
	void compileProperties(std::string& text);
	void clearBlobs();
	void clearBlob(ID3DBlob*& blob);
	std::tuple<std::string, size_t> parsePropsLine(const std::string& line);
};

