//-----------------------------------------------
// PhysicsQuery3D.cpp
// 制作日：2026/01/14
// 制作者：安田晴人
//-----------------------------------------------
#include <algorithm>
#include "physics/PhysicsQuery3D.h"
#include "physics/RigidBody.h"
#include "physics/Collider3D.h"
#include "physics/SphereCollider.h"
#include "physics/BoxCollider3D.h"
#include "physics/AABB3D.h"
#include "physics/Ray.h"

//-----------------------------------------------
// 最初にヒットしたものをレイキャスト
//-----------------------------------------------
bool PHYSICS_QUERY_3D::Raycast(const std::vector<RIGID_BODY*>& bodies, const RAY& ray, RAY_CAST& hit_info, float max_distance)
{
	bool hit = false;
	float closest_distance = max_distance;

	for (RIGID_BODY* body : bodies)
	{
		if (!body)
		{// nullチェック
			continue;
		}

		COLLIDER_3D* collider = body->GetCollider();

		if (!collider)
		{// nullチェック
			continue;
		}

		RAY_CAST temp_hit;
		if (collider->Raycast(ray, temp_hit, closest_distance))
		{// ヒットした
			if (temp_hit.distance < closest_distance)
			{// 最も近いヒットを保存
				closest_distance = temp_hit.distance;
				hit_info = temp_hit;
				hit_info.collider = collider;
				hit = true;
			}
		}
	}
	return hit;
}

//-----------------------------------------------
// すべてのヒットをレイキャスト
//-----------------------------------------------
std::vector<RAY_CAST> PHYSICS_QUERY_3D::RaycastAll(const std::vector<RIGID_BODY*>& bodies, const RAY& ray, float max_distance)
{
	std::vector<RAY_CAST> hits;
	float closest_distance = max_distance;

	for (RIGID_BODY* body : bodies)
	{
		if (!body)
		{// nullチェック
			continue;
		}

		COLLIDER_3D* collider = body->GetCollider();

		if (!collider)
		{// nullチェック
			continue;
		}

		RAY_CAST temp_hit;
		if (collider->Raycast(ray, temp_hit, closest_distance))
		{// ヒットした
			closest_distance = temp_hit.distance;
			temp_hit.collider = collider;
			hits.push_back(temp_hit);
		}
	}

	// ヒットしたものを距離順にソート
	std::sort(hits.begin(), hits.end(), [](const RAY_CAST& a, const RAY_CAST& b) { return a.distance < b.distance; });
	return hits;
}

//-----------------------------------------------
// 球と重なっている剛体を取得
//-----------------------------------------------
std::vector<RIGID_BODY*> PHYSICS_QUERY_3D::OverlapSphere(const std::vector<RIGID_BODY*>& bodies, const VECTOR3& center, float radius)
{
	std::vector<RIGID_BODY*> results;
	float radius_sq = radius * radius;
	for (RIGID_BODY* body : bodies)
	{
		if (!body)
		{// nullチェック
			continue;
		}

		COLLIDER_3D* collider = body->GetCollider();
		if (!collider)
		{// nullチェック
			continue;
		}

		// コライダーのAABB取得
		VECTOR3 min_b, max_b;
		collider->ComputeAABB(min_b, max_b);

		// AABBと球の重なり判定
		VECTOR3 closest;
		closest.x = std::max(min_b.x, std::min(center.x, max_b.x));
		closest.y = std::max(min_b.y, std::min(center.y, max_b.y));
		closest.z = std::max(min_b.z, std::min(center.z, max_b.z));
		VECTOR3 diff = closest - center;

		if (diff.LengthSquared() <= radius_sq)
		{// 重なっている
			results.push_back(body);
		}
	}
	return results;
}

//-----------------------------------------------
// AABBと重なっている剛体を取得
//-----------------------------------------------
std::vector<RIGID_BODY*> PHYSICS_QUERY_3D::OverlapBox(const std::vector<RIGID_BODY*>& bodies, const VECTOR3& center, const VECTOR3& half_extents)
{
	std::vector<RIGID_BODY*> results;
	VECTOR3 box_min = center - half_extents;
	VECTOR3 box_max = center + half_extents;

	for (RIGID_BODY* body : bodies)
	{
		if (!body)
		{// nullチェック
			continue;
		}

		COLLIDER_3D* collider = body->GetCollider();
		if (!collider)
		{// nullチェック
			continue;
		}

		// コライダーのAABB取得
		VECTOR3 min_b, max_b;
		collider->ComputeAABB(min_b, max_b);

		// AABB同士の重なり判定
		if (box_min.x <= max_b.x && box_max.x >= min_b.x &&
			box_min.y <= max_b.y && box_max.y >= min_b.y &&
			box_min.z <= max_b.z && box_max.z >= min_b.z)
		{// 重なっている
			results.push_back(body);
		}
	}
	return results;
}