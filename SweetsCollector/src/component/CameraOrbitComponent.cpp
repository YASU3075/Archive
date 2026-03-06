//-----------------------------------------------
// CameraOrbitComponent.cpp
// 制作日：2025/12/10
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "component/CameraOrbitComponent.h"
#include "entity/GameObject.h"
#include "object/Camera.h"
#include "input/Input.h"
#include "input/Mouse.h"
#include "input/GamePad.h"

#include "scene/Manager.h"
#include "renderer/Renderer.h"


//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void CAMERA_ORBIT_COMPONENT::Init()
{
	if (m_target)
	{
		VECTOR3 target_pos = m_target->GetPos();
		VECTOR3 cam_pos = m_p_owner->GetTransform()->GetPos();
		VECTOR3 offset = cam_pos - target_pos;

		float horiaontal_distance = sqrtf(offset.x * offset.x + offset.z * offset.z);
		m_distance = horiaontal_distance;
		ClampDistance();

		if (horiaontal_distance > 1e-4f)
		{
			m_yaw = atan2f(offset.z, offset.x);
		}
		
		m_height = offset.y;
	}
}

//-----------------------------------------------
// 終了処理
//-----------------------------------------------
void CAMERA_ORBIT_COMPONENT::Uninit()
{

}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void CAMERA_ORBIT_COMPONENT::Update()
{
	if (!m_target)
	{
		return;
	}

	// マウス入力取得
	Mouse_State mouse{};
	Mouse_GetState(&mouse);

	static bool prev_right_button = false;
	const bool cur_right = mouse.rightButton;

	if (cur_right && !prev_right_button)
	{
		Mouse_SetMode(MOUSE_POSITION_MODE_RELATIVE);
	}
	else if (!cur_right && prev_right_button)
	{
		Mouse_SetMode(MOUSE_POSITION_MODE_ABSOLUTE);
	}

	prev_right_button = cur_right;

	// オービット操作
	float orbit_x = 0.0f;
	float zoom = 0.0f;

	if (Input::GetKeyPress(VK_LEFT))
	{
		orbit_x += 0.4f;
	}
	if (Input::GetKeyPress(VK_RIGHT))
	{
		orbit_x -= 0.4f;
	}

	if (INPUT_SYSTEM::Instance().GetGamePad(0).Connected())
	{
		float rx = INPUT_SYSTEM::Instance().GetGamePad(0).RightStick().x;
		orbit_x += rx * -0.3f;
	}

	if (cur_right && mouse.positionMode == MOUSE_POSITION_MODE_RELATIVE)
	{
		orbit_x = -GetOrbitInputHorizontal(mouse);
	}

	zoom = GetOrbitInputZoom(mouse);

	m_yaw += orbit_x * m_mouse_sensitivity;

	m_distance -= zoom * m_wheel_sensitivity;
	ClampDistance();

	// カメラ位置計算
	const VECTOR3 target_pos = m_target->GetPos();

	const float cos_y = cosf(m_yaw);
	const float sin_y = sinf(m_yaw);

	VECTOR3 cam_pos;
	cam_pos.x = target_pos.x + m_distance * cos_y;
	cam_pos.y = target_pos.y + m_height;
	cam_pos.z = target_pos.z + m_distance * sin_y;

	m_p_owner->GetTransform()->SetPos(cam_pos);
	m_p_owner->GetTransform()->SetRot(m_p_owner->GetTransform()->GetRot().LookRotation((target_pos - cam_pos).GetNormalized(), VECTOR3(0.0f, 1.0f, 0.0f)));
}

//-----------------------------------------------
// 距離のクランプ
//-----------------------------------------------
void CAMERA_ORBIT_COMPONENT::ClampDistance()
{
	if (m_distance < m_min_distance)
	{
		m_distance = m_min_distance;
	}
	if (m_distance > m_max_distance)
	{
		m_distance = m_max_distance;
	}
}

//-----------------------------------------------
// オービット入力の取得（水平）
//-----------------------------------------------
float CAMERA_ORBIT_COMPONENT::GetOrbitInputHorizontal(const Mouse_State& state)
{
	if (state.positionMode == MOUSE_POSITION_MODE_RELATIVE)
	{
		// 相対モードなら x が「そのフレームの移動量」
		return static_cast<float>(state.x);
	}
	else
	{
		// 絶対モードの場合は、とりあえず何もしない（必要なら差分計算もアリ）
		return 0.0f;
	}
}

//-----------------------------------------------
// ズーム入力の取得
//-----------------------------------------------
float CAMERA_ORBIT_COMPONENT::GetOrbitInputZoom(const Mouse_State& state)
{
	if (state.scrollWheelValue != 0)
	{
		const float zoomSteps = static_cast<float>(state.scrollWheelValue) / static_cast<float>(WHEEL_DELTA);

		// 使い終わったのでリセットしておく
		Mouse_ResetScrollWheelValue();

		return zoomSteps;
	}

	return 0.0f;
}
