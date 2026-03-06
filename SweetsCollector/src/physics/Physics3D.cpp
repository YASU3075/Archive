//-----------------------------------------------
// Physics3D.cpp
// 制作日：20205/07/10
// 制作者：安田晴人
//-----------------------------------------------
#include "physics/Physics3D.h"
#include "physics/RigidBody.h"
#include "physics/Collider3D.h"
#include "physics/Ray.h"
#include "physics/PhysicsQuery3D.h"
#include "physics/PhysicsBroadPhase3D.h"
#include "physics/PhysicsNarrowPhase3D.h"
#include "physics/PhysicsSolver3D.h"
#include "physics/PhysicsContactManifold3D.h"
#include "physics/PhysicsContactSolver3D.h"
#include "physics/joint/Joint3D.h"

// シングルトンインスタンスの初期化
PHYSICS_3D* PHYSICS_3D::m_instance = nullptr;

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void PHYSICS_3D::Init()
{
	// 衝突管理の初期化
	m_contact_manager.ClearContacts();

	// 剛体管理の初期化
	m_rigid_body_registry.Reset();

	// フレームカウント初期化
	m_frame_id = 0;
}

//-----------------------------------------------
// 終了処理
//-----------------------------------------------
void PHYSICS_3D::Uninit()
{
	// 各種リストのクリア
	m_contact_manager.ClearContacts();
	m_rigid_body_registry.Reset();
}

//-----------------------------------------------
// シングルトンインスタンス取得
//-----------------------------------------------
PHYSICS_3D& PHYSICS_3D::GetInstance()
{
	if (!m_instance)
	{// インスタンスが無ければ生成
		m_instance = new PHYSICS_3D();
		m_instance->Init();
	}
	return *m_instance;
}

//-----------------------------------------------
// 剛体の追加
//-----------------------------------------------
void PHYSICS_3D::AddRigidBody(RIGID_BODY* body)
{
	m_rigid_body_registry.Add(body);
}

//-----------------------------------------------
// 剛体の削除
//-----------------------------------------------
void PHYSICS_3D::RemoveRigidBody(RIGID_BODY* body)
{
	m_joint_manager.RemoveJointsForRigidBody(body);

	m_rigid_body_registry.Remove(body);
}

//-----------------------------------------------
// 物理ステップシミュレーション
//-----------------------------------------------
void PHYSICS_3D::Simulate(float dt)
{
	if (dt <= 0.0f)
	{// dtチェック
		return;
	}

	// デバッグ用フレームカウント更新
	m_frame_id++;

	// サブステップ分割
	int steps = std::max(1, static_cast<int>(std::ceil(dt / MAX_SUBSTEP)));
	float sub_dt = dt / static_cast<float>(steps);

	// ステップ情報の設定
	m_contact_manager.SetStepInfo((int)m_frame_id, -1, steps);

	// フレーム開始処理
	m_contact_manager.BeginFrame();

	// サブステップループ
	for (int i = 0; i < steps; i++)
	{
		// ステップ情報の設定
		m_contact_manager.SetStepInfo((int)m_frame_id, i, steps);

		// サブステップ開始処理
		m_contact_manager.BeginSubstep();

		// 保留中の剛体の追加・削除を反映
		SyncPendingBodies();

		// 力を速度に反映
		StepBodiesForces(sub_dt);

		// 広域判定
		BroadPhase();

		// 狭域判定
		NarrowPhase();

		// 衝突情報の準備
		PHYSICS_SOLVER_3D::PreStep(m_contact_manager, sub_dt);

		// ジョイントの前処理
		for (auto& j : m_joint_manager.GetJoints())
		{
			j->PreStep(sub_dt);
		}

		// ウォームスタート
		PHYSICS_SOLVER_3D::WarmStart(m_contact_manager);

		// 衝突制約の解決
		//PHYSICS_SOLVER_3D::SolveVelocity(m_contact_manager, sub_dt, m_velocity_iterations);

		{
			for (int iter = 0; iter < m_velocity_iterations; ++iter)
			{
				m_contact_manager.BeginSolverIteration(iter);

				// Contact
				for (auto& contact : m_contact_manager.GetContacts())
				{
					if (contact.is_trigger) continue;
					if (!contact.rb_a || !contact.rb_b) continue;
					if (contact.points.empty()) continue;
					if (!contact.touched_substep) continue;

					PHYSICS_CONTACT_SOLVER_3D::SolveNormal(contact, sub_dt);
					PHYSICS_CONTACT_SOLVER_3D::SolveFriction(contact, sub_dt);
				}

				// ★Joint
				for (auto& j : m_joint_manager.GetJoints())
				{
					if (j) j->SolveVelocity(sub_dt);
				}
			}
		}

		// 速度を位置に反映
		StepBodiesVelocities(sub_dt);

		// 位置修正
		PHYSICS_SOLVER_3D::SolvePosition(m_contact_manager, sub_dt, m_position_iterations);

		// ジョイントの位置補正
		for (auto& j : m_joint_manager.GetJoints())
		{
			if (j) j->SolvePosition(sub_dt);
		}

		// スリープ判定
		UpdateSleepStates(sub_dt);
	}

	// 連続力のクリア
	for (RIGID_BODY* rb : m_rigid_body_registry.GetRigidBodies())
	{
		if (rb)
		{
			rb->ClearContinuousForces();
		}
	}

	// フレーム終了処理
	m_contact_manager.EndFrame();

#if _DEBUG
	if (m_debug_draw)
	{
		DebugDraw();
	}
#endif
}

