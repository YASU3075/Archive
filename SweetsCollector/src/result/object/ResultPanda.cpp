//-----------------------------------------------
// ResultPanda.cpp
// 制作日：2026/02/07
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "result/object/ResultPanda.h"
#include "result/component/ResultPandaAnimationComponent.h"
#include "component/renderer/ModelRendererComponent.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void RESULT_PANDA::Init()
{
	MODEL_RENDERER_COMPONENT* model_renderer = AddComponent<MODEL_RENDERER_COMPONENT>();
	model_renderer->SetModel("asset\\model\\panda.obj");
	model_renderer->SetLocalPos(VECTOR3(0.0f, -2.0f, 0.0f));
	model_renderer->SetLocalSca(VECTOR3(2.0f, 2.0f, 2.0f));

	RESULT_PANDA_ANIMATION_COMPONENT* animation_component = AddComponent<RESULT_PANDA_ANIMATION_COMPONENT>();
	animation_component->SetAnimationInterval(1.0f);
	animation_component->SetModelRenderer(model_renderer);
}