//-----------------------------------------------
// NoiseModule.h
// 制作日：2025/07/02
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "ParticleModule.h"
#include "math/PerlinNoise.h"

class NOISE_MODULE : public PARTICLE_MODULE
{
private:
	PERLIN_NOISE m_noise_generator;  // パーリンノイズ生成器
	float m_strength = 1.0f;		// ノイズの強さ
	float m_frequency = 0.5f;		// 空間スケール
	float m_time_scale = 0.2f;		// 時間スケール

public:
	NOISE_MODULE(float strength = 1.0f, float frequency = 0.5f, float time_scale = 0.2f)
		: m_strength(strength), m_frequency(frequency), m_time_scale(time_scale) {}

	void Apply(PARTICLE& particle, float delta_time) override
	{
		if (!particle.m_alive)
		{
			return;
		}

		float t = particle.m_age * m_time_scale; // 時間に基づくノイズの変化
		
		float nx = m_noise_generator.Noise(particle.m_pos.y * m_frequency, t);
		float ny = m_noise_generator.Noise(particle.m_pos.z * m_frequency, t + 31.7f);
		float nz = m_noise_generator.Noise(particle.m_pos.x * m_frequency, t + 78.1f);

		VECTOR3 nvec = VECTOR3(nx, ny, nz) * m_strength;
		particle.m_vel += nvec * delta_time; // ノイズを速度に加える
	}

	std::unique_ptr<PARTICLE_MODULE> Clone() const override
	{
		return std::make_unique<NOISE_MODULE>(m_strength, m_frequency, m_time_scale);
	}

};
