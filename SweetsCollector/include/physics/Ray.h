//-----------------------------------------------
// Ray.h
// 制作日：2025/09/25
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "math/Vector3.h"

class COLLIDER_3D;

struct RAY
{
	VECTOR3 origin;
	VECTOR3 direction;

	RAY(const VECTOR3& o, const VECTOR3& d) : origin(o), direction(d.GetNormalized()) {}

	VECTOR3 GetPoint(float distance) const
	{
		return origin + direction * distance;
	}
};

struct RAY_CAST
{
	VECTOR3 point;    // 衝突点
	VECTOR3 normal;
	float distance; // 衝突距離
	class COLLIDER_3D* collider = nullptr;
};
