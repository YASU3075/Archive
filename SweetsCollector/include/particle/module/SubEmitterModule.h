//-----------------------------------------------
// SubEmitterModule.h
// 制作日：2025/07/03
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include "particle/core/ParticleSystemManager.h"
#include "ParticleModule.h"

enum class SUB_EMITTER_TRIGGER
{
	OnSpawn,    // パーティクルが生成されたとき
	OnDeath,    // パーティクルが消滅するとき
};


class SUB_EMITTER_MODULE : public PARTICLE_MODULE
{
private:
	bool ShouldSpawn(const PARTICLE& p) const 
	{
		switch (m_trigger) 
		{
		case SUB_EMITTER_TRIGGER::OnSpawn:
			return !p.m_sub_emitted; // パーティクルがサブエミッターを発火していない場合
		case SUB_EMITTER_TRIGGER::OnDeath:
			return p.m_alive == false && !p.m_sub_emitted; // パーティクルが死亡していて、サブエミッターを発火していない場合
		}
	}

public:
	std::string m_name = "Template";		// テンプレート名
	PARTICLE_SYSTEM* m_system = nullptr; // サブエミッターのシステム
	SUB_EMITTER_TRIGGER m_trigger = SUB_EMITTER_TRIGGER::OnSpawn; // トリガー条件
	bool m_inherit_velocity = true; // 親パーティクルの速度を継承するかどうか

	SUB_EMITTER_MODULE(std::string name, PARTICLE_SYSTEM* system, SUB_EMITTER_TRIGGER trigger, bool inherit_velocity = true)
		: m_name(name), m_system(system), m_trigger(trigger), m_inherit_velocity(inherit_velocity) {}

	void Apply(PARTICLE& particle, float delta_time) override
	{
		if (ShouldSpawn(particle)) {
			if (m_system) {
				PARTICLE_SYSTEM_MANAGER::GetInstance().SpawnFromTemplate(
					m_name,
					particle.m_pos,
					m_inherit_velocity ? particle.m_vel : VECTOR3(0.0f, 0.0f, 0.0f)
				);
				particle.m_sub_emitted = true;
			}
		}
	}

	bool IsEventModule() const override
	{
		return true; // サブエミッターモジュールはイベントモジュールとして扱う
	}

	std::unique_ptr<PARTICLE_MODULE> Clone() const override
	{
		return std::make_unique<SUB_EMITTER_MODULE>(*this); // クローンを生成
	}
};