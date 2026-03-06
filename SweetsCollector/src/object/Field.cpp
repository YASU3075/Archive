//-----------------------------------------------
// Field.cpp
// 制作日：2025/05/14
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "entity/GameObject.h"
#include "object/Field.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "physics/RigidBody.h"
#include "physics/BoxCollider3D.h"

#include "component/renderer/MeshRendererComponent.h"

#include "renderer/MaterialManager.h"

#include "game/object/MapGenerater.h"
#include "game/component/GenerateMapComponent.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void FIELD::Init()
{
	MAP_GENERATER* map_gen = Manager::GetScene()->GetGameObject<MAP_GENERATER>();
	if (map_gen)
	{
		GENERATE_MAP_COMPONENT* gen_comp = map_gen->GetComponent<GENERATE_MAP_COMPONENT>();
		m_transform.SetSca(gen_comp->GetMapWorldSize());
	}
	else
	{
		m_transform.SetSca(VECTOR3(1.0f, 1.0f, 1.0f));
	}
	
	m_transform.SetPos(VECTOR3(0.0f, 0.0f, 0.0f));

	AddComponent<BOX_COLLIDER_3D>();
	RIGID_BODY* rb = AddComponent<RIGID_BODY>();
	rb->SetType(RIGID_BODY_TYPE::STATIC);
	rb->SetMass(0.0f);
	rb->SetUseGravity(false);
	
	rb->SetRestitution(0.0f);
	rb->SetDynamicFriction(0.0f);
	rb->SetStaticFriction(0.0f);

	MESH_RENDERER_COMPONENT* mesh = AddComponent<MESH_RENDERER_COMPONENT>();
	mesh->SetMaterial(MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\FieldMaterial.mtl"));
}