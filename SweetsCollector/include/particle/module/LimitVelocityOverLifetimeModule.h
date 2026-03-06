//-----------------------------------------------
// LimitVelocityOverLifetimeModule.h
// 制作日：2025/07/02
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "ParticleModule.h"

class LIMIT_VELOCITY_OVER_LIFETIME_MODULE : public PARTICLE_MODULE
{
private:
	float m_limit = 3.0f;		// 速度制限値
	float m_dampen = 0.8f;		// 減衰率

	VECTOR3 Lerp(const VECTOR3& start, const VECTOR3& end, float t)
	{
		return VECTOR3(
			start.x + (end.x - start.x) * t,
			start.y + (end.y - start.y) * t,
			start.z + (end.z - start.z) * t
		);
	}

public:
	LIMIT_VELOCITY_OVER_LIFETIME_MODULE(float limit = 3.0f, float dampen = 0.8f)
		: m_limit(limit), m_dampen(dampen) {}

	void Apply(PARTICLE& particle, float delta_time) override
	{
		if (!particle.m_alive)
		{
			return;
		}

		float speed = particle.m_vel.length();
		if (speed > m_limit)
		{
			VECTOR3 direction = particle.m_vel.GetNormalized() * m_limit;
			particle.m_vel = Lerp(particle.m_vel, direction, 1.0f - m_dampen);
		}
	};

	std::unique_ptr<PARTICLE_MODULE> Clone() const override
	{
		return std::make_unique<LIMIT_VELOCITY_OVER_LIFETIME_MODULE>(m_limit, m_dampen);
	}
};