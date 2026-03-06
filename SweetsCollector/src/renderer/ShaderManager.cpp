//-----------------------------------------------
// ShaderManager.cpp
// 制作日：2025/11/10
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "renderer/ShaderManager.h"

// 静的メンバ変数の初期化
std::unordered_map<std::string, VertexShaderResource> SHADER_MANAGER::m_vertex_shader_pool;
std::unordered_map<std::string, ID3D11PixelShader*> SHADER_MANAGER::m_pixel_shader_pool;

//-----------------------------------------------
// 頂点シェーダーの読み込み
//-----------------------------------------------
const VertexShaderResource& SHADER_MANAGER::LoadVertexShader(const char* name)
{
	auto it = m_vertex_shader_pool.find(name);
	if (it != m_vertex_shader_pool.end())
	{// 既にロード済み
		return it->second;
	}

	VertexShaderResource vsr;
	Renderer::CreateVertexShader(&vsr.shader, &vsr.layout, name);
	m_vertex_shader_pool[name] = vsr;
	return m_vertex_shader_pool[name];
}

//-----------------------------------------------
// ピクセルシェーダーの読み込み
//-----------------------------------------------
ID3D11PixelShader* SHADER_MANAGER::LoadPixelShader(const char* name)
{
	auto it = m_pixel_shader_pool.find(name);
	if (it != m_pixel_shader_pool.end())
	{// 既にロード済み
		return it->second;
	}
	ID3D11PixelShader* ps = nullptr;
	Renderer::CreatePixelShader(&ps, name);
	m_pixel_shader_pool[name] = ps;
	return ps;
}

//-----------------------------------------------
// 全シェーダーの解放
//-----------------------------------------------
void SHADER_MANAGER::ReleaseAll()
{
	for (auto& vs_pair : m_vertex_shader_pool)
	{
		if (vs_pair.second.shader)
		{
			vs_pair.second.shader->Release();
			vs_pair.second.shader = nullptr;
		}
		if (vs_pair.second.layout)
		{
			vs_pair.second.layout->Release();
			vs_pair.second.layout = nullptr;
		}
	}
	m_vertex_shader_pool.clear();
	for (auto& ps_pair : m_pixel_shader_pool)
	{
		if (ps_pair.second)
		{
			ps_pair.second->Release();
			ps_pair.second = nullptr;
		}
	}
	m_pixel_shader_pool.clear();
}