//-----------------------------------------------
// ShaderManager.h
// 制作日：2025/11/10
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <unordered_map>
#include <string>

struct VertexShaderResource
{
	ID3D11VertexShader* shader = nullptr;
	ID3D11InputLayout* layout = nullptr;
};

class SHADER_MANAGER
{
private:
	static std::unordered_map<std::string, VertexShaderResource> m_vertex_shader_pool;
	static std::unordered_map<std::string, ID3D11PixelShader*> m_pixel_shader_pool;

public:
	static const VertexShaderResource& LoadVertexShader(const char* name);

	static ID3D11VertexShader* GetVertexShader(const char* name)
	{
		return m_vertex_shader_pool[name].shader;
	}

	static ID3D11InputLayout* GetVertexLayout(const char* name)
	{
		return m_vertex_shader_pool[name].layout;
	}

	static ID3D11PixelShader* LoadPixelShader(const char* name);
	static void ReleaseAll();
};