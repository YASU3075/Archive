//-----------------------------------------------
// PhysicsSolver3D.h
// 制作日：2026/01/15
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

class CONTACT_MANAGER;

namespace PHYSICS_SOLVER_3D
{
	void PreStep(CONTACT_MANAGER& contact_manager, float dt);
	void WarmStart(CONTACT_MANAGER& contact_manager);
	void SolveVelocity(CONTACT_MANAGER& contact_manager, float dt, int velocity_iterations);
	void SolvePosition(CONTACT_MANAGER& contact_manager, float dt, int position_iterations);
}
