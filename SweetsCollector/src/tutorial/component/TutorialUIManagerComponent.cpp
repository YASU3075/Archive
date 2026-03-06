//-----------------------------------------------
// TutorialUIManagerComponent.cpp
// 制作日：2026/02/11
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "entity/GameObject.h"
#include "tutorial/component/TutorialUIManagerComponent.h"
#include "component/UIAnimatorComponent.h"
#include "component/renderer/SpriteRendererComponent.h"
#include "renderer/Material.h"
#include "renderer/MaterialManager.h"
#include "EngineTime.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void TUTORIAL_UI_MANAGER_COMPONENT::Init()
{
	m_ui_animator_component = m_p_owner->GetComponent<UI_ANIMATOR_COMPONENT>();
	m_sprite_renderer_component = m_p_owner->GetComponent<SPRITE_RENDERER_COMPONENT>();

	if (m_ui_animator_component == nullptr)
	{// nullチェック
		assert(false);
		return;
	}
	// スプライトレンダラーコンポーネントが無い場合はエラー
	if (m_sprite_renderer_component == nullptr)
	{
		assert(false);
		return;
	}

	// 元のスケールを保存
	m_original_scale = m_sprite_renderer_component->GetLocalSca();

	m_ui_animator_component->SetTargetSpriteRenderer(m_sprite_renderer_component);

	STEP_UI_PARAM move{};
	move.material = MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\MoveTextMaterial.mtl");
	move.in_pos = VECTOR3(0.0f, -SCREEN_HEIGHT * 0.5f - 300.0f, 0.0f);
	move.show_pos = VECTOR3(0.0f, SCREEN_HEIGHT * 0.5f - 250.0f, 0.0f);
	move.out_pos = VECTOR3(0.0f, -SCREEN_HEIGHT * 0.5f - 300.0f, 0.0f);
	m_params[TUTORIAL_STATE::MOVE] = move;

	STEP_UI_PARAM weight{};
	weight.material = MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\WeightTextMaterial.mtl");
	weight.in_pos = VECTOR3(0.0f, -SCREEN_HEIGHT * 0.5f - 300.0f, 0.0f);
	weight.show_pos = VECTOR3(0.0f, SCREEN_HEIGHT * 0.5f - 250.0f, 0.0f);
	weight.out_pos = VECTOR3(0.0f, -SCREEN_HEIGHT * 0.5f - 300.0f, 0.0f);
	m_params[TUTORIAL_STATE::WEIGHT] = weight;

	STEP_UI_PARAM shoot_to_goal{};
	shoot_to_goal.material = MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\ShootTextMaterial.mtl");
	shoot_to_goal.in_pos = VECTOR3(0.0f, -SCREEN_HEIGHT * 0.5f - 300.0f, 0.0f);
	shoot_to_goal.show_pos = VECTOR3(0.0f, SCREEN_HEIGHT * 0.5f - 250.0f, 0.0f);
	shoot_to_goal.out_pos = VECTOR3(0.0f, -SCREEN_HEIGHT * 0.5f - 300.0f, 0.0f);
	m_params[TUTORIAL_STATE::SHOOT_TO_GOAL] = shoot_to_goal;
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void TUTORIAL_UI_MANAGER_COMPONENT::Update()
{
	if (!m_visible)
	{
		return;
	}

	m_time += TIME::DeltaTime();

	VECTOR3 scale = m_original_scale;

	if (m_pulse)
	{
		float pulse_amount = (sinf((m_time / m_params.begin()->second.pulse_period) * 6.283185f) + 1.0f) * 0.5f;
		float scale_factor = 1.0f + (m_params.begin()->second.pulse_scale - 1.0f) * pulse_amount;
		scale = scale * scale_factor;
	}

	m_sprite_renderer_component->SetLocalSca(scale);
}

//-----------------------------------------------
// パラメータ適用
//-----------------------------------------------
void TUTORIAL_UI_MANAGER_COMPONENT::ApplyParam(const STEP_UI_PARAM& param)
{
	if (!m_sprite_renderer_component)
	{// nullチェック
		return;
	}

	m_sprite_renderer_component->SetMaterial(param.material);
	m_sprite_renderer_component->SetLocalPos(param.in_pos);

	MATERIAL_DATA& mat_data = m_sprite_renderer_component->GetMaterial()->m_material_data;
	mat_data.m_diffuse.w = 0.0f;

	m_pulse = param.pulse;
	m_time = 0.0f;
}

//-----------------------------------------------
// パラメータ取得
//-----------------------------------------------
const STEP_UI_PARAM& TUTORIAL_UI_MANAGER_COMPONENT::GetParam(TUTORIAL_STATE state) const
{
	auto it = m_params.find(state);
	if (it != m_params.end())
	{
		return it->second;
	}
	else
	{
		assert(false);						// 存在しないステート
		return m_params.begin()->second;	// デフォルトを返す
	}
}

//-----------------------------------------------
// チュートリアルUI表示
//-----------------------------------------------
void TUTORIAL_UI_MANAGER_COMPONENT::ShowStep(TUTORIAL_STATE state)
{
	if (!m_ui_animator_component)
	{// nullチェック
		return;
	}

	const STEP_UI_PARAM& param = GetParam(state);
	ApplyParam(param);

	m_ui_animator_component->KillAll();
	m_ui_animator_component->MoveAndFadeTo(param.show_pos, 1.0f, param.in_time);

	m_visible = true;
}

//-----------------------------------------------
// チュートリアルUI非表示
//-----------------------------------------------
void TUTORIAL_UI_MANAGER_COMPONENT::HideStep(bool immediate)
{
	if (!m_ui_animator_component)
	{// nullチェック
		return;
	}

	if (immediate)
	{
		m_ui_animator_component->KillAll();
		if (m_sprite_renderer_component)
		{
			MATERIAL_DATA& mat_data = m_sprite_renderer_component->GetMaterial()->m_material_data;
			mat_data.m_diffuse.w = 0.0f;
		}
	}
	else
	{
		const STEP_UI_PARAM& param = m_params.begin()->second;
		m_ui_animator_component->KillAll();
		m_ui_animator_component->MoveAndFadeTo(param.out_pos, 0.0f, param.out_time);
	}
	m_visible = false;
}

//-----------------------------------------------
// 成功時のパルス再生
//-----------------------------------------------
void TUTORIAL_UI_MANAGER_COMPONENT::PlaySuccessPulse()
{
	VECTOR3 s1 = VECTOR3(1.10f, 1.10f, 1.0f);
	VECTOR3 s0 = VECTOR3(1.00f, 1.00f, 1.0f);
	m_ui_animator_component->ScaleTo(s1, 0.08f, EASE_TYPE::EASE_OUT_CUBIC);
	m_ui_animator_component->ScaleTo(s0, 0.10f, EASE_TYPE::EASE_OUT_CUBIC);
}