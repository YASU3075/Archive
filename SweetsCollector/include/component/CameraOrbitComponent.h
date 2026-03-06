//-----------------------------------------------
// CameraOrbitComponent.h
// 制作日：2025/12/10
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "component/Component.h"
#include "input/Mouse.h"

class TRANSFORM;

class CAMERA_ORBIT_COMPONENT : public COMPONENT
{
private:
	TRANSFORM* m_target = nullptr;

	float m_yaw = 0.0f;
	float m_distance = 10.0f;
	float m_min_distance = 2.0f;
	float m_max_distance = 20.0f;
	float m_height = 2.0f;

	float m_mouse_sensitivity = 0.2f;
	float m_wheel_sensitivity = 1.0f;

	void ClampDistance();

	float GetOrbitInputHorizontal(const Mouse_State& state);
	float GetOrbitInputZoom(const Mouse_State& state);

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	
	void SetTarget(TRANSFORM* target) { m_target = target; }

	void SetDistance(float distance) { m_distance = distance; ClampDistance(); }
	void SetMinDistance(float min_distance) { m_min_distance = min_distance; ClampDistance(); }
	void SetMaxDistance(float max_distance) { m_max_distance = max_distance; ClampDistance(); }
	void SetHeight(float height) { m_height = height; }

	void SetMouseSensitivity(float sensitivity) { m_mouse_sensitivity = sensitivity; }
	void SetWheelSensitivity(float sensitivity) { m_wheel_sensitivity = sensitivity; }
};