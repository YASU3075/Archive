//-----------------------------------------------
// PhysicsBroadPhase3D.cpp
// 制作日：2026/01/14
// 制作者：安田晴人
//-----------------------------------------------
#include "physics/PhysicsBroadPhase3D.h"
#include "physics/RigidBody.h"
#include "physics/Collider3D.h"
#include "physics/AABB3D.h"


//-----------------------------------------------
// 潜在的な衝突ペアを計算
//-----------------------------------------------
std::vector<PHYSICS_BROAD_PHASE_3D::Pair> PHYSICS_BROAD_PHASE_3D::ComputePotentialCollisionPairs(const std::vector<RIGID_BODY*>& bodies)
{
	std::vector<Pair> pairs;

	for (size_t i = 0; i < bodies.size(); i++)
	{
		RIGID_BODY* a = bodies[i];
		if (!a || !a->GetCollider())
		{// コライダーが無い場合はスキップ
			continue;
		}

		// AABBの計算
		VECTOR3 min_a, max_a;
		a->GetCollider()->ComputeAABB(min_a, max_a);

		for (size_t j = i + 1; j < bodies.size(); j++)
		{
			RIGID_BODY* b = bodies[j];
			if (!b || !b->GetCollider())
			{// コライダーが無い場合はスキップ
				continue;
			}

			if (a->GetType() == RIGID_BODY_TYPE::STATIC && b->GetType() == RIGID_BODY_TYPE::STATIC)
			{// 静的同士の衝突は無視
				continue;
			}

			// AABBの計算
			VECTOR3 min_b, max_b;
			b->GetCollider()->ComputeAABB(min_b, max_b);

			// AABBの重なり判定
			if (min_a.x <= max_b.x && max_a.x >= min_b.x &&
				min_a.y <= max_b.y && max_a.y >= min_b.y &&
				min_a.z <= max_b.z && max_a.z >= min_b.z)
			{
				RIGID_BODY* rb_a = a;
				RIGID_BODY* rb_b = b;
				if (rb_a->GetID() > rb_b->GetID())
				{// ID順に並べ替え
					std::swap(rb_a, rb_b);
				}

				pairs.emplace_back(rb_a, rb_b);
			}
		}
	}

	return pairs;
}
