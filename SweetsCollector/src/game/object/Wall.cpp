//-----------------------------------------------
// Wall.cpp
// 制作日：2026/01/06
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "game/object/Wall.h"

#include "physics/BoxCollider3D.h"
#include "physics/RigidBody.h"

#include "component/renderer/MeshRendererComponent.h"
#include "renderer/MeshFactory.h"
#include "renderer/MaterialManager.h"

//------------------------------------------------
// 初期化処理
//------------------------------------------------
void WALL::Init()
{
	m_transform.SetSca({ 1.0f, 1.0f, 1.0f });

	MESH_RENDERER_COMPONENT* mesh_renderer = AddComponent<MESH_RENDERER_COMPONENT>();
	mesh_renderer->SetMeshData(MESH_FACTORY::CreateCubeMesh(1.0f, 1.0f, 1.0f));
	mesh_renderer->SetMaterial(MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\WallMaterial.mtl"));

	AddComponent<BOX_COLLIDER_3D>();

	RIGID_BODY* rigid_body = AddComponent<RIGID_BODY>();
	rigid_body->SetType(RIGID_BODY_TYPE::STATIC);
	rigid_body->SetMass(0.0f);
	//rigid_body->SetRestitution(1.0f);
}

