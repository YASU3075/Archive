//-----------------------------------------------
// OutlineRendererComponent.h
// 制作日：2026/02/11
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "Main.h"
#include "renderer/Renderer.h"
#include "component/renderer/RendererComponent.h"

class MATERIAL;

class OUTLINE_RENDERER_COMPONENT : public RENDERER_COMPONENT
{
private:
	RENDERER_COMPONENT* m_target = nullptr;

	ID3D11Buffer* m_outline_cbuffer = nullptr;
	
	OUTLINE_PARAMS m_outline_cbuffer_data;

public:
	void Init() override;
	void Uninit() override;
	void Update() override {};
	void Draw() override;

	XMMATRIX BuildWorldMatrix() const override { return XMMatrixIdentity(); }
	void BindGeometry() const override {};
	void DrawGeometyrOnly() const override {};

	void SetOutlineColor(const XMFLOAT4& color);
	void SetOutlineWidth(float width);
};