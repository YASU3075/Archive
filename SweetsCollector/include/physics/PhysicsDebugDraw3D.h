//-----------------------------------------------
// PhysicsDebugDraw3D.h
// 制作日：2026/01/16
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <vector>
#include "math/Vector3.h"

class Renderer;

namespace PHYSICS_DEBUG_DRAW_3D
{
	void CLEAR();
	void AddLine(const VECTOR3& from, const VECTOR3& to, const unsigned int color);
	void AddAABB(const VECTOR3& min, const VECTOR3& max, const unsigned int color);
	void AddOBB(const VECTOR3& center, const VECTOR3& axis_x, const VECTOR3& axis_y, const VECTOR3& axis_z, const VECTOR3& half_size, const unsigned int color);
	void AddSphere(const VECTOR3& center, float radius, const unsigned int color, int segments = 12);

	void Flush();
};