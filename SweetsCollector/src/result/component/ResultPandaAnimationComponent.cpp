//-----------------------------------------------
// ResultPandaAnimationComponent.cpp
// 制作日：2026/02/07
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "result/component/ResultPandaAnimationComponent.h"
#include "component/renderer/ModelRendererComponent.h"
#include "math/Easing.h"
#include "EngineTime.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void RESULT_PANDA_ANIMATION_COMPONENT::Init()
{
	m_model_renderer = GetOwner()->GetComponent<MODEL_RENDERER_COMPONENT>();

	if (m_model_renderer)
	{
		m_original_local_pos = m_model_renderer->GetLocalPos();
	}

	m_animation_timer = 0.0f;
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void RESULT_PANDA_ANIMATION_COMPONENT::Update()
{
	if (!m_model_renderer)
	{
		return;
	}

	m_animation_timer += TIME::DeltaTime();
	m_animation_timer = fmodf(m_animation_timer, m_animation_interval);

	VECTOR3 hop_offset = VECTOR3(0.0f, 0.0f, 0.0f);
	float half_interval = m_animation_interval / 2.0f;
	if (m_animation_timer < half_interval)
	{
		// 上昇フェーズ
		float t = m_animation_timer / half_interval;
		t = EASING::EaseOutQuad(t);
		hop_offset.y = t * m_hop_height;
	}
	else
	{
		// 下降フェーズ
		float t = (m_animation_timer - half_interval) / half_interval;
		t = EASING::EaseInQuad(t);
		hop_offset.y = (1.0f - t) * m_hop_height;
	}
	m_model_renderer->SetLocalPos(m_original_local_pos + hop_offset);
}