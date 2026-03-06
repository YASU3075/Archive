//-----------------------------------------------
// ChocolateFountainParticle.cpp
// 制作日：2026/02/02
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "object/particle/ChocolateFountainParticle.h"

#include "particle/core/ParticleSystemComponent.h"

#include "particle/shape/SphereShape.h"

#include "particle/module/GravityModule.h"
#include "particle/module/SizeOverLifetimeModule.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void CHOCOLATE_FOUNTAIN_PARTICLE::Init()
{
	PARTICLE_SYSTEM_COMPONENT* particle_system = AddComponent<PARTICLE_SYSTEM_COMPONENT>();

	PARTICLE_EMITTER* emitter = particle_system->GetEmitter();

	emitter->m_emission_rate = 100.0f;
	emitter->m_particle_lifetime = 2.0f;
	emitter->m_looping = true;
	emitter->m_start_delay = 0.0f;

	// 形状指定
	emitter->m_shape = std::make_unique<SPHERE_SHAPE>(5.0f);

	emitter->m_base_vel = VECTOR3(-10.0f, 1.5f, 0.1f);
	emitter->m_max_vel = VECTOR3(10.0f, 3.0f, 0.2f);

	emitter->m_min_speed = 0.5f;
	emitter->m_max_speed = 1.0f;

	emitter->m_size_min = VECTOR3(1.0f, 1.0f, 1.0f);
	emitter->m_size_max = VECTOR3(2.0f, 2.0f, 2.0f);
	emitter->m_start_color_min = XMFLOAT4(0.31f, 0.09f, 0.02f, 1.0f);
	emitter->m_start_color_max = XMFLOAT4(0.31f, 0.09f, 0.02f, 0.5f);

	particle_system->GetModules().push_back(std::make_unique<GRAVITY_MODULE>(VECTOR3(0.0f, -9.8f, 0.0f)));
	
	particle_system->GetModules().push_back(std::make_unique<SIZE_OVER_LIFETIME_MODULE>(1.0f, 0.1f));

	particle_system->SetBlendMode(PARTICLE_BLEND_MODE::ALPHA);
}