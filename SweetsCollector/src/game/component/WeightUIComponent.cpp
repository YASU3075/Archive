//-----------------------------------------------
// WeightUIComponent.cpp
// 制作日：2026/01/21
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "game/component/WeightUIComponent.h"
#include "game/component/SweetsManagementComponent.h"
#include "component/renderer/SpriteRendererComponent.h"
#include "object/Player.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void WEIGHT_UI_COMPONENT::Init()
{
	m_p_label_sprite_renderer = nullptr;
	for (int i = 0; i < 3; i++)
	{
		m_p_sprite_renderer_component[i] = nullptr;
	}

	m_p_sweets_manager = Manager::GetScene()->GetGameObject<PLAYER>()->GetComponent<SWEETS_MANAGEMENT_COMPONENT>();
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void WEIGHT_UI_COMPONENT::Update()
{
	if (m_p_sweets_manager == nullptr)
	{
		m_p_sweets_manager = Manager::GetScene()->GetGameObject<PLAYER>()->GetComponent<SWEETS_MANAGEMENT_COMPONENT>();
	}

	if (m_p_sweets_manager)
	{
		int weight = m_p_sweets_manager->GetCurrentSweetsWeight();
		// 重さを3桁の数字に分解して表示
		for (int i = 0; i < 3; i++)
		{
			if (m_p_sprite_renderer_component[i])
			{
				int digit = (weight / static_cast<int>(std::pow(10, 2 - i))) % 10;
				m_p_sprite_renderer_component[i]->SetSpriteIndex(digit);
			}
		}
	}
}

//-----------------------------------------------
// スプライトレンダラーの初期化
//-----------------------------------------------
void WEIGHT_UI_COMPONENT::InitSpriteRenderers()
{
	if (m_p_label_sprite_renderer)
	{
		m_p_label_sprite_renderer->SetLocalPos(VECTOR3(0.0f, SCREEN_HEIGHT * 0.5f - 100.0f, 0.0f));
		m_p_label_sprite_renderer->SetLocalSca(VECTOR3(200.0f, 200.0f, 1.0f));
	}

	for (int i = 0; i < 3; i++)
	{
		if (m_p_sprite_renderer_component[i])
		{
			m_p_sprite_renderer_component[i]->SetLocalPos(VECTOR3(-30.0f + 30.0f * i, SCREEN_HEIGHT * 0.5f - 75.0f, 0.0f));
			m_p_sprite_renderer_component[i]->SetLocalSca(VECTOR3(30.0f, 30.0f, 1.0f));
		}
	}
}