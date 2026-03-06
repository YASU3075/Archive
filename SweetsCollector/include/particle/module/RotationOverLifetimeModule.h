//-----------------------------------------------
// RotationOverLifetimeModule.h
// 制作日：2025/07/02
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "ParticleModule.h"

class ROTATION_OVER_LIFETIME_MODULE : public PARTICLE_MODULE
{
private:
	float m_start_angllar_velocity = 0.0f;  // 初期角速度
	float m_end_angllar_velocity = 0.0f;    // 終了時の角速度

public:
	ROTATION_OVER_LIFETIME_MODULE(float start_angllar_velocity, float end_angllar_velocity)
		: m_start_angllar_velocity(start_angllar_velocity), m_end_angllar_velocity(end_angllar_velocity) {}

	void Apply(PARTICLE& particle, float delta_time) override
	{
		if (!particle.m_alive)
		{
			return;
		}

		float t = particle.m_age / particle.m_life_time;
		if (t < 0.0f)
		{
			t = 0.0f;
		}
		else if (t > 1.0f)
		{
			t = 1.0f;
		}

		float angular_velocity = m_start_angllar_velocity + (m_end_angllar_velocity - m_start_angllar_velocity) * t;

		particle.m_rot += angular_velocity * delta_time; // 角速度を回転に適用
	}

	std::unique_ptr<PARTICLE_MODULE> Clone() const override
	{
		return std::make_unique<ROTATION_OVER_LIFETIME_MODULE>(m_start_angllar_velocity, m_end_angllar_velocity);
	}
	
};