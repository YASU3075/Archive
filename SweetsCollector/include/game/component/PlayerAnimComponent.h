//-----------------------------------------------
// PlayerAnimComponent.h
// 制作日：2026/01/24
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <algorithm>
#include "math/Vector3.h"
#include "component/Component.h"

class PLAYER_ANIM_COMPONENT : public COMPONENT
{
private:
	class RENDERER_COMPONENT* m_renderer;

	VECTOR3 m_base_local_pos = VECTOR3(0.0f, 0.0f, 0.0f);
	VECTOR3 m_base_local_sca = VECTOR3(1.0f, 1.0f, 1.0f);

	float m_time = 0.0f;

    float m_move_target = 0.0f;
    float m_move = 0.0f;
    float m_move_follow = 12.0f;

    float m_idle_amp_y = 0.05f;
    float m_idle_freq = 4.0f;

    float m_move_amp = 0.08f;
    float m_move_freq = 7.0f;

    bool  m_shooting = false;
    float m_shoot_t = 0.0f;
    float m_shoot_duration = 0.18f;
    float m_shoot_amp = 0.18f;

    float m_move_freq_min = 1.0f;
    float m_move_freq_max = 5.0f;

    float m_hop_amp_min = 0.1f;
    float m_hop_amp_max = 0.5f;

    float m_move_phase = 0.0f;
    float m_move_freq_current = 10.0f;

    float m_hop_amp = 0.5f;

public:
	void Init() override;
	void Update() override;

    void SetMoveAmount(float amount) { m_move_target = std::clamp(amount, 0.0f, 1.0f); }
	void TriggerShootAnimation();
};