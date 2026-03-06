//-----------------------------------------------
// PlayerAnimComponent.cpp
// 制作日：2026/01/24
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "game/component/PlayerAnimComponent.h"
#include "entity/GameObject.h"
#include "component/renderer/RendererComponent.h"
#include "EngineTime.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void PLAYER_ANIM_COMPONENT::Init()
{
	m_renderer = m_p_owner->GetComponent<RENDERER_COMPONENT>();
	m_base_local_pos = m_renderer->GetLocalPos();
	m_base_local_sca = m_renderer->GetLocalSca();

	m_time = 0.0f;
	m_move_target = 0.0f;
	m_move = 0.0f;
	m_shooting = false;
	m_shoot_t = 0.0f;
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void PLAYER_ANIM_COMPONENT::Update()
{
    if (!m_renderer)
	{// nullチェック
        return;
    }

	// 時間更新
    const float dt = TIME::DeltaTime();
    m_time += dt;

	// 移動量追従
    {
        float k = 1.0f - std::exp(-m_move_follow * dt);
        m_move = m_move + (m_move_target - m_move) * k;
    }

	// 待機アニメーション
    float idle_wave = std::sin(m_time * m_idle_freq);
    float idle_y = 1.0f + (m_idle_amp_y * idle_wave) * (1.0f - m_move);

	// 移動アニメーション
    float speed = m_move;

    // 目標周波数と振幅
    float freq_target = m_move_freq_min + (m_move_freq_max - m_move_freq_min) * speed;
    float amp = m_hop_amp_min + (m_hop_amp_max - m_hop_amp_min) * speed;

    // 周波数を平滑化
    {
        float follow = 10.0f; // 追従の強さ
        float k = 1.0f - std::exp(-follow * dt);
        m_move_freq_current = m_move_freq_current + (freq_target - m_move_freq_current) * k;
    }

    // 位相を積分
    m_move_phase += m_move_freq_current * dt;

    // 大きくなりすぎないように巻き戻し
    if (m_move_phase > 1000.0f)
    {
        m_move_phase = std::fmod(m_move_phase, 6.2831853f);
    }

	// hop量計算
    float move_wave = std::sin(m_move_phase);
    float hop = std::abs(move_wave) * amp * speed;


	// 撃つアニメーション
    float shoot_s = 1.0f;
    if (m_shooting)
    {
        m_shoot_t += dt;
        float t = (m_shoot_duration > 1e-6f) ? (m_shoot_t / m_shoot_duration) : 1.0f;

        if (t >= 1.0f)
        {
            m_shooting = false;
            shoot_s = 1.0f;
        }
        else
        {
            float pulse = std::sin(3.14159265f * t);
            shoot_s = 1.0f + m_shoot_amp * pulse;

            float rebound = std::sin(2.0f * 3.14159265f * t) * 0.08f;
            shoot_s *= (1.0f + rebound);
        }
    }

    
    VECTOR3 scale = m_base_local_sca;
    scale.x *= shoot_s;
    scale.z *= shoot_s;
    scale.y *= idle_y * shoot_s;

    VECTOR3 pos = m_base_local_pos;
    pos.y += hop;

   
    m_renderer->SetLocalPos(pos);
    m_renderer->SetLocalSca(scale);
}

//-----------------------------------------------
// 撃つアニメーションを開始
//-----------------------------------------------
void PLAYER_ANIM_COMPONENT::TriggerShootAnimation()
{
	m_shooting = true;
	m_shoot_t = 0.0f;
}