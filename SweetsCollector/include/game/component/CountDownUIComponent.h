//-----------------------------------------------
// CountDownUIComponent.h
// 制作日：2026/01/26
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "component/Component.h"

enum class COUNT_DOWN_UI_STATE
{
	COUNT_DOWN_UI_STATE_NONE,
	COUNT_DOWN_UI_STATE_THREE,
	COUNT_DOWN_UI_STATE_TWO,
	COUNT_DOWN_UI_STATE_ONE,
	COUNT_DOWN_UI_STATE_GO,
	COUNT_DOWN_UI_STATE_DONE,
};

class SPRITE_RENDERER_COMPONENT;

class COUNT_DOWN_UI_COMPONENT : public COMPONENT
{
private:
	COUNT_DOWN_UI_STATE m_state = COUNT_DOWN_UI_STATE::COUNT_DOWN_UI_STATE_NONE;
	float m_time = 0.0f;
	const float dulation = 1.0f;

	SPRITE_RENDERER_COMPONENT* m_p_label_sprite_renderer = nullptr;
	SPRITE_RENDERER_COMPONENT* m_p_number_sprite_renderer = nullptr;

public:
	void Init() override;
	void Update() override;

	void SetLabelSpriteRenderer(SPRITE_RENDERER_COMPONENT* p_sprite_renderer) { m_p_label_sprite_renderer = p_sprite_renderer; }
	void SetNumberSpriteRenderer(SPRITE_RENDERER_COMPONENT* p_sprite_renderer) { m_p_number_sprite_renderer = p_sprite_renderer; }

	void InitSpriteRenderers();
};
