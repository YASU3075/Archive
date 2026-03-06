//-----------------------------------------------
// RigidBody.cpp
// 制作日：2025/07/07
// 制作者：安田晴人
//-----------------------------------------------
#include "physics/RigidBody.h"
#include "Main.h"
#include "scene/Manager.h"
#include "renderer/Renderer.h"
#include "entity/GameObject.h"
#include "math/Matrix3x3.h"
#include "physics/BoxCollider3D.h"
#include "physics/SphereCollider.h"
#include "physics/Physics3D.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void RIGID_BODY::Init()
{
	// 質量の設定（逆質量の計算も含む）
	SetMass(m_mass);

	// 物理エンジンに登録
	PHYSICS_3D::GetInstance().AddRigidBody(this);
}

//-----------------------------------------------
// 終了処理
//-----------------------------------------------
void RIGID_BODY::Uninit()
{
	// 物理エンジンから登録解除
	PHYSICS_3D::GetInstance().RemoveRigidBody(this);
}

//-----------------------------------------------
// アタッチ時の処理
//-----------------------------------------------
void RIGID_BODY::OnAttach()
{
	// コライダーの取得
	m_collider = GetOwner()->GetComponent<COLLIDER_3D>();
}

//-----------------------------------------------
// 剛体タイプの設定
//-----------------------------------------------
void RIGID_BODY::SetType(RIGID_BODY_TYPE type)
{
	m_type = type;

	// 質量の再設定
	SetMass(m_mass);

	if (m_type != RIGID_BODY_TYPE::DYNAMIC)
	{// 静的・キネマティックは慣性テンソルを単位テンソルに
		m_inertia_local = MATRIX3x3::Identity();
		m_inv_inertia_local = MATRIX3x3::Zero();
		m_inv_inertia_world = MATRIX3x3::Zero();

		m_linear_velocity = { 0,0,0 };
		m_angular_velocity = { 0,0,0 };
		ClearForces();
	}
	else
	{
		if (m_collider)
		{// 動的かつコライダーがある場合は慣性テンソルを再計算
			ComputeInertiaTensorFromCollider();
			UpdateInertiaTensor();
		}
	}
}

//-----------------------------------------------
// 質量の設定
//-----------------------------------------------
void RIGID_BODY::SetMass(float mass)
{
	m_mass = mass;
	if (m_mass <= 0.0f || m_type != RIGID_BODY_TYPE::DYNAMIC)
	{// 静的・キネマティックは逆質量0
		m_inv_mass = 0.0f;
	}
	else
	{
		m_inv_mass = 1.0f / m_mass;
	}

	if (m_type == RIGID_BODY_TYPE::DYNAMIC && m_collider)
	{// 動的かつコライダーがある場合は慣性テンソルを再計算
		ComputeInertiaTensorFromCollider();
		UpdateInertiaTensor();
	}
}

//-----------------------------------------------
// 角速度の制約の適用
//-----------------------------------------------
void RIGID_BODY::ApplyAngularVelocityConstraint()
{
	if (HasConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_ROT_X))
	{
		m_angular_velocity.x = 0.0f;
	}
	if (HasConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_ROT_Y))
	{
		m_angular_velocity.y = 0.0f;
	}
	if (HasConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_ROT_Z))
	{
		m_angular_velocity.z = 0.0f;
	}
}

//-----------------------------------------------
// 線形速度の制約の適用
//-----------------------------------------------
void RIGID_BODY::ApplyLinearVelocityConstraint()
{
	if (HasConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_POS_X))
	{
		m_linear_velocity.x = 0.0f;
	}
	if (HasConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_POS_Y))
	{
		m_linear_velocity.y = 0.0f;
	}
	if (HasConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_POS_Z))
	{
		m_linear_velocity.z = 0.0f;
	}
}

//-----------------------------------------------
// 力の加算
//-----------------------------------------------
void RIGID_BODY::AddForce(const VECTOR3& force, FORCE_MODE mode)
{
	if (m_type == RIGID_BODY_TYPE::DYNAMIC)
	{// 動的のみ
		switch (mode)
		{
		case FORCE_MODE::FORCE:
			m_force += force;
			break;
		case FORCE_MODE::IMPULSE:
			m_linear_impulse += force;
			break;
		case FORCE_MODE::VELOCITY_CHANGE:
			m_linear_velocity_change += force;
			break;
		case FORCE_MODE::ACCELERATION:
			m_force += force * m_mass;
			break;
		}
		WakeUp();
	}
}

