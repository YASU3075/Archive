//-----------------------------------------------
// TitleRogo.cpp
// 制作日：2025/09/14
// 制作者：安田晴人
//-----------------------------------------------

#include "Main.h"
#include "scene/Manager.h"
#include "renderer/Renderer.h"
#include "object/TitleRogo.h"

#include "component/renderer/SpriteRendererComponent.h"
#include "renderer/MaterialManager.h"

#include "audio.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void TITLE_ROGO::Init()
{
	m_transform.SetPos(VECTOR3(0.0f, -SCREEN_HEIGHT * 0.5f + 150.0f * 1.5f * 0.5f, 0.0f));
	m_transform.SetSca(VECTOR3(750.0f * 1.5f, 150.0f * 1.5f, 1.0f));

	SPRITE_RENDERER_COMPONENT* sprite_renderer = AddComponent<SPRITE_RENDERER_COMPONENT>();
	sprite_renderer->SetMaterial(MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\TitleMaterial.mtl"));
	sprite_renderer->SetRenderPass(RENDER_PASS::RENDER_PASS_UI);

	AUDIO* audio = AddComponent<AUDIO>();
	audio->Load("asset\\sound\\Title.wav");
	audio->Play(true);
	audio->SetVolume(0.3f);
}