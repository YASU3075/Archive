//-----------------------------------------------
// PhysicsContactSolver3D.h
// 制作日：2026/01/15
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "math/Vector3.h"
#include "physics/PhysicsContact3D.h"

namespace PHYSICS_CONTACT_SOLVER_3D
{
	// 事前準備
    void PreStep(CONTACT_3D& contact, float dt);

    // 速度ソルバ（PGS）
    void SolveNormal(CONTACT_3D& contact, float dt);
    void SolveFriction(CONTACT_3D& contact, float dt);

	// ヘルパー関数
    VECTOR3 ComputeTangent(const VECTOR3& normal);
}
