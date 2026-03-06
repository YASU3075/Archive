//-----------------------------------------------
// SweetsFloatRotateComponent.cpp
// 制作日：2026/01/20
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "entity/GameObject.h"
#include "game/component/SweetsFloatRotateComponent.h"
#include "component/renderer/ModelRendererComponent.h"
#include "EngineTime.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void SWEETS_FLOAT_ROTATE_COMPONENT::Init()
{
	if (m_model_renderer_component == nullptr)
	{
		m_model_renderer_component = m_p_owner->GetComponent<MODEL_RENDERER_COMPONENT>();
	}
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void SWEETS_FLOAT_ROTATE_COMPONENT::Update()
{
	if (!m_model_renderer_component)
	{
		return;
	}

	// 浮遊処理
	VECTOR3 position = m_model_renderer_component->GetLocalPos();
	float floating_offset = position.y - m_original_y;
	if (m_is_going_up)
	{
		floating_offset += m_floating_speed * TIME::DeltaTime();
		if (floating_offset >= m_floating_range)
		{
			floating_offset = m_floating_range;
			m_is_going_up = false;
		}
	}
	else
	{
		floating_offset -= m_floating_speed * TIME::DeltaTime();
		if (floating_offset <= -m_floating_range)
		{
			floating_offset = -m_floating_range;
			m_is_going_up = true;
		}
	}

	position.y = m_original_y + floating_offset;
	m_model_renderer_component->SetLocalPos(position);

	// 回転処理
	QUATERNION rotation = m_model_renderer_component->GetLocalRot();
	rotation *= QUATERNION::FromEuler(VECTOR3(0.0f, m_rotate_speed * TIME::DeltaTime(), 0.0f));
	m_model_renderer_component->SetLocalRot(rotation);
}