//-----------------------------------------------
// GravityModule.h
// 制作日：2025/07/02
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "ParticleModule.h"

class GRAVITY_MODULE : public PARTICLE_MODULE
{
private:
	VECTOR3 m_gravity = {0.0f, 9.8f, 0.0f};  // 重力ベクトル

public:
	GRAVITY_MODULE(const VECTOR3& gravity = { 0.0f, 9.8f, 0.0f }) : m_gravity(gravity) {}

	void Apply(PARTICLE& particle, float delta_time) override
	{
		if (!particle.m_alive)
		{
			return;
		}

		// 重力を適用
		particle.m_vel += m_gravity * delta_time;
	}

	std::unique_ptr<PARTICLE_MODULE> Clone() const override
	{
		return std::make_unique<GRAVITY_MODULE>(m_gravity);
	}
};