//-----------------------------------------------
// PhysicsContactSolver3D.cpp
// 制作日：2026/01/15
// 制作者：安田晴人
//-----------------------------------------------
#include <algorithm>
#include "physics/PhysicsContactSolver3D.h"
#include "Main.h"
#include "scene/Manager.h"
#include "renderer/Renderer.h"
#include "entity/GameObject.h"

#include "physics/RigidBody.h"
#include "math/Matrix3x3.h"


//-----------------------------------------------
// 事前準備
//-----------------------------------------------
void PHYSICS_CONTACT_SOLVER_3D::PreStep(CONTACT_3D& contact, float dt)
{
	const float baumgarte = 0.08f;		// 位置修正係数
	const float slop = 0.01f;			// ペネトレーションの許容値
	const float rest_threshold = 1.0f;	// 反発のしきい値

	if (dt <= 0.0f)
	{// dt が不正なら処理しない
		return;
	}

	if (!contact.touched_substep)
	{// サブステップで接触していないなら処理しない
		return;
	}

	if (contact.is_trigger || !contact.rb_a || !contact.rb_b)
	{// トリガーか、剛体がセットされていないなら処理しない
		return;
	}

	if (contact.points.empty())
	{// 接触点がないなら処理しない
		return;
	}

	// 剛体の取得
	RIGID_BODY* a = contact.rb_a;
	RIGID_BODY* b = contact.rb_b;

	// 逆質量、逆慣性テンソルの取得
	const float inv_mass_a = a->GetInvMass();
	const float inv_mass_b = b->GetInvMass();
	const MATRIX3x3& inv_inertia_a = a->GetInvInertiaWorld();
	const MATRIX3x3& inv_inertia_b = b->GetInvInertiaWorld();

	// 剛体の位置の取得
	const VECTOR3 pos_a = a->GetOwner()->GetTransform()->GetPos();
	const VECTOR3 pos_b = b->GetOwner()->GetTransform()->GetPos();

	// 各接触点の準備
	for (auto& p : contact.points)
	{
		p.accumulated_normal_impulse = 0.0f;
		p.accumulated_tangent_impulse_world = VECTOR3(0.0f, 0.0f, 0.0f);

		const VECTOR3 n = p.normal;			// 法線ベクトル
		const VECTOR3 ra = p.pos - pos_a;	// 接触点 - 剛体Aの位置ベクトル
		const VECTOR3 rb = p.pos - pos_b;	// 接触点 - 剛体Bの位置ベクトル

		// 法線ベクトルと剛体の位置ベクトルの外積
		VECTOR3 ra_x_n = VECTOR3::Cross(ra, n);
		VECTOR3 rb_x_n = VECTOR3::Cross(rb, n);

		// 法線方向の質量の計算
		float k_n =
			inv_mass_a + inv_mass_b +
			VECTOR3::Dot(n, VECTOR3::Cross(inv_inertia_a.MultiplyVector(ra_x_n), ra)) +
			VECTOR3::Dot(n, VECTOR3::Cross(inv_inertia_b.MultiplyVector(rb_x_n), rb));

		p.normal_mass = (k_n < 1e-7f) ? 0.0f : 1.0f / k_n;

		// 接触点での相対速度の計算
		VECTOR3 va = a->GetLinearVelocity() + VECTOR3::Cross(a->GetAngularVelocity(), ra);
		VECTOR3 vb = b->GetLinearVelocity() + VECTOR3::Cross(b->GetAngularVelocity(), rb);
		float vn = VECTOR3::Dot(vb - va, n);

		// 位置バイアス計算
		float bias_pos = 0.0f;
		float pen = p.penetration;
		if (pen > slop && baumgarte > 0.0f)
		{// 位置補正が必要なら
			bias_pos = baumgarte * (pen - slop) / dt;
		}
		
		// 反発バイアス計算
		float bias_rest = 0.0f;
		if (p.just_started && p.restitution > 0.0f && vn < -rest_threshold)
		{// 反発が必要なら
			bias_rest = p.restitution * vn;
		}

		// 速度バイアスの設定
		p.velocity_bias = -(bias_pos + bias_rest);
	}
}

