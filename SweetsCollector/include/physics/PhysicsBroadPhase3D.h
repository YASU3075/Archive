//-----------------------------------------------
// PhysicsBroadPhase3D.h
// 制作日：2026/01/14
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <vector>
#include <utility>

class RIGID_BODY;

namespace PHYSICS_BROAD_PHASE_3D
{
	using Pair = std::pair<RIGID_BODY*, RIGID_BODY*>;
	std::vector<Pair> ComputePotentialCollisionPairs(const std::vector<RIGID_BODY*>& bodies);
}
