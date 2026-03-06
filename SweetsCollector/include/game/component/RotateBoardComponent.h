//-----------------------------------------------
// RotateBoardComponent.h
// 制作日：2026/02/15
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "component/Component.h"
#include "math/Vector3.h"

class ROTATE_BOARD_COMPONENT : public COMPONENT
{
private:
	float m_current_angle = 0.0f;
	float m_previous_angle = 0.0f;
	float m_sum_angle = 0.0f;
	VECTOR3 m_spawn_range = VECTOR3(0.0f, 0.0f, 0.0f);

public:
	void Init() override;
	void Update() override;

	void SetSpawnRange(const VECTOR3& range) { m_spawn_range = range; }
};