//-----------------------------------------------
// 保留中の剛体の追加・削除を反映
//-----------------------------------------------
void PHYSICS_3D::SyncPendingBodies()
{
	m_rigid_body_registry.Synchronize(m_contact_manager);
}

//-----------------------------------------------
// 広域判定
//-----------------------------------------------
void PHYSICS_3D::BroadPhase()
{
	// 潜在的な衝突ペアのクリア
	m_contact_manager.ClearPotentialPairs();

	// 潜在的な衝突ペアを計算
	auto pairs = PHYSICS_BROAD_PHASE_3D::ComputePotentialCollisionPairs(m_rigid_body_registry.GetRigidBodies());

	// 潜在的な衝突ペアを登録
	for (const auto& pair : pairs)
	{
		m_contact_manager.AddPotentialPair(pair.first, pair.second);
	}
}

//-----------------------------------------------
// 狭域判定
//-----------------------------------------------
void PHYSICS_3D::NarrowPhase()
{
	// 潜在的な衝突ペアごとに衝突判定とコンタクト点の生成
	const auto& pairs = m_contact_manager.GetPotentialPairs();

	for (auto& pair : pairs)
	{
		// RigidBody取得
		RIGID_BODY* a = pair.first;
		RIGID_BODY* b = pair.second;

		if (!a || !b)
		{// nullチェック
			continue;
		}

		if (!a->GetCollider() || !b->GetCollider())
		{// コライダーが無い場合はスキップ
			continue;
		}

		std::vector<CONTACT_POINT> contacts;
		bool collided = PHYSICS_NARROW_PHASE_3D::GenerateContacts(a, b, contacts);

		if (collided)
		{
			bool is_trigger = a->GetCollider()->IsTrigger() || b->GetCollider()->IsTrigger();

			if (is_trigger)
			{// トリガーの場合はコンタクト点を空にする
				static const std::vector<CONTACT_POINT> empty;
				PHYSICS_CONTACT_MANIFOLD_3D::Update(m_contact_manager.GetContacts(), a, b, empty);
			}
			else
			{// 通常の衝突
				PHYSICS_CONTACT_MANIFOLD_3D::Update(m_contact_manager.GetContacts(), a, b, contacts);
			}
		}
	}
}
	
//-----------------------------------------------
// 力を速度に反映
//-----------------------------------------------
void PHYSICS_3D::StepBodiesForces(float dt)
{
	for (RIGID_BODY* rb : m_rigid_body_registry.GetRigidBodies())
	{
		rb->IntegrateForces(m_gravity, dt);
	}
}

//-----------------------------------------------
// 速度を位置に反映
//-----------------------------------------------
void PHYSICS_3D::StepBodiesVelocities(float dt)
{
	for (RIGID_BODY* rb : m_rigid_body_registry.GetRigidBodies())
	{
		rb->IntegrateVelocities(dt);
	}
}

