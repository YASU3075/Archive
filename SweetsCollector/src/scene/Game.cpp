//-----------------------------------------------
// Game.cpp
// 制作日：2025/06/25
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "scene/Manager.h"
#include "renderer/Renderer.h"
#include "input/Input.h"

#include "entity/GameObject.h"
#include "object/Camera.h"
#include "object/Player.h"
#include "object/Enemy.h"
#include "object/Field.h"
#include "object/Fade.h"
#include "object/SkyDome.h"
#include "object/PointLight.h"
#include "object/DirectionalLight.h"
#include "game/object/MapGenerater.h"
#include "game/object/Wall.h"
#include "game/object/Timer.h"
#include "game/object/TimeUpUI.h"
#include "game/object/CountDownUI.h"
#include "game/object/RotateBoard.h"
#include "game/object/Block.h"

#include "game/object/Sweets.h"
#include "game/object/Goal.h"
#include "game/object/ScoreUI.h"
#include "game/object/WeightUI.h"

#include "component/CameraOrbitComponent.h"

#include "component/SimpleFadeComponent.h"
#include "component/renderer/ModelRendererComponent.h"

#include "renderer/MaterialManager.h"

#include "scene/Game.h"
#include "scene/Result.h"

#include "physics/RigidBody.h"
#include "physics/Physics3D.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void GAME::Init()
{
	{
		// カメラ
		CAMERA* cam = AddGameObject<CAMERA>(LAYER::LAYER_CAMERA);
		CAMERA_ORBIT_COMPONENT* orbit = cam->GetComponent<CAMERA_ORBIT_COMPONENT>();

		// ライト
		DIRECTIONAL_LIGHT* dir_light = AddGameObject<DIRECTIONAL_LIGHT>(LAYER::LAYER_GAME_OBJECT);
		dir_light->GetTransform()->SetPos(VECTOR3(0.0f, 100.0f, 0.0f));
		dir_light->GetTransform()->SetRot(QUATERNION::FromEuler({ -90.0f, 0.0f, 0.0f }));

		// 背景
		AddGameObject<SKY_DOME>(LAYER::LAYER_BACKGROUND);

		PLAYER* player = AddGameObject<PLAYER>(LAYER::LAYER_GAME_OBJECT);
		player->GetTransform()->SetPos(VECTOR3(0.0f, 4.0f, 0.0f));
		orbit->SetTarget(player->GetTransform());

		AddGameObject<ENEMY>(LAYER::LAYER_GAME_OBJECT)->GetTransform()->SetPos(VECTOR3(90.0f, 2.0f, 0.0f));

		AddGameObject<SCORE_UI>(LAYER::LAYER_UI);

		AddGameObject<TIMER>(LAYER::LAYER_UI);

		AddGameObject<WEIGHT_UI>(LAYER::LAYER_UI);

		AddGameObject<TIME_UP_UI>(LAYER::LAYER_UI);

		AddGameObject<COUNT_DOWN_UI>(LAYER::LAYER_UI);

		AddGameObject<GOAL>(LAYER::LAYER_GAME_OBJECT)->GetTransform()->SetPos(VECTOR3(-90.0f, 2.0f, 0.0f));

		AddGameObject<MAP_GENERATER>(LAYER::LAYER_GAME_OBJECT);

		FIELD* field = AddGameObject<FIELD>(LAYER::LAYER_GAME_OBJECT);
		field->GetTransform()->SetPos(VECTOR3(-40.0f, 1.5f, 15.0f));
		field->GetTransform()->SetSca(VECTOR3(10.0f, 1.0f, 10.0f));
		field->GetTransform()->SetRot(QUATERNION::FromEuler({ 0.0f, 0.0f, -15.0f }));
	
		AddGameObject<ROTATE_BOARD>(LAYER::LAYER_GAME_OBJECT)->GetTransform()->SetPos(VECTOR3(0.0f, 1.5f, 40.0f));

		AddGameObject<ROTATE_BOARD>(LAYER::LAYER_GAME_OBJECT)->GetTransform()->SetPos(VECTOR3(0.0f, 1.5f, -40.0f));

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				AddGameObject<BLOCK>(LAYER::LAYER_GAME_OBJECT)->GetTransform()->SetPos(VECTOR3(i, j + 1.0f, 20.0f));
			}
		}

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				AddGameObject<BLOCK>(LAYER::LAYER_GAME_OBJECT)->GetTransform()->SetPos(VECTOR3(i, j + 1.0f, -20.0f));
			}
		}

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				AddGameObject<BLOCK>(LAYER::LAYER_GAME_OBJECT)->GetTransform()->SetPos(VECTOR3(20.0f, j + 1.0f, i));
			}
		}

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				AddGameObject<BLOCK>(LAYER::LAYER_GAME_OBJECT)->GetTransform()->SetPos(VECTOR3(-20.0f, j + 1.0f, i));
			}
		}

		BLOCK* sweets_a = AddGameObject<BLOCK>(LAYER::LAYER_GAME_OBJECT);
		sweets_a->GetTransform()->SetPos(VECTOR3(10.0f, 5.0f, 10.0f));
		RIGID_BODY* rb_a = sweets_a->GetComponent<RIGID_BODY>();
		//rb_a->SetType(RIGID_BODY_TYPE::STATIC);

		BLOCK* sweets_b = AddGameObject<BLOCK>(LAYER::LAYER_GAME_OBJECT);
		sweets_b->GetTransform()->SetPos(VECTOR3(15.0f, 3.0f, 10.0f));
		RIGID_BODY* rb_b = sweets_b->GetComponent<RIGID_BODY>();

		//PHYSICS_3D::GetInstance().CreateDistanceJoint(rb_a, rb_b, VECTOR3(10.0f, 5.0f, 10.0f), VECTOR3(15.0f, 3.0f, 10.0f), 4.0f);
		//PHYSICS_3D::GetInstance().CreateRopeJointMax(rb_a, rb_b, VECTOR3(10.0f, 5.0f, 10.0f), VECTOR3(15.0f, 7.0f, 10.0f), 4.0f);
		
		PHYSICS_3D::GetInstance().CreateLimitsJoint(rb_a, rb_b, VECTOR3(10.0f, 5.0f, 10.0f), VECTOR3(15.0f, 3.0f, 10.0f), 4.0f, 10.0f);

		FADE* fade = AddGameObject<FADE>(LAYER::LAYER_UI);
		fade->GetComponent<SIMPLE_FADE_COMPONENT>()->SetCurrentScene(SIMPLE_FADE_COMPONENT::CURRENT_SCENE::GAME);
		fade->GetComponent<SIMPLE_FADE_COMPONENT>()->StartFadeIn();
	}
}

void GAME::Uninit()
{
	SCENE::Uninit();
}

void GAME::Update()
{
	SCENE::Update();
}