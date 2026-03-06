//-----------------------------------------------
// Explosion.h
// 制作日：2025/06/11
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "entity/GameObject.h"

class EXPLOSION : public GAME_OBJECT
{
private:
	ID3D11Buffer* m_vertex_buffer;

	ID3D11VertexShader* m_vertex_shader;
	ID3D11PixelShader* m_pixel_shader;
	ID3D11InputLayout* m_vertex_layout;

	ID3D11ShaderResourceView* m_texture;

	int m_frame = 0;

public:
	void Init() override;
	void Draw() override;
};