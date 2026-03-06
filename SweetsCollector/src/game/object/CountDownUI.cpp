//-----------------------------------------------
// CountDownUI.cpp
// 制作日：2026/01/27
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "game/object/CountDownUI.h"
#include "game/component/CountDownUIComponent.h"
#include "component/renderer/SpriteRendererComponent.h"
#include "renderer/MaterialManager.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void COUNT_DOWN_UI::Init()
{
	m_transform.SetPos(VECTOR3(0.0f, 0.0f, 0.0f));
	m_transform.SetSca(VECTOR3(1.0f, 1.0f, 1.0f));

	COUNT_DOWN_UI_COMPONENT* p_count_down_ui_component = AddComponent<COUNT_DOWN_UI_COMPONENT>();

	SPRITE_RENDERER_COMPONENT* p_label_sprite_renderer = AddComponent<SPRITE_RENDERER_COMPONENT>();
	p_label_sprite_renderer->SetMaterial(MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\CountDownLabelMaterial.mtl"));
	p_label_sprite_renderer->SetRenderPass(RENDER_PASS::RENDER_PASS_UI);
	p_label_sprite_renderer->SetOrderInPass(1);

	p_count_down_ui_component->SetLabelSpriteRenderer(p_label_sprite_renderer);

	SPRITE_RENDERER_COMPONENT* p_number_sprite_renderer = AddComponent<SPRITE_RENDERER_COMPONENT>();
	p_number_sprite_renderer->SetMaterial(MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\CountDownNumberMaterial.mtl"));
	p_number_sprite_renderer->SetRenderPass(RENDER_PASS::RENDER_PASS_UI);
	p_number_sprite_renderer->SetSpriteNum(5, 2);
	p_number_sprite_renderer->SetUVSheet(true);
	p_number_sprite_renderer->SetOrderInPass(2);

	p_count_down_ui_component->SetNumberSpriteRenderer(p_number_sprite_renderer);

	p_count_down_ui_component->InitSpriteRenderers();
}