//-----------------------------------------------
// Tutorial.cpp
// 制作日：2026/02/11
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "scene/Manager.h"
#include "renderer/Renderer.h"

#include "input/Input.h"

#include "object/SkyDome.h"
#include "object/Field.h"
#include "game/object/Wall.h"
#include "game/object/Sweets.h"
#include "tutorial/component/TutorialSweetsComponent.h"
#include "object/Player.h"
#include "game/object/WeightUI.h"
#include "tutorial/component/TutorialWeightUIComponent.h"
#include "game/object/Goal.h"
#include "tutorial/component/TutorialGoalComponent.h"
#include "game/object/ScoreUI.h"

#include "object/Camera.h"
#include "component/CameraOrbitComponent.h"
#include "object/Fade.h"
#include "component/SimpleFadeComponent.h"

#include "tutorial/object/TutorialManager.h"
#include "tutorial/object/TutorialUIManager.h"

#include "scene/Tutorial.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void TUTORIAL::Init()
{
	CAMERA* camera = AddGameObject<CAMERA>(LAYER::LAYER_CAMERA);
	CAMERA_ORBIT_COMPONENT* camera_orbit = camera->GetComponent<CAMERA_ORBIT_COMPONENT>();

	AddGameObject<SKY_DOME>(LAYER::LAYER_BACKGROUND);
	
	// 地形
	{
		//--------------------
		// FIELD
		//--------------------
		FIELD* field = AddGameObject<FIELD>(LAYER::LAYER_GAME_OBJECT);
		field->GetTransform()->SetPos(VECTOR3(-0.0f, -5.0f, 0.0f));
		field->GetTransform()->SetSca(VECTOR3(20.0f, 1.0f, 20.0f));

		FIELD* field1 = AddGameObject<FIELD>(LAYER::LAYER_GAME_OBJECT);
		field1->GetTransform()->SetPos(VECTOR3(-50.0f, -5.0f, 0.0f));
		field1->GetTransform()->SetSca(VECTOR3(20.0f, 1.0f, 20.0f));

		FIELD* field2 = AddGameObject<FIELD>(LAYER::LAYER_GAME_OBJECT);
		field2->GetTransform()->SetPos(VECTOR3(-100.0f, -5.0f, 0.0f));
		field2->GetTransform()->SetSca(VECTOR3(20.0f, 1.0f, 20.0f));

		FIELD* field3 = AddGameObject<FIELD>(LAYER::LAYER_GAME_OBJECT);
		field3->GetTransform()->SetPos(VECTOR3(-25.0f, -5.0f, 0.0f));
		field3->GetTransform()->SetSca(VECTOR3(30.0f, 1.0f, 10.0f));

		FIELD* field4 = AddGameObject<FIELD>(LAYER::LAYER_GAME_OBJECT);
		field4->GetTransform()->SetPos(VECTOR3(-75.0f, -5.0f, 0.0f));
		field4->GetTransform()->SetSca(VECTOR3(30.0f, 1.0f, 10.0f));

		//--------------------
		// WALL
		//--------------------
		WALL* wall1 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
		wall1->GetTransform()->SetPos(VECTOR3(11.0f, -3.0f, 0.0f));
		wall1->GetTransform()->SetSca(VECTOR3(2.0f, 3.0f, 20.0f));

		WALL* wall2 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
		wall2->GetTransform()->SetPos(VECTOR3(-0.0f, -3.0f, 11.0f));
		wall2->GetTransform()->SetSca(VECTOR3(20.0f, 3.0f, 2.0f));

		WALL* wall3 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
		wall3->GetTransform()->SetPos(VECTOR3(-0.0f, -3.0f, -11.0f));
		wall3->GetTransform()->SetSca(VECTOR3(20.0f, 3.0f, 2.0f));

		WALL* wall4 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
		wall4->GetTransform()->SetPos(VECTOR3(-11.0f, -3.0f, 7.5f));
		wall4->GetTransform()->SetSca(VECTOR3(2.0f, 3.0f, 5.5f));

		WALL* wall5 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
		wall5->GetTransform()->SetPos(VECTOR3(-11.0f, -3.0f, -7.5f));
		wall5->GetTransform()->SetSca(VECTOR3(2.0f, 3.0f, 5.5f));

		WALL* wall6 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
		wall6->GetTransform()->SetPos(VECTOR3(-25.0f, -3.0f, 6.0f));
		wall6->GetTransform()->SetSca(VECTOR3(28.0f, 3.0f, 2.0f));

		WALL* wall7 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
		wall7->GetTransform()->SetPos(VECTOR3(-25.0f, -3.0f, -6.0f));
		wall7->GetTransform()->SetSca(VECTOR3(28.0f, 3.0f, 2.0f));

		WALL* wall8 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
		wall8->GetTransform()->SetPos(VECTOR3(-39.0f, -3.0f, 7.5f));
		wall8->GetTransform()->SetSca(VECTOR3(2.0f, 3.0f, 5.5f));

		WALL* wall9 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
		wall9->GetTransform()->SetPos(VECTOR3(-39.0f, -3.0f, -7.5f));
		wall9->GetTransform()->SetSca(VECTOR3(2.0f, 3.0f, 5.5f));

		WALL* wall10 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
		wall10->GetTransform()->SetPos(VECTOR3(-50.0f, -3.0f, 11.0f));
		wall10->GetTransform()->SetSca(VECTOR3(20.0f, 3.0f, 2.0f));

		WALL* wall11 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
		wall11->GetTransform()->SetPos(VECTOR3(-50.0f, -3.0f, -11.0f));
		wall11->GetTransform()->SetSca(VECTOR3(20.0f, 3.0f, 2.0f));

		WALL* wall12 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
		wall12->GetTransform()->SetPos(VECTOR3(-61.0f, -3.0f, 7.5f));
		wall12->GetTransform()->SetSca(VECTOR3(2.0f, 3.0f, 5.5f));

		WALL* wall13 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
		wall13->GetTransform()->SetPos(VECTOR3(-61.0f, -3.0f, -7.5f));
		wall13->GetTransform()->SetSca(VECTOR3(2.0f, 3.0f, 5.5f));

		WALL* wall14 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
		wall14->GetTransform()->SetPos(VECTOR3(-75.0f, -3.0f, 6.0f));
		wall14->GetTransform()->SetSca(VECTOR3(28.0f, 3.0f, 2.0f));

		WALL* wall15 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
		wall15->GetTransform()->SetPos(VECTOR3(-75.0f, -3.0f, -6.0f));
		wall15->GetTransform()->SetSca(VECTOR3(28.0f, 3.0f, 2.0f));

		WALL* wall16 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
		wall16->GetTransform()->SetPos(VECTOR3(-89.0f, -3.0f, 7.5f));
		wall16->GetTransform()->SetSca(VECTOR3(2.0f, 3.0f, 5.5f));

		WALL* wall17 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
		wall17->GetTransform()->SetPos(VECTOR3(-89.0f, -3.0f, -7.5f));
		wall17->GetTransform()->SetSca(VECTOR3(2.0f, 3.0f, 5.5f));

		WALL* wall18 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
		wall18->GetTransform()->SetPos(VECTOR3(-100.0f, -3.0f, -11.0f));
		wall18->GetTransform()->SetSca(VECTOR3(20.0f, 3.0f, 2.0f));

		WALL* wall19 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
		wall19->GetTransform()->SetPos(VECTOR3(-100.0f, -3.0f, 11.0f));
		wall19->GetTransform()->SetSca(VECTOR3(20.0f, 3.0f, 2.0f));

		WALL* wall20 = AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);
		wall20->GetTransform()->SetPos(VECTOR3(-111.0f, -3.0f, 0.0f));
		wall20->GetTransform()->SetSca(VECTOR3(2.0f, 3.0f, 20.0f));
	}

	PLAYER* player = AddGameObject<PLAYER>(LAYER::LAYER_GAME_OBJECT);
	player->GetTransform()->SetPos(VECTOR3(0.0f, 4.0f, 0.0f));
	camera_orbit->SetTarget(player->GetTransform());

	SWEETS* sweets = AddGameObject<SWEETS>(LAYER::LAYER_GAME_OBJECT);
	sweets->AddComponent<TUTORIAL_SWEETS_COMPONENT>();
	sweets->GetTransform()->SetPos(VECTOR3(-50.0f, 0.0f, 0.0f));

	SCORE_UI* score_ui = AddGameObject<SCORE_UI>(LAYER::LAYER_UI);

	GOAL* goal = AddGameObject<GOAL>(LAYER::LAYER_GAME_OBJECT);
	goal->AddComponent<TUTORIAL_GOAL_COMPONENT>();
	goal->GetTransform()->SetPos(VECTOR3(-100.0f, -3.0f, 0.0f));

	WEIGHT_UI* weight = AddGameObject<WEIGHT_UI>(LAYER::LAYER_UI);
	weight->AddComponent<TUTORIAL_WEIGHT_UI_COMPONENT>();

	AddGameObject<TUTORIAL_UI_MANAGER>(LAYER::LAYER_UI);
	AddGameObject<TUTORIAL_MANAGER>(LAYER::LAYER_GAME_OBJECT);

	FADE* fade = AddGameObject<FADE>(LAYER::LAYER_UI);
	SIMPLE_FADE_COMPONENT* fade_component = fade->GetComponent<SIMPLE_FADE_COMPONENT>();
	fade_component->SetCurrentScene(SIMPLE_FADE_COMPONENT::CURRENT_SCENE::TUTORIAL);
	fade_component->StartFadeIn();
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void TUTORIAL::Update()
{
	SCENE::Update();
	/*if (Input::GetKeyTrigger(VK_RETURN))
	{
		GetGameObject<FADE>()->GetComponent<SIMPLE_FADE_COMPONENT>()->StartFadeOut();
	}*/
}
