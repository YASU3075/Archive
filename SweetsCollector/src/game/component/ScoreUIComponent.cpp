//-----------------------------------------------
// ScoreUIComponent.cpp
// 制作日：2026/01/01
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "game/component/ScoreUIComponent.h"
#include "game/component/ScoreManagerComponent.h"
#include "entity/GameObject.h"
#include "component/renderer/SpriteRendererComponent.h"

//------------------------------------------------
// 初期化処理
//------------------------------------------------
void SCORE_UI_COMPONENT::Init()
{
	if (m_p_score_manager == nullptr)
	{
		m_p_score_manager = GetOwner()->GetComponent<SCORE_MANAGER_COMPONENT>();
	}
}

//------------------------------------------------
// 更新処理
//------------------------------------------------
void SCORE_UI_COMPONENT::Update()
{
	if (m_p_score_manager == nullptr)
	{
		m_p_score_manager = GetOwner()->GetComponent<SCORE_MANAGER_COMPONENT>();
	}
	if (m_p_score_manager)
	{
		int score = m_p_score_manager->GetCurrentDisplayScore();
		// スコアを4桁の数字に分解して表示
		for (int i = 0; i < 4; i++)
		{
			if (m_p_digit_sprite_renderers[i])
			{
				int digit = (score / static_cast<int>(std::pow(10, 3 - i))) % 10;
				m_p_digit_sprite_renderers[i]->SetSpriteIndex(digit);
			}
		}
	}
}

//------------------------------------------------
// スプライトレンダラーの初期化
//------------------------------------------------
void SCORE_UI_COMPONENT::InitSpriteRenderers()
{
	if (m_p_label_sprite_renderer)
	{
		m_p_label_sprite_renderer->SetLocalPos(VECTOR3(SCREEN_WIDTH * 0.5f - 90.0f * 0.5f - 115.0f, -SCREEN_HEIGHT * 0.5f + 25.0f, 0.0f));
		m_p_label_sprite_renderer->SetLocalSca(VECTOR3(90.0f, 50.0f, 1.0f));
	}

	for (int i = 0; i < 4; i++)
	{
		if (m_p_digit_sprite_renderers[i])
		{
			m_p_digit_sprite_renderers[i]->SetLocalPos(VECTOR3(SCREEN_WIDTH * 0.5f - 100.0f + 25.0f * i, -SCREEN_HEIGHT * 0.5f + 25.0f, 0.0f));
			m_p_digit_sprite_renderers[i]->SetLocalSca(VECTOR3(50.0f, 50.0f, 1.0f));
			m_p_digit_sprite_renderers[i]->SetUVSheet(true);
			m_p_digit_sprite_renderers[i]->SetSpriteNum(5, 2); // 0-9の10種類
		}
	}
}
