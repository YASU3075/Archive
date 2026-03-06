//-----------------------------------------------
// EnemyMoveComponent.h
// 制作日：2025/12/28
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <random>
#include "math/Vector3.h"
#include "component/Component.h"

enum class ENEMY_STATE
{
	ENEMY_STATE_IDLE = 0,	// 待機
	ENEMY_STATE_PATROL,		// 巡回
	ENEMY_STATE_CHASE,		// 追跡
	ENEMY_STATE_HIT_STUN,	// ヒットスタン
	ENEMY_STATE_MISS,		// 見失い
	ENEMY_STATE_RETURN,		// 帰還
	ENEMY_STATE_MAX
};

class ENEMY_MOVE_COMPONENT : public COMPONENT
{
private:
	ENEMY_STATE m_state = ENEMY_STATE::ENEMY_STATE_IDLE; // 敵の状態

	class RIGID_BODY* m_p_rigid_body = nullptr; // リジッドボディ
	class MODEL_RENDERER_COMPONENT* m_p_model_renderer = nullptr; // モデルレンダラー

	VECTOR3 m_home = VECTOR3(0.0f, 0.0f, 0.0f); // 定位置
	VECTOR3 m_target_position = VECTOR3(0.0f, 0.0f, 0.0f); // 目標位置

	// タイマー類
	float m_state_time = 0.0f;			// 状態遷移用タイマー
	float m_out_of_range_time = 0.0f;	// 範囲外時間
	float m_return_stuck_time = 0.0f;	// 帰還時のスタック時間

	// パラメーター
	float m_patrol_radius = 6.0f;		// 巡回範囲
	float m_patrol_speed = 2.0f;		// 巡回速度
	float m_chase_speed = 3.5f;			// 追跡速度
	float m_return_speed = 2.5f;		// 帰還速度

	float m_patrol_accel = 8.0f;		// 巡回加速度
	float m_chase_accel = 10.0f;		// 追跡加速度
	float m_return_accel = 10.0f;		// 帰還加速度

	float m_stop_accel = 20.0f;			// 停止時のブレーキ強さ
	float m_chase_range = 8.0f;			// 追跡範囲

	float m_retarget_stuck_time = 2.5f;	// 目標再設定用スタック時間
	float m_lost_grace_time = 1.0f;		// 見失い判定までの猶予時間
	float m_lost_wait_time = 1.2f;		// 見失い後の待機時間
	float m_hit_stun_time = 0.8f;		// ヒットスタン時間
	float m_return_stuck_limit = 1.5f;	// 帰還時のスタック判定時間

	float m_target_reach_dist = 0.5f;	// 目標到達距離
	float m_home_reach_dist = 0.6f;		// 定位置到達距離

	std::mt19937 m_rng{}; // 乱数生成器

	void PatrolUpdate();
	void ChaseUpdate();
	void MissUpdate();
	void HitStunUpdate();
	void ReturnUpdate();

	void PickNewPatrolTarget();
	void ApplyDesiredVelocity(const VECTOR3& desired_vel, float max_accel);
	void RotateToDirectionYaw(const VECTOR3& direction);

public:
	void Init() override;
	void Update() override;

	void SetState(ENEMY_STATE state) { m_state = state; }
	ENEMY_STATE GetState() const { return m_state; }

	void SetPatrolRadius(float radius) { m_patrol_radius = radius; }
	float GetPatrolRadius() const { return m_patrol_radius; }

	void SetChaseRange(float range) { m_chase_range = range; }
	float GetChaseRange() const { return m_chase_range; }	

	void SetModelRenderer(class MODEL_RENDERER_COMPONENT* model_renderer) { m_p_model_renderer = model_renderer; }
};