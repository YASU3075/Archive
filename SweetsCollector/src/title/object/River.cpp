//-----------------------------------------------
// River.cpp
// 制作日：2026/01/28
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "title/object/River.h"

#include "component/renderer/MeshRendererComponent.h"
#include "renderer/MeshFactory.h"
#include "renderer/MaterialManager.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void RIVER::Init()
{
	m_transform.SetPos(VECTOR3(0.0f, 0.0f, 0.0f));
	m_transform.SetSca(VECTOR3(1.0f, 1.0f, 1.0f));

	MESH_RENDERER_COMPONENT* mesh_renderer = AddComponent<MESH_RENDERER_COMPONENT>();
	mesh_renderer->SetMeshData(MESH_FACTORY::CreatePlaneMesh(1.0f, 1.0f, 10, 100));
	mesh_renderer->SetMaterial(MATERIAL_MANAGER::LoadFromCustomFile("asset/material/RiverMaterial.mtl"));
	mesh_renderer->SetUVScrollEnable(true);
	mesh_renderer->SetUVSpeed(XMFLOAT2(0.0f, 0.0005f));
}