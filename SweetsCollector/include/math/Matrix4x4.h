//-----------------------------------------------
// Matrix4x4.h
// 制作日：2025/07/06
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "Vector3.h"
#include "Quaternion.h"

class MATRIX4X4
{
public:
	float m[4][4];

	MATRIX4X4();
	MATRIX4X4
	(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33
	);

	static MATRIX4X4 Identity();							//単位行列を返す
	static MATRIX4X4 Translation(const VECTOR3& pos);		//平行移動行列
	static MATRIX4X4 Scale(const VECTOR3& sca);				//拡大縮小行列
	static MATRIX4X4 Rotation(const QUATERNION& rot);		//回転行列

	float Determinant() const;							//行列式を返す
	MATRIX4X4 Adjoint() const;							//随伴行列を返す

	MATRIX4X4 Transpose() const;							//転置行列を返す
	MATRIX4X4 Inverse() const;								//逆行列を返す

	VECTOR3 MultiplyPoint(const VECTOR3& v) const;			// 点の変換
	VECTOR3 MultiplyVector(const VECTOR3& v) const;			// ベクトルの変換

	MATRIX4X4 operator*(const MATRIX4X4& other) const;
	MATRIX4X4 operator*(float scalar) const;

};