//-----------------------------------------------
// トルクの加算
//-----------------------------------------------
void RIGID_BODY::AddTorque(const VECTOR3& torque, FORCE_MODE mode)
{
	if (m_type == RIGID_BODY_TYPE::DYNAMIC)
	{// 動的のみ
		switch (mode)
		{
		case FORCE_MODE::FORCE:
			m_torque += torque;
			break;
		case FORCE_MODE::IMPULSE:
			m_angular_impulse += torque;
			break;
		case FORCE_MODE::VELOCITY_CHANGE:
			m_angular_velocity_change += torque;
			break;
		case FORCE_MODE::ACCELERATION:
			m_torque += torque;
			break;
		}
	}
}

//-----------------------------------------------
// インパルスの適用
//-----------------------------------------------
void RIGID_BODY::ApplyImpulse(const VECTOR3& impulse, const VECTOR3& contact_point)
{
	if (m_type != RIGID_BODY_TYPE::DYNAMIC)
	{// 動的以外は無視
		return;
	}

	// 並進分
	m_linear_velocity += impulse * m_inv_mass;

	// 回転分
	VECTOR3 r = contact_point - GetOwner()->GetTransform()->GetPos();
	VECTOR3 angular_impulse = VECTOR3::Cross(r, impulse);
	m_angular_velocity += m_inv_inertia_world.MultiplyVector(angular_impulse);

	// スリープ解除
	WakeUp();
}

//-----------------------------------------------
// 指定位置への力の加算
//-----------------------------------------------
void RIGID_BODY::AddForceAtPoint(const VECTOR3& force, const VECTOR3& point, FORCE_MODE mode)
{
	// 力分の加算
	AddForce(force, mode);

	// トルク分の計算と加算
	VECTOR3 r = point - GetOwner()->GetTransform()->GetPos();
	AddTorque(VECTOR3::Cross(r, force), mode);
}

//-----------------------------------------------
// 力を速度に反映
//-----------------------------------------------
void RIGID_BODY::IntegrateForces(const VECTOR3& gravity, float delta_time)
{
	if (m_type != RIGID_BODY_TYPE::DYNAMIC)
	{// 動的以外はスキップ
		return;
	}

	if (IsSleeping())
	{// スリープ中はスキップ
		return;
	}

	// 瞬間系
	m_linear_velocity += m_linear_impulse * m_inv_mass;
	m_linear_velocity += m_linear_velocity_change;

	m_angular_velocity += m_inv_inertia_world.MultiplyVector(m_angular_impulse);
	m_angular_velocity += m_angular_velocity_change;

	// 瞬間系を消す
	ClearInstantForces();

	// 連続系
	VECTOR3 accel = m_force * m_inv_mass;
	if (IsUseGravity())
	{// 重力加速度を加算
		accel += gravity;
	}

	// 力から加速度を計算
	m_linear_velocity += accel * delta_time;

	// トルクから角加速度を計算
	VECTOR3 ang_accel = m_inv_inertia_world.MultiplyVector(m_torque);
	m_angular_velocity += ang_accel * delta_time;

	// 減衰 clamp
	m_linear_velocity = m_linear_velocity * std::pow(m_linear_damping, delta_time);
	m_angular_velocity = m_angular_velocity * std::pow(m_angular_damping, delta_time);

	const float ANGULAR_SLEEP_EPS = 1e-3f;
	if (m_angular_velocity.LengthSquared() < ANGULAR_SLEEP_EPS * ANGULAR_SLEEP_EPS)
	{// 微小な角速度は0にする
		m_angular_velocity = VECTOR3(0, 0, 0);
	}

	// Freeze を反映
	ApplyAngularVelocityConstraint();
}

//-----------------------------------------------
// 速度を位置に反映
//-----------------------------------------------
void RIGID_BODY::IntegrateVelocities(float delta_time)
{
	if (m_type != RIGID_BODY_TYPE::DYNAMIC)
	{// 動的以外はスキップ
		return;
	}

	if (IsSleeping())
	{// スリープ中はスキップ
		return;
	}

	TRANSFORM* transform = GetOwner()->GetTransform();

	// 位置の更新
	VECTOR3 cur_pos = transform->GetPos();
	VECTOR3 new_pos = cur_pos + m_linear_velocity * delta_time;

	// Freeze を反映
	if (HasConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_POS_X))
	{
		new_pos.x = cur_pos.x;
	}
	if (HasConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_POS_Y))
	{
		new_pos.y = cur_pos.y;
	}
	if (HasConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_POS_Z))
	{
		new_pos.z = cur_pos.z;
	}

	transform->SetPos(new_pos);

	// Freeze を反映してから積分
	ApplyAngularVelocityConstraint();

	// 回転の更新
	VECTOR3 w = m_angular_velocity;
	float wlen2 = w.LengthSquared();
	if (wlen2 > 1e-8f)
	{
		// 角速度ベクトルから回転量を計算
		float wlen = std::sqrt(wlen2);
		VECTOR3 axis = w / wlen;
		float angle = wlen * delta_time;

		// クォータニオン化
		float half = 0.5f * angle;
		float s = std::sin(half);
		float c = std::cos(half);

		// 回転適用
		QUATERNION dq(axis.x * s, axis.y * s, axis.z * s, c);
		QUATERNION q = transform->GetRot();
		q = (dq * q).GetNormalized();
		transform->SetRot(q);
	}

	// 慣性テンソル更新
	UpdateInertiaTensor();
}

