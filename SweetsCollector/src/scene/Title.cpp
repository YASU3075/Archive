//-----------------------------------------------
// Title.cpp
// 制作日：2025/06/25
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "scene/Manager.h"
#include "renderer/Renderer.h"

#include "input/Input.h"
#include "input/GamePad.h"

#include "object/Polygon.h"
#include "object/TitleRogo.h"
#include "object/FlushRogo.h"
#include "object/Camera.h"
#include "title/object/WaterFall.h"
#include "title/object/River.h"
#include "title/object/Shore.h"

#include "title/object/Spline.h"
#include "title/component/SplineComponent.h"

#include "title/object/TitleSweets.h"
#include "title/component/SplineMoveComponent.h"

#include "title/object/TitleSweetsGenerator.h"

#include "title/object/Panda.h"

#include "object/particle/ChocolateFountainParticle.h"

#include "object/DirectionalLight.h"

#include "object/Fade.h"
#include "component/SimpleFadeComponent.h"
#include "component/CameraOrbitComponent.h"

#include "object/SkyDome.h"

#include "scene/Scene.h"
#include "scene/Game.h"
#include "scene/Title.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void TITLE::Init()
{
	CAMERA* cam = AddGameObject<CAMERA>(LAYER::LAYER_CAMERA);
	CAMERA_ORBIT_COMPONENT* orbit = cam->GetComponent<CAMERA_ORBIT_COMPONENT>();
	orbit->SetDistance(10.0f);
	orbit->SetHeight(5.0f);
	orbit->SetMouseSensitivity(0.0f);

	AddGameObject<SKY_DOME>(LAYER::LAYER_BACKGROUND);

	DIRECTIONAL_LIGHT* dir_light = AddGameObject<DIRECTIONAL_LIGHT>(LAYER::LAYER_GAME_OBJECT);
	dir_light->GetTransform()->SetPos(VECTOR3(0.0f, 100.0f, 0.0f));
	dir_light->GetTransform()->SetRot(QUATERNION::FromEuler({ 90.0f, 0.0f, 0.0f }));

	WATER_FALL* water_fall = AddGameObject<WATER_FALL>(LAYER::LAYER_GAME_OBJECT);
	water_fall->GetTransform()->SetPos(VECTOR3(0.0f, 15.0f, 20.0f));
	water_fall->GetTransform()->SetSca(VECTOR3(10.0f, 40.0f, 1.0f));
	
	RIVER* river = AddGameObject<RIVER>(LAYER::LAYER_GAME_OBJECT);
	river->GetTransform()->SetPos(VECTOR3(0.0f, -0.5f, 5.0f));
	river->GetTransform()->SetSca(VECTOR3(15.0f, 1.0f, 30.0f));

	SHORE* shore = AddGameObject<SHORE>(LAYER::LAYER_GAME_OBJECT);
	shore->GetTransform()->SetPos(VECTOR3(0.0f, 0.0f, 0.0f));

	SPLINE_COMPONENT* spline_component = AddGameObject<SPLINE>(LAYER::LAYER_GAME_OBJECT)->GetComponent<SPLINE_COMPONENT>();
	spline_component->AddControlPoint(0, VECTOR3(0.0f, 40.0f, 19.0f));
	spline_component->AddControlPoint(1, VECTOR3(0.0f, 0.0f, 19.0f));
	spline_component->AddControlPoint(2, VECTOR3(0.0f, 0.0f, -7.0f));
	spline_component->AddControlPoint(3, VECTOR3(0.0f, -5.0f, -7.0f));
	spline_component->AddControlPoint(4, VECTOR3(0.0f, -5.0f, 22.0f));
	spline_component->AddControlPoint(4, VECTOR3(0.0f, 40.0f, 22.0f));

	AddGameObject<TITLE_SWEETS_GENERATOR>(LAYER::LAYER_GAME_OBJECT);

	PANDA* panda = AddGameObject<PANDA>(LAYER::LAYER_GAME_OBJECT);
	panda->GetTransform()->SetPos(VECTOR3(-10.0f, 5.0f, 10.0f));
	panda->GetTransform()->SetRot(QUATERNION::FromEuler(VECTOR3(0.0f, 150.0f, 0.0f)));

	AddGameObject<CHOCOLATE_FOUNTAIN_PARTICLE>(LAYER::LAYER_GAME_OBJECT)->GetTransform()->SetPos(VECTOR3(0.0f, 1.0f, 16.0f));

	AddGameObject<TITLE_ROGO>(LAYER::LAYER_UI);

	AddGameObject<FLUSH_ROGO>(LAYER::LAYER_UI);

	FADE* fade = AddGameObject<FADE>(LAYER::LAYER_UI);
	auto fade_component = fade->GetComponent<SIMPLE_FADE_COMPONENT>();
	fade_component->SetCurrentScene(SIMPLE_FADE_COMPONENT::CURRENT_SCENE::TITLE);
	fade_component->StartFadeIn();
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void TITLE::Update()
{
	SCENE::Update();
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		GetGameObject<FADE>()->GetComponent<SIMPLE_FADE_COMPONENT>()->StartFadeOut();
	}

	if (INPUT_SYSTEM::Instance().GetGamePad(0).Pressed(PAD_BUTTON::A))
	{
		GetGameObject<FADE>()->GetComponent<SIMPLE_FADE_COMPONENT>()->StartFadeOut();
	}
}