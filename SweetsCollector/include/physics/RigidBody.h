//-----------------------------------------------
// RigidBody.h
// 制作日：2025/07/07
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "math/Vector3.h"
#include "math/Matrix3x3.h"
#include "component/Component.h"

enum class RIGID_BODY_TYPE
{
	NONE = 0,
	DYNAMIC,	// 動的
	KINEMATIC,	// キネマティック
	STATIC,		// 静的
};

enum class RIGID_BODY_CONSTRAINTS : uint32_t
{
	NONE = 0,
	FREEZE_POS_X = 1 << 0,
	FREEZE_POS_Y = 1 << 1,
	FREEZE_POS_Z = 1 << 2,

	FREEZE_ROT_X = 1 << 3,
	FREEZE_ROT_Y = 1 << 4,
	FREEZE_ROT_Z = 1 << 5,
};

enum class FORCE_MODE
{
	FORCE,				// Nを加算(時間で積分)
	ACCELERATION,		// m/s²を加算(時間で積分)
	IMPULSE,			// Nsを加算(瞬間的に)
	VELOCITY_CHANGE,	// m/sを加算(瞬間的に)
};

class RIGID_BODY : public COMPONENT
{
private:
	friend class PHYSICS_3D;
	friend class CONTACT_MANAGER;

	uint32_t m_id = 0; // 剛体ID

	RIGID_BODY_TYPE m_type = RIGID_BODY_TYPE::DYNAMIC; // 剛体のタイプ

	float m_mass = 1.0f;               // 質量
	float m_inv_mass = 1.0f;           // 逆質量

	MATRIX3x3 m_inertia_local = MATRIX3x3::Identity();     // 慣性テンソル
	MATRIX3x3 m_inv_inertia_local = MATRIX3x3::Identity(); // 逆慣性テンソル
	MATRIX3x3 m_inv_inertia_world = MATRIX3x3::Identity(); // ワールド空間の逆慣性テンソル

	VECTOR3 m_linear_velocity = VECTOR3(0.0f, 0.0f, 0.0f);   // 線形速度
	VECTOR3 m_angular_velocity = VECTOR3(0.0f, 0.0f, 0.0f);  // 角速度

	VECTOR3 m_force = VECTOR3(0.0f, 0.0f, 0.0f);   // 力
	VECTOR3 m_torque = VECTOR3(0.0f, 0.0f, 0.0f);  // トルク

	VECTOR3 m_linear_impulse = VECTOR3(0.0f, 0.0f, 0.0f);   // 線形インパルス
	VECTOR3 m_angular_impulse = VECTOR3(0.0f, 0.0f, 0.0f);  // 角インパルス
	VECTOR3 m_linear_velocity_change = VECTOR3(0.0f, 0.0f, 0.0f);   // 線形速度変化
	VECTOR3 m_angular_velocity_change = VECTOR3(0.0f, 0.0f, 0.0f);  // 角速度変化

	bool m_use_gravity = true; // 重力を使用するかどうか

	uint32_t m_constraints = 0; // 制約

	float m_linear_damping = 0.99f;   // 線形減衰
	float m_angular_damping = 0.99f;  // 角減衰

	float m_restitution = 0.5f; // 反発係数

	float m_dynamic_friction = 0.4f;	// 動摩擦係数
	float m_static_friction = 0.6f;	// 静止摩擦係数
	
	float m_sleep_threshold = 0.2f; // 睡眠判定のしきい値
	bool m_is_awake = true; // 起きているかどうか

	float m_sleep_timer = 0.0f; // スリープタイマー
	bool m_can_sleep = true; // スリープ可能かどうか

	class COLLIDER_3D* m_collider = nullptr; // コライダー

public:
	// 初期化・終了処理
	void Init() override;
	void Uninit() override;
	void OnAttach() override;

	// 剛体IDの設定
	void SetID(uint32_t id) { m_id = id; }
	uint32_t GetID() const { return m_id; }

	// 剛体タイプの設定
	void SetType(RIGID_BODY_TYPE type);
	RIGID_BODY_TYPE GetType() const { return m_type; }

