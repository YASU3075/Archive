//-----------------------------------------------
// ResultPandaAnimationComponent.h
// 制作日：2026/02/07
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "component/Component.h"

class RESULT_PANDA_ANIMATION_COMPONENT : public COMPONENT
{
private:
	class MODEL_RENDERER_COMPONENT* m_model_renderer = nullptr;

	float m_animation_timer = 0.0f;
	float m_animation_interval = 1.0f;
	float m_hop_height = 1.0f;

	VECTOR3 m_original_local_pos = VECTOR3(0.0f, 0.0f, 0.0f);

public:
	void Init() override;
	void Update() override;
	void SetAnimationInterval(float interval) { m_animation_interval = interval; }
	void SetModelRenderer(MODEL_RENDERER_COMPONENT* model_renderer) { m_model_renderer = model_renderer; }
};