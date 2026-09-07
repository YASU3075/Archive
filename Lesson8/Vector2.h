//-----------------------------------------------
// Vector2.h
// 制作日：2025/08/05
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <cmath>

class Vector2
{
public:
	float x; // X座標
	float y; // Y座標

	Vector2() : x(0), y(0) {} // コンストラクタ

	Vector2(float x, float y) : x(x), y(y) {} // コンストラクタ

	Vector2 operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y); // ベクトルの加算
	}
	Vector2 operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y); // ベクトルの減算
	}
	Vector2 operator*(float scalar) const
	{
		return Vector2(x * scalar, y * scalar); // スカラー倍
	}
	Vector2 operator/(float scalar) const
	{
		if (scalar == 0) return Vector2(0, 0); // ゼロ除算を避ける
		return Vector2(x / scalar, y / scalar); // スカラー除算
	}
	Vector2& operator+=(const Vector2& other)
	{
		x += other.x; // ベクトルの加算
		y += other.y;
		return *this; // 自分自身を返す
	}

	bool operator==(const Vector2& other) const
	{
		return (x == other.x && y == other.y); // ベクトルの等価比較
	}

	// ベクトルの長さを計算するメソッド
	float Length() const 
	{
		return std::sqrt(x * x + y * y);
	}

	// ベクトルの正規化を行うメソッド
	Vector2 Normalize() const 
	{
		float len = Length();
		if (len == 0) return Vector2(0, 0); // ゼロベクトルの場合はそのまま返す
		return Vector2(x / len, y / len);
	}
};