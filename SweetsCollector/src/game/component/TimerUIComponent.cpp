//-----------------------------------------------
// TimerUIComponent.cpp
// 制作日：2026/01/10
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "game/component/TimerUIComponent.h"
#include "game/component/TimerManagerComponent.h"
#include "component/renderer/SpriteRendererComponent.h"

//------------------------------------------------
// 初期化処理
//------------------------------------------------
void TIMER_UI_COMPONENT::Init()
{
	InitSpriteRenderers();
}

//------------------------------------------------
// 更新処理
//------------------------------------------------
void TIMER_UI_COMPONENT::Update()
{
	if (m_p_timer_manager == nullptr)
	{
		return;
	}
	// 残り時間を計算
	double time_limit = m_p_timer_manager->GetTimeLimit();
	double elapsed_time = m_p_timer_manager->GetElapsedTime();
	double remaining_time = time_limit - elapsed_time;
	if (remaining_time < 0.0)
	{
		remaining_time = 0.0;
	}
	// 残り時間を秒単位で取得
	int seconds = static_cast<int>(remaining_time + 0.999); // 切り上げ
	// 2桁の数字に分割
	int tens = seconds / 10;
	int units = seconds % 10;
	// スプライトレンダラーに数字を設定
	if (m_p_digit_sprite_renderers[0] != nullptr)
	{
		m_p_digit_sprite_renderers[0]->SetSpriteIndex(tens);
	}
	if (m_p_digit_sprite_renderers[1] != nullptr)
	{
		m_p_digit_sprite_renderers[1]->SetSpriteIndex(units);
	}
}

//------------------------------------------------
// スプライトレンダラー初期化
//------------------------------------------------
void TIMER_UI_COMPONENT::InitSpriteRenderers()
{
	if (m_p_label_sprite_renderer)
	{
		m_p_label_sprite_renderer->SetLocalPos(VECTOR3(-SCREEN_WIDTH * 0.5f + 75.0f * 0.5f, -SCREEN_HEIGHT * 0.5f + 25.0f, 0.0f));
		m_p_label_sprite_renderer->SetLocalSca(VECTOR3(75.0f, 50.0f, 1.0f));
	}
	for (int i = 0; i < 2; i++)
	{
		if (m_p_digit_sprite_renderers[i])
		{
			m_p_digit_sprite_renderers[i]->SetLocalPos(VECTOR3(-SCREEN_WIDTH * 0.5f + 90.0f + 25.0f * i, -SCREEN_HEIGHT * 0.5f + 25.0f, 0.0f));
			m_p_digit_sprite_renderers[i]->SetLocalSca(VECTOR3(50.0f, 50.0f, 1.0f));
		}
	}
}