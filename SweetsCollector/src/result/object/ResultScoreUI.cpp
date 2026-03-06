//-----------------------------------------------
// ResultScoreUI.cpp
// 制作日：2026/02/07
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "result/object/ResultScoreUI.h"
#include "result/component/ResultScoreUIComponent.h"
#include "component/renderer/SpriteRendererComponent.h"
#include "renderer/MaterialManager.h"

#include "audio.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void RESULT_SCORE_UI::Init()
{
	m_transform.SetPos(VECTOR3(0.0f, 0.0f, 0.0f));
	m_transform.SetSca(VECTOR3(1.0f, 1.0f, 1.0f));

	RESULT_SCORE_UI_COMPONENT* score_ui_component = AddComponent<RESULT_SCORE_UI_COMPONENT>();
	// ラベルスプライトレンダラーの作成
	SPRITE_RENDERER_COMPONENT* label_sprite_renderer = AddComponent<SPRITE_RENDERER_COMPONENT>();
	label_sprite_renderer->SetMaterial(MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\ScoreLabelMaterial.mtl"));
	label_sprite_renderer->SetRenderPass(RENDER_PASS::RENDER_PASS_UI);

	score_ui_component->SetLabelSpriteRenderer(label_sprite_renderer);
	// 桁ごとのスプライトレンダラーの作成
	for (int i = 0; i < 4; i++)
	{
		SPRITE_RENDERER_COMPONENT* digit_sprite_renderer = AddComponent<SPRITE_RENDERER_COMPONENT>();
		digit_sprite_renderer->SetMaterial(MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\ScoreDigitMaterial.mtl"));
		digit_sprite_renderer->SetRenderPass(RENDER_PASS::RENDER_PASS_UI);
		digit_sprite_renderer->SetUVSheet(true);
		digit_sprite_renderer->SetSpriteNum(5, 2);
		digit_sprite_renderer->SetSpriteIndex(0);

		score_ui_component->SetDigitSpriteRenderer(i, digit_sprite_renderer);
	}

	// スプライトレンダラーの初期化
	score_ui_component->InitSpriteRenderers();

	AUDIO* audio = AddComponent<AUDIO>();
	audio->Load("asset\\sound\\Result.wav");
	audio->Play(true);
	audio->SetVolume(0.3f);
}