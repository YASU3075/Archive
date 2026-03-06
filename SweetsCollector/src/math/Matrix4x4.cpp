//-----------------------------------------------
// Matrix4x4.cpp
// 制作日：2025/07/06
// 制作者：安田晴人
//-----------------------------------------------
#include "math/Matrix4x4.h"
#include <cmath>

//------------------------------------------------
// デフォルトコンストラクタ
//------------------------------------------------
MATRIX4X4::MATRIX4X4()
{
	*this = Identity();
}

//------------------------------------------------
// 引数付きコンストラクタ
//------------------------------------------------
MATRIX4X4::MATRIX4X4
(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33
)
{
	m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
	m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
	m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
	m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
}

//------------------------------------------------
// 単位行列を返す
//------------------------------------------------
MATRIX4X4 MATRIX4X4::Identity()
{
	return MATRIX4X4
	(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}

//------------------------------------------------
// 平行移動行列を返す
//------------------------------------------------
MATRIX4X4 MATRIX4X4::Translation(const VECTOR3& pos)
{
	return MATRIX4X4
	(
		1, 0, 0, pos.x,
		0, 1, 0, pos.y,
		0, 0, 1, pos.z,
		0, 0, 0, 1
	);
}

//------------------------------------------------
// 拡大縮小行列を返す
//------------------------------------------------
MATRIX4X4 MATRIX4X4::Scale(const VECTOR3& sca)
{
	return MATRIX4X4
	(
		sca.x, 0, 0, 0,
		0, sca.y, 0, 0,
		0, 0, sca.z, 0,
		0, 0, 0, 1
	);
}

//------------------------------------------------
// 回転行列を返す
//------------------------------------------------
MATRIX4X4 MATRIX4X4::Rotation(const QUATERNION& rot)
{
	QUATERNION q = rot.GetNormalized(); // 正規化されたクォータニオンを使用

	float xx = q.x * q.x;
	float yy = q.y * q.y;
	float zz = q.z * q.z;
	float xy = q.x * q.y;
	float xz = q.x * q.z;
	float yz = q.y * q.z;
	float wx = q.w * q.x;
	float wy = q.w * q.y;
	float wz = q.w * q.z;

	return MATRIX4X4
	(
		1 - 2 * (yy + zz), 2 * (xy - wz), 2 * (xz + wy), 0,
		2 * (xy + wz), 1 - 2 * (xx + zz), 2 * (yz - wx), 0,
		2 * (xz - wy), 2 * (yz + wx), 1 - 2 * (xx + yy), 0,
		0, 0, 0, 1
	);
}

//-----------------------------------------------
// 行列式を返す
//-----------------------------------------------
float MATRIX4X4::Determinant() const
{
	return
		m[0][0] * 
		(
			m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
			m[1][2] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) +
			m[1][3] * (m[2][1] * m[3][2] - m[2][2] * m[3][1])
		) -
		m[0][1] * 
		(
			m[1][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
			m[1][2] * (m[2][0] * m[3][3] - m[2][3] * m[3][0]) +
			m[1][3] * (m[2][0] * m[3][2] - m[2][2] * m[3][0])
		) +
		m[0][2] * 
		(
			m[1][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) -
			m[1][1] * (m[2][0] * m[3][3] - m[2][3] * m[3][0]) +
			m[1][3] * (m[2][0] * m[3][1] - m[2][1] * m[3][0])
		) -
		m[0][3] * 
		(
			m[1][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) -
			m[1][1] * (m[2][0] * m[3][2] - m[2][2] * m[3][0]) +
			m[1][2] * (m[2][0] * m[3][1] - m[2][1] * m[3][0])
		);
}

//-----------------------------------------------
// 随伴行列を返す
//-----------------------------------------------
MATRIX4X4 MATRIX4X4::Adjoint() const
{
	MATRIX4X4 cof;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			float sub[3][3];
			int subi = 0;
			for (int k = 0; k < 4; k++)
			{
				if (k == i)
				{
					continue;
				}

				int subj = 0;
				for (int l = 0; l < 4; l++)
				{
					if (l == j)
					{
						continue;
					}
					sub[subi][subj] = m[k][l];
					subj++;
				}
				subi++;
			}

			float det =
				sub[0][0] * (sub[1][1] * sub[2][2] - sub[1][2] * sub[2][1]) -
				sub[0][1] * (sub[1][0] * sub[2][2] - sub[1][2] * sub[2][0]) + 
				sub[0][2] * (sub[1][0] * sub[2][1] - sub[1][1] * sub[2][0]);

			cof.m[i][j] = ((i + j) % 2 == 0 ? 1.0f : -1.0f) * det;
		}
	}

	return cof.Transpose();
}

//-----------------------------------------------
// 転置行列
//-----------------------------------------------
MATRIX4X4 MATRIX4X4::Transpose() const
{
	MATRIX4X4 ret;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			ret.m[i][j] = m[j][i];
		}
	}
	return ret;
}

//-----------------------------------------------
// 逆行列
//-----------------------------------------------
MATRIX4X4 MATRIX4X4::Inverse() const
{
	float d = Determinant();
	if (fabs(d) < 1e-8f)
	{
		// 行列式が0に近い場合、逆行列は存在しない
		return Identity();
	}
	MATRIX4X4 adj = Adjoint();
	return adj * (1.0f / d);
}

//-----------------------------------------------
// 点の変換
//-----------------------------------------------
VECTOR3 MATRIX4X4::MultiplyPoint(const VECTOR3& v) const
{
	float x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
	float y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
	float z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
	float w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3];

	if (fabs(w) > 1e-6f)
	{
		return VECTOR3(x / w, y / w, z / w);
	}
	else
	{
		return VECTOR3(x, y, z);
	}
}

//-----------------------------------------------
// ベクトルの変換
//-----------------------------------------------
VECTOR3 MATRIX4X4::MultiplyVector(const VECTOR3& v) const
{
	float x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
	float y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
	float z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;
	return VECTOR3(x, y, z);
}

//-----------------------------------------------
// 行列の乗算
//-----------------------------------------------
MATRIX4X4 MATRIX4X4::operator*(const MATRIX4X4& other) const
{
	MATRIX4X4 result;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = 0.0f;
			for (int k = 0; k < 4; ++k)
			{
				result.m[i][j] += m[i][k] * other.m[k][j];
			}
		}
	}
	return result;
}

//------------------------------------------------
// スカラーとの乗算
//------------------------------------------------
MATRIX4X4 MATRIX4X4::operator*(float scalar) const
{
	MATRIX4X4 result;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			result.m[i][j] = m[i][j] * scalar;
		}
	}
	return result;
}