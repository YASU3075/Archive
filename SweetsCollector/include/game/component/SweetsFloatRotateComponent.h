//-----------------------------------------------
// SweetsFloatRotateComponent.h
// 制作日：2026/01/20
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "component/Component.h"

class SWEETS_FLOAT_ROTATE_COMPONENT : public COMPONENT
{
private:
	class MODEL_RENDERER_COMPONENT* m_model_renderer_component = nullptr;	// モデルレンダラーコンポーネント
	float m_original_y = 0.0f;			// 元のY座標
	float m_floating_speed = 0.0f;		// 浮遊速度
	float m_rotate_speed = 0.0f;		// 回転速度
	float m_floating_range = 0.0f;		// 浮遊範囲
	bool m_is_going_up = true;		// 上昇中かどうか
public:
	void Init() override;
	void Update() override;

	void SetModelRendererComponent(class MODEL_RENDERER_COMPONENT* model_renderer_component) { m_model_renderer_component = model_renderer_component; }
	void SetOriginalY(float original_y) { m_original_y = original_y; }
	void SetFloatingSpeed(float floating_speed) { m_floating_speed = floating_speed; }
	void SetRotateSpeed(float rotate_speed) { m_rotate_speed = rotate_speed; }
	void SetFloatingRange(float floating_range) { m_floating_range = floating_range; }
};
