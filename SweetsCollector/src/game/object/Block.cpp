//-----------------------------------------------
// Block.cpp
// 制作日：2026/02/15
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"

#include "game/object/Block.h"
#include "physics/BoxCollider3D.h"
#include "physics/RigidBody.h"

#include "component/renderer/MeshRendererComponent.h"
#include "renderer/MeshFactory.h"
#include "renderer/MaterialManager.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void BLOCK::Init()
{
	m_transform.SetPos(VECTOR3(0.0f, 0.0f, 0.0f));
	m_transform.SetSca(VECTOR3(1.0f, 1.0f, 1.0f));

	MESH_RENDERER_COMPONENT* mesh = AddComponent<MESH_RENDERER_COMPONENT>();
	mesh->SetMeshData(MESH_FACTORY::CreateCubeMesh(1.0f, 1.0f, 1.0f));
	mesh->SetMaterial(MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\BlockMaterial.mtl"));
	BOX_COLLIDER_3D* collider = AddComponent<BOX_COLLIDER_3D>();
	RIGID_BODY* rb = AddComponent<RIGID_BODY>();
	rb->SetType(RIGID_BODY_TYPE::DYNAMIC);
	rb->SetMass(0.1f);
	rb->SetDynamicFriction(0.5f);
	rb->SetStaticFriction(0.5f);

}