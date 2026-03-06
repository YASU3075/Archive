//-----------------------------------------------
// Physics3D.h
// 制作日：2025/07/10
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <vector>
#include <memory>
#include "ContactManager.h"
#include "physics/PhysicsRigidBodyRegistry3D.h"
#include "physics/joint/JointManager3D.h"

class RIGID_BODY;
class COLLIDER_3D;
class SPHERE_COLLIDER;
class BOX_COLLIDER_3D;
struct RAY;
struct RAY_CAST;


class PHYSICS_3D
{
private:
	bool m_debug_draw = false; // デバッグ描画フラグ
	uint64_t m_frame_id = 0; // フレームカウント

	// シングルトンインスタンス
	static PHYSICS_3D* m_instance;

	// 衝突管理
	CONTACT_MANAGER m_contact_manager;

	// ジョイント管理
	JOINT_MANAGER_3D m_joint_manager;

	// 剛体管理
	PHYSICS_RIGID_BODY_REGISTRY_3D m_rigid_body_registry;

	// グローバル設定
	VECTOR3 m_gravity = { 0.0f, -9.81f, 0.0f };
	int m_velocity_iterations = 8;
	int m_position_iterations = 4;

	static constexpr float MAX_SUBSTEP = 1.0f / 60.0f; // サブステップの最大dt（60Hz）

private:
	// コンストラクタ・デストラクタ
	PHYSICS_3D() = default;
	~PHYSICS_3D() = default;

	// コピー禁止
	PHYSICS_3D(const PHYSICS_3D&) = delete;
	PHYSICS_3D& operator=(const PHYSICS_3D&) = delete;

	// 内部処理
	void SyncPendingBodies();				// 剛体の追加・削除を反映
	void BroadPhase();						// 広域判定
	void NarrowPhase();						// 狭域判定
	void StepBodiesForces(float dt);		// 力を速度に反映
	void StepBodiesVelocities(float dt);	// 速度を位置に反映
	void PositionCorrection();				// 位置補正
	void ApplyPositionalCorrection(RIGID_BODY* a, RIGID_BODY* b, const std::vector<CONTACT_POINT>& pts, float slop, float percent);

	// スリープ判定
	void UpdateSleepStates(float dt);
public:
	// シングルトンインスタンス取得
	static PHYSICS_3D& GetInstance();

	// 初期化・終了
	void Init();
	void Uninit();

	// 物理ステップシミュレーション
	void Simulate(float dt);

	// 剛体管理
	void AddRigidBody(RIGID_BODY* body);
	void RemoveRigidBody(RIGID_BODY* body);

	// グローバル設定
	void SetGravity(const VECTOR3& gravity) { m_gravity = gravity; }
	const VECTOR3& GetGravity() const { return m_gravity; }

	// ソルバー設定
	void SetSolverIterations(int velocity_iters, int position_iters)
	{
		m_velocity_iterations = velocity_iters;
		m_position_iterations = position_iters;
	}
	int GetVelocityIterations() const { return m_velocity_iterations; }
	int GetPositionIterations() const { return m_position_iterations; }

	// クエリ（レイキャストなど）
	bool Raycast(const RAY& ray, RAY_CAST& hit_info, float max_distance = FLT_MAX) const;
	std::vector<RAY_CAST> RaycastAll(const RAY& ray, float max_distance = FLT_MAX) const;
	std::vector<RIGID_BODY*> OverlapSphere(const VECTOR3& center, float radius) const;
	std::vector<RIGID_BODY*> OverlapBox(const VECTOR3& center, const VECTOR3& half_extents) const;

	// ジョイント管理
	DISTANCE_JOINT_3D* CreateDistanceJoint(RIGID_BODY* a, RIGID_BODY* b, const VECTOR3& world_anchor_a, const VECTOR3& world_anchor_b, float rest_length)
	{
		return m_joint_manager.CreateDistanceJoint(a, b, world_anchor_a, world_anchor_b, rest_length);
	}

	DISTANCE_JOINT_3D* CreateRopeJointMax(RIGID_BODY* a, RIGID_BODY* b, const VECTOR3& world_anchor_a, const VECTOR3& world_anchor_b, float max_length)
	{
		return m_joint_manager.CreateRopeJointMax(a, b, world_anchor_a, world_anchor_b, max_length);
	}

	DISTANCE_JOINT_3D* CreateLimitsJoint(RIGID_BODY* a, RIGID_BODY* b, const VECTOR3& world_anchor_a, const VECTOR3& world_anchor_b, float min_length, float max_length)
	{
		return m_joint_manager.CreateLimitsJoint(a, b, world_anchor_a, world_anchor_b, min_length, max_length);
	}

	// デバッグ描画
	void SetDebugDraw(bool enable) { m_debug_draw = enable; }
	bool IsDebugDraw() const { return m_debug_draw; }
	void DebugDraw() const;
};