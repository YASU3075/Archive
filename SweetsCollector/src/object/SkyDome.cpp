//-----------------------------------------------
// SkyDome.cpp
// 制作日：2025/09/14
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"

#include "object/SkyDome.h"

#include "component/RotationObjComponent.h"

#include "component/renderer/ModelRendererComponent.h"
#include "renderer/MaterialManager.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void SKY_DOME::Init()
{
	auto mesh_renderer = AddComponent<MODEL_RENDERER_COMPONENT>();
	mesh_renderer->SetModel("asset\\model\\sky.obj");
	mesh_renderer->SetMaterial(MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\SkyDomeMaterial.mtl"));
	mesh_renderer->SetUseModelMaterial(false);
	m_transform.SetSca(VECTOR3(700.0f, 700.0f, 700.0f));

	AddComponent<ROTATION_OBJ_COMPONENT>()->SetRotationSpeed(VECTOR3(0.0f, 0.01f, 0.0f));
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void SKY_DOME::Draw()
{
	GetComponent<MODEL_RENDERER_COMPONENT>()->Draw();
}