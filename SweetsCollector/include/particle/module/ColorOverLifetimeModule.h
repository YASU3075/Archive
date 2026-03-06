//-----------------------------------------------
// ColorOverLifetimeModule.h
// 制作日：2025/07/02
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "Main.h"
#include "renderer/Renderer.h"
#include "ParticleModule.h"

class COLOR_OVER_LIFETIME_MODULE : public PARTICLE_MODULE
{
private:
	XMFLOAT4 m_start_color;  // 初期色
	XMFLOAT4 m_end_color;    // 終了色

	XMFLOAT4 Lerp(const XMFLOAT4& start, const XMFLOAT4& end, float t) const
	{
		return
		{
			start.x + (end.x - start.x) * t,
			start.y + (end.y - start.y) * t,
			start.z + (end.z - start.z) * t,
			start.w + (end.w - start.w) * t,
		};
	}

public:
	COLOR_OVER_LIFETIME_MODULE(const XMFLOAT4& start_color, const XMFLOAT4& end_color)
		: m_start_color(start_color), m_end_color(end_color) {}

	//パーティクルに色を適用
	void Apply(PARTICLE& particle, float deltaTime) override
	{
		if (!particle.m_alive)
		{
			return;
		}

		float t = particle.m_age / particle.m_life_time; // 齢を寿命で割って0〜1の範囲に正規化
		if (t < 0.0f)
		{//範囲外の値をクリップ
			t = 0.0f;
		}
		else if (t > 1.0f)
		{
			t = 1.0f;
		}

		particle.m_color = Lerp(m_start_color, m_end_color, t);
	}

	std::unique_ptr<PARTICLE_MODULE> Clone() const override
	{
		return std::make_unique<COLOR_OVER_LIFETIME_MODULE>(m_start_color, m_end_color);
	}
};