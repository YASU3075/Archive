//-----------------------------------------------
// ParticleSystemComponent.cpp
// 制作日：2026/02/02
// 制作者：安田晴人
//-----------------------------------------------
#include <algorithm>
#include "Main.h"
#include "renderer/Renderer.h"

#include "entity/GameObject.h"
#include "renderer/MaterialManager.h"

#include "EngineTime.h"

#include "particle/core/ParticleSystemComponent.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void PARTICLE_SYSTEM_COMPONENT::Init()
{
	// レンダーパス設定
    m_render_pass = RENDER_PASS::RENDER_PASS_PARTICLE;

	// エミッタ、レンダラ生成
    m_emitter = std::make_unique<PARTICLE_EMITTER>();
    m_renderer = std::make_unique<PARTICLE_RENDERER>();

	// マテリアル読み込み
    m_p_material = MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\ParticleMaterial.mtl");
    if (m_p_material)
    {
        m_renderer->SetMaterial(m_p_material);
    }

	// レンダラ初期化
    m_renderer->Init("asset\\material\\ParticleMaterial.mtl");

	SetBlendMode(m_blend_mode);

    if (m_play_on_start)
	{// 最初から再生するなら
        Play();
    }
}

//-----------------------------------------------
// 終了処理
//-----------------------------------------------
void PARTICLE_SYSTEM_COMPONENT::Uninit()
{
    m_particles.clear();
    m_modules.clear();
    m_renderer.reset();
    m_emitter.reset();
}

//-----------------------------------------------
// 再生開始
//-----------------------------------------------
void PARTICLE_SYSTEM_COMPONENT::Play()
{
    if (!m_emitter)
	{// エミッタなし
        return;
    }

    if (m_follow_owner && m_p_owner)
	{// オーナーに追従するなら
        VECTOR3 pos = m_p_owner->GetTransform()->GetPos();
        pos += m_local_pos;
        m_emitter->m_pos = pos;
    }

	// エミッタ再生
    m_emitter->Play();
}

//-----------------------------------------------
// 再生停止
//-----------------------------------------------
void PARTICLE_SYSTEM_COMPONENT::Stop()
{
    if (!m_emitter)
	{// エミッタなし
        return;
    }

	// エミッタ停止
    m_emitter->Stop();
}

//-----------------------------------------------
// 生存確認
//-----------------------------------------------
bool PARTICLE_SYSTEM_COMPONENT::IsAlive() const
{
    if (!m_emitter)
    {
        return false;
    }

    return (!m_particles.empty() || m_emitter->m_looping);
}

//-----------------------------------------------
// ブレンドモード設定
//-----------------------------------------------
void PARTICLE_SYSTEM_COMPONENT::SetBlendMode(PARTICLE_BLEND_MODE mode)
{
	m_blend_mode = mode;

	if (!m_renderer)
	{// レンダラなし
		return;
	}

	switch (m_blend_mode)
	{
	case PARTICLE_BLEND_MODE::ADD:
        m_render_pass = RENDER_PASS::RENDER_PASS_PARTICLE;
		break;
	case PARTICLE_BLEND_MODE::ALPHA:
        m_render_pass = RENDER_PASS::RENDER_PASS_TRANSPARENT;
		break;
	default:
		break;
	}
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void PARTICLE_SYSTEM_COMPONENT::Update()
{
    if (!m_emitter)
	{// エミッタなし
        return;
    }

	// DeltaTime取得
    const float dt = (float)TIME::DeltaTime();

    if (m_follow_owner && m_p_owner)
	{// オーナーに追従するなら
        VECTOR3 pos = m_p_owner->GetTransform()->GetPos();
        pos += m_local_pos;
        m_emitter->m_pos = pos;
    }

	// エミッタ更新
    m_emitter->Update(dt, m_particles);

	// 通常モジュール適用
    for (auto& p : m_particles)
    {
        if (!p.m_alive)
		{// 生存していない
            continue;
        }

		// 基本更新
        p.Update(dt);

		// モジュール適用
        for (auto& module : m_modules)
        {
            if (!module->IsEventModule())
			{// 通常モジュール
                module->Apply(p, dt);
            }
        }
    }

	// イベントモジュール適用
    for (auto& p : m_particles)
    {
        for (auto& module : m_modules)
        {
            if (module->IsEventModule())
			{// イベントモジュール
                module->Apply(p, dt);
            }
        }
    }

	// 死んだ粒子を削除
    m_particles.erase(
        std::remove_if(m_particles.begin(), m_particles.end(),
            [](const PARTICLE& p) { return !p.m_alive; }),
        m_particles.end()
    );
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void PARTICLE_SYSTEM_COMPONENT::Draw()
{
    if (!m_renderer)
	{// レンダラなし
        return;
    }

    if (m_p_material)
	{// マテリアル設定
        m_renderer->SetMaterial(m_p_material);
    }

	// 描画
    m_renderer->Draw(m_particles);
}