//-----------------------------------------------
// 慣性テンソルの設定
//-----------------------------------------------
void RIGID_BODY::SetInertiaTensor(const MATRIX3x3& inertia)
{
	m_inertia_local = inertia;
	m_inv_inertia_local = inertia.Inverse();
}

//-----------------------------------------------
// コライダーから慣性テンソルを計算して設定
//-----------------------------------------------
void RIGID_BODY::ComputeInertiaTensorFromCollider()
{
	if (m_type != RIGID_BODY_TYPE::DYNAMIC || !m_collider)
	{// 動的でないかコライダーが無い場合は単位テンソル
		m_inertia_local = MATRIX3x3::Identity();
		m_inv_inertia_local = MATRIX3x3::Zero();
		return;
	}

	switch (m_collider->GetColliderType())
	{
	case COLLIDER_TYPE_3D::SPHERE:
	{
		float r = static_cast<SPHERE_COLLIDER*>(m_collider)->GetRadius();
		float i = (2.0f / 5.0f) * m_mass * r * r;
		m_inertia_local = MATRIX3x3::Identity() * i;
		break;
	}
	case COLLIDER_TYPE_3D::BOX:
	{
		VECTOR3 he = static_cast<BOX_COLLIDER_3D*>(m_collider)->GetWorldSize() * 0.5f;
		float x2 = 2.0f * he.x * he.x;
		float y2 = 2.0f * he.y * he.y;
		float z2 = 2.0f * he.z * he.z;

		float ix = (1.0f / 3.0f) * m_mass * (y2 + z2);
		float iy = (1.0f / 3.0f) * m_mass * (x2 + z2);
		float iz = (1.0f / 3.0f) * m_mass * (x2 + y2);

		m_inertia_local = MATRIX3x3::Zero();
		m_inertia_local.m[0][0] = ix;
		m_inertia_local.m[1][1] = iy;
		m_inertia_local.m[2][2] = iz;
		break;
	}
	default:
		m_inertia_local = MATRIX3x3::Identity();
		break;
	}

	m_inv_inertia_local = m_inertia_local.Inverse();
}

//-----------------------------------------------
// 慣性テンソルの更新
//-----------------------------------------------
void RIGID_BODY::UpdateInertiaTensor()
{
	TRANSFORM* transform = GetOwner()->GetTransform();
	MATRIX3x3 r = transform->GetRot().ToMatrix3x3();
	m_inv_inertia_world = r * m_inv_inertia_local * r.Transpose();
}

//-----------------------------------------------
// コライダーの設定
//-----------------------------------------------
void RIGID_BODY::SetCollider(COLLIDER_3D* collider)
{
	m_collider = collider;

	if (m_collider && m_type == RIGID_BODY_TYPE::DYNAMIC)
	{// 慣性テンソルの再計算
		ComputeInertiaTensorFromCollider();
		UpdateInertiaTensor();
	}
}

//-----------------------------------------------
// 連続力のクリア
//-----------------------------------------------
void RIGID_BODY::ClearContinuousForces()
{
	m_force = VECTOR3(0.0f, 0.0f, 0.0f);
	m_torque = VECTOR3(0.0f, 0.0f, 0.0f);
}

//-----------------------------------------------
// 連続力のクリア
//-----------------------------------------------
void RIGID_BODY::ClearInstantForces()
{
	m_linear_impulse = VECTOR3(0.0f, 0.0f, 0.0f);
	m_angular_impulse = VECTOR3(0.0f, 0.0f, 0.0f);
	m_linear_velocity_change = VECTOR3(0.0f, 0.0f, 0.0f);
	m_angular_velocity_change = VECTOR3(0.0f, 0.0f, 0.0f);
}

//-----------------------------------------------
// 力クリア
//-----------------------------------------------
void RIGID_BODY::ClearForces()
{
	ClearContinuousForces();
	ClearInstantForces();
}
