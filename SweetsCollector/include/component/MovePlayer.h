//-----------------------------------------------
// MovePlayer.h
// 制作日：2025/07/23
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "math/Vector3.h"
#include "Component.h"

class MOVE_PLAYER : public COMPONENT
{
private:
	class CAMERA* m_p_camera; // カメラ
	class RIGID_BODY* m_p_rigid_body; // リジッドボディ

	float m_speed; // 移動速度
	const float MAX_SPEED = 5.0f; // 最大速度
	bool m_is_active = true; // 有効フラグ

	void RotateToDirectionYaw(const VECTOR3& direction);
public:
	void Init() override;
	void Update() override;

	void SetSpeed(float speed) { m_speed = speed; }
	float GetSpeed() const { return m_speed; }

	const float GetMaxSpeed() const { return MAX_SPEED; }

	void SetActive(bool is_active) { m_is_active = is_active; }
};
