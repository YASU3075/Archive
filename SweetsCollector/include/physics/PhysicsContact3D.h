//-----------------------------------------------
// PhysicsContact3D.h
// 制作日：2026/01/15
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <vector>
#include "math/Vector3.h"

class RIGID_BODY;

struct CONTACT_POINT
{
	VECTOR3 pos = VECTOR3(0.0f, 0.0f, 0.0f);
	VECTOR3 normal = VECTOR3(0.0f, 0.0f, 0.0f);
	float penetration = 0.0f;
	float restitution = 0.0f;

	float dynamic_friction = 0.0f;
	float static_friction = 0.0f;

	float accumulated_normal_impulse = 0.0f; // 法線方向の累積インパルス
	float normal_mass = 0.0f; // 法線方向の質量
	float velocity_bias = 0.0f; // 速度バイアス

	VECTOR3 accumulated_tangent_impulse_world = { 0.0f, 0.0f, 0.0f }; // 接線方向の累積インパルス（ベクトル版）
	VECTOR3 cached_tangent = { 0.0f, 0.0f, 0.0f }; // 前回の接線ベクトル

	bool just_started = false; // ウォームスタート用フラグ

	float vn0 = 0.0f; // 前回の法線方向の相対速度
};

struct CONTACT_3D
{
	RIGID_BODY* rb_a = nullptr;
	RIGID_BODY* rb_b = nullptr;

	std::vector<CONTACT_POINT> points;

	VECTOR3 tangent = { 0.0f, 0.0f, 0.0f }; // 接線ベクトル

	// 衝突フラグ
	bool is_trigger = false;
	bool touched_frame = false;
	bool touched_substep = false;
	bool was_touching = false;

	bool restitution_used = false; // 反発が使用されたかどうか
	float restitution_bias = 0.0f; // 反発バイアス
	bool do_restitution = false; // 反発を行うかどうか

	bool   has_restitution_impulse = false;
	float  restitution_impulse = 0.0f;
	VECTOR3 restitution_point = { 0,0,0 };
	VECTOR3 restitution_normal = { 0,1,0 };

	void Clear()
	{
		points.clear();
		is_trigger = false;
		touched_frame = false;
		touched_substep = false;
		was_touching = false;
	}

};