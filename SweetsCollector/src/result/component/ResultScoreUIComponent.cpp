//-----------------------------------------------
// ResultScoreUIComponent.cpp
// 制作日：2026/02/07
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "result/component/ResultScoreUIComponent.h"
#include "component/renderer/SpriteRendererComponent.h"
#include "EngineTime.h"

#include "object/Fade.h"
#include "component/SimpleFadeComponent.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void RESULT_SCORE_UI_COMPONENT::Init()
{
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void RESULT_SCORE_UI_COMPONENT::Update()
{
	if (!m_score_updated)
	{
		FADE* fade = Manager::GetScene()->GetGameObject<FADE>();
		SIMPLE_FADE_COMPONENT* fade_component = fade->GetComponent<SIMPLE_FADE_COMPONENT>();
		if (!fade_component->IsFading())
		{// フェードイン完了後にスコア更新を開始
			m_score_updated = true;
			m_target_score = Manager::GetScore();
		}
	}

	else
	{
		// 表示スコアを目標スコアに近づける
		if (m_displayed_score < m_target_score)
		{
			m_displayed_score += m_score_update_speed * TIME::DeltaTime();
			if (m_displayed_score > m_target_score)
			{
				m_displayed_score = m_target_score;
			}
		}
		else if (m_displayed_score > m_target_score)
		{
			m_displayed_score -= m_score_update_speed * TIME::DeltaTime();
			if (m_displayed_score < m_target_score)
			{
				m_displayed_score = m_target_score;
			}
		}

		// スコアの桁ごとにスプライトを更新
		int score = static_cast<int>(m_displayed_score);
		for (int i = 3; i >= 0; --i)
		{
			int digit = score % 10;
			score /= 10;
			if (m_p_digit_sprite_renderers[i])
			{// 桁のスプライトレンダラーが存在するなら更新
				m_p_digit_sprite_renderers[i]->SetSpriteIndex(digit);
			}
		}
	}
}

//-----------------------------------------------
// スプライトレンダラーの初期化
//-----------------------------------------------
void RESULT_SCORE_UI_COMPONENT::InitSpriteRenderers()
{
	// ラベルスプライトレンダラーの初期化
	if (m_p_label_sprite_renderer)
	{
		m_p_label_sprite_renderer->SetLocalPos(VECTOR3(0.0f, -SCREEN_HEIGHT * 0.5f + 100.0f, 0.0f));
		m_p_label_sprite_renderer->SetLocalSca(VECTOR3(400.0f, 200.0f, 1.0f));
	}
	// 各桁のスプライトレンダラーの初期化
	for (int i = 0; i < 4; ++i)
	{
		if (m_p_digit_sprite_renderers[i])
		{
			m_p_digit_sprite_renderers[i]->SetLocalPos(VECTOR3(-75.0f + 50.0f * i, -SCREEN_HEIGHT * 0.5f + 250.0f, 0.0f));
			m_p_digit_sprite_renderers[i]->SetLocalSca(VECTOR3(100.0f, 100.0f, 1.0f));
			m_p_digit_sprite_renderers[i]->SetUVSheet(true);
			m_p_digit_sprite_renderers[i]->SetSpriteNum(5, 2);
		}
	}
}