//-----------------------------------------------
// WeightUI.cpp
// 制作日：2026/01/21
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "game/object/WeightUI.h"
#include "game/component/WeightUIComponent.h"
#include "component/renderer/SpriteRendererComponent.h"

#include "renderer/MaterialManager.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void WEIGHT_UI::Init()
{
	m_transform.SetPos(VECTOR3(0.0f, 0.0f, 0.0f));
	m_transform.SetSca(VECTOR3(1.0f, 1.0f, 1.0f));

	// WeightUIコンポーネントを追加
	WEIGHT_UI_COMPONENT* p_weight_ui_component = AddComponent<WEIGHT_UI_COMPONENT>();
	// ラベルスプライトレンダラーの作成
	SPRITE_RENDERER_COMPONENT* p_label_sprite_renderer = AddComponent<SPRITE_RENDERER_COMPONENT>();
	p_label_sprite_renderer->SetMaterial(MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\WeightLabelMaterial.mtl"));
	p_label_sprite_renderer->SetSpriteIndex(0); // ラベルのスプライトインデックスを設定
	p_label_sprite_renderer->SetSpriteNum(2, 2);
	p_label_sprite_renderer->SetUVSheet(true);
	p_label_sprite_renderer->SetRenderPass(RENDER_PASS::RENDER_PASS_UI);
	p_label_sprite_renderer->SetOrderInPass(10);
	p_weight_ui_component->SetLabelSpriteRenderer(p_label_sprite_renderer);

	// 数字スプライトレンダラーの作成
	for (int i = 0; i < 3; i++)
	{
		SPRITE_RENDERER_COMPONENT* p_digit_sprite_renderer = AddComponent<SPRITE_RENDERER_COMPONENT>();
		p_digit_sprite_renderer->SetMaterial(MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\WeightDigitMaterial.mtl"));
		p_digit_sprite_renderer->SetSpriteIndex(0); // 初期値は0
		p_digit_sprite_renderer->SetSpriteNum(5, 2);
		p_digit_sprite_renderer->SetUVSheet(true);
		p_digit_sprite_renderer->SetRenderPass(RENDER_PASS::RENDER_PASS_UI);
		p_digit_sprite_renderer->SetOrderInPass(15);

		p_weight_ui_component->SetSpriteRenderer(i, p_digit_sprite_renderer);
	}

	// スプライトレンダラーの初期化
	p_weight_ui_component->InitSpriteRenderers();
}