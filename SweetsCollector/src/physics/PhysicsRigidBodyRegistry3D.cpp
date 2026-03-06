//-----------------------------------------------
// PhysicsRigidBodyRegistry3D.cpp
// 制作日：2026/01/15
// 制作者：安田晴人
//-----------------------------------------------
#include <algorithm>

#include "physics/PhysicsRigidBodyRegistry3D.h"
#include "physics/RigidBody.h"
#include "physics/ContactManager.h"


//-----------------------------------------------
// リセット
//-----------------------------------------------
void PHYSICS_RIGID_BODY_REGISTRY_3D::Reset()
{
	m_rigid_bodies.clear();
	m_pending_add.clear();
	m_pending_remove.clear();
	m_next_rigid_body_id = 1;
}

//-----------------------------------------------
// 剛体の追加
//-----------------------------------------------
void PHYSICS_RIGID_BODY_REGISTRY_3D::Add(RIGID_BODY* body)
{
	if (!body)
	{// nullチェック
		return;
	}
	
	if (std::find(m_rigid_bodies.begin(), m_rigid_bodies.end(), body) != m_rigid_bodies.end() || std::find(m_pending_add.begin(), m_pending_add.end(), body) != m_pending_add.end())
	{// すでに登録されている場合は追加しない
		return;
	}

	if (body->GetID() == 0)
	{// 未割り当ての場合はIDを割り当てる
		body->SetID(m_next_rigid_body_id++);
	}

	// 追加予定リストに登録
	m_pending_add.push_back(body);
}

//-----------------------------------------------
// 剛体の削除
//-----------------------------------------------
void PHYSICS_RIGID_BODY_REGISTRY_3D::Remove(RIGID_BODY* body)
{
	if (!body)
	{// nullチェック
		return;
	}
	
	if (std::find(m_rigid_bodies.begin(), m_rigid_bodies.end(), body) == m_rigid_bodies.end() && std::find(m_pending_add.begin(), m_pending_add.end(), body) == m_pending_add.end())
	{// 登録されていない場合は削除しない
		return;
	}

	// 削除予定リストに登録
	m_pending_remove.push_back(body);
}

//-----------------------------------------------
// 保留中の剛体の追加・削除を反映
//-----------------------------------------------
void PHYSICS_RIGID_BODY_REGISTRY_3D::Synchronize(CONTACT_MANAGER& contact_manager)
{
	// 追加
	for (RIGID_BODY* body : m_pending_add)
	{
		if (!body)
		{// nullチェック
			continue;
		}
		
		if (std::find(m_rigid_bodies.begin(), m_rigid_bodies.end(), body) == m_rigid_bodies.end())
		{// 未登録の場合のみ追加
			m_rigid_bodies.push_back(body);
		}
	}

	// クリア
	m_pending_add.clear();

	// 削除
	for (RIGID_BODY* body : m_pending_remove)
	{
		if (body)
		{
			auto it = std::find(m_rigid_bodies.begin(), m_rigid_bodies.end(), body);
			if (it != m_rigid_bodies.end())
			{// 登録されている場合のみ削除

				// 剛体の削除
				m_rigid_bodies.erase(it);

				// 剛体に関するコンタクトの削除
				contact_manager.RemoveContactsForRigidBody(body);
			}
		}
	}

	// クリア
	m_pending_remove.clear();
}