//-----------------------------------------------
// TimeUpUIComponent.cpp
// 制作日：2026/01/19
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "entity/GameObject.h"
#include "game/component/TimeUpUIComponent.h"
#include "component/renderer/SpriteRendererComponent.h"
#include "EngineTime.h"
#include "math/Easing.h"
#include "object/Fade.h"
#include "component/SimpleFadeComponent.h"
#include "game/object/ScoreUI.h"
#include "game/component/ScoreManagerComponent.h"

//------------------------------------------------
// 初期化処理
//------------------------------------------------
void TIME_UP_UI_COMPONENT::Init()
{
	m_time = 0.0f;
	m_duration = 1.0f;
}

//------------------------------------------------
// 更新処理
//------------------------------------------------
void TIME_UP_UI_COMPONENT::Update()
{
	if (!m_is_active)
	{
		return;
	}

	// 経過時間更新
	m_time += TIME::DeltaTime();
	float t = m_time / m_duration;

	if (t > 1.0f)
	{// 終了
		t = 1.0f;

		FADE* p_fade = Manager::GetScene()->GetGameObject<FADE>();
		if (p_fade)
		{
			SIMPLE_FADE_COMPONENT* p_simple_fade_component = p_fade->GetComponent<SIMPLE_FADE_COMPONENT>();
			if (p_simple_fade_component)
			{
				p_simple_fade_component->StartFadeOut();
			}
		}

		SCORE_UI* p_score_ui = Manager::GetScene()->GetGameObject<SCORE_UI>();
		if (p_score_ui)
		{
			SCORE_MANAGER_COMPONENT* p_score_manager = p_score_ui->GetComponent<SCORE_MANAGER_COMPONENT>();
			if (p_score_manager)
			{
				int final_score = p_score_manager->GetScore();
				Manager::SetScore(final_score);
			}
		}
	}

	// イージング適用
	float ease_t = EASING::EaseOutBack(t);

	// 位置補間
	VECTOR3 new_position = VECTOR3::Lerp(m_start_position, m_target_position, ease_t);
	m_p_owner->GetTransform()->SetPos(new_position);
}