//-----------------------------------------------
// DistanceJoint3D.h
// 制作日：2026/02/26
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "physics/joint/Joint3D.h"

enum class DISTANCE_LIMIT_MODE
{
	FIXED,		// 固定距離
	ROPE_MAX_ONLY,	// ロープのように、距離が目標値を超えないようにする
	LIMITS_MIN_MAX,	// 最小距離と最大距離の両方を設定する
};

enum class DISTANCE_LIMIT_SIDE
{
	NONE,
	MIN_SIDE, // dist < min の違反を直している
	MAX_SIDE  // dist > max の違反を直している
};

class DISTANCE_JOINT_3D : public JOINT_3D
{
private:
	RIGID_BODY* m_rb_a = nullptr;	// 剛体A
	RIGID_BODY* m_rb_b = nullptr;	// 剛体B

	// ローカルアンカー
	VECTOR3 m_local_anchor_a = VECTOR3(0.0f, 0.0f, 0.0f);
	VECTOR3 m_local_anchor_b = VECTOR3(0.0f, 0.0f, 0.0f);

	float m_rest_length = 0.0f;	// 目標距離

	DISTANCE_LIMIT_MODE m_mode = DISTANCE_LIMIT_MODE::FIXED;	// 距離制限のモード

	float m_min_length = 0.0f;	// 最小距離
	float m_max_length = 0.0f;	// 最大距離

	bool m_active = true;

	DISTANCE_LIMIT_SIDE m_side = DISTANCE_LIMIT_SIDE::NONE;

	// ソルバ用キャッシュ
	VECTOR3 m_normal = VECTOR3(1.0f, 0.0f, 0.0f);	// 正規化された距離ベクトル
	VECTOR3 m_ra = VECTOR3(0.0f, 0.0f, 0.0f);	// 剛体Aのアンカー位置
	VECTOR3 m_rb = VECTOR3(0.0f, 0.0f, 0.0f);	// 剛体Bのアンカー位置
	float m_effective_mass = 0.0f;	// 有効質量
	float m_bias = 0.0f;	// バイアス

public:
	DISTANCE_JOINT_3D(RIGID_BODY* a, RIGID_BODY* b);
	
	void SetAnchorsWorld(const VECTOR3& anchor_a, const VECTOR3& anchor_b);
	void SetRestLength(float length) { m_rest_length = length; }

	void PreStep(float dt) override;
	void SolveVelocity(float dt) override;
	void SolvePosition(float dt) override;

	RIGID_BODY* GetBodyA() const override { return m_rb_a; }
	RIGID_BODY* GetBodyB() const override { return m_rb_b; }

	void SetFixedLength(float length) { m_mode = DISTANCE_LIMIT_MODE::FIXED; m_rest_length = length; m_min_length = length; m_max_length = length; }
	void SetRopeLength(float max_length) { m_mode = DISTANCE_LIMIT_MODE::ROPE_MAX_ONLY; m_rest_length = max_length; m_min_length = 0.0f; m_max_length = max_length; }
	void SetLimits(float min_length, float max_length) 
	{
		m_mode = DISTANCE_LIMIT_MODE::LIMITS_MIN_MAX;
		m_min_length = min_length;
		m_max_length = max_length;
		m_rest_length = std::clamp(m_rest_length, m_min_length, m_max_length);
	}
};