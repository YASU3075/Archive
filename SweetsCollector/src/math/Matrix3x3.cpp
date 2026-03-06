//-----------------------------------------------
// Matrix3x3.cpp
// 制作日：2025/09/27
// 制作者：安田晴人
//-----------------------------------------------
#include "math/Matrix3x3.h"
#include "math/Quaternion.h"

//-----------------------------------------------
// ゼロ行列を返す
//-----------------------------------------------
MATRIX3x3 MATRIX3x3::Zero()
{
	return MATRIX3x3
	(
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	);
}

//-----------------------------------------------
// 単位行列を返す
//-----------------------------------------------
MATRIX3x3 MATRIX3x3::Identity()
{
	return MATRIX3x3
	(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	);
}

//-----------------------------------------------
// 転置行列を返す
//-----------------------------------------------
MATRIX3x3 MATRIX3x3::Transpose() const
{
	MATRIX3x3 transposed;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			transposed.m[i][j] = m[j][i];
		}
	}
	return transposed;
}

//-----------------------------------------------
// ベクトルを掛ける
//-----------------------------------------------
VECTOR3 MATRIX3x3::MultiplyVector(const VECTOR3& v) const
{
	return VECTOR3
	(
		m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
		m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
		m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
	);
}

//-----------------------------------------------
// 行列式を計算
//-----------------------------------------------
float MATRIX3x3::Determinant() const
{
	return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
		m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
		m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
}

//-----------------------------------------------
// 逆行列を計算
//-----------------------------------------------
MATRIX3x3 MATRIX3x3::Inverse() const
{
	float det = Determinant();
	if (fabsf(det) < 1e-6f)
	{
		return MATRIX3x3::Identity();
	}

	float inv_det = 1.0f / det;

	MATRIX3x3 inv;
	inv.m[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * inv_det;
	inv.m[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * inv_det;
	inv.m[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * inv_det;

	inv.m[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * inv_det;
	inv.m[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * inv_det;
	inv.m[1][2] = (m[0][2] * m[1][0] - m[0][0] * m[1][2]) * inv_det;

	inv.m[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * inv_det;
	inv.m[2][1] = (m[0][1] * m[2][0] - m[0][0] * m[2][1]) * inv_det;
	inv.m[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * inv_det;

	return inv;
}

//-----------------------------------------------
// 慣性テンソルの回転変換
//-----------------------------------------------
MATRIX3x3 MATRIX3x3::RotateTensor(const MATRIX3x3& rotation_matrix) const
{
	return rotation_matrix * (*this) * rotation_matrix.Transpose();
}

//-----------------------------------------------
// クォータニオンから回転行列を作成
//-----------------------------------------------
MATRIX3x3 MATRIX3x3::RotationFromQuaternion(const QUATERNION& q)
{
	QUATERNION quat = q.GetNormalized(); // 正規化されたクォータニオンを使用
	MATRIX3x3 rotation;

	float x = quat.x;
	float y = quat.y;
	float z = quat.z;
	float w = quat.w;

	//回転行列を計算
	rotation.m[0][0] = 1 - 2 * (y * y + z * z);
	rotation.m[0][1] = 2 * (x * y - w * z);
	rotation.m[0][2] = 2 * (x * z + w * y);

	rotation.m[1][0] = 2 * (x * y + w * z);
	rotation.m[1][1] = 1 - 2 * (x * x + z * z);
	rotation.m[1][2] = 2 * (y * z - w * x);

	rotation.m[2][0] = 2 * (x * z - w * y);
	rotation.m[2][1] = 2 * (y * z + w * x);
	rotation.m[2][2] = 1 - 2 * (x * x + y * y);

	return rotation;
}

//-----------------------------------------------
// 指定列を取得
//-----------------------------------------------
VECTOR3 MATRIX3x3::GetColumn(int index) const
{
	if (index < 0 || index >= 3)
	{
		return VECTOR3(0.0f, 0.0f, 0.0f);
	}
	return VECTOR3(m[0][index], m[1][index], m[2][index]);
}

//-----------------------------------------------
// 全要素がゼロかどうかをチェック
//-----------------------------------------------
bool MATRIX3x3::IsZero() const
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (m[i][j] != 0.0f)
			{
				return false;
			}
		}
	}
	return true;
}