//-----------------------------------------------
// 法線方向の衝突解決
//-----------------------------------------------
void PHYSICS_CONTACT_SOLVER_3D::SolveNormal(CONTACT_3D& contact, float dt)
{
	// 剛体の取得
	RIGID_BODY* a = contact.rb_a;
	RIGID_BODY* b = contact.rb_b;

	if (!a || !b)
	{// nullチェック
		return;
	}

	if (contact.points.empty())
	{// 接触点がないなら処理しない
		return;
	}

	if (contact.is_trigger)
	{// トリガーなら処理しない
		return;
	}

	// 逆質量、逆慣性テンソルの取得
	const float inv_mass_a = a->GetInvMass();
	const float inv_mass_b = b->GetInvMass();
	const MATRIX3x3& inv_inertia_a = a->GetInvInertiaWorld();
	const MATRIX3x3& inv_inertia_b = b->GetInvInertiaWorld();

	// 剛体の位置の取得
	const VECTOR3 pos_a = a->GetOwner()->GetTransform()->GetPos();
	const VECTOR3 pos_b = b->GetOwner()->GetTransform()->GetPos();

	// 反発のしきい値
	const float bounceThreshold = 1.0f;

	// 各接触点の衝突解決(Gauss-Seidel法)
	for (auto& p : contact.points)
	{
		if (p.normal_mass <= 0.0f)
		{// 質量が無限大なら処理しない
			continue;
		}

		// 接触点情報の取得
		const VECTOR3& n = p.normal;
		const VECTOR3 ra = p.pos - pos_a;
		const VECTOR3 rb = p.pos - pos_b;

		// 接触点での相対速度の計算
		VECTOR3 v_a = a->GetLinearVelocity();
		VECTOR3 w_a = a->GetAngularVelocity();
		VECTOR3 v_b = b->GetLinearVelocity();
		VECTOR3 w_b = b->GetAngularVelocity();

		VECTOR3 va = v_a + VECTOR3::Cross(w_a, ra);
		VECTOR3 vb = v_b + VECTOR3::Cross(w_b, rb);
		float vn = VECTOR3::Dot(vb - va, n);

		// スロップ判定
		const float slop = 0.01f;
		if (vn > 0.0f && p.penetration <= slop)
		{// 接触が離れているなら処理しない
			continue;
		}

		// 速度バイアスの取得
		float bias = p.velocity_bias;

		// 法線方向の相対速度に速度バイアスを加味
		float c_dot = vn - p.velocity_bias;
		float lambda = -p.normal_mass * c_dot;

		// インパルスの累積とクランプ
		float old = p.accumulated_normal_impulse;
		float neu = std::max(old + lambda, 0.0f);
		lambda = neu - old;
		p.accumulated_normal_impulse = neu;

		if (std::fabs(lambda) < 1e-8f)
		{// インパルスが微小なら処理しない
			continue;
		}

		// 衝突インパルスの適用
		VECTOR3 impulse = n * lambda;
		VECTOR3 impulse_a = -impulse;

		if (inv_mass_a > 0.0f)
		{// 剛体Aにインパルスを適用
			a->SetLinearVelocity(v_a + impulse_a * inv_mass_a);
			a->SetAngularVelocity(w_a + inv_inertia_a.MultiplyVector(VECTOR3::Cross(ra, impulse_a)));
			a->WakeUp();
		}

		if (inv_mass_b > 0.0f)
		{// 剛体Bにインパルスを適用
			b->SetLinearVelocity(v_b + impulse * inv_mass_b);
			b->SetAngularVelocity(w_b + inv_inertia_b.MultiplyVector(VECTOR3::Cross(rb, impulse)));
			b->WakeUp();
		}
	}
}