//-----------------------------------------------
// 位置補正
//-----------------------------------------------
void PHYSICS_3D::PositionCorrection()
{
	//return;

	//const float k_slop = 0.01f;
	//const float k_percent = 0.2f; // 0.2 → 0.4 に増やす

	//for (const auto& c : m_contact_manager.GetContacts()) 
	//{
	//	RIGID_BODY* a = c.rb_a;
	//	RIGID_BODY* b = c.rb_b;
	//	if (!a || !b) continue;
	//	if (c.points.empty()) continue;
	//	if (c.is_trigger) continue;

	//	float invMassA = a->GetInvMass();
	//	float invMassB = b->GetInvMass();
	//	float invMassSum = invMassA + invMassB;
	//	if (invMassSum == 0.0f) continue;

	//	VECTOR3 posA = a->GetOwner()->GetTransform()->GetPos();
	//	VECTOR3 posB = b->GetOwner()->GetTransform()->GetPos();

	//	// 複数点あるなら、一番深いやつだけ見る
	//	float maxPen = 0.0f;
	//	VECTOR3 n = c.points[0].normal; // A→B

	//	for (auto& p : c.points)
	//	{
	//		float pen = p.penetration;
	//		if (pen > maxPen)
	//		{
	//			maxPen = pen;
	//			n = p.normal;
	//		}
	//	}

	//	float pen = std::max(maxPen - k_slop, 0.0f);
	//	if (pen <= 0.0f) continue;

	//	VECTOR3 correction = n * (k_percent * pen / invMassSum); // A→B 方向

	//	// A は法線の逆方向に、B は法線方向に動かす
	//	if (invMassA > 0.0f)
	//		posA -= correction * invMassA;
	//	if (invMassB > 0.0f)
	//		posB += correction * invMassB;

	//	a->GetOwner()->GetTransform()->SetPos(posA);
	//	b->GetOwner()->GetTransform()->SetPos(posB);

	//	// 補正後の距離確認
	//	//for (const auto& p : c.points)
	//	//{
	//	//	VECTOR3 n = p.normal;
	//	//	VECTOR3 pa = a->GetOwner()->GetTransform()->GetPos();
	//	//	VECTOR3 pb = b->GetOwner()->GetTransform()->GetPos();

	//	//	// 接触点付近の代表点を取って、法線方向の距離を見る
	//	//	float sep = VECTOR3::Dot(pb - pa, n); // A→B の法線距離
	//	//	printf("[AfterPos] sep=%.6f\n", sep);
	//	//}
	//	//ApplyPositionalCorrection(c.rb_a, c.rb_b, c.points, k_slop, percent);
	//}
}

