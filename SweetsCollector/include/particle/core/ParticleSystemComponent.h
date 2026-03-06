//-----------------------------------------------
// ParticleSystemComponent.h
// 制作日：2026/02/02
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include <memory>
#include <vector>

#include "component/renderer/RendererComponent.h"
#include "particle/core/Particle.h"
#include "particle/core/ParticleEmitter.h"
#include "particle/core/ParticleRenderer.h"
#include "particle/module/ParticleModule.h"

enum class PARTICLE_BLEND_MODE
{
	ADD,
	ALPHA,
};

class PARTICLE_SYSTEM_COMPONENT : public RENDERER_COMPONENT
{
private:
	std::unique_ptr<PARTICLE_EMITTER> m_emitter;
	std::unique_ptr<PARTICLE_RENDERER> m_renderer;

	std::vector<PARTICLE> m_particles;
	std::vector<std::unique_ptr<PARTICLE_MODULE>> m_modules;

	bool m_play_on_start = true;
	bool m_follow_owner = true;

	PARTICLE_BLEND_MODE m_blend_mode = PARTICLE_BLEND_MODE::ADD;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	XMMATRIX BuildWorldMatrix() const override { return XMMatrixIdentity(); };
	void BindGeometry() const override {};
	void DrawGeometyrOnly() const override {};

	void Play();
	void Stop();
	bool IsAlive() const;

	PARTICLE_EMITTER* GetEmitter() const { return m_emitter.get(); }
	PARTICLE_RENDERER* GetParticleRenderer() const { return m_renderer.get(); }
	std::vector<std::unique_ptr<PARTICLE_MODULE>>& GetModules() { return m_modules; }

	void SetPlayOnStart(bool play) { m_play_on_start = play; }
	void SetFollowOwner(bool follow) { m_follow_owner = follow; }

	void SetBlendMode(PARTICLE_BLEND_MODE mode);
	PARTICLE_BLEND_MODE GetBlendMode() const { return m_blend_mode; }
};
