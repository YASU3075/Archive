//-----------------------------------------------
// PhysicsContactManifold3D.cpp
// 制作日：2026/01/15
// 制作者：安田晴人
//-----------------------------------------------
#include "physics/PhysicsContactManifold3D.h"
#include "physics/PhysicsContact3D.h"
#include "physics/RigidBody.h"
#include "physics/Collider3D.h"
#include "physics/PhysicsContactSolver3D.h"

namespace
{
	//-----------------------------------------------
	// ペアをID順にソート
	//-----------------------------------------------
	void SortPair(RIGID_BODY*& a, RIGID_BODY*& b, bool& reversed)
	{
		reversed = false;
		if (a->GetID() > b->GetID())
		{
			std::swap(a, b);
			reversed = true;
		}
	}

	//-----------------------------------------------
	// トリガーかどうかを計算
	//-----------------------------------------------
	bool ComputeIsTrigger(RIGID_BODY* a, RIGID_BODY* b)
	{
		auto* col_a = a ? a->GetCollider() : nullptr;
		auto* col_b = b ? b->GetCollider() : nullptr;
		return (col_a && col_a->IsTrigger()) || (col_b && col_b->IsTrigger());
	}
}

//-----------------------------------------------
// マニフォールドの更新
//-----------------------------------------------
void PHYSICS_CONTACT_MANIFOLD_3D::Update(std::vector<CONTACT_3D>& contacts, RIGID_BODY* a, RIGID_BODY* b, const std::vector<CONTACT_POINT>& new_points)
{
	if (!a || !b)
	{// nullチェック
		return;
	}

	// ペアをソート
	RIGID_BODY* rb_a = a;
	RIGID_BODY* rb_b = b;
	bool reversed = false;
	SortPair(rb_a, rb_b, reversed);

	// トリガーかどうかを計算
	const bool is_trigger = ComputeIsTrigger(rb_a, rb_b);

	// 既存検索
	auto it = std::find_if(contacts.begin(), contacts.end(),
		[rb_a, rb_b](const CONTACT_3D& c) { return c.rb_a == rb_a && c.rb_b == rb_b; });

	// 新規作成
	if (it == contacts.end())
	{
		CONTACT_3D c;
		c.rb_a = rb_a;
		c.rb_b = rb_b;
		c.is_trigger = is_trigger;
		c.touched_frame = true;
		c.touched_substep = true;
		c.was_touching = false;
		c.restitution_bias = 0.0f;
		c.do_restitution = false;
		c.restitution_used = false;

		if (!is_trigger)
		{// 非Triggerなら points 作成

			std::vector<CONTACT_POINT> pts = new_points;

			if (reversed)
			{// 順番が逆なら法線反転
				for (auto& p : pts)
				{
					p.normal = -p.normal;
				}
			}

			float max_pen = 0.0f;
			for (auto& p : pts)
			{
				max_pen = std::max(max_pen, p.penetration);
			}

			if (pts.size() > 4)
			{// 最大4点に削減
				std::sort(pts.begin(), pts.end(),
					[](const CONTACT_POINT& x, const CONTACT_POINT& y) { return x.penetration > y.penetration; });
				pts.resize(4);
			}

			for (auto& p : pts)
			{// ウォームスタート用初期化
				p.accumulated_normal_impulse = 0.0f;
				p.accumulated_tangent_impulse_world = VECTOR3(0, 0, 0);
				p.cached_tangent = { 0,0,0 };
				p.normal_mass = 0.0f;
				p.velocity_bias = 0.0f;
				p.just_started = true;
				c.points.push_back(p);
			}

			if (!c.points.empty())
			{// 接線ベクトル計算
				VECTOR3 n = c.points[0].normal.GetNormalized();
				c.tangent = PHYSICS_CONTACT_SOLVER_3D::ComputeTangent(n);
				
				for (auto& p : c.points)
				{// 接線ベクトルキャッシュ
					p.cached_tangent = c.tangent;
				}
			}
		}
		else
		{// Triggerなら points は空
			c.points.clear();
		}

		contacts.push_back(std::move(c));
		return;
	}


	// 既存更新
	CONTACT_3D& man = *it;
	man.touched_frame = true;
	man.touched_substep = true;
	man.is_trigger = is_trigger;

	if (is_trigger)
	{// Triggerなら points は空にする
		man.points.clear();
		man.tangent = { 0,0,0 };
		return;
	}

	// 非Triggerなら points 更新
	std::vector<CONTACT_POINT> updated_points = new_points;
	if (reversed)
	{// 順番が逆なら法線反転
		for (auto& p : updated_points)
		{
			p.normal = -p.normal;
		}
	}

	if (updated_points.size() > 4)
	{// 最大4点に削減
		std::sort(updated_points.begin(), updated_points.end(),
			[](const CONTACT_POINT& x, const CONTACT_POINT& y) { return x.penetration > y.penetration; });
		updated_points.resize(4);
	}

	const float match_eps = 0.03f;
	const float match_eps_sq = match_eps * match_eps;

	std::vector<CONTACT_POINT> replaced;
	replaced.reserve(updated_points.size());

	// ウォームスタート用に既存点とマッチング
	for (auto& np : updated_points)
	{
		int best = -1;
		float best_d2 = 1e9f;

		// 既存点と比較
		for (int i = 0; i < (int)man.points.size(); ++i)
		{
			float d2 = (man.points[i].pos - np.pos).LengthSquared();
			if (d2 < best_d2)
			{// 最良更新
				best_d2 = d2;
				best = i;
			}
		}

		if (best >= 0 && best_d2 <= match_eps_sq)
		{// マッチした
			const CONTACT_POINT& old_point = man.points[best];

			np.accumulated_normal_impulse = 0.0f;
			np.accumulated_tangent_impulse_world = VECTOR3(0, 0, 0);

			np.cached_tangent = old_point.cached_tangent;
			np.normal_mass = 0.0f;
			np.velocity_bias = 0.0f;

			np.just_started = false;
		}
		else
		{// マッチしなかったので初期化
			np.accumulated_normal_impulse = 0.0f;
			np.accumulated_tangent_impulse_world = VECTOR3(0, 0, 0);
			np.cached_tangent = { 0,0,0 };
			np.normal_mass = 0.0f;
			np.velocity_bias = 0.0f;
			np.just_started = true;
		}

		replaced.push_back(np);
		if (replaced.size() == 4)
		{// 最大4点まで
			break;
		}
	}

	man.points.swap(replaced);

	if (!man.points.empty())
	{// 接線ベクトル計算
		VECTOR3 n = man.points[0].normal.GetNormalized();
		man.tangent = PHYSICS_CONTACT_SOLVER_3D::ComputeTangent(n);
		for (auto& p : man.points) p.cached_tangent = man.tangent;
	}
	
}