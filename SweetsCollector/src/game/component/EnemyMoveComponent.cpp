//-----------------------------------------------
// EnemyMoveComponent.cpp
// 制作日：2025/12/28
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "renderer/Renderer.h"
#include "game/component/EnemyMoveComponent.h"
#include "object/Player.h"
#include "physics/RigidBody.h"
#include "component/renderer/ModelRendererComponent.h"
#include "EngineTime.h"

//------------------------------------------------
// 初期化処理
//------------------------------------------------
void ENEMY_MOVE_COMPONENT::Init()
{
	m_state_time = 0.0f;
	m_out_of_range_time = 0.0f;
	m_return_stuck_time = 0.0f;


	m_p_rigid_body = GetOwner()->GetComponent<RIGID_BODY>();

	m_home = GetOwner()->GetTransform()->GetPos();

	std::random_device rd;
	m_rng = std::mt19937(rd());

	PickNewPatrolTarget();

	m_state = ENEMY_STATE::ENEMY_STATE_IDLE;

	m_p_model_renderer = GetOwner()->GetComponent<MODEL_RENDERER_COMPONENT>();
}

//------------------------------------------------
// 更新処理
//------------------------------------------------
void ENEMY_MOVE_COMPONENT::Update()
{
	m_state_time += TIME::DeltaTime();

	switch (m_state)
	{
	case ENEMY_STATE::ENEMY_STATE_IDLE:
		m_home = GetOwner()->GetTransform()->GetPos();
		m_state = ENEMY_STATE::ENEMY_STATE_PATROL;
		m_state_time = 0.0f;
		break;
	case ENEMY_STATE::ENEMY_STATE_PATROL:
		PatrolUpdate();
		break;
	case ENEMY_STATE::ENEMY_STATE_CHASE:
		ChaseUpdate();
		break;
	case ENEMY_STATE::ENEMY_STATE_HIT_STUN:
		HitStunUpdate();
		break;
	case ENEMY_STATE::ENEMY_STATE_MISS:
		MissUpdate();
		break;
	case ENEMY_STATE::ENEMY_STATE_RETURN:
		ReturnUpdate();
		break;
	case ENEMY_STATE::ENEMY_STATE_MAX:
		break;
	default:
		break;
	}
}

//------------------------------------------------
// 巡回状態の更新処理
//------------------------------------------------
void ENEMY_MOVE_COMPONENT::PatrolUpdate()
{
	const VECTOR3 pos = GetOwner()->GetTransform()->GetPos();

	// 目標地点への方向と距離を計算
	VECTOR3 to_target = m_target_position - pos;
	float dist = to_target.Length();

	if (dist <= m_target_reach_dist || m_state_time >= m_retarget_stuck_time)
	{
		PickNewPatrolTarget();
		m_state_time = 0.0f;
		to_target = m_target_position - pos;
		dist = to_target.Length();
	}

	// 目的の速度を適用
	VECTOR3 dir = (dist > 1e-6f) ? to_target / dist : VECTOR3(0.0f, 0.0f, 0.0f);

	RotateToDirectionYaw(dir);

	ApplyDesiredVelocity(dir * m_patrol_speed, m_patrol_accel);

	PLAYER* p_player = Manager::GetScene()->GetGameObject<PLAYER>();
	if (p_player)
	{
		const VECTOR3 player_pos = p_player->GetTransform()->GetPos();
		const float player_dist = VECTOR3::Distance(pos, player_pos);
		if (player_dist <= m_chase_range)
		{
			// プレイヤーが追跡範囲内に入ったら追跡状態へ遷移
			m_state = ENEMY_STATE::ENEMY_STATE_CHASE;
			m_state_time = 0.0f;
		}
	}
}

//------------------------------------------------
// 追跡状態の更新処理
//------------------------------------------------
void ENEMY_MOVE_COMPONENT::ChaseUpdate()
{
	const VECTOR3 pos = GetOwner()->GetTransform()->GetPos();

	PLAYER* p_player = Manager::GetScene()->GetGameObject<PLAYER>();
	if (!p_player)
	{// プレイヤーが存在しない場合は見失い状態へ遷移
		m_state = ENEMY_STATE::ENEMY_STATE_MISS;
		m_state_time = 0.0f;
		m_out_of_range_time = 0.0f;
		return;
	}

	VECTOR3 player_pos = p_player->GetTransform()->GetPos();
	float dist = VECTOR3::Distance(pos, player_pos);

	if (dist > m_chase_range)
	{
		// プレイヤーが追跡範囲外に出た場合、猶予時間をカウント
		m_out_of_range_time += TIME::DeltaTime();

		if (m_out_of_range_time >= m_lost_grace_time)
		{// 猶予時間を超えたら見失い状態へ遷移
			m_state = ENEMY_STATE::ENEMY_STATE_MISS;
			m_state_time = 0.0f;
			m_out_of_range_time = 0.0f;
			return;
		}
	}
	else
	{// プレイヤーが追跡範囲内にいる場合、猶予時間をリセット
		m_out_of_range_time = 0.0f;
	}

	// 目的の速度を適用
	VECTOR3 to_player = player_pos - pos;
	float len = to_player.Length();
	VECTOR3 dir = (len > 1e-6f) ? to_player / len : VECTOR3(0.0f, 0.0f, 0.0f);

	RotateToDirectionYaw(dir);

	ApplyDesiredVelocity(dir * m_chase_speed, m_chase_accel);
}

