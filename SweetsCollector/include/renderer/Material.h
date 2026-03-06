//-----------------------------------------------
// Material.h
// 制作日：2025/07/18
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "Main.h"
#include "Renderer.h"

class MATERIAL
{
public:
	ID3D11VertexShader* m_vertex_shader = nullptr;
	ID3D11PixelShader* m_pixel_shader = nullptr;
	ID3D11InputLayout* m_vertex_layout = nullptr;
	ID3D11ShaderResourceView* m_texture = nullptr;

	MATERIAL_DATA m_material_data;

	MATERIAL() : m_vertex_shader(nullptr), m_pixel_shader(nullptr), m_vertex_layout(nullptr), m_texture(nullptr) { m_material_data = MATERIAL_DATA(); }

	void Uninit()
	{
		if (m_vertex_shader)
		{
			m_vertex_shader->Release();
			m_vertex_shader = nullptr;
		}
		if (m_pixel_shader)
		{
			m_pixel_shader->Release();
			m_pixel_shader = nullptr;
		}
		if (m_vertex_layout)
		{
			m_vertex_layout->Release();
			m_vertex_layout = nullptr;
		}
	}

	void CreateShaders(const char* vertexShaderPath, const char* pixelShaderPath)
	{

		Renderer::CreateVertexShader(&m_vertex_shader, &m_vertex_layout, vertexShaderPath);
		Renderer::CreatePixelShader(&m_pixel_shader, pixelShaderPath);
	}
};