//-----------------------------------------------
// CameraShakeComponent.cpp
// 制作日：2025/12/18
// 制作者：安田晴人
//-----------------------------------------------
#include "component/CameraShakeComponent.h"
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "entity/GameObject.h"
#include "object/Camera.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void CAMERA_SHAKE_COMPONENT::Init()
{
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void CAMERA_SHAKE_COMPONENT::Update()
{
	if (m_time <= 0.0f)
	{
		return;
	}

	float dt = GetDeltaTimeSafe();

	m_time -= dt;
	if (m_time < 0.0f)
	{
		m_time = 0.0f;
	}

	float life01 = (m_duration > 0.0f) ? (m_time / m_duration) : 0.0f;
	float strength = Smooth01(life01);

	m_phase += m_frequency * dt;

	VECTOR3 pos_offset = VECTOR3(
		Noise1D(m_phase, 12.9898f, 78.233f) * m_pos_amp.x,
		Noise1D(m_phase, 45.164f, 39.425f) * m_pos_amp.y,
		Noise1D(m_phase, 94.673f, 12.345f) * m_pos_amp.z
	) * strength;

	GetOwner()->GetTransform()->AddPos(pos_offset);

	if (m_rot_strength > 0.0f)
	{
		VECTOR3 rot_offset_deg;
		rot_offset_deg.x = Noise1D(m_phase, 21.7f, 11.3f);
		rot_offset_deg.y = Noise1D(m_phase, 17.1f, 29.9f);
		rot_offset_deg.z = Noise1D(m_phase, 9.2f, 31.4f);
		rot_offset_deg *= m_rot_amp * strength * m_rot_strength;

		QUATERNION qx = QUATERNION::FromAxisAngle(VECTOR3(1.0f, 0.0f, 0.0f), rot_offset_deg.x * 3.141592f / 180.0f);
		QUATERNION qy = QUATERNION::FromAxisAngle(VECTOR3(0.0f, 1.0f, 0.0f), rot_offset_deg.y * 3.141592f / 180.0f);
		QUATERNION qz = QUATERNION::FromAxisAngle(VECTOR3(0.0f, 0.0f, 1.0f), rot_offset_deg.z * 3.141592f / 180.0f);

		QUATERNION addRot = qy * qx * qz;

		GetOwner()->GetTransform()->AddRot(addRot);
	}

	if (m_time <= 0.0f)
	{
		m_pos_amp = VECTOR3(0.0f, 0.0f, 0.0f);
		m_rot_amp = VECTOR3(0.0f, 0.0f, 0.0f);
		m_duration = 0.0f;
	}

}

//-----------------------------------------------
// 揺らす処理（等倍）
//-----------------------------------------------
void CAMERA_SHAKE_COMPONENT::Shake(float amplitude, float frequency, float duration)
{
	VECTOR3 pos_amp = VECTOR3(amplitude, amplitude, amplitude);
	VECTOR3 rot_amp = VECTOR3(amplitude * 2.0f, amplitude * 2.0f, amplitude * 1.0f);
	Shake(pos_amp, rot_amp, frequency, duration);
}

//-----------------------------------------------
// 揺らす処理（位置・回転別指定）
//-----------------------------------------------
void CAMERA_SHAKE_COMPONENT::Shake(const VECTOR3& pos_amplitude, const VECTOR3& rot_amplitude_deg, float frequency, float duration)
{
	m_pos_amp = VECTOR3(
		std::max(m_pos_amp.x, pos_amplitude.x),
		std::max(m_pos_amp.y, pos_amplitude.y),
		std::max(m_pos_amp.z, pos_amplitude.z)
	);

	m_rot_amp = VECTOR3(
		std::max(m_rot_amp.x, rot_amplitude_deg.x),
		std::max(m_rot_amp.y, rot_amplitude_deg.y),
		std::max(m_rot_amp.z, rot_amplitude_deg.z)
	);

	m_frequency = std::max(m_frequency, frequency);

	m_time = std::max(m_time, duration);
	m_duration = std::max(m_duration, duration);
}

//-----------------------------------------------
// 停止処理
//-----------------------------------------------
void CAMERA_SHAKE_COMPONENT::Stop()
{
	m_time = 0.0f;
	m_pos_amp = VECTOR3(0.0f, 0.0f, 0.0f);
	m_rot_amp = VECTOR3(0.0f, 0.0f, 0.0f);
	m_duration = 0.0f;
}

//-----------------------------------------------
// 安全なデルタタイム取得
//-----------------------------------------------
float CAMERA_SHAKE_COMPONENT::GetDeltaTimeSafe() const
{
	return 1.0f / 60.0f;
}

//-----------------------------------------------
// 0->1のスムーズステップ
//-----------------------------------------------
float CAMERA_SHAKE_COMPONENT::Smooth01(float t) const
{
	t = std::clamp(t, 0.0f, 1.0f);
	return t * t * (3.0f - 2.0f * t);
}

//-----------------------------------------------
// 1Dノイズ関数
//-----------------------------------------------
float CAMERA_SHAKE_COMPONENT::Noise1D(float t, float a, float b) const
{
	return 0.5f * std::sinf(t * a) + 0.5f * std::sinf(t * b);
}