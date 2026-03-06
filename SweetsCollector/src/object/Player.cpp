//-----------------------------------------------
// Player.cpp
// 制作日：2025/05/14
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "renderer/Renderer.h"
#include "entity/GameObject.h"
#include "renderer/ModelRenderer.h"
#include "renderer/MaterialManager.h"
#include "object/Player.h"
#include "input/Input.h"
#include "object/Camera.h"

#include "physics/RigidBody.h"
#include "physics/BoxCollider3D.h"

#include "component/renderer/ModelRendererComponent.h"

#include "component/MovePlayer.h"

#include "game/component/SweetsManagementComponent.h"
#include "game/component/SweetsShootComponent.h"
#include "game/component/PlayerAnimComponent.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void PLAYER::Init()
{
	m_transform.SetPos(VECTOR3(0.0f, 1.0f, 0.0f));
	m_transform.SetSca(VECTOR3(2.0f, 2.0f, 2.0f));
	m_transform.SetRot(QUATERNION::FromEuler(VECTOR3(0.0f, 0.0f, 0.0f)));

	AddComponent<BOX_COLLIDER_3D>();

	RIGID_BODY* rb = AddComponent<RIGID_BODY>();
	rb->SetMass(0.5f);
	rb->SetDynamicFriction(0.4f);
	rb->SetStaticFriction(0.8f);
	rb->SetRestitution(0.5f);

	rb->AddConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_ROT_X);
	rb->AddConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_ROT_Y);
	rb->AddConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_ROT_Z);

	
	MODEL_RENDERER_COMPONENT* model = AddComponent<MODEL_RENDERER_COMPONENT>();
	model->SetModel("asset\\model\\candy_bag.obj");
	model->SetLocalPos(VECTOR3(0.0f, -0.5f, 0.0f));
	model->SetLocalSca(VECTOR3(1.0f, 2.0f, 1.0f));

	AddComponent<MOVE_PLAYER>()->SetSpeed(5.0f);
	AddComponent<SWEETS_MANAGEMENT_COMPONENT>();
	AddComponent<SWEETS_SHOOT_COMPONENT>();
	AddComponent<PLAYER_ANIM_COMPONENT>();
}