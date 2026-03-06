//-----------------------------------------------
// PhysicsContactManifold3D.h
// 制作日：2026/01/15
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <vector>

class RIGID_BODY;
struct CONTACT_POINT;
struct CONTACT_3D;

namespace PHYSICS_CONTACT_MANIFOLD_3D
{
	// マニフォールドの更新
	void Update(std::vector<CONTACT_3D>& contacts, RIGID_BODY* a, RIGID_BODY* b, const std::vector<CONTACT_POINT>& new_points);

	// 剛体に関するコンタクトの削除
	void RemoveByBody(std::vector<CONTACT_3D>& contacts, RIGID_BODY* rb);

	// 指定剛体が接触中かどうか
	bool IsBodyTouching(const std::vector<CONTACT_3D>& contacts, RIGID_BODY* rb);
}
