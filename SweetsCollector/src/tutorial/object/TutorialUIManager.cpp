//-----------------------------------------------
// TutorialUIManager.cpp
// 制作日：2026/02/11
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "tutorial/object/TutorialUIManager.h"
#include "tutorial/component/TutorialUIManagerComponent.h"
#include "component/UIAnimatorComponent.h"
#include "component/renderer/SpriteRendererComponent.h"
#include "renderer/MaterialManager.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void TUTORIAL_UI_MANAGER::Init()
{
	m_transform.SetPos(VECTOR3(0.0f, 0.0f, 0.0f));
	m_transform.SetSca(VECTOR3(1.0f, 1.0f, 1.0f));

	SPRITE_RENDERER_COMPONENT* sprite = AddComponent<SPRITE_RENDERER_COMPONENT>();
	sprite->SetMaterial(MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\MoveTextMaterial.mtl"));
	sprite->SetLocalSca(VECTOR3(600.0f, 150.0f, 1.0f));
	sprite->SetRenderPass(RENDER_PASS::RENDER_PASS_UI);

	AddComponent<UI_ANIMATOR_COMPONENT>();
	AddComponent<TUTORIAL_UI_MANAGER_COMPONENT>();
}
