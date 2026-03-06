//-----------------------------------------------
// TitleRogo.cpp
// 制作日：2025/09/14
// 制作者：安田晴人
//-----------------------------------------------

#include "Main.h"
#include "scene/Manager.h"
#include "renderer/Renderer.h"
#include "object/ResultRogo.h"

#include "component/renderer/SpriteRendererComponent.h"
#include "renderer/MaterialManager.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void RESULT_ROGO::Init()
{
	SPRITE_RENDERER_COMPONENT* sprite_renderer = AddComponent<SPRITE_RENDERER_COMPONENT>();
	
	sprite_renderer->SetBillboard(true);
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void RESULT_ROGO::Draw()
{
	GetComponent<SPRITE_RENDERER_COMPONENT>()->Draw();
}