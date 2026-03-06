//-----------------------------------------------
// ResultScoreUIComponent.h
// 制作日：2026/02/07
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "component/Component.h"

class SPRITE_RENDERER_COMPONENT;

class RESULT_SCORE_UI_COMPONENT : public COMPONENT
{
private:
	SPRITE_RENDERER_COMPONENT* m_p_label_sprite_renderer = nullptr;
	SPRITE_RENDERER_COMPONENT* m_p_digit_sprite_renderers[4] = {};

	bool m_score_updated = false;
	float m_displayed_score = 0;
	int m_target_score = 0;
	float m_score_update_speed = 50.0f;

public:
	void Init() override;
	void Update() override;
	void InitSpriteRenderers();

	void SetLabelSpriteRenderer(SPRITE_RENDERER_COMPONENT* p_sprite_renderer) { m_p_label_sprite_renderer = p_sprite_renderer; }
	void SetDigitSpriteRenderer(int index, SPRITE_RENDERER_COMPONENT* p_sprite_renderer)
	{
		if (index >= 0 && index < 4)
		{
			m_p_digit_sprite_renderers[index] = p_sprite_renderer;
		}
	}
	void SetTargetScore(int score)
	{
		m_target_score = score;
		m_score_updated = true;
	}
};