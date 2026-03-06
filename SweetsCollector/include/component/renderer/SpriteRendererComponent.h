//-----------------------------------------------
// SpriteRendererComponent.h
// 制作日：2025/07/17
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "Main.h"
#include "RendererComponent.h"

class SPRITE_RENDERER_COMPONENT : public RENDERER_COMPONENT
{
private:
	ID3D11Buffer* m_vertex_buffer;
	bool m_is_billboard = false;
	bool m_uv_sheet = false;
	int m_sprite_index = 0;
	int m_x_num = 1;
	int m_y_num = 1;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	XMMATRIX BuildWorldMatrix() const override { return XMMatrixIdentity(); }
	void BindGeometry() const override {};
	void DrawGeometyrOnly() const override {};

	void SetTexture(const char* name);

	void SetBillboard(bool is_billboard) { m_is_billboard = is_billboard; }
	void SetUVSheet(bool uv_sheet) { m_uv_sheet = uv_sheet; }
	void SetSpriteIndex(int index) { m_sprite_index = index; }
	void SetSpriteNum(int x_num, int y_num) { m_x_num = x_num; m_y_num = y_num; }
};