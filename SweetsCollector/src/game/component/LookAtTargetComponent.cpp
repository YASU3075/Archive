//-----------------------------------------------
// LookAtTargetComponent.cpp
// 制作日：2026/01/24
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "game/component/LookAtTargetComponent.h"
#include "entity/GameObject.h"
#include "component/renderer/RendererComponent.h"
#include "EngineTime.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void LOOK_AT_TARGET_COMPONENT::Init()
{
	m_target_object = nullptr;
	m_renderer_component = m_p_owner->GetComponent<RENDERER_COMPONENT>();
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void LOOK_AT_TARGET_COMPONENT::Update()
{
	if (m_target_object == nullptr)
	{// nullチェック
		return;
	}
	if (m_renderer_component == nullptr)
	{// nullチェック
		return;
	}

	// 座標取得
	VECTOR3 pos = m_p_owner->GetTransform()->GetPos();
	VECTOR3 target_pos = m_target_object->GetTransform()->GetPos();

	// 方向ベクトル計算
	VECTOR3 direction = (target_pos - pos).GetNormalized();

	// 回転補間
	QUATERNION target_rot = QUATERNION::LookRotation(direction, VECTOR3(0.0f, 1.0f, 0.0f));
	QUATERNION current_rot = m_renderer_component->GetLocalRot();
	m_renderer_component->SetLocalRot(QUATERNION::Slerp(current_rot, target_rot, m_rotation_speed * TIME::DeltaTime()));
}