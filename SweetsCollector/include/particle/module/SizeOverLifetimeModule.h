//-----------------------------------------------
// SizeOverLifetimeModule.h
// 制作日：2025/07/02
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "ParticleModule.h"

class SIZE_OVER_LIFETIME_MODULE : public PARTICLE_MODULE
{
public:
	float m_start_scale = 0.0f;		//初期サイズ
	float m_end_scale = 1.0f;		//終了サイズ

	SIZE_OVER_LIFETIME_MODULE(float start_scale, float end_scale) : m_start_scale(start_scale), m_end_scale(end_scale) {};

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

		float scale = m_start_scale + (m_end_scale - m_start_scale) * t;
		particle.m_sca = particle.m_initial_sca * scale;
	}

	std::unique_ptr<PARTICLE_MODULE> Clone() const override
	{
		return std::make_unique<SIZE_OVER_LIFETIME_MODULE>(m_start_scale, m_end_scale);
	}
};