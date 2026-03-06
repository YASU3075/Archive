//-----------------------------------------------
// Panda.cpp
// 制作日：2026/01/29
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "title/object/Panda.h"
#include "title/component/PandaAnimationComponent.h"
#include "component/renderer/ModelRendererComponent.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void PANDA::Init()
{
	m_transform.SetPos(VECTOR3(0.0f, 0.0f, 0.0f));
	m_transform.SetSca(VECTOR3(1.0f, 1.0f, 1.0f));

	MODEL_RENDERER_COMPONENT* model_renderer = AddComponent<MODEL_RENDERER_COMPONENT>();
	model_renderer->SetModel("asset\\model\\panda.obj");
	model_renderer->SetLocalPos(VECTOR3(0.0f, -1.0f, 0.0f));
	model_renderer->SetLocalSca(VECTOR3(5.0f, 5.0f, 5.0f));

	AddComponent<PANDA_ANIMATION_COMPONENT>();
}