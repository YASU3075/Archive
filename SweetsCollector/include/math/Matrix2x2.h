//-----------------------------------------------
// Matrix2x2.h
// 制作日：2025/06/13
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include "vector3.h"

class MATRIX2x2
{
public:
	float m00, m01;
	float m10, m11;

	static MATRIX2x2 Rotation(float angle)
	{
		float c = cosf(angle);
		float s = sinf(angle);
		return { c, -s, s, c };
	}

	//転置を返す
	MATRIX2x2 Transpose() const
	{
		return MATRIX2x2{ m00, m10, m01, m11 };
	}

	VECTOR3 operator*(const VECTOR3& v) const
	{
		return { m00 * v.x + m01 * v.y, m10 * v.x + m11 * v.y , 0.0f};
	}
};
