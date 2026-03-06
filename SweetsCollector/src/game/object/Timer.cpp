//-----------------------------------------------
// Timer.cpp
// 制作日：2026/01/10
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "game/object/Timer.h"
#include "game/component/TimerManagerComponent.h"
#include "game/component/TimerUIComponent.h"
#include "component/renderer/SpriteRendererComponent.h"
#include "renderer/MaterialManager.h"
#include "audio.h"

//------------------------------------------------
// 初期化処理
//------------------------------------------------
void TIMER::Init()
{
	m_transform.SetPos(VECTOR3(0.0f, 0.0f, 0.0f));
	m_transform.SetSca(VECTOR3(1.0f, 1.0f, 1.0f));

	SPRITE_RENDERER_COMPONENT* label_sprite_renderer = AddComponent<SPRITE_RENDERER_COMPONENT>();
	label_sprite_renderer->SetMaterial(MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\TimeLabelMaterial.mtl"));
	label_sprite_renderer->SetRenderPass(RENDER_PASS::RENDER_PASS_UI);
	label_sprite_renderer->SetOrderInPass(15);

	TIMER_UI_COMPONENT* timer_ui_component = AddComponent<TIMER_UI_COMPONENT>();
	timer_ui_component->SetLabelSpriteRenderer(label_sprite_renderer);

	for (int i = 0; i < 2; i++)
	{
		SPRITE_RENDERER_COMPONENT* digit_sprite_renderer = AddComponent<SPRITE_RENDERER_COMPONENT>();
		digit_sprite_renderer->SetMaterial(MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\TimeDigitMaterial.mtl"));
		digit_sprite_renderer->SetUVSheet(true);
		digit_sprite_renderer->SetSpriteNum(5, 2);
		digit_sprite_renderer->SetRenderPass(RENDER_PASS::RENDER_PASS_UI);
		digit_sprite_renderer->SetOrderInPass(10);

		timer_ui_component->SetDigitSpriteRenderer(i, digit_sprite_renderer);
	}

	TIMER_MANAGER_COMPONENT* timer_manager = AddComponent<TIMER_MANAGER_COMPONENT>();
	timer_manager->SetTimeLimit(99.0);

	timer_ui_component->SetTimerManager(timer_manager);

	timer_ui_component->InitSpriteRenderers();

	AUDIO* audio = AddComponent<AUDIO>();
	audio->Load("asset\\sound\\BGM.wav");
	audio->Play(true);
	audio->SetVolume(0.3f);
}