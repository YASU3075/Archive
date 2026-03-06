//-----------------------------------------------
// TimerUIComponent.h
// 制作日：2026/01/10
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "component/Component.h"

class SPRITE_RENDERER_COMPONENT;

class TIMER_UI_COMPONENT : public COMPONENT
{
private:
	SPRITE_RENDERER_COMPONENT* m_p_label_sprite_renderer = nullptr;
	SPRITE_RENDERER_COMPONENT* m_p_digit_sprite_renderers[2] = {};

	class TIMER_MANAGER_COMPONENT* m_p_timer_manager = nullptr; // タイマー管理コンポーネント

public:
	void Init() override;
	void Update() override;
	void SetLabelSpriteRenderer(SPRITE_RENDERER_COMPONENT* p_sprite_renderer) { m_p_label_sprite_renderer = p_sprite_renderer; }
	void SetDigitSpriteRenderer(int index, SPRITE_RENDERER_COMPONENT* p_sprite_renderer)
	{
		if (index >= 0 && index < 2)
		{
			m_p_digit_sprite_renderers[index] = p_sprite_renderer;
		}
	}
	void SetTimerManager(TIMER_MANAGER_COMPONENT* p_timer_manager) { m_p_timer_manager = p_timer_manager; }
	void InitSpriteRenderers();
};