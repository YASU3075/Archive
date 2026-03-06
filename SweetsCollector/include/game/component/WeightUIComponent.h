//-----------------------------------------------
// WeightUIComponent.h
// 制作日：2026/01/21
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "component/Component.h"
class SPRITE_RENDERER_COMPONENT;

class WEIGHT_UI_COMPONENT : public COMPONENT
{
private:
	SPRITE_RENDERER_COMPONENT* m_p_label_sprite_renderer = nullptr;
	SPRITE_RENDERER_COMPONENT* m_p_sprite_renderer_component[3] = {};
	class SWEETS_MANAGEMENT_COMPONENT* m_p_sweets_manager = nullptr; // スイーツ管理コンポーネント
public:
	void Init() override;
	void Update() override;

	void SetLabelSpriteRenderer(SPRITE_RENDERER_COMPONENT* p_sprite_renderer) { m_p_label_sprite_renderer = p_sprite_renderer; }
	void SetSpriteRenderer(int index, SPRITE_RENDERER_COMPONENT* p_sprite_renderer)
	{
		if (index >= 0 && index < 3)
		{
			m_p_sprite_renderer_component[index] = p_sprite_renderer;
		}
	}
	void SetSweetsManager(class SWEETS_MANAGEMENT_COMPONENT* p_sweets_manager) { m_p_sweets_manager = p_sweets_manager; }
	void InitSpriteRenderers();
};