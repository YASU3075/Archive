//-----------------------------------------------
// Quaternion.h
// 制作日：2025/06/25
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "Vector3.h"
#include "Matrix3x3.h"

class QUATERNION
{
public:
	float x, y, z, w;

	//コンストラクタ
	QUATERNION() : x(0), y(0), z(0), w(1) {}
	QUATERNION(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	//単位クオータニオン
	static QUATERNION Identity()
	{
		return QUATERNION(0, 0, 0, 1);
	}

	//軸と角度から
	static QUATERNION FromAxisAngle(const VECTOR3& axis, float rad)
	{
		float half = rad * 0.5f;
		float sin_half = sinf(half);
		VECTOR3 norm = axis.GetNormalized();

		return QUATERNION(norm.x * sin_half, norm.y * sin_half, norm.z * sin_half, cosf(half));
	}

	//オイラー角から
	//static QUATERNION Euler

	//正規化
	QUATERNION GetNormalized() const
	{
		float len = Length();
		if (len < 1e-8f)
		{
			return Identity();
		}
		return QUATERNION(x / len, y / len, z / len, w / len);
	}

	//長さ
	float Length() const
	{
		return sqrtf(x * x + y * y + z * z + w * w);
	}

	//複素共役
	QUATERNION Conjugate() const
	{
		return QUATERNION(-x, -y, -z, w);
	}

	//逆
	QUATERNION Inverse() const
	{
		float len_sq = x * x + y * y + z * z + w * w;
		if (len_sq == 0)
		{
			return Identity();
		}
		return Conjugate() * (1.0f / len_sq);
	}

	//ベクトル回転
	VECTOR3 Rotate(const VECTOR3& v) const
	{
		QUATERNION qv(v.x, v.y, v.z, 0.0f);
		QUATERNION ret = (*this) * qv * Inverse();
		return VECTOR3(ret.x, ret.y, ret.z);
	}

	//----------------------------------------------
	// 演算子オーバーロード
	//----------------------------------------------

	//乗算
	QUATERNION operator*(const QUATERNION& q) const
	{
		return QUATERNION
		(
			w * q.x + x * q.w + y * q.z - z * q.y,
			w * q.y - x * q.z + y * q.w + z * q.x,
			w * q.z + x * q.y - y * q.x + z * q.w,
			w * q.w - x * q.x - y * q.y - z * q.z
		);
	}

	QUATERNION& operator*=(const QUATERNION& q)
	{
		*this = (*this) * q;
		return *this;
	}

	//スカラー倍
	QUATERNION operator*(float scalar) const
	{
		return QUATERNION(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	//加算
	QUATERNION operator+(const QUATERNION& a) const
	{
		return QUATERNION(x + a.x, y + a.y, z + a.z, w + a.w);
	}

	//線形補間
	static QUATERNION Lerp(const QUATERNION& a, const QUATERNION& b, float t)
	{
		return (a * (1 - t) + b * t).GetNormalized();
	}

	//球面補間
	static QUATERNION Slerp(const QUATERNION& a, const QUATERNION& b, float t)
	{
		float dot = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
		QUATERNION b_copy = b;

		if (dot < 0.0f)
		{
			dot = -dot;
			b_copy = QUATERNION(-b.x, -b.y, -b.z, -b.w);
		}

		if (dot > 0.9995f)
		{
			return Lerp(a, b_copy, t);
		}

		float theta_0 = acosf(dot);
		float theta = theta_0 * t;

		float sin_theta = sinf(theta);
		float sin_theta_0 = sinf(theta_0);

		float s0 = cosf(theta) - dot * sin_theta / sin_theta_0;
		float s1 = sin_theta / sin_theta_0;

		return (a * s0 + b_copy * s1).GetNormalized();
	}

	//オイラー角からクオータニオンに
	static QUATERNION FromEuler(const VECTOR3& euler)
	{
		VECTOR3 rad = euler * 3.141592f / 180.0f; //度からラジアンへ変換

		float cx = cosf(rad.x * 0.5f);
		float sx = sinf(rad.x * 0.5f);
		float cy = cosf(rad.y * 0.5f);
		float sy = sinf(rad.y * 0.5f);
		float cz = cosf(rad.z * 0.5f);
		float sz = sinf(rad.z * 0.5f);

		QUATERNION q
		(
			sx * cy * cz - cx * sy * sz,
			cx * sy * cz + sx * cy * sz,
			cx * cy * sz - sx * sy * cz,
			cx * cy * cz + sx * sy * sz
		);

		return q.GetNormalized();
	}

	//クオータニオンからオイラー角に
	VECTOR3 ToEuler() const
	{
		VECTOR3 euler;

		//ロール(X)
		float sinr_cosp = 2 * (w * x + y * z);
		float cosr_cosp = 1 - 2 * (x * x + y * y);
		euler.x = atan2f(sinr_cosp, cosr_cosp);

		// ピッチ(Y)
		float sinp = 2 * (w * y - z * x);
		if (fabs(sinp) >= 1)
		{//クランプ
			euler.y = (sinp > 0 ? 1.0f : -1.0f) * (3.14159f / 2.0f);
		}
		else
		{
			euler.y = asinf(sinp);
		}

		// ヨー(Z)
		float siny_cosp = 2 * (w * z + x * y);
		float cosy_cosp = 1 - 2 * (y * y + z * z);
		euler.z = atan2f(siny_cosp, cosy_cosp);

		euler = euler * 180.0f / 3.141592f; //ラジアンから度へ変換

		return euler;
	}

	//forward + up ベクトルから回転生成
	static QUATERNION LookRotation(const VECTOR3& forward, const VECTOR3& up)
	{
		VECTOR3 f = forward.GetNormalized();
		VECTOR3 r = VECTOR3::Cross(up, f).GetNormalized();
		VECTOR3 u = VECTOR3::Cross(f, r);

		// 回転行列からクオータニオンを作成
		float m00 = r.x, m01 = u.x, m02 = f.x;
		float m10 = r.y, m11 = u.y, m12 = f.y;
		float m20 = r.z, m21 = u.z, m22 = f.z;

		float trace = m00 + m11 + m22;
		float x, y, z, w;

		if (trace > 0.0f) {
			float s = sqrtf(trace + 1.0f) * 2.0f;
			w = 0.25f * s;
			x = (m21 - m12) / s;
			y = (m02 - m20) / s;
			z = (m10 - m01) / s;
		}
		else if ((m00 > m11) && (m00 > m22)) {
			float s = sqrtf(1.0f + m00 - m11 - m22) * 2.0f;
			w = (m21 - m12) / s;
			x = 0.25f * s;
			y = (m01 + m10) / s;
			z = (m02 + m20) / s;
		}
		else if (m11 > m22) {
			float s = sqrtf(1.0f + m11 - m00 - m22) * 2.0f;
			w = (m02 - m20) / s;
			x = (m01 + m10) / s;
			y = 0.25f * s;
			z = (m12 + m21) / s;
		}
		else {
			float s = sqrtf(1.0f + m22 - m00 - m11) * 2.0f;
			w = (m10 - m01) / s;
			x = (m02 + m20) / s;
			y = (m12 + m21) / s;
			z = 0.25f * s;
		}

		return QUATERNION(x, y, z, w).GetNormalized();
	}

	static QUATERNION FromToRotation(const VECTOR3& from, const VECTOR3& to)
	{
		VECTOR3 f = from.GetNormalized();
		VECTOR3 t = to.GetNormalized();

		float dot = VECTOR3::Dot(f, t);
		if (dot < -0.99999f)
		{
			VECTOR3 ortho = (fabs(f.x) < 0.1f ? VECTOR3(1.0f, 0.0f, 0.0f) : VECTOR3(0.0f, 1.0f, 0.0f));
			VECTOR3 axis = VECTOR3::Cross(f, ortho).GetNormalized();
			return FromAxisAngle(axis, 3.141592f); //180度回転
		}

		VECTOR3 axis = VECTOR3::Cross(f, t);
		float s = sqrtf((1 + dot) * 2);
		float inv_s = 1.0f / s;
		return QUATERNION(axis.x * inv_s, axis.y * inv_s, axis.z * inv_s, s * 0.5f).GetNormalized();

	}

	MATRIX3x3 ToMatrix3x3() const
	{
		float xx = x * x;
		float yy = y * y;
		float zz = z * z;
		float xy = x * y;
		float xz = x * z;
		float yz = y * z;
		float wx = w * x;
		float wy = w * y;
		float wz = w * z;

		MATRIX3x3 m;
		m.m[0][0] = 1.0f - 2.0f * (yy + zz);
		m.m[0][1] = 2.0f * (xy - wz);
		m.m[0][2] = 2.0f * (xz + wy);

		m.m[1][0] = 2.0f * (xy + wz);
		m.m[1][1] = 1.0f - 2.0f * (xx + zz);
		m.m[1][2] = 2.0f * (yz - wx);

		m.m[2][0] = 2.0f * (xz - wy);
		m.m[2][1] = 2.0f * (yz + wx);
		m.m[2][2] = 1.0f - 2.0f * (xx + yy);

		return m;
	}

};