//-----------------------------------------------
// ScoreUI.cpp
// 制作日：2026/01/01
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "game/object/ScoreUI.h"
#include "game/component/ScoreUIComponent.h"
#include "game/component/ScoreManagerComponent.h"
#include "component/renderer/SpriteRendererComponent.h"
#include "renderer/MaterialManager.h"

//------------------------------------------------
// 初期化処理
//------------------------------------------------
void SCORE_UI::Init()
{
	m_transform.SetPos(VECTOR3(0.0f, 0.0f, 0.0f));
	m_transform.SetSca(VECTOR3(1.0f, 1.0f, 1.0f));

	AddComponent<SCORE_MANAGER_COMPONENT>();

	SCORE_UI_COMPONENT* score_ui_component = AddComponent<SCORE_UI_COMPONENT>();

	SPRITE_RENDERER_COMPONENT* label_sprite_renderer = AddComponent<SPRITE_RENDERER_COMPONENT>();
	label_sprite_renderer->SetMaterial(MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\ScoreLabelMaterial.mtl"));
	label_sprite_renderer->SetRenderPass(RENDER_PASS::RENDER_PASS_UI);
	label_sprite_renderer->SetOrderInPass(15);
	score_ui_component->SetLabelSpriteRenderer(label_sprite_renderer);

	for (int i = 0; i < 4; i++)
	{
		SPRITE_RENDERER_COMPONENT* digit_sprite_renderer = AddComponent<SPRITE_RENDERER_COMPONENT>();
		digit_sprite_renderer->SetMaterial(MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\ScoreDigitMaterial.mtl"));
		digit_sprite_renderer->SetRenderPass(RENDER_PASS::RENDER_PASS_UI);
		digit_sprite_renderer->SetOrderInPass(10);
		score_ui_component->SetDigitSpriteRenderer(i, digit_sprite_renderer);
	}

	score_ui_component->InitSpriteRenderers();
}