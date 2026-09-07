//-----------------------------------------------
// primitive3D.h
// 制作日：2024/11/15
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include "math.h"

#define GAP_NUM			(0.00001f)

class FLOAT3
{
public:
	float x, y, z;

	FLOAT3() {}		//デフォルトコンストラクタ
	FLOAT3(float x, float y, float z) : x(x), y(y), z(z) {}	//引数付きコンストラクタ
	FLOAT3(XMFLOAT3 f) : x(f.x), y(f.y), z(f.z) {}	//引数付きコンストラクタ
	
	~FLOAT3() {}	//デストラクタ

	FLOAT3 operator =(const XMFLOAT3& r)
	{
		return FLOAT3(r.x, r.y, r.z);
	}

	FLOAT3 operator +(const FLOAT3& r) const
	{//FLOAT3型同士の足し算
		return FLOAT3(x + r.x, y + r.y, z + r.z);
	}

	FLOAT3 operator -(const FLOAT3& r) const
	{//FLOAT3型同士の引き算
		return FLOAT3(x - r.x, y - r.y, z - r.z);
	}

	FLOAT3 operator -() const
	{//マイナスにする
		return FLOAT3(x * -1.0f, y * -1.0f, z * -1.0f);
	}

	FLOAT3 operator *(const FLOAT3& r) const
	{//FLOAT3型同士の掛け算
		return FLOAT3(x * r.x, y * r.y, z * r.z);
	}

	FLOAT3 operator /(const FLOAT3& r) const
	{//FLOAT3型同士の割り算
		return FLOAT3(x / r.x, y / r.y, z / r.z);
	}

	FLOAT3 operator *(float r) const
	{//FLOAT3に定数を掛ける
		return FLOAT3(x * r, y * r, z * r);
	}

	FLOAT3 operator /(float r) const
	{//FLOAT3に定数を割る
		return FLOAT3(x / r, y / r, z / r);
	}

	float Dot(const FLOAT3& r) const
	{//内積
		return (x * r.x + y * r.y + z * r.z);
	}

	FLOAT3 Cross(const FLOAT3& r) const
	{//外積（3Dだと法線ベクトルで返す）
		return FLOAT3(y * r.z - z * r.y, z * r.x - x * r.z, x * r.y - y * r.x);
	}

	float Length() const
	{//長さ
		return sqrtf(LengthSq());
	}

	float LengthSq() const
	{//長さの二乗
		return (x * x + y * y + z * z);
	}

	void Norm()
	{//正規化
		const float len = Length();
		if (len > 0.0f)
		{
			x /= len;
			y /= len;
			z /= len;
		}
	}

	FLOAT3 getNorm() const
	{//正規化した値を取得（数値はそのまま）
		const float len = Length();
		if (len > 0.0f)
		{
			return FLOAT3(x / len, y / len, z / len);
		}
		return FLOAT3(0.0f, 0.0f, 0.0f);
	}
};

//-----------------------------------------------
//点
//-----------------------------------------------
typedef FLOAT3 Point;

//-----------------------------------------------
//3次元ベクトル
//-----------------------------------------------
class VEC3 : public FLOAT3
{
public:
	VEC3() {}		//デフォルトコンストラクタ
	VEC3(float x, float y, float z) : FLOAT3(x, y, z) {}		//引数付きコンストラクタ
	VEC3(const FLOAT3& r) : FLOAT3(r) {}
	~VEC3 () {}		//デストラクタ

	VEC3& operator = (const FLOAT3& r)
	{
		x = r.x;
		y = r.y;
		z = r.z;
		return *this;
	}

	bool isVertical(const FLOAT3& r) const
	{//垂直かどうか
		float d = Dot(r);
		return (-GAP_NUM < d && d < GAP_NUM);
	}

	bool isParallel(const VEC3& r) const
	{//平行かどうか
		float d = Cross(r).LengthSq();
		return (-GAP_NUM < d && d < GAP_NUM);
	}

	bool isSharpAngle(const VEC3& r) const
	{//鋭角関係かどうか
		return (Dot(r) >= 0.0f);
	}

};


//-----------------------------------------------
// 直線
//-----------------------------------------------
class LINE
{
public:
	Point p;		//始点
	VEC3 v;			//方向ベクトル

	LINE() : p(0.0f, 0.0f, 0.0f), v(1.0f, 0.0f, 0.0f) {}			//デフォルトコンストラクタ
	LINE(const Point& p, const VEC3& v) : p(p), v(v) {}
	~LINE() {}		//デストラクタ

	//ベクトルをt倍した直線状の点を取得する
	FLOAT3 GetPoint(float t) const
	{
		return p + (v * t);
	}
};

//-----------------------------------------------
// 線分
//-----------------------------------------------
class SEGMENT : public LINE
{
public:
	SEGMENT();										//デフォルトコンストラクタ
	SEGMENT(const Point& p, const VEC3& v) : LINE(p, v) {}
	SEGMENT(const Point& p1, const Point& p2) : LINE(p1, p2 - p1) {}
	~SEGMENT() {}										//デストラクタ

	//終点を取得
	FLOAT3 GetEndPoint() const
	{
		return p + v;
	}
};

//-----------------------------------------------
// 球
//-----------------------------------------------
class SPHERE
{
public:
	Point p;
	float r;	//半径

	SPHERE() : p(0.0f, 0.0f, 0.0f), r(0.5f) {}			//デフォルトコンストラクタ
	SPHERE(const Point& p, float r)
	{
		this->p = p;
		this->r = r;
	}
	~SPHERE() {}		//デストラクタ
};

//-----------------------------------------------
// カプセル
//-----------------------------------------------
class CAPSULE
{
	SEGMENT s;
	float r;		//半径
	CAPSULE() : r(0.5f) {}		//デフォルトコンストラクタ
	CAPSULE(const SEGMENT& s, float r)
	{
		this->s = s;
		this->r = r;
	}
	CAPSULE(const Point& p1, const Point& p2, float r) : s(SEGMENT(p1, p2))
	{
		this->r = r;
	}
	~CAPSULE() {}	//デストラクタ
};