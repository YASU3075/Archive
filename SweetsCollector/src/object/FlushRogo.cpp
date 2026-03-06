//-----------------------------------------------
// FlushRogo.cpp
// 制作日：2025/09/14
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "scene/Manager.h"
#include "renderer/Renderer.h"

#include "object/FlushRogo.h"
#include "component/renderer/SpriteRendererComponent.h"
#include "renderer/MaterialManager.h"

#include "component/MaterialFlashComponent.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void FLUSH_ROGO::Init()
{
	m_transform.SetPos(VECTOR3(0.0f, SCREEN_HEIGHT * 0.5f - 150.0f * 0.5f, 0.0f));
	m_transform.SetSca(VECTOR3(800.0f, 100.0f, 1.0f));

	SPRITE_RENDERER_COMPONENT* sprite_renderer = AddComponent<SPRITE_RENDERER_COMPONENT>();
	sprite_renderer->SetMaterial(MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\EnterMaterial.mtl"));
	sprite_renderer->SetRenderPass(RENDER_PASS::RENDER_PASS_UI);
	MATERIAL_FLASH_COMPONENT* flash = AddComponent<MATERIAL_FLASH_COMPONENT>();
	flash->SetFlashTime(5.0f);
}