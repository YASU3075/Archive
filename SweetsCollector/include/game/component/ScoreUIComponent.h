//-----------------------------------------------
// ScoreUIComponent.h
// 制作日：2026/01/01
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "math/Vector3.h"
#include "component/Component.h"

class GAME_OBJECT;
class SPRITE_RENDERER_COMPONENT;

class SCORE_UI_COMPONENT : public COMPONENT
{
private:
	SPRITE_RENDERER_COMPONENT* m_p_label_sprite_renderer = nullptr;
	SPRITE_RENDERER_COMPONENT* m_p_digit_sprite_renderers[4] = {};

	class SCORE_MANAGER_COMPONENT* m_p_score_manager = nullptr; // スコア管理コンポーネント
public:
	void Init() override;
	void Update() override;

	void SetLabelSpriteRenderer(SPRITE_RENDERER_COMPONENT* p_sprite_renderer) { m_p_label_sprite_renderer = p_sprite_renderer; }
	void SetDigitSpriteRenderer(int index, SPRITE_RENDERER_COMPONENT* p_sprite_renderer)
	{
		if (index >= 0 && index < 4)
		{
			m_p_digit_sprite_renderers[index] = p_sprite_renderer;
		}
	}
	void SetScoreManager(SCORE_MANAGER_COMPONENT* p_score_manager) { m_p_score_manager = p_score_manager; }

	void InitSpriteRenderers();
};