//-----------------------------------------------
// RotationObjComponent.h
// 制作日：2025/12/19
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "math/Vector3.h"
#include "component/Component.h"

class ROTATION_OBJ_COMPONENT : public COMPONENT
{
private:
	VECTOR3 m_rotation_speed = VECTOR3(0.0f, 0.0f, 0.0f); // 回転速度
public:
	void Init() override;
	void Update() override;

	void SetRotationSpeed(const VECTOR3& speed) { m_rotation_speed = speed; }
};