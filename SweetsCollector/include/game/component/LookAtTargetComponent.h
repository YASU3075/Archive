//-----------------------------------------------
// LookAtTargetComponent.h
// 制作日：2026/01/24
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "component/Component.h"

class LOOK_AT_TARGET_COMPONENT : public COMPONENT
{
private:
	class GAME_OBJECT* m_target_object;
	class RENDERER_COMPONENT* m_renderer_component;
	float m_rotation_speed = 1.0f;

public:
	void Init() override;
	void Update() override;

	void SetTargetObject(class GAME_OBJECT* target) { m_target_object = target; }
	void SetRotationSpeed(float speed) { m_rotation_speed = speed; }
};
