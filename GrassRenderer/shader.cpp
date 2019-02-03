#include <fstream>
#include <cassert>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "shader.h"
#include "logger.h"

const std::string Shader::pathToShaders = "shaders/";
const std::string Shader::shaderExtension = ".hlsl";
const std::string Shader::propertiesStartString = "//PROPERTIES";
const std::string Shader::propertiesEndString = "//END_PROPERTIES";

std::unordered_map<std::string, std::shared_ptr<Shader>> Shader::initializedShaders;

Shader::Shader(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string shaderName)
			: device(device), deviceContext(deviceContext), name(shaderName)
{
	compile();
}


Shader::~Shader()
{
	if(vertexShader != nullptr)
	{
		vertexShader->Release();
	}

	if(pixelShader != nullptr)
	{
		pixelShader->Release();
	}

	if(geomShader != nullptr)
	{
		geomShader->Release();
	}

	if(hullShader != nullptr)
	{
		hullShader->Release();
	}

	if(domainShader != nullptr)
	{
		domainShader->Release();
	}

	if(layout != nullptr)
	{
		layout->Release();
	}
}

std::shared_ptr<Shader> Shader::findOrCreate(ID3D11Device * device, ID3D11DeviceContext* deviceContext, std::string name)
{
	auto findShader = initializedShaders.find(name);
	if(findShader == initializedShaders.end())
	{
		std::string path = pathToShaders + name + shaderExtension;
		std::ifstream file(path);
		if(!file.good())
		{ 
			return std::shared_ptr<Shader>();
		}
		file.close();

		std::shared_ptr<Shader> shader = std::make_shared<Shader>(device, deviceContext, name);
		if(shader->isCompiled())
		{
			initializedShaders[name] = shader;
			return shader;
		}
		else
		{
			return std::shared_ptr<Shader>();
		}
	}

	return findShader->second;
}

bool Shader::isCompiled() const
{
	return compiled;
}

bool Shader::isGeometryShaderCompiled() const
{
	return geomShader != nullptr;
}

bool Shader::isTesselationShaderCompiled() const
{
	return hullShader != nullptr && domainShader != nullptr;
}

void Shader::setForRender()
{
	deviceContext->IASetInputLayout(layout);
	deviceContext->VSSetShader(vertexShader, NULL, 0);
	deviceContext->PSSetShader(pixelShader, NULL, 0);
	if(geomShader != nullptr)
	{
		deviceContext->GSSetShader(geomShader, NULL, 0);
	}
	if(domainShader != nullptr)
	{
		deviceContext->DSSetShader(domainShader, NULL, 0);
	}
	if(hullShader != nullptr)
	{
		deviceContext->HSSetShader(hullShader, NULL, 0);
	}
}

const std::string& Shader::getName() const
{
	return name;
}

size_t Shader::getPropertiesSizeInBytes() const
{
	return propertiesSizeInBytes;
}

int Shader::getPropertyOffsetInShader(const std::string& propertyName) const
{
	auto find = propertiesOffsetsMap.find(propertyName);
	if(find == propertiesOffsetsMap.end())
	{
		return -1;
	}

	return find->second;
}

void Shader::compile()
{
	std::string path = pathToShaders + name + shaderExtension;
	std::ifstream file(path);
	if(!file.is_open())
	{
		LogError("Unable to open shader: " + path);
		return;
	}
	std::string text = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	file.close();

	if(compileBlob(text, vertexFunctionName, vertexShModel, vertexBlob))
	{
		HRESULT hr = device->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), NULL, &vertexShader);
		if(FAILED(hr))
		{
			LogError("Failed compile vertex shader: " + path);
			return;
		}
	}
	else
	{
		LogError("Missing VertMain in: " + path);
		return;
	}
	
	if(compileBlob(text, pixelFunctionName, pixelShModel, pixelBlob))
	{
		HRESULT hr = device->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), NULL, &pixelShader);
		if(FAILED(hr))
		{
			LogError("Failed compile pixel shader: " + path);
			return;
		}
	}
	else
	{
		LogError("Missing PixelMain in: " + path);
		return;
	}
	
	if(compileBlob(text, hullFunctionName, hullShModel, hullBlob))
	{
		HRESULT hr = device->CreateHullShader(hullBlob->GetBufferPointer(), hullBlob->GetBufferSize(), NULL, &hullShader);
		if(FAILED(hr))
		{
			LogError("Failed compile hull shader: " + path);
			return;
		}
	}
	
	if(compileBlob(text, domainFunctionName, domainShModel, domainBlob))
	{
		HRESULT hr = device->CreateDomainShader(domainBlob->GetBufferPointer(), domainBlob->GetBufferSize(), NULL, &domainShader);
		if(FAILED(hr))
		{
			LogError("Failed compile domain shader: " + path);
			return;
		}
	}
	
	if(compileBlob(text, geomFunctionName, geomShModel, geomBlob))
	{
		HRESULT hr = device->CreateGeometryShader(geomBlob->GetBufferPointer(), geomBlob->GetBufferSize(), NULL, &geomShader);
		if(FAILED(hr))
		{
			LogError("Failed compile geometry shader: " + path);
			return;
		}
	}

	compileLayout(text);
	compileProperties(text);
	clearBlobs();
	compiled = true;
}

