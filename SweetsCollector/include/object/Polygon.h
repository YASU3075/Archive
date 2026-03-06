//-----------------------------------------------
// Polygon.h
// 制作日：2025/04/23
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include "entity/GameObject.h"

class POLYGON_2D : public GAME_OBJECT
{
private:

	ID3D11Buffer* m_vertex_buffer;

	ID3D11VertexShader* m_vertex_shader;
	ID3D11PixelShader* m_pixel_shader;
	ID3D11InputLayout* m_vertex_layout;

	ID3D11ShaderResourceView* m_texture;
public:
	void Init() override;
	void Draw() override;
};