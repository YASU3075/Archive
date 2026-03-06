//-----------------------------------------------
// MovePlayer.cpp
// 制作日：2025/07/23
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "scene/Manager.h"
#include "renderer/Renderer.h"
#include "entity/GameObject.h"
#include "scene/Scene.h"
#include "object/Camera.h"
#include "physics/RigidBody.h"
#include "component/MovePlayer.h"
#include "game/component/PlayerAnimComponent.h"
#include "component//renderer/RendererComponent.h"
#include "input/Input.h"
#include "input/GamePad.h"
#include "EngineTime.h"


//------------------------------------------------
// 初期化処理
//------------------------------------------------
void MOVE_PLAYER::Init()
{
	m_speed = 1.0f;
	m_p_camera = Manager::GetScene()->GetGameObject<CAMERA>();
	m_p_rigid_body = GetOwner()->GetComponent<RIGID_BODY>();
	m_is_active = true;
}

//------------------------------------------------
// 更新処理
//------------------------------------------------
void MOVE_PLAYER::Update()
{
	if (!m_is_active)
	{
		return;
	}

	QUATERNION cam_q = m_p_camera->GetTransform()->GetRot();

	VECTOR3 forward = cam_q.Rotate({0.0f, 0.0f, 1.0f});
	VECTOR3 right = cam_q.Rotate({ 1.0f, 0.0f, 0.0f });

	forward.y = 0.0f; // 水平方向の移動
	right.y = 0.0f;   // 水平方向の移動

	forward.Normalize();
	right.Normalize();

	VECTOR3 input = { 0.0f, 0.0f, 0.0f };

	if (Input::GetKeyPress('W'))
	{
		input += VECTOR3(0.0f, 0.0f, 1.0f);
	}

	if (INPUT_SYSTEM::Instance().GetGamePad().Connected())
	{
		float lx = INPUT_SYSTEM::Instance().GetGamePad().LeftStick().x;
		float ly = INPUT_SYSTEM::Instance().GetGamePad().LeftStick().y;
		input += VECTOR3(lx, 0.0f, ly);
	}

	if (Input::GetKeyPress('S'))
	{
		input += VECTOR3(0.0f, 0.0f, -1.0f);
	}

	if (Input::GetKeyPress('A'))
	{
		input += VECTOR3(-1.0f, 0.0f, 0.0f);
	}

	if (Input::GetKeyPress('D'))
	{
		input += VECTOR3(1.0f, 0.0f, 0.0f);
	}

	{
		VECTOR3 v = m_p_rigid_body->GetLinearVelocity();
		VECTOR3 vh(v.x, 0, v.z);

		VECTOR3 input_dir = forward * input.z + right * input.x;
		if (input_dir.LengthSquared() > 1e-8f)
		{
			input_dir.Normalize();
			RotateToDirectionYaw(input_dir);
		}
		else
		{
			input_dir = VECTOR3(0, 0, 0);
		}

		VECTOR3 v_target = input_dir * m_speed;
		VECTOR3 dv = v_target - vh;

		// 1秒あたりどれだけ誤差を潰すか
		float gain = 20.0f;

		// 加速度として与える
		VECTOR3 accel = dv * gain;

		
		float max_accel = 50.0f;
		float len = accel.Length();
		if (len > max_accel)
		{
			accel = accel * (max_accel / len);
		}

		PLAYER_ANIM_COMPONENT* anim = GetOwner()->GetComponent<PLAYER_ANIM_COMPONENT>();
		if (anim)
		{
			VECTOR3 v = m_p_rigid_body->GetLinearVelocity();
			VECTOR3 vh(v.x, 0, v.z);
			float speed = std::clamp(vh.Length() / m_speed, 0.0f, 1.0f);
			anim->SetMoveAmount(speed);
		}
		
		m_p_rigid_body->AddForce(VECTOR3(accel.x, 0, accel.z), FORCE_MODE::ACCELERATION);
	}
}

//------------------------------------------------
// プレイヤーを指定方向にY軸回転させる
//------------------------------------------------
void MOVE_PLAYER::RotateToDirectionYaw(const VECTOR3& direction)
{
	RENDERER_COMPONENT* renderer = GetOwner()->GetComponent<RENDERER_COMPONENT>();

	VECTOR3 fwd = direction;
	fwd.y = 0.0f;
	if (fwd.LengthSquared() < 1e-8f)
	{
		return;
	}

	fwd.Normalize();
	
	// 目的の回転を計算
	QUATERNION target = QUATERNION::LookRotation(fwd, VECTOR3(0, 1, 0));

	// モデルの向き補正
	QUATERNION offset = QUATERNION::FromAxisAngle(VECTOR3(0, 1, 0), 3.141592f * 0.5f);
	target = target * offset;

	// 現在の回転を取得
	QUATERNION cur = renderer->GetLocalRot();

	// スムーズに回転させる
	float turn_speed = 12.0f;
	float t = 1.0f - std::exp(-turn_speed * TIME::DeltaTime());
	QUATERNION new_rot = QUATERNION::Slerp(cur, target, t);
	
	// 回転を適用
	
	if (renderer)
	{
		renderer->SetLocalRot(new_rot);
	}
}
