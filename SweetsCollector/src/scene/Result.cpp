//-----------------------------------------------
// Result.cpp
// 制作日：2025/07/02
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "scene/Manager.h"
#include "renderer/Renderer.h"

#include "input/Input.h"
#include "input/GamePad.h"

#include "object/Polygon.h"
#include "object/ResultRogo.h"
#include "object/FlushRogo.h"
#include "object/Camera.h"
#include "object/Field.h"
#include "game/object/Wall.h"

#include "object/Fade.h"
#include "component/SimpleFadeComponent.h"

#include "object/SkyDome.h"
#include "result/object/ResultSweetsSpawner.h"
#include "result/object/ResultPanda.h"
#include"result/object/ResultScoreUI.h"

#include "game/object/Sweets.h"

#include "scene/Scene.h"
#include "scene/Title.h"
#include "scene/Result.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void RESULT::Init()
{
	AddGameObject<SKY_DOME>(LAYER::LAYER_BACKGROUND);

	RESULT_SCORE_UI* score_ui = AddGameObject<RESULT_SCORE_UI>(LAYER::LAYER_UI);

	FLUSH_ROGO* flush = AddGameObject<FLUSH_ROGO>(LAYER::LAYER_GAME_OBJECT);

	CAMERA* camera = AddGameObject<CAMERA>(LAYER::LAYER_CAMERA);
	camera->GetTransform()->SetPos(VECTOR3(0.0f, 5.0f, -20.0f));

	FIELD* field = AddGameObject<FIELD>(LAYER::LAYER_GAME_OBJECT);
	field->GetTransform()->SetPos(VECTOR3(0.0f, -5.0f, 0.0f));
	field->GetTransform()->SetSca(VECTOR3(20.0f, 1.0f, 20.0f));

	WALL* wall1 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
	wall1->GetTransform()->SetPos(VECTOR3(0.0f, -3.0f, 6.0f));
	wall1->GetTransform()->SetSca(VECTOR3(10.0f, 2.0f, 2.0f));
	WALL* wall2 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
	wall2->GetTransform()->SetPos(VECTOR3(0.0f, -3.0f, -6.0f));
	wall2->GetTransform()->SetSca(VECTOR3(10.0f, 2.0f, 2.0f));
	WALL* wall3 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
	wall3->GetTransform()->SetPos(VECTOR3(6.0f, -3.0f, 0.0f));
	wall3->GetTransform()->SetSca(VECTOR3(2.0f, 2.0f, 10.0f));
	WALL* wall4 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
	wall4->GetTransform()->SetPos(VECTOR3(-6.0f, -3.0f, 0.0f));
	wall4->GetTransform()->SetSca(VECTOR3(2.0f, 2.0f, 10.0f));

	RESULT_SWEETS_SPAWNER* sweets_spawner = AddGameObject<RESULT_SWEETS_SPAWNER>(LAYER::LAYER_GAME_OBJECT);
	sweets_spawner->GetTransform()->SetPos(VECTOR3(0.0f, 3.0f, 0.0f));

	RESULT_PANDA* panda = AddGameObject<RESULT_PANDA>(LAYER::LAYER_GAME_OBJECT);
	panda->GetTransform()->SetPos(VECTOR3(0.0f, 2.0f, 0.0f));
	panda->GetTransform()->SetRot(QUATERNION::FromEuler(VECTOR3(0.0f, 180.0f, 0.0f)));

	FADE* fade = AddGameObject<FADE>(LAYER::LAYER_UI);
	auto fade_component = fade->GetComponent<SIMPLE_FADE_COMPONENT>();
	fade_component->SetCurrentScene(SIMPLE_FADE_COMPONENT::CURRENT_SCENE::RESULT);
	fade_component->StartFadeIn();
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void RESULT::Update()
{
	SCENE::Update();
	if (Input::GetKeyTrigger(VK_RETURN) || INPUT_SYSTEM::Instance().GetGamePad(0).Pressed(PAD_BUTTON::A))
	{
		GetGameObject<FADE>()->GetComponent<SIMPLE_FADE_COMPONENT>()->StartFadeOut();
	}
}