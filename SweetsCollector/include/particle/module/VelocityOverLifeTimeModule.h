//-----------------------------------------------
// VelocityOverLifeTimeModule.h
// 制作日：2025/07/02
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "ParticleModule.h"

class VELOCITY_OVER_LIFETIME_MODULE : public PARTICLE_MODULE
{
private:
	VECTOR3 m_start_velocity;  // 初期速度
	VECTOR3 m_end_velocity;    // 終了時の速度

	VECTOR3 Lerp(const VECTOR3& start, const VECTOR3& end, float t)
	{
		return VECTOR3(
			start.x + (end.x - start.x) * t,
			start.y + (end.y - start.y) * t,
			start.z + (end.z - start.z) * t
		);
	}

public:

	VELOCITY_OVER_LIFETIME_MODULE(VECTOR3 start_velocity, VECTOR3 end_velocity) : m_start_velocity(start_velocity), m_end_velocity(end_velocity) {}

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

		VECTOR3 velocity = Lerp(m_start_velocity, m_end_velocity, t);

		particle.m_vel = velocity;
	};

	std::unique_ptr<PARTICLE_MODULE> Clone() const override
	{
		return std::make_unique<VELOCITY_OVER_LIFETIME_MODULE>(m_start_velocity, m_end_velocity);
	}
};