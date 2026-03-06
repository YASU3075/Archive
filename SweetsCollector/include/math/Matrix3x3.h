//-----------------------------------------------
// Matrix3x3.h
// 制作日：2025/07/07
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "Vector3.h"

class QUATERNION;

class MATRIX3x3
{
public:
	float m[3][3]; // 3x3行列の要素

	MATRIX3x3()
	{
		*this = Identity();
	}

	MATRIX3x3(float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22)
	{
		m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
		m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
		m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
	}

	static MATRIX3x3 Zero();

	static MATRIX3x3 Identity();

	MATRIX3x3 Transpose() const;

	MATRIX3x3 operator*(float scalar) const
	{
		return MATRIX3x3
		(
			m[0][0] * scalar, m[0][1] * scalar, m[0][2] * scalar,
			m[1][0] * scalar, m[1][1] * scalar, m[1][2] * scalar,
			m[2][0] * scalar, m[2][1] * scalar, m[2][2] * scalar
		);
	}

	MATRIX3x3 operator*(const MATRIX3x3& other) const
	{
		MATRIX3x3 result;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				result.m[i][j] = 0.0f;
				for (int k = 0; k < 3; k++)
				{
					result.m[i][j] += m[i][k] * other.m[k][j];
				}
			}
		}
		return result;
	}

	VECTOR3 MultiplyVector(const VECTOR3& v) const;

	float Determinant() const;

	MATRIX3x3 Inverse() const;

	MATRIX3x3 RotateTensor(const MATRIX3x3& rotation_matrix) const;

	
	static MATRIX3x3 RotationFromQuaternion(const QUATERNION& q);

	VECTOR3 GetColumn(int index) const;

	bool IsZero() const;

};