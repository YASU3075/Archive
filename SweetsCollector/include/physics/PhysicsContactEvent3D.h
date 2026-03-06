//-----------------------------------------------
// PhysicsContactEvent3D.h
// 制作日：2026/01/15
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

struct CONTACT_3D;

namespace PHYSICS_CONTACT_EVENT_3D
{
	void Dispatch(const CONTACT_3D& contact, bool now, bool prev);
}
