//-----------------------------------------------
// PhysicsQuery3D.h
// 制作日：2026/01/14
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <vector>
#include <cfloat>
#include "math/Vector3.h"

class RIGID_BODY;
struct RAY;
struct RAY_CAST;

namespace PHYSICS_QUERY_3D
{
	// 最初にヒットしたものをレイキャスト
    bool Raycast(const std::vector<RIGID_BODY*>& bodies, const RAY& ray, RAY_CAST& hit_info, float max_distance = FLT_MAX);

	// すべてのヒットをレイキャスト
    std::vector<RAY_CAST> RaycastAll(const std::vector<RIGID_BODY*>& bodies, const RAY& ray, float max_distance = FLT_MAX);

	// 球と重なっている剛体を取得
    std::vector<RIGID_BODY*> OverlapSphere(const std::vector<RIGID_BODY*>& bodies, const VECTOR3& center, float radius);

	// AABBと重なっている剛体を取得
    std::vector<RIGID_BODY*> OverlapBox(const std::vector<RIGID_BODY*>& bodies, const VECTOR3& center, const VECTOR3& half_extents);
}