//-----------------------------------------------
// TimeUpUI.cpp
// 制作日：2026/01/19
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "game/object/TimeUpUI.h"
#include "game/component/TimeUpUIComponent.h"
#include "component/renderer/SpriteRendererComponent.h"
#include "renderer/MaterialManager.h"

//------------------------------------------------
// 初期化処理
//------------------------------------------------
void TIME_UP_UI::Init()
{
	m_transform.SetPos(VECTOR3(0.0f, -SCREEN_HEIGHT * 0.5f - 50.0f, 0.0f));
	m_transform.SetSca(VECTOR3(400.0f, 100.0f, 1.0f));

	SPRITE_RENDERER_COMPONENT* p_sprite_renderer_component = AddComponent<SPRITE_RENDERER_COMPONENT>();
	p_sprite_renderer_component->SetMaterial(MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\TimeUpMaterial.mtl"));
	p_sprite_renderer_component->SetRenderPass(RENDER_PASS::RENDER_PASS_UI);
	p_sprite_renderer_component->SetOrderInPass(100);


	TIME_UP_UI_COMPONENT* p_time_up_ui_component = AddComponent<TIME_UP_UI_COMPONENT>();
	p_time_up_ui_component->SetStartPosition(VECTOR3(0.0f, -SCREEN_HEIGHT * 0.5f - 50.0f, 0.0f));
	p_time_up_ui_component->SetTargetPosition(VECTOR3(0.0f, 0.0f, 0.0f));
}