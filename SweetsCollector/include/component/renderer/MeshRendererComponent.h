//-----------------------------------------------
// MeshRendererComponent.h
// 制作日：2025/11/30
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <vector>
#include "renderer/MeshRendererData.h"
#include "component/renderer/RendererComponent.h"

class MATERIAL;


class MESH_RENDERER_COMPONENT : public RENDERER_COMPONENT
{
private:
	MESH_RENDERER_DATA m_mesh_data;
	ID3D11Buffer* m_vertex_buffer = nullptr;
	ID3D11Buffer* m_index_buffer = nullptr;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	XMMATRIX BuildWorldMatrix() const override;
	void BindGeometry() const override;
	void DrawGeometyrOnly() const override;

	void SetMeshData(const MESH_RENDERER_DATA& mesh_data);
};