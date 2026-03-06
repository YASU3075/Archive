//-----------------------------------------------
// PandaAnimationComponent.h
// 制作日：2026/01/29
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "math/Vector3.h"
#include "component/Component.h"

class PANDA_ANIMATION_COMPONENT : public COMPONENT
{
private:
	class MODEL_RENDERER_COMPONENT* m_model_renderer = nullptr; // モデルレンダラーコンポーネント
	VECTOR3 m_origin_sca = VECTOR3(1.0f, 1.0f, 1.0f); // 元のスケール
	float m_animation_speed = 1.0f; // アニメーション速度
	float m_timer = 0.0f;           // タイマー
public:
	void Init() override;
	void Update() override;

	void SetModelRenderer(class MODEL_RENDERER_COMPONENT* model_renderer) { m_model_renderer = model_renderer; }
};
