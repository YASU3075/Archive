//-----------------------------------------------
// PandaAnimationComponent.cpp
// 制作日：2026/01/29
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "entity/GameObject.h"
#include "title/component/PandaAnimationComponent.h"
#include "component/renderer/ModelRendererComponent.h"
#include "EngineTime.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void PANDA_ANIMATION_COMPONENT::Init()
{
	m_timer = 0.0f;

	m_model_renderer = GetOwner()->GetComponent<MODEL_RENDERER_COMPONENT>();
	if (m_model_renderer != nullptr)
	{
		m_origin_sca = m_model_renderer->GetLocalSca();
	}
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void PANDA_ANIMATION_COMPONENT::Update()
{
	if (m_model_renderer == nullptr)
	{
		return;
	}

	// タイマー更新
	m_timer += TIME::DeltaTime() * m_animation_speed;
	if (m_timer >= 2.0f * 3.14159f)
	{
		m_timer -= 2.0f * 3.14159f;
	}

	
	float scale_factor = 1.0f + 0.01f * std::sin(m_timer);
	VECTOR3 new_scale = m_origin_sca * scale_factor;
	m_model_renderer->SetLocalSca(new_scale);
}