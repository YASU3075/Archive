//-----------------------------------------------
// Vector3.h
// 制作日：2025/05/13
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include <algorithm>
#include <cmath>
#include "math.h"

class VECTOR3
{
public:
	float x, y, z;

	//コンストラクタ
	VECTOR3() : x(0.0f), y(0.0f), z(0.0f) {}
	VECTOR3(const VECTOR3 &a) : x(a.x), y(a.y), z(a.z) {}
	VECTOR3(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}

	//代入
	VECTOR3& operator =(const VECTOR3& a)
	{
		x = a.x;
		y = a.y;
		z = a.z;
		return *this;
	}

	//等しさチェック
	bool operator == (const VECTOR3& a) const
	{
		return x == a.x && y == a.y && z == a.z;
	}
	bool operator != (const VECTOR3& a) const
	{
		return x != a.x && y != a.y && z != a.z;
	}

	//ベクトル操作

	//ベクトルを反転
	VECTOR3 operator -() { return VECTOR3(-x, -y, -z); }

	//ベクトルの加算、減算
	VECTOR3 operator +(const VECTOR3& a) const
	{
		return VECTOR3(x + a.x, y + a.y, z + a.z);
	}

	VECTOR3 operator -(const VECTOR3& a) const
	{
		return VECTOR3(x - a.x, y - a.y, z - a.z);
	}

	//スカラーによる乗算、除算
	VECTOR3 operator *(float a) const
	{
		return VECTOR3(x * a, y * a, z * a);
	}

	VECTOR3 operator /(float a) const
	{
		return VECTOR3(x / a, y / a, z / a);
	}

	//組み合わせ代入演算
	VECTOR3& operator +=(const VECTOR3& a)
	{
		x += a.x; y += a.y; z += a.z;
		return *this;
	}

	VECTOR3& operator -=(const VECTOR3& a)
	{
		x -= a.x; y -= a.y; z -= a.z;
		return *this;
	}

	VECTOR3& operator *=(const VECTOR3& a)
	{
		x *= a.x; y *= a.y; z *= a.z;
		return *this;
	}

	VECTOR3& operator /=(const VECTOR3& a)
	{
		x /= a.x; y /= a.y; z /= a.z;
		return *this;
	}

	//正規化
	void Normalize()
	{
		float mag_sq = x * x + y * y + z * z;
		if (mag_sq > 1e-12f)
		{
			float one_over_mag = 1.0f / std::sqrt(mag_sq);
			x *= one_over_mag;
			y *= one_over_mag;
			z *= one_over_mag;
		}
	}

	VECTOR3 GetNormalized() const
	{
		float mag_sq = x * x + y * y + z * z;
		if (mag_sq > 1e-12f)
		{
			VECTOR3 ret = { x, y, z };

			float one_over_mag = 1.0f / std::sqrt(mag_sq);
			ret.x *= one_over_mag;
			ret.y *= one_over_mag;
			ret.z *= one_over_mag;

			return ret;
		}

		return VECTOR3(0.0f, 0.0f, 0.0f);
	}

	//------------------------------------
	// 非メンバ関数
	//------------------------------------
	//外積
	static inline VECTOR3 Cross(const VECTOR3& a, const VECTOR3& b)
	{
		return VECTOR3(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		);
	}

	static inline float CrossZ(const VECTOR3& a, const VECTOR3& b)
	{
		return a.x * b.y - a.y * b.x;
	}

	static inline float Dot(const VECTOR3& a, const VECTOR3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	float LengthSquared() const
	{
		return x * x + y * y + z * z;
	}

	float Length() const
	{
		return sqrtf(x * x + y * y + z * z);
	}

	static inline float Distance(const VECTOR3& a, const VECTOR3& b)
	{
		float dx = a.x - b.x;
		float dy = a.y - b.y;
		float dz = a.z - b.z;
		return sqrt(dx * dx + dy * dy + dz * dz);
	}

	VECTOR3 Clamp(const VECTOR3& min, const VECTOR3& max) const
	{
		VECTOR3 ret = *this;
		if (x < min.x)
		{
			ret.x = min.x;
		}
		else if (x > max.x)
		{
			ret.x = max.x;
		}
		if (y < min.y)
		{
			ret.y = min.y;
		}
		else if (y > max.y)
		{
			ret.y = max.y;
		}
		if (z < min.z)
		{
			ret.z = min.z;
		}
		else if (z > max.z)
		{
			ret.z = max.z;
		}
		return ret;
	}
	static inline VECTOR3 Lerp(const VECTOR3& start, const VECTOR3& end, float t)
	{
		return start + (end - start) * t;
	}
};