//------------------------------------------------
// 見失い状態の更新処理
//------------------------------------------------
void ENEMY_MOVE_COMPONENT::MissUpdate()
{
	// ブレーキをかけて停止する
	ApplyDesiredVelocity(VECTOR3(0.0f, 0.0f, 0.0f), m_stop_accel);

	if (m_state_time >= m_lost_wait_time)
	{
		// 待機時間が経過したら帰還状態へ遷移
		m_state = ENEMY_STATE::ENEMY_STATE_RETURN;
		m_state_time = 0.0f;
		m_return_stuck_time = 0.0f;
	}
}

//------------------------------------------------
// ヒットスタン状態の更新処理
//------------------------------------------------
void ENEMY_MOVE_COMPONENT::HitStunUpdate()
{
	
}

//------------------------------------------------
// 帰還状態の更新処理
//------------------------------------------------
void ENEMY_MOVE_COMPONENT::ReturnUpdate()
{
	const float dt = TIME::DeltaTime();
	const VECTOR3 pos = GetOwner()->GetTransform()->GetPos();

	VECTOR3 to_home = m_home - pos;
	to_home.y = 0.0f; // 水平方向のみ考慮
	float dist = to_home.Length();

	if (dist <= m_home_reach_dist)
	{// 定位置に到達したら巡回状態へ遷移
		m_state = ENEMY_STATE::ENEMY_STATE_PATROL;
		m_state_time = 0.0f;
		PickNewPatrolTarget();
		return;
	}

	VECTOR3 dir = (dist > 1e-6f) ? to_home / dist : VECTOR3(0.0f, 0.0f, 0.0f);

	RotateToDirectionYaw(dir);

	ApplyDesiredVelocity(dir * m_return_speed, m_return_accel);

	VECTOR3 v = m_p_rigid_body->GetLinearVelocity();
	if (v.LengthSquared() < 0.05f * 0.05f && dist > (m_home_reach_dist * 2.0f))
	{
		m_return_stuck_time += dt;
		if (m_return_stuck_time >= m_return_stuck_limit)
		{// その場を新ホームにしてパトロールへ
			m_home = pos;
			m_state = ENEMY_STATE::ENEMY_STATE_PATROL;
			m_state_time = 0.0f;
			PickNewPatrolTarget();
			return;
		}
	}
	else
	{
		m_return_stuck_time = 0.0f;
	}
}

//------------------------------------------------
// 新しい巡回目標地点を選択する
//------------------------------------------------
void ENEMY_MOVE_COMPONENT::PickNewPatrolTarget()
{
	std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

	VECTOR3 dir(dist(m_rng), 0.0f, dist(m_rng));
	if (dir.LengthSquared() < 1e-6f)
	{
		dir = VECTOR3(0.0f, 0.0f, 1.0f);
	}
	dir.Normalize();

	std::uniform_real_distribution<float> dist_radius(0.0f, m_patrol_radius);
	float radius = dist_radius(m_rng);

	m_target_position = m_home + dir * radius;
}

//------------------------------------------------
// 目的の速度を適用する
//------------------------------------------------
void ENEMY_MOVE_COMPONENT::ApplyDesiredVelocity(const VECTOR3& desired_vel, float max_accel)
{
	if (!m_p_rigid_body)
	{// nullチェック
		return;
	}

	const float dt = TIME::DeltaTime();
	VECTOR3 v = m_p_rigid_body->GetLinearVelocity();

	VECTOR3 dv = desired_vel - v;

	const float max_dv = max_accel * dt;
	const float len = dv.Length();

	if (len > max_dv && len > 1e-6f)
	{
		dv = dv * (max_dv / len);
	}

	// 速度変化を適用
	m_p_rigid_body->AddForce(dv, FORCE_MODE::VELOCITY_CHANGE);
}

//------------------------------------------------
// 進行方向へY軸回転（水平のみ）
//------------------------------------------------
void ENEMY_MOVE_COMPONENT::RotateToDirectionYaw(const VECTOR3& direction)
{
	VECTOR3 fwd = direction;
	fwd.y = 0.0f;

	if (fwd.LengthSquared() < 1e-8f)
		return;

	fwd.Normalize();

	QUATERNION target = QUATERNION::LookRotation(fwd, VECTOR3(0, 1, 0));

	
	QUATERNION offset = QUATERNION::FromAxisAngle(VECTOR3(0, 1, 0), 90.0f * 3.141592f / 180.0f);
	target = target * offset; // プレイヤーで直った掛け順と同じ
	

	QUATERNION cur = GetOwner()->GetTransform()->GetRot();

	float turn_speed = 8.0f;
	float t = 1.0f - std::exp(-turn_speed * TIME::DeltaTime());

	QUATERNION newRot = QUATERNION::Slerp(cur, target, t);
	GetOwner()->GetTransform()->SetRot(newRot);
}