	// 質量の設定
	void SetMass(float mass);
	float GetMass() const { return m_mass; }
	float GetInvMass() const { return m_inv_mass; }

	// 重力の使用設定
	void SetUseGravity(bool use) { m_use_gravity = use; }
	bool IsUseGravity() const { return m_use_gravity; }

	// 制約の設定
	void AddConstraints(RIGID_BODY_CONSTRAINTS constraints) { m_constraints |= (uint32_t)constraints; }
	void RemoveConstraints(RIGID_BODY_CONSTRAINTS constraints) { m_constraints &= ~(uint32_t)constraints; }
	bool HasConstraints(RIGID_BODY_CONSTRAINTS constraints) const { return (m_constraints & (uint32_t)constraints) != 0; }

	// 制約の適用
	void ApplyAngularVelocityConstraint();
	void ApplyLinearVelocityConstraint();

	// 力の適用
	void AddForce(const VECTOR3& force, FORCE_MODE mode);
	void AddTorque(const VECTOR3& torque, FORCE_MODE mode);
	void ApplyImpulse(const VECTOR3& impulse, const VECTOR3& contact_point);

	// 力の適用(簡易版)
	void AddForce(const VECTOR3& force) { AddForce(force, FORCE_MODE::FORCE); }
	void AddAcceleration(const VECTOR3& acceleration) { AddForce(acceleration, FORCE_MODE::ACCELERATION); }
	void AddImpulse(const VECTOR3& impulse) { AddForce(impulse, FORCE_MODE::IMPULSE); }
	void AddVelocityChange(const VECTOR3& velocity_change) { AddForce(velocity_change, FORCE_MODE::VELOCITY_CHANGE); }

	// 指定位置への力の適用
	void AddForceAtPoint(const VECTOR3& force, const VECTOR3& point, FORCE_MODE mode);

	void IntegrateForces(const VECTOR3& gravity,  float dt);
	void IntegrateVelocities(float dt);

	// 速度の設定
	void SetLinearVelocity(const VECTOR3& velocity) { m_linear_velocity = velocity; ApplyLinearVelocityConstraint(); }
	VECTOR3 GetLinearVelocity() const { return m_linear_velocity; }

	// 角速度の設定
	void SetAngularVelocity(const VECTOR3& velocity) { m_angular_velocity = velocity; ApplyAngularVelocityConstraint(); }
	VECTOR3 GetAngularVelocity() const { return m_angular_velocity; }

	// 反発係数の設定
	void SetRestitution(float restitution) { m_restitution = restitution; }
	float GetRestitution() const { return m_restitution; }

	// 摩擦係数の設定
	void SetStaticFriction(float friction) { m_static_friction = friction; }
	float GetStaticFriction() const { return m_static_friction; }
	void SetDynamicFriction(float friction) { m_dynamic_friction = friction; }
	float GetDynamicFriction() const { return m_dynamic_friction; }
	float GetFriction() const { return (m_static_friction + m_dynamic_friction) * 0.5f; }

	// スリープ関連
	void Sleep() { m_is_awake = false; m_sleep_timer = 0.0f; m_linear_velocity = VECTOR3(0.0f, 0.0f, 0.0f); m_angular_velocity = VECTOR3(0.0f, 0.0f, 0.0f); }
	void WakeUp() { m_is_awake = true;  m_sleep_timer = 0.0f; }
	bool IsSleeping() const { return !m_is_awake; }
	void SetCanSleep(bool can) { m_can_sleep = can; if (!can) { WakeUp(); } }
	bool CanSleep() const { return m_can_sleep; }
	float GetSleepThreshold() const { return m_sleep_threshold; }

	// 慣性テンソル関連
	void SetInertiaTensor(const MATRIX3x3& inertia);
	const MATRIX3x3& GetInvInertiaWorld() const { return m_inv_inertia_world; }
	void ComputeInertiaTensorFromCollider();
	void UpdateInertiaTensor();

	// コライダーの設定
	void SetCollider(class COLLIDER_3D* collider);

	// 力のクリア
	void ClearContinuousForces();
	void ClearInstantForces();
	void ClearForces();

	COLLIDER_3D* GetCollider() const { return m_collider; }
};