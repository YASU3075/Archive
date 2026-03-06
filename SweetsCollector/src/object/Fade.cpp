//-----------------------------------------------
// Fade.cpp
// 制作日：2025/09/14
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"

#include "object/Fade.h"

#include "component/renderer/SpriteRendererComponent.h"
#include "renderer/MaterialManager.h"

#include "component/SimpleFadeComponent.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void FADE::Init()
{
	auto sprite_renderer = AddComponent<SPRITE_RENDERER_COMPONENT>();
	sprite_renderer->SetMaterial(MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\FadeMaterial.mtl"));
	sprite_renderer->SetRenderPass(RENDER_PASS::RENDER_PASS_UI);
	sprite_renderer->SetOrderInPass(1000);
	m_transform.SetPos(VECTOR3(0.0f, 0.0f, 0.0f));
	m_transform.SetSca(VECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f));

	auto fade_component = AddComponent<SIMPLE_FADE_COMPONENT>();
	fade_component->SetFadeMode(FADE_MODE::IRIS);
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void FADE::Draw()
{
	GetComponent<SPRITE_RENDERER_COMPONENT>()->Draw();
}