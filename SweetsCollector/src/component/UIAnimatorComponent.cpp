//-----------------------------------------------
// UIAnimatorComponent.cpp
// 制作日：2026/02/10
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "entity/GameObject.h"
#include "component/UIAnimatorComponent.h"
#include "component/renderer/SpriteRendererComponent.h"
#include "renderer/Material.h"
#include "EngineTime.h"

//-----------------------------------------------
// ユーティリティ関数
//-----------------------------------------------
static float Clamp01(float x) { return std::max(0.0f, std::min(1.0f, x)); }
static float Lerp(float a, float b, float t) { return a + (b - a) * t; }

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void UI_ANIMATOR_COMPONENT::Init()
{
	m_p_sprite_renderer = m_p_owner->GetComponent<SPRITE_RENDERER_COMPONENT>();
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void UI_ANIMATOR_COMPONENT::Update()
{
	if (!m_p_sprite_renderer)
	{// nullチェック
		return;
	}

	if (!m_is_playing)
	{// 次のアクション開始
		if (m_actions.empty())
		{// アクションなし
			return;
		}
		m_current_action = m_actions.front();
		m_actions.pop_front();
		StartAction(m_current_action);
		m_is_playing = true;
	}

	if (StepAction(m_current_action, TIME::DeltaTime()))
	{// アクション完了
		m_is_playing = false;
	}
}

//-----------------------------------------------
// アクション開始
//-----------------------------------------------
void UI_ANIMATOR_COMPONENT::StartAction(ACTION& action)
{
	if (!m_p_sprite_renderer)
	{// nullチェック
		return;
	}

	m_current_action = action;
	m_current_action.t = 0.0f;
	m_is_playing = true;

	// 初期値設定
	switch (action.type)
	{
	case ACTION::TYPE::MOVE:
	{
		VECTOR3 current_pos = m_p_sprite_renderer->GetLocalPos();
		m_current_action.from_vec = current_pos;
	}
	break;
	case ACTION::TYPE::SCALE:
	{
		VECTOR3 current_scale = m_p_sprite_renderer->GetLocalSca();
		m_current_action.from_vec = current_scale;
	}
	break;
	case ACTION::TYPE::FADE:
	{
		if (m_p_sprite_renderer)
		{
			float current_alpha = m_p_sprite_renderer->GetMaterial()->m_material_data.m_diffuse.w;
			m_current_action.from_alpha = current_alpha;
		}
	}
	break;
	case ACTION::TYPE::MOVE_FADE:
	{
		VECTOR3 current_pos = m_p_sprite_renderer->GetLocalPos();
		m_current_action.from_pos = current_pos;
		if (m_p_sprite_renderer)
		{
			float current_alpha = m_p_sprite_renderer->GetMaterial()->m_material_data.m_diffuse.w;
			m_current_action.from_alpha = current_alpha;
		}
	}
	break;
	case ACTION::TYPE::WAIT:
	default:
		break;
	}
}

//-----------------------------------------------
// アクション進行
//-----------------------------------------------
bool UI_ANIMATOR_COMPONENT::StepAction(ACTION& action, float dt)
{
	action.t += dt;
	
	if (!m_p_sprite_renderer)
	{
		return true;
	}
	
	float u = Clamp01(action.t / action.duration);

	if (action.type == ACTION::TYPE::WAIT)
	{
		return u >= 1.0f;
	}

	if (action.type == ACTION::TYPE::MOVE)
	{
		float e = EASING::Ease(action.ease_type, u);
		m_p_sprite_renderer->SetLocalPos(VECTOR3::Lerp(action.from_vec, action.to_vec, e));
		return u >= 1.0f;
	}
	if (action.type == ACTION::TYPE::SCALE)
	{
		float e = EASING::Ease(action.ease_type, u);
		m_p_sprite_renderer->SetLocalSca(VECTOR3::Lerp(action.from_vec, action.to_vec, e));
		return u >= 1.0f;
	}
	if (action.type == ACTION::TYPE::FADE)
	{
		
		float e = EASING::Ease(action.ease_type, u);

		MATERIAL_DATA& mat_data = m_p_sprite_renderer->GetMaterial()->m_material_data;
		mat_data.m_diffuse.w = Lerp(action.from_alpha, action.to_alpha, e);
		return u >= 1.0f;
	}
	if (action.type == ACTION::TYPE::MOVE_FADE)
	{
		float e1 = EASING::Ease(action.ease_type, u);
		float e2 = EASING::Ease(action.ease_type2, u);

		m_p_sprite_renderer->SetLocalPos(VECTOR3::Lerp(action.from_pos, action.to_pos, e1));

		MATERIAL_DATA& mat_data = m_p_sprite_renderer->GetMaterial()->m_material_data;
		mat_data.m_diffuse.w = Lerp(action.from_a, action.to_a, e2);

		return u >= 1.0f;
	}

	return true;
}

//-----------------------------------------------
// すべてのアクションを削除
//-----------------------------------------------
void UI_ANIMATOR_COMPONENT::KillAll()
{
	m_actions.clear();
	m_is_playing = false;
}

//-----------------------------------------------
// 移動アクション追加
//-----------------------------------------------
void UI_ANIMATOR_COMPONENT::MoveTo(const VECTOR3& to, float time, EASE_TYPE ease)
{
	ACTION action;
	action.type = ACTION::TYPE::MOVE;
	action.to_vec = to;
	action.duration = time;
	action.ease_type = ease;
	m_actions.push_back(action);
}

//-----------------------------------------------
// 拡大縮小アクション追加
//-----------------------------------------------
void UI_ANIMATOR_COMPONENT::ScaleTo(const VECTOR3& to, float time, EASE_TYPE ease)
{
	ACTION action;
	action.type = ACTION::TYPE::SCALE;
	action.to_vec = to;
	action.duration = time;
	action.ease_type = ease;
	m_actions.push_back(action);
}

//-----------------------------------------------
// フェードアクション追加
//-----------------------------------------------
void UI_ANIMATOR_COMPONENT::FadeTo(float to_alpha, float time, EASE_TYPE ease)
{
	ACTION action;
	action.type = ACTION::TYPE::FADE;
	action.to_alpha = to_alpha;
	action.duration = time;
	action.ease_type = ease;
	m_actions.push_back(action);
}

//-----------------------------------------------
// 移動＆フェードアクション追加
//-----------------------------------------------
void UI_ANIMATOR_COMPONENT::MoveAndFadeTo(const VECTOR3& to_pos, float to_alpha, float time, EASE_TYPE ease_pos, EASE_TYPE ease_alpha)
{
	ACTION action;
	action.type = ACTION::TYPE::MOVE_FADE;
	action.to_pos = to_pos;
	action.to_a = to_alpha;
	action.duration = time;
	action.ease_type = ease_pos;
	action.ease_type2 = ease_alpha;
	m_actions.push_back(action);
}

//-----------------------------------------------
// 待機アクション追加
//-----------------------------------------------
void UI_ANIMATOR_COMPONENT::Wait(float time)
{
	ACTION action;
	action.type = ACTION::TYPE::WAIT;
	action.duration = time;
	m_actions.push_back(action);
}