//-----------------------------------------------
// CountDownUIComponent.cpp
// 制作日：2026/01/26
// 制作者：安田晴人
//-----------------------------------------------
#include <algorithm>
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "entity/GameObject.h"
#include "game/component/CountDownUIComponent.h"
#include "component/renderer/SpriteRendererComponent.h"
#include "renderer/MaterialManager.h"
#include "object/Player.h"
#include "component/MovePlayer.h"
#include "EngineTime.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void COUNT_DOWN_UI_COMPONENT::Init()
{
	m_state = COUNT_DOWN_UI_STATE::COUNT_DOWN_UI_STATE_THREE;
	m_time = 0.0f;
	InitSpriteRenderers();

	Manager::GetScene()->GetGameObject<PLAYER>()->GetComponent<MOVE_PLAYER>()->SetActive(false);

	TIME::SetTimeScale(0.0f);
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void COUNT_DOWN_UI_COMPONENT::Update()
{
	if (m_state == COUNT_DOWN_UI_STATE::COUNT_DOWN_UI_STATE_DONE)
	{
		return;
	}

	m_time += TIME::UnscaledDeltaTime();
	float progress = std::clamp(m_time / dulation, 0.0f, 1.0f);

	CB_CIRCLE_WIPE circle_wipe_data{};
	circle_wipe_data.param0.x = progress;
	circle_wipe_data.param0.y = 0.02f; // ぼかし幅
	circle_wipe_data.param0.z = 1.0f;
	circle_wipe_data.param0.w = static_cast<float>(POST_EFFECT_MODE::POST_EFFECT_MODE_NONE);

	circle_wipe_data.param1.x = 0.5f; // 中心X
	circle_wipe_data.param1.y = 0.5f; // 中心Y

	Renderer::SetCircleWipeParams(circle_wipe_data);

	if (progress >= 1.0f)
	{
		m_time = 0.0f;
		switch (m_state)
		{
		case COUNT_DOWN_UI_STATE::COUNT_DOWN_UI_STATE_THREE:
			m_state = COUNT_DOWN_UI_STATE::COUNT_DOWN_UI_STATE_TWO;
			if (m_p_number_sprite_renderer)
			{
				m_p_number_sprite_renderer->SetSpriteIndex(2);
			}
			break;
		case COUNT_DOWN_UI_STATE::COUNT_DOWN_UI_STATE_TWO:
			m_state = COUNT_DOWN_UI_STATE::COUNT_DOWN_UI_STATE_ONE;
			if (m_p_number_sprite_renderer)
			{
				m_p_number_sprite_renderer->SetSpriteIndex(1);
			}
			break;
		case COUNT_DOWN_UI_STATE::COUNT_DOWN_UI_STATE_ONE:
			m_state = COUNT_DOWN_UI_STATE::COUNT_DOWN_UI_STATE_GO;
			if (m_p_number_sprite_renderer)
			{
				m_p_label_sprite_renderer->SetLocalSca(VECTOR3(0.0f, 0.0f, 0.0f));
				m_p_number_sprite_renderer->SetUVSheet(false);
				m_p_number_sprite_renderer->SetMaterial(MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\GameStartMaterial.mtl"));
			}
			break;
		case COUNT_DOWN_UI_STATE::COUNT_DOWN_UI_STATE_GO:
			m_state = COUNT_DOWN_UI_STATE::COUNT_DOWN_UI_STATE_DONE;
			// ゲームスタート
			TIME::SetTimeScale(1.0f);
			Manager::GetScene()->GetGameObject<PLAYER>()->GetComponent<MOVE_PLAYER>()->SetActive(true);
			m_p_owner->SetDestroy(true);
			break;
		default:
			break;
		}
	}
}

//-----------------------------------------------
// スプライトレンダラーの初期化
//-----------------------------------------------
void COUNT_DOWN_UI_COMPONENT::InitSpriteRenderers()
{
	if (m_p_label_sprite_renderer)
	{
		m_p_label_sprite_renderer->SetSpriteIndex(0);
		m_p_label_sprite_renderer->SetLocalSca(VECTOR3(200.0f, 200.0f, 1.0f));
	}
	if (m_p_number_sprite_renderer)
	{
		m_p_number_sprite_renderer->SetSpriteIndex(3);
		m_p_number_sprite_renderer->SetLocalSca(VECTOR3(150.0f, 150.0f, 1.0f));
	}
}