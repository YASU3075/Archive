//-----------------------------------------------
// Camera.cpp
// 制作日：2025/05/14
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "object/Camera.h"
#include "scene/Manager.h"
#include "input/Input.h"
#include "entity/GameObject.h"
#include "component/CameraComponent.h"
#include "component/CameraOrbitComponent.h"
#include "component/CameraShakeComponent.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void CAMERA::Init(void)
{
	m_transform.SetPos(VECTOR3(0.0f, 5.0f, -10.0f));
	m_target = VECTOR3(0.0f, 0.0f, 0.0f);
	AddComponent<CAMERA_COMPONENT>();
	CAMERA_ORBIT_COMPONENT* component = AddComponent<CAMERA_ORBIT_COMPONENT>();

	component->SetDistance(10.0f);
	component->SetMinDistance(2.0f);
	component->SetMaxDistance(20.0f);
	component->SetHeight(10.0f);
	component->SetMouseSensitivity(0.05f);

	AddComponent<CAMERA_SHAKE_COMPONENT>();
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void CAMERA::Draw(void)
{
	GetTransform()->ClearAdditive();
}