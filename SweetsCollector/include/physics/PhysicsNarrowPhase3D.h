//-----------------------------------------------
// PhysicsNarrowPhase3D.h
// 制作日：2026/01/14
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include <vector>

struct CONTACT_POINT;
class RIGID_BODY;

namespace PHYSICS_NARROW_PHASE_3D
{
	bool GenerateContacts(RIGID_BODY* a, RIGID_BODY* b, std::vector<CONTACT_POINT>& out_contacts);
}