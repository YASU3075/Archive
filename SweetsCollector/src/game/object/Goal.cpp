//-----------------------------------------------
// Goal.cpp
// 制作日：2026/01/01
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "game/object/Goal.h"
#include "object/Player.h"
#include "game/component/GoalComponent.h"

#include "physics/BoxCollider3D.h"
#include "physics/RigidBody.h"

#include "component/renderer/ModelRendererComponent.h"
#include "game/component/LookAtTargetComponent.h"


//------------------------------------------------
// 初期化処理
//------------------------------------------------
void GOAL::Init()
{
	m_transform.SetSca(VECTOR3(2.0f, 2.0f, 2.0f));
	MODEL_RENDERER_COMPONENT* model = AddComponent<MODEL_RENDERER_COMPONENT>();
	model->SetModel("asset\\model\\panda.obj");
	// ゴールコンポーネントを追加
	AddComponent<GOAL_COMPONENT>();
	// ボックスコライダーを追加
	AddComponent<BOX_COLLIDER_3D>();
	// リジッドボディを追加
	auto rigid_body = AddComponent<RIGID_BODY>();
	rigid_body->SetUseGravity(false);
	rigid_body->SetType(RIGID_BODY_TYPE::STATIC);

	// 注視コンポーネントを追加
	LOOK_AT_TARGET_COMPONENT* look_at = AddComponent<LOOK_AT_TARGET_COMPONENT>();
	look_at->SetTargetObject(Manager::GetScene()->GetGameObject<PLAYER>());
	look_at->SetRotationSpeed(3.0f);
}