//-----------------------------------------------
// 接線方向の衝突解決
//-----------------------------------------------
void PHYSICS_CONTACT_SOLVER_3D::SolveFriction(CONTACT_3D& contact, float dt)
{
	// 剛体の取得
	RIGID_BODY* a = contact.rb_a;
	RIGID_BODY* b = contact.rb_b;

	if (!a || !b)
	{// nullチェック
		return;
	}

	if (contact.points.empty())
	{// 接触点がないなら処理しない
		return;
	}

	if (contact.is_trigger)
	{// トリガーなら処理しない
		return;
	}

	// 逆質量、逆慣性テンソルの取得
	const float inv_mass_a = a->GetInvMass();
	const float inv_mass_b = b->GetInvMass();
	const MATRIX3x3& inv_inertia_a = a->GetInvInertiaWorld();
	const MATRIX3x3& inv_inertia_b = b->GetInvInertiaWorld();

	// 剛体の位置の取得
	const VECTOR3 pos_a = a->GetOwner()->GetTransform()->GetPos();
	const VECTOR3 pos_b = b->GetOwner()->GetTransform()->GetPos();

	// 各接触点の衝突解決(Gauss-Seidel法)
	for (auto& p : contact.points)
	{
		float jn = p.accumulated_normal_impulse;
		if (jn <= 0.0f)
		{// 法線インパルスが無いなら処理しない
			continue;
		}

		// 接触点情報の取得
		const VECTOR3 n = p.normal;
		const VECTOR3 ra = p.pos - pos_a;
		const VECTOR3 rb = p.pos - pos_b;

		// 接触点での相対速度の計算
		const VECTOR3 v_a = a->GetLinearVelocity();
		const VECTOR3 w_a = a->GetAngularVelocity();
		const VECTOR3 v_b = b->GetLinearVelocity();
		const VECTOR3 w_b = b->GetAngularVelocity();

		const VECTOR3 va = v_a + VECTOR3::Cross(w_a, ra);
		const VECTOR3 vb = v_b + VECTOR3::Cross(w_b, rb);
		const VECTOR3 rel = vb - va;

		// 接線ベクトルの計算
		VECTOR3 t1;
		if (p.cached_tangent.LengthSquared() > 1e-8f)
		{// 前回の接線ベクトルが有効ならそれを使う
			t1 = p.cached_tangent - n * VECTOR3::Dot(p.cached_tangent, n);
			if (t1.LengthSquared() < 1e-8f)
			{// ほぼ直交していないなら再計算
				t1 = ComputeTangent(n);
			}
			else
			{// 正規化
				t1.Normalize();
			}
		}
		else
		{// 新しく計算
			t1 = ComputeTangent(n);
		}

		// 第二接線ベクトル
		VECTOR3 t2 = VECTOR3::Cross(n, t1);

		// 接線方向の相対速度
		float vt1 = VECTOR3::Dot(rel, t1);
		float vt2 = VECTOR3::Dot(rel, t2);

		// 接線方向の質量計算
		auto AngTerm = [](const MATRIX3x3& invI, const VECTOR3& r, const VECTOR3& d)->float
			{
				return VECTOR3::Dot(d, VECTOR3::Cross(invI.MultiplyVector(VECTOR3::Cross(r, d)), r));
			};

		// 接線1方向の質量
		float denom1 = inv_mass_a + inv_mass_b;

		if (inv_mass_a > 0.0f)
		{
			denom1 += AngTerm(inv_inertia_a, ra, t1);
		}

		if (inv_mass_b > 0.0f)
		{
			denom1 += AngTerm(inv_inertia_b, rb, t1);
		}

		if (denom1 < 1e-12f)
		{// 質量無限大なら
			continue;
		}

		// 接線2方向の質量
		float denom2 = inv_mass_a + inv_mass_b;

		if (inv_mass_a > 0.0f)
		{
			denom2 += AngTerm(inv_inertia_a, ra, t2);
		}

		if (inv_mass_b > 0.0f)
		{
			denom2 += AngTerm(inv_inertia_b, rb, t2);
		}

		if (denom2 < 1e-12f)
		{// 質量無限大なら
			continue;
		}

		// 接線方向のインパルス計算
		float dJ1 = -vt1 / denom1;
		float dJ2 = -vt2 / denom2;

		// インパルスの累積と摩擦円によるクランプ
		VECTOR3 old_w = p.accumulated_tangent_impulse_world;
		float old1 = VECTOR3::Dot(old_w, t1);
		float old2 = VECTOR3::Dot(old_w, t2);

		// 新しい累積インパルス（接線1、接線2方向）
		float star1 = old1 + dJ1;
		float star2 = old2 + dJ2;

		// 摩擦円によるクランプ
		float star_len = std::sqrt(star1 * star1 + star2 * star2);

		// 摩擦係数の取得
		float mu_s = p.static_friction;
		float mu_d = p.dynamic_friction;
		float max_s = mu_s * jn;
		float max_d = mu_d * jn;

		float new1, new2;
		if (star_len <= max_s)
		{// 静止摩擦：そのまま適用
			new1 = star1; new2 = star2;
		}
		else
		{// 動摩擦：摩擦円にクランプ
			if (star_len > 1e-12f)
			{// ゼロベクトルでないなら
				float s = max_d / star_len;
				new1 = star1 * s;
				new2 = star2 * s;
			}
			else
			{// ゼロベクトルならゼロにする
				new1 = new2 = 0.0f;
			}
		}

		// 実際に適用するインパルス
		float apply1 = new1 - old1;
		float apply2 = new2 - old2;

		// 累積インパルスの更新
		p.accumulated_tangent_impulse_world = t1 * new1 + t2 * new2;

		// インパルスの適用
		VECTOR3 pt = t1 * apply1 + t2 * apply2;

		if (inv_mass_a > 0.0f)
		{// 剛体Aにインパルスを適用
			a->SetLinearVelocity(a->GetLinearVelocity() - pt * inv_mass_a);
			a->SetAngularVelocity(a->GetAngularVelocity() + inv_inertia_a.MultiplyVector(VECTOR3::Cross(ra, -pt)));
		}
		if (inv_mass_b > 0.0f)
		{// 剛体Bにインパルスを適用
			b->SetLinearVelocity(b->GetLinearVelocity() + pt * inv_mass_b);
			b->SetAngularVelocity(b->GetAngularVelocity() + inv_inertia_b.MultiplyVector(VECTOR3::Cross(rb, pt)));
		}

		// 接線ベクトルのキャッシュ
		p.cached_tangent = t1;
	}
}

//-----------------------------------------------
// 接線ベクトルの計算
//-----------------------------------------------
VECTOR3 PHYSICS_CONTACT_SOLVER_3D::ComputeTangent(const VECTOR3& normal)
{
	VECTOR3 tangent;

	if (std::fabs(normal.y) < 0.9f)
	{// 法線ベクトルがY軸に近くないなら
		tangent = VECTOR3::Cross(VECTOR3(0.0f, 1.0f, 0.0f), normal);
	}
	else
	{// 法線ベクトルがY軸に近いなら
		tangent = VECTOR3::Cross(VECTOR3(1.0f, 0.0f, 0.0f), normal);
	}

	if (tangent.LengthSquared() > 1e-8f)
	{// ゼロベクトルでないなら正規化
		tangent.Normalize();
	}
	else
	{// ゼロベクトルなら適当に初期化
		tangent = VECTOR3(0.0f, 0.0f, 0.0f);
	}

	return tangent;
}