bool Shader::compileBlob(const std::string& text, const std::string& functionName, const std::string& shaderModel, ID3DBlob*& blob)
{
	if(text.find(functionName) == std::string::npos)
	{
		return false;
	}
	
	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* errorBlob;
	HRESULT hr = D3DCompile(text.c_str(), text.size(), name.c_str(), NULL, NULL,
							functionName.c_str(), shaderModel.c_str(), flags, 0, &blob, &errorBlob);

	if(FAILED(hr)) 
	{
		if(errorBlob != NULL) 
		{
			LogError(name + ":" + functionName + " shader compile error: " + (char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		else
		{
			LogError(name + ":" + functionName + " shader compile error: " + Logger::hresultToString(hr));
		}
		return false;
	}

	if(errorBlob != NULL) 
	{
		LogWarning(name + ":" + functionName + " shader compile issue: " + (char*)errorBlob->GetBufferPointer());
		errorBlob->Release();
	}

	return true;
}

void Shader::compileLayout(std::string& text)
{
	const int elementsCount = 1;
	const D3D11_INPUT_ELEMENT_DESC layoutFields[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	device->CreateInputLayout(layoutFields, elementsCount,
										  vertexBlob->GetBufferPointer(),
										  vertexBlob->GetBufferSize(), &layout);

}

void Shader::compileProperties(std::string& text)
{
	propertiesSizeInBytes = 0;
	auto propsStart = text.find(propertiesStartString);
	auto propsEnd = text.find(propertiesEndString);
	if(propsStart == std::string::npos
	   || propsEnd == std::string::npos)
	{
		return;
	}
	
	propertiesOffsetsMap.clear();
	std::istringstream textStream(text);
	std::string line;
	textStream.seekg(propsStart);
	int lineCounter = 0;
	while(textStream.tellg() < propsEnd)
	{
		std::getline(textStream, line);
		lineCounter++;
		if(lineCounter == 1)
		{
			continue;
		}

		std::string propName;
		unsigned int propSize;
		std::tie(propName, propSize) = parsePropsLine(line);
		if(!propName.empty())
		{
			propertiesOffsetsMap[propName] = propertiesSizeInBytes;
			propertiesSizeInBytes += propSize;
		}
	}
}

void Shader::clearBlobs()
{
	clearBlob(vertexBlob);
	clearBlob(pixelBlob);
	clearBlob(geomBlob);
	clearBlob(hullBlob);
	clearBlob(domainBlob);
}

void Shader::clearBlob(ID3DBlob *& blob)
{
	if(blob != nullptr)
	{
		blob->Release();
		blob = nullptr;
	}
}

std::tuple<std::string, size_t> Shader::parsePropsLine(const std::string& line)
{
	std::string propName;
	size_t propSize = 0;
	std::stringstream ss;
	int paramsCounter = 0;
	for(int i = 0; i < line.length(); i++)
	{
		if(line[i] == '/')
		{
			continue;
		}

		if(line[i] == ';')
		{
			paramsCounter++;
			std::string param = ss.str();
			if(paramsCounter == 1)
			{
				propName = param;
			}
			else if(paramsCounter == 2)
			{
				propSize = propertiesSizesMap.at(param);
			}
			else
			{
				break;
			}

			ss.str(std::string());
			continue;
		}

		ss << line[i];
	}

	return std::make_tuple(propName, propSize);
}

