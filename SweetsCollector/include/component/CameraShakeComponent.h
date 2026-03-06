//-----------------------------------------------
// CameraShakeComponent.h
// 制作日：2025/12/18
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "math/Vector3.h"
#include "component/Component.h"

class CAMERA_SHAKE_COMPONENT : public COMPONENT
{
private:
	float m_time = 0.0f;
	float m_duration = 0.0f;
	float m_frequency = 0.0f;
	float m_phase = 0.0f;

	VECTOR3 m_pos_amp = VECTOR3(0.0f, 0.0f, 0.0f);
	VECTOR3 m_rot_amp = VECTOR3(0.0f, 0.0f, 0.0f);

	float m_rot_strength = 0.0f;

	float GetDeltaTimeSafe() const;
	float Smooth01(float t) const;

	float Noise1D(float t, float a, float b) const;
public:
	void Init() override;
	void Update() override;

	void Shake(float amplitude, float frequency, float dulation);

	void Shake(const VECTOR3& pos_amplitude, const VECTOR3& rot_amplitude_deg, float frequency, float duration);

	void Stop();

	void SetRotStrength(float strength) { m_rot_strength = strength; }


};
