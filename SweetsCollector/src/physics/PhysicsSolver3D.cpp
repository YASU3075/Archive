//-----------------------------------------------
// PhysicsSolver3D.cpp
// 制作日：2026/01/15
// 制作者：安田晴人
//-----------------------------------------------
#include "physics/PhysicsSolver3D.h"

#include "Main.h"
#include "scene/Manager.h"
#include "renderer/Renderer.h"
#include "entity/GameObject.h"

#include "physics/RigidBody.h"
#include "physics/ContactManager.h"
#include "physics/PhysicsContactSolver3D.h"

//-----------------------------------------------
// 事前準備
//-----------------------------------------------
void PHYSICS_SOLVER_3D::PreStep(CONTACT_MANAGER& contact_manager, float dt)
{
	for (auto& contact : contact_manager.GetContacts())
	{
		if (contact.is_trigger)
		{// トリガーなら処理しない
			continue;
		}
		if (contact.rb_a == nullptr || contact.rb_b == nullptr)
		{// 剛体がセットされていないなら処理しない
			continue;
		}
		if (contact.points.empty())
		{// 接触点がないなら処理しない
			continue;
		}
		if (!contact.touched_substep)
		{// 今回のサブステップで接触していないなら処理しない
			continue;
		}

		PHYSICS_CONTACT_SOLVER_3D::PreStep(contact, dt);
	}
}

//-----------------------------------------------
// ウォームスタート
void PHYSICS_SOLVER_3D::WarmStart(CONTACT_MANAGER& contact_manager)
{
	contact_manager.WarmStart();
}

//-----------------------------------------------
// 速度解決
//-----------------------------------------------
void PHYSICS_SOLVER_3D::SolveVelocity(CONTACT_MANAGER& contact_manager, float dt, int velocity_iterations)
{
	for (int iter = 0; iter < velocity_iterations; iter++)
	{
		contact_manager.BeginSolverIteration(iter);

		for (auto& contact : contact_manager.GetContacts())
		{
			if (contact.is_trigger)
			{// トリガーなら処理しない
				continue;
			}
			if (contact.rb_a == nullptr || contact.rb_b == nullptr)
			{// 剛体がセットされていないなら処理しない
				continue;
			}
			if (contact.points.empty())
			{// 接触点がないなら処理しない
				continue;
			}
			if (!contact.touched_substep)
			{// 今回のサブステップで接触していないなら処理しない
				continue;
			}

			// 法線方向の解決
			PHYSICS_CONTACT_SOLVER_3D::SolveNormal(contact, dt);
			// 接線方向の解決（摩擦）
			PHYSICS_CONTACT_SOLVER_3D::SolveFriction(contact, dt);
		}
	}
}

//-----------------------------------------------
// 位置解決
//-----------------------------------------------
void PHYSICS_SOLVER_3D::SolvePosition(CONTACT_MANAGER& contact_manager, float dt, int position_iterations)
{
	const float slop = 0.01f;
	const float beta = 0.4f;

	for (int iter = 0; iter < position_iterations; ++iter)
	{
		for (auto& c : contact_manager.GetContacts())
		{
			if (!c.touched_substep) continue;
			if (c.is_trigger) continue;
			if (!c.rb_a || !c.rb_b) continue;
			if (c.points.empty()) continue;

			RIGID_BODY* a = c.rb_a;
			RIGID_BODY* b = c.rb_b;

			float inv_a = a->GetInvMass();
			float inv_b = b->GetInvMass();
			float inv_sum = inv_a + inv_b;
			if (inv_sum <= 0.0f)
			{
				continue;
			}

			TRANSFORM* ta = a->GetOwner()->GetTransform();
			TRANSFORM* tb = b->GetOwner()->GetTransform();

			VECTOR3 pos_a = ta->GetPos();
			VECTOR3 pos_b = tb->GetPos();

			// 各接触点の位置修正
			for (auto& p : c.points)
			{
				float C = p.penetration - slop;
				if (C <= 0.0f)
				{
					continue;
				}

				VECTOR3 n = p.normal; // A->B

				// 修正量の計算
				VECTOR3 corr = n * (beta * C / inv_sum);

				VECTOR3 new_a = pos_a - corr * inv_a;
				VECTOR3 new_b = pos_b + corr * inv_b;

				// 拘束条件の適用
				if (a->HasConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_POS_X)) new_a.x = pos_a.x;
				if (a->HasConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_POS_Y)) new_a.y = pos_a.y;
				if (a->HasConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_POS_Z)) new_a.z = pos_a.z;

				if (b->HasConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_POS_X)) new_b.x = pos_b.x;
				if (b->HasConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_POS_Y)) new_b.y = pos_b.y;
				if (b->HasConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_POS_Z)) new_b.z = pos_b.z;

				pos_a = new_a;
				pos_b = new_b;
			}

			ta->SetPos(pos_a);
			tb->SetPos(pos_b);
		}
	}
}