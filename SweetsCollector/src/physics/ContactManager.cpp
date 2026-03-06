//-----------------------------------------------
// ContactManager.cpp
// 制作日：2025/07/20
// 制作者：安田晴人
//-----------------------------------------------
#include "physics/RigidBody.h"
#include "physics/Collider3D.h"
#include "physics/ContactManager.h"
#include "physics/PhysicsContactEvent3D.h"

#define FSS() DbgFrame(), DbgSub(), DbgSteps()

//-----------------------------------------------
// ペアをID順にソート
//-----------------------------------------------
void CONTACT_MANAGER::SortPair(RIGID_BODY*& a, RIGID_BODY*& b, bool& reversed)
{
	reversed = false;
	if (a->GetID() > b->GetID())
	{
		std::swap(a, b);
		reversed = true;
	}
}

//-----------------------------------------------
// トリガーかどうかを計算
//-----------------------------------------------
bool CONTACT_MANAGER::ComputeIsTrigger(RIGID_BODY* a, RIGID_BODY* b)
{
	auto* col_a = a ? a->m_collider : nullptr;
	auto* col_b = b ? b->m_collider : nullptr;
	return (col_a && col_a->IsTrigger()) || (col_b && col_b->IsTrigger());
}

//-----------------------------------------------
// サブステップ開始処理
//-----------------------------------------------
void CONTACT_MANAGER::BeginSubstep()
{
	for (auto& contact : m_contacts)
	{
		contact.touched_substep = false;
	}
}

//-----------------------------------------------
// 
//-----------------------------------------------
void CONTACT_MANAGER::BeginFrame()
{
	for (auto& contact : m_contacts)
	{ 
		contact.was_touching = contact.touched_frame;
		contact.touched_frame = false; 
	}
}

//-----------------------------------------------
// フレーム終了処理
//-----------------------------------------------
void CONTACT_MANAGER::EndFrame()
{
	for (auto it = m_contacts.begin(); it != m_contacts.end(); )
	{
		CONTACT_3D& c = *it;

		const bool now = c.touched_frame;
		const bool prev = c.was_touching;

		// イベントのディスパッチ
		PHYSICS_CONTACT_EVENT_3D::Dispatch(c, now, prev);

		if (now)
		{// 接触継続
			it++;
		}
		else
		{// 非接触
			it = m_contacts.erase(it);
		}
	}
}


//-----------------------------------------------
// 全コンタクトをクリア
//-----------------------------------------------
void CONTACT_MANAGER::ClearContacts()
{
	m_contacts.clear();
}

//-----------------------------------------------
// 剛体に関するコンタクトの削除
//-----------------------------------------------
void CONTACT_MANAGER::RemoveContactsForRigidBody(RIGID_BODY* rb)
{
	m_contacts.erase(
		std::remove_if(m_contacts.begin(), m_contacts.end(),
			[rb](const CONTACT_3D& contact)
			{
				return contact.rb_a == rb || contact.rb_b == rb;
			}),
		m_contacts.end());
}

//-----------------------------------------------
// ウォームスタート（累積インパルスのキャッシュなど）
//-----------------------------------------------
void CONTACT_MANAGER::WarmStart()
{
	return;

	/*for (auto& contact : m_contacts)
	{
		RIGID_BODY* a = contact.rb_a;
		RIGID_BODY* b = contact.rb_b;
		if (!a || !b) continue;
		if (contact.is_trigger) continue;

		for (auto& point : contact.points)
		{
			float jn = point.accumulated_normal_impulse;
			float jt = point.accumulated_tangent_impulse;
			jt = 0.0f;

			if (jn == 0.0f && jt == 0.0f) continue;

			VECTOR3 n = point.normal;
			VECTOR3 t = point.cached_tangent;

			VECTOR3 impulse = n * jn;
			if (t.LengthSquared() > 1e-8f)
			{
				impulse += t * jt;
			}

			VECTOR3 ra = point.pos - a->GetOwner()->GetTransform()->GetPos();
			VECTOR3 rb = point.pos - b->GetOwner()->GetTransform()->GetPos();

			float inv_mass_a = a->GetInvMass();
			float inv_mass_b = b->GetInvMass();
			const MATRIX3x3& inv_ia = a->GetInvInertiaWorld();
			const MATRIX3x3& inv_ib = b->GetInvInertiaWorld();

			if (inv_mass_a > 0.0f)
			{
				a->SetLinearVelocity(a->GetLinearVelocity() - impulse * inv_mass_a);
				a->SetAngularVelocity(
					a->GetAngularVelocity()
					+ inv_ia.MultiplyVector(VECTOR3::Cross(ra, -impulse)));
			}
			if (inv_mass_b > 0.0f)
			{
				b->SetLinearVelocity(b->GetLinearVelocity() + impulse * inv_mass_b);
				b->SetAngularVelocity(
					b->GetAngularVelocity()
					+ inv_ib.MultiplyVector(VECTOR3::Cross(rb, impulse)));
			}
		}
	}*/
}

//-----------------------------------------------
// 指定剛体が接触中かどうか
//-----------------------------------------------
bool CONTACT_MANAGER::IsBodyTouching(RIGID_BODY* rb) const
{
	for (auto& c : m_contacts)
	{
		if (!c.touched_frame || c.is_trigger)
		{
			continue;
		}

		if (!c.rb_a || !c.rb_b)
		{
			continue;
		}

		if (c.rb_a == rb || c.rb_b == rb)
		{
			return true;
		}
	}
	return false;
}