//-----------------------------------------------
// 位置補正の適用
//-----------------------------------------------
void PHYSICS_3D::ApplyPositionalCorrection(RIGID_BODY* a, RIGID_BODY* b, const std::vector<CONTACT_POINT>& pts, float slop, float percent)
{
	//if (!a || !b)
	//{// nullチェック
	//	return;
	//}

	//TRANSFORM* ta = a->GetOwner()->GetTransform();
	//TRANSFORM* tb = b->GetOwner()->GetTransform();
	//const VECTOR3 pos_a = ta->GetPos();
	//const VECTOR3 pos_b = tb->GetPos();

	//const float inv_mass_a = a->GetInvMass();
	//const float inv_mass_b = b->GetInvMass();

	//// 動的でないものは回転補正もしない
	//const bool dyn_a = (a->GetType() == RIGID_BODY_TYPE::DYNAMIC) && (inv_mass_a > 0.0f);
	//const bool dyn_b = (b->GetType() == RIGID_BODY_TYPE::DYNAMIC) && (inv_mass_b > 0.0f);

	//// 1点ずつ、角項込みで補正
	//for (const auto& pt : pts)
	//{
	//	float pen = pt.penetration - slop;
	//	if (pen <= 0.0f) continue;

	//	// 浅いめり込みは Baumgarte に任せる
	//	// 深く刺さったときだけ位置で“緊急退避”
	//	const float deep_threshold = 0.03f; // 3cm 以上だけ補正
	//	if (pen < deep_threshold)
	//	{
	//		continue;
	//	}

	//	VECTOR3 n = pt.normal;
	//	// 接触点のアーム
	//	VECTOR3 rA = pt.pos - pos_a;
	//	VECTOR3 rB = pt.pos - pos_b;

	//	// 有効質量 K = mA^-1 + mB^-1 + n·[(IA^-1 (rA×n))×rA + (IB^-1 (rB×n))×rB]
	//	float K = inv_mass_a + inv_mass_b;

	//	if (dyn_a)
	//	{
	//		VECTOR3 rnA = VECTOR3::Cross(rA, n);
	//		VECTOR3 angA = a->m_inv_inertia_world.MultiplyVector(rnA);
	//		K += VECTOR3::Dot(VECTOR3::Cross(angA, rA), n);
	//	}
	//	if (dyn_b)
	//	{
	//		VECTOR3 rnB = VECTOR3::Cross(rB, n);
	//		VECTOR3 angB = b->m_inv_inertia_world.MultiplyVector(rnB);
	//		K += VECTOR3::Dot(VECTOR3::Cross(angB, rB), n);
	//	}

	//	if (K < 1e-6f) continue;

	//	// 分割インパルス相当の位置修正量（過度な押し戻し防止のため percent を掛ける）
	//	const float lambda = (percent * pen) / K;

	//	// 並進分
	//	VECTOR3 dPA = dyn_a ? n * (-lambda * inv_mass_a) : VECTOR3(0, 0, 0);
	//	VECTOR3 dPB = dyn_b ? n * (lambda * inv_mass_b) : VECTOR3(0, 0, 0);

	//	// 角分（小角として直接クォート更新）
	//	VECTOR3 dThetaA(0, 0, 0), dThetaB(0, 0, 0);
	//	if (dyn_a)
	//	{
	//		VECTOR3 rnA = VECTOR3::Cross(rA, n);
	//		dThetaA = -a->m_inv_inertia_world.MultiplyVector(rnA) * lambda;
	//	}
	//	if (dyn_b)
	//	{
	//		VECTOR3 rnB = VECTOR3::Cross(rB, n);
	//		dThetaB = b->m_inv_inertia_world.MultiplyVector(rnB) * lambda;
	//	}

	//	// 位置反映
	//	if (dyn_a) ta->SetPos(ta->GetPos() + dPA);
	//	if (dyn_b) tb->SetPos(tb->GetPos() + dPB);

	//	// 回転反映（dq = 0.5 * omega * q * dt の小角近似をそのまま “位置補正用” に使う）
	//	auto applySmallRotation = [](TRANSFORM* t, const VECTOR3& dth) {
	//		float ang = dth.Length();
	//		if (ang < 1e-8f) return;
	//		VECTOR3 axis = dth / ang;
	//		float s = std::sin(ang * 0.5f);
	//		float c = std::cos(ang * 0.5f);
	//		QUATERNION dq(axis.x * s, axis.y * s, axis.z * s, c);
	//		QUATERNION q = t->GetRot();
	//		q = (dq * q).GetNormalized();
	//		t->SetRot(q);
	//		};
	//	if (dyn_a) applySmallRotation(ta, dThetaA);
	//	if (dyn_b) applySmallRotation(tb, dThetaB);

	//	// 慣性テンソル更新（回転が変わったので）
	//	if (dyn_a) a->UpdateInertiaTensor();
	//	if (dyn_b) b->UpdateInertiaTensor();
	//}
}

//-----------------------------------------------
// スリープ状態の更新
//-----------------------------------------------
void PHYSICS_3D::UpdateSleepStates(float dt)
{
	const float SLEEP_TIME = 5.0f; // 0.5秒間静止でスリープ

	for (auto* rb : m_rigid_body_registry.GetRigidBodies())
	{
		if (!rb || rb->GetType() != RIGID_BODY_TYPE::DYNAMIC || !rb->CanSleep())
		{// nullチェック・動的チェック・スリープ可能チェック
			continue;
		}

		if (rb->IsSleeping())
		{// すでにスリープ中
			continue;
		}

		// 速度が閾値以下かチェック
		float lin_vel_sq = rb->m_linear_velocity.LengthSquared();
		float ang_vel_sq = rb->m_angular_velocity.LengthSquared();
		float threshold_sq = rb->m_sleep_threshold * rb->m_sleep_threshold;

		bool low_velocity = (lin_vel_sq < threshold_sq) && (ang_vel_sq < threshold_sq);

		if (low_velocity)
		{
			rb->m_sleep_timer += dt;
			if (rb->m_sleep_timer >= SLEEP_TIME)
			{
				rb->Sleep();
			}
		}
		else
		{
			rb->m_sleep_timer = 0.0f;
		}
		
	}
}

