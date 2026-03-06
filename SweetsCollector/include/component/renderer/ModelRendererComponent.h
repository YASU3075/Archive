//-----------------------------------------------
// ModelRendererComponent.h
// 制作日：2025/07/18
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "Main.h"
#include "RendererComponent.h"
#include "renderer/Material.h"

class MODEL_RENDERER_COMPONENT : public RENDERER_COMPONENT
{
private:
	bool m_use_model_material = true;
public:
	class ModelRenderer* m_model_renderer;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	XMMATRIX BuildWorldMatrix() const override;
	void BindGeometry() const override;
	void DrawGeometyrOnly() const override;

	void SetModel(const char* name);

	void SetUseModelMaterial(bool use);
};
