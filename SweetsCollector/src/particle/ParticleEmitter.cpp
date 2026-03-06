//-----------------------------------------------
// ParticleEmitter.cpp
// 制作日：2025/06/26
// 制作者：安田晴人
//-----------------------------------------------
#include <random>
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "particle/core/Particle.h"
#include "particle/core/ParticleEmitter.h"
#include "particle/shape/SphereShape.h"
#include "particle/shape/ConeShape.h"

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void PARTICLE_EMITTER::Update(float delta_time, std::vector<PARTICLE>& particles)
{
	if (!m_is_playing)
	{//プレイ中でない
		return;
	}

	//時間経過
	m_total_elapsed += delta_time;
	if (m_total_elapsed < m_start_delay)
	{//遅延処理
		return;
	}

	//実行時間
	float active_time = m_total_elapsed - m_start_delay;
	if (active_time >= m_dulation)
	{
		if (m_looping)
		{//ループするならリセット
			m_total_elapsed = m_start_delay;
			active_time = 0.0f;
			m_next_burst_index = 0;
			m_time_accumlator = 0.0f;
		}
		else
		{//一度きりなら終了
			m_is_playing = false;
			return;
		}
	}

	//BurstEvent処理
	while (m_next_burst_index < m_burst_events.size() && active_time >= m_burst_events[m_next_burst_index].m_time)
	{
		int c = m_burst_events[m_next_burst_index].m_count;
		for (int i = 0; i < c; i++)
		{
			PARTICLE p;
			Emit(p);
			particles.push_back(p);
		}
		m_next_burst_index++;
	}
	
	//通常Emit処理
	//時間蓄積
	m_time_accumlator += delta_time;

	if (m_emission_rate <= 0.0f)
	{//放出率が0以下なら放出しない
		return;
	}

	//放出間隔
	float interval = 1.0f / m_emission_rate;

	while (m_time_accumlator >= interval)
	{
		m_time_accumlator -= interval;
		PARTICLE p;
		Emit(p);
		particles.push_back(p);
	}
}

//-----------------------------------------------
// 再生
//-----------------------------------------------
void PARTICLE_EMITTER::Play()
{
	m_is_playing = true;
	m_total_elapsed = 0.0f;
	m_time_accumlator = 0.0f;
	m_next_burst_index = 0;
}

//-----------------------------------------------
// 停止
//-----------------------------------------------
void PARTICLE_EMITTER::Stop()
{
	m_is_playing = false;
}


//-----------------------------------------------
// 放出
//-----------------------------------------------
void PARTICLE_EMITTER::Emit(PARTICLE& p)
{
	VECTOR3 offset = m_shape ? m_shape->GetPositionOffset(m_rng) : VECTOR3(0.0f, 0.0f, 0.0f);
	p.m_pos = m_pos + offset;

	VECTOR3 dir = m_shape ? m_shape->GetDirection(m_rng) : VECTOR3(0.0f, 1.0f, 0.0f);
	p.m_vel = dir * RandomRange(m_min_speed, m_max_speed);

	p.m_vel += RandomVector(m_base_vel, m_max_vel - m_base_vel);

	p.m_acc = { 0.0f, 0.0f, 0.0f };

	p.m_age = 0;
	p.m_life_time = m_particle_lifetime;

	p.m_sca = p.m_initial_sca = RandomVector(m_size_min, m_size_max - m_size_min);
	p.m_color = p.m_initial_color = RandomColor(m_start_color_min, m_start_color_max);

	p.m_rot = RandomRange(0.0f, 360.0f);
	p.m_angular_velocity = RandomRange(-30.0f, 30.0f);
	p.m_alive = true;
}

//-----------------------------------------------
// float乱数
//-----------------------------------------------
float PARTICLE_EMITTER::RandomRange(float min, float max)
{
	if (min >= max)
	{
		std::swap(min, max);
	}

	std::uniform_real_distribution<float> dist(min, max);
	return dist(m_rng);
}

//-----------------------------------------------
// VECTOR3乱数
//-----------------------------------------------
VECTOR3 PARTICLE_EMITTER::RandomVector(const VECTOR3& base, const VECTOR3& range)
{
	return
	{
		base.x + RandomRange(0.0f, range.x),
		base.y + RandomRange(0.0f, range.y),
		base.z + RandomRange(0.0f, range.z),
	};
}

//-----------------------------------------------
// 色乱数
//-----------------------------------------------
XMFLOAT4 PARTICLE_EMITTER::RandomColor(const XMFLOAT4& min, const XMFLOAT4& max)
{
	return
	{
		RandomRange(min.x, max.x),
		RandomRange(min.y, max.y),
		RandomRange(min.z, max.z),
		RandomRange(min.w, max.w),
	};
}