//-----------------------------------------------
// 線分と線分の最近接点計算
//-----------------------------------------------
void ClosestPtSegmentSegment(const VECTOR3& p1, const VECTOR3& q1, const VECTOR3& p2, const VECTOR3& q2, float& s, float& t, VECTOR3& c1, VECTOR3& c2)
{
	const VECTOR3 d1 = q1 - p1;   // 1本目の方向
	const VECTOR3 d2 = q2 - p2;   // 2本目の方向
	const VECTOR3 r = p1 - p2;

	float a = VECTOR3::Dot(d1, d1); // d1•d1
	float e = VECTOR3::Dot(d2, d2); // d2•d2
	float f = VECTOR3::Dot(d2, r);

	if (a <= 1e-8f && e <= 1e-8f)
	{// 両方点状
		s = t = 0.0f;
		c1 = p1;
		c2 = p2;
		return;
	}

	if (a <= 1e-8f)
	{// 1本目が点状
		s = 0.0f;
		t = f / e;
		t = std::clamp(t, 0.0f, 1.0f);
	}
	else
	{// 1本目が線分状

		float c = VECTOR3::Dot(d1, r);

		if (e <= 1e-8f)
		{// 2本目が点状
			t = 0.0f;
			s = std::clamp(-c / a, 0.0f, 1.0f);
		}
		else
		{// 両方線分状

			// s と t の最適値を計算
			float b = VECTOR3::Dot(d1, d2);
			float denom = a * e - b * b;

			if (denom != 0.0f)
			{
				s = std::clamp((b * f - c * e) / denom, 0.0f, 1.0f);
			}
			else
			{
				s = 0.0f;
			}

			float tnom = b * s + f;
			if (tnom < 0.0f)
			{// t < 0 の場合
				t = 0.0f;
				s = std::clamp(-c / a, 0.0f, 1.0f);
			}
			else if (tnom > e)
			{// t > 1 の場合
				t = 1.0f;
				s = std::clamp((b - c) / a, 0.0f, 1.0f);
			}
			else
			{// 0 <= t <= 1 の場合
				t = tnom / e;
			}
		}
	}

	// 最近接点を計算
	c1 = p1 + d1 * s;
	c2 = p2 + d2 * t;
}

//-----------------------------------------------
// レイキャスト
//-----------------------------------------------
bool PHYSICS_3D::Raycast(const RAY& ray, RAY_CAST& hit_info, float max_distance) const
{
	return PHYSICS_QUERY_3D::Raycast(m_rigid_body_registry.GetRigidBodies(), ray, hit_info, max_distance);
}

//-----------------------------------------------
// 全てのヒットをレイキャスト
//-----------------------------------------------
std::vector<RAY_CAST> PHYSICS_3D::RaycastAll(const RAY& ray, float max_distance) const
{
	return PHYSICS_QUERY_3D::RaycastAll(m_rigid_body_registry.GetRigidBodies(), ray, max_distance);
}

//-----------------------------------------------
// 球と重なっている剛体を取得
//-----------------------------------------------
std::vector<RIGID_BODY*> PHYSICS_3D::OverlapSphere(const VECTOR3& center, float radius) const
{
	return PHYSICS_QUERY_3D::OverlapSphere(m_rigid_body_registry.GetRigidBodies(), center, radius);
}

//-----------------------------------------------
// AABBと重なっている剛体を取得
//-----------------------------------------------
std::vector<RIGID_BODY*> PHYSICS_3D::OverlapBox(const VECTOR3& center, const VECTOR3& half_extents) const
{
	return PHYSICS_QUERY_3D::OverlapBox(m_rigid_body_registry.GetRigidBodies(), center, half_extents);
}

//-----------------------------------------------
// デバッグ描画
//-----------------------------------------------
void PHYSICS_3D::DebugDraw() const
{
	// 登録されている剛体をデバッグ描画
	for (RIGID_BODY* rb : m_rigid_body_registry.GetRigidBodies())
	{
		if (rb)
		{
			rb->m_collider->DebugDraw();
		}
	}
}