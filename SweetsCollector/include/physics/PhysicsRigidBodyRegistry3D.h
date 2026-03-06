//-----------------------------------------------
// PhysicsRigidBodyRegistry3D.h
// 制作日：2026/01/15
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <vector>
#include <cstdint>

class RIGID_BODY;
class CONTACT_MANAGER;

class PHYSICS_RIGID_BODY_REGISTRY_3D
{
private:
	// 登録済み剛体リスト
	std::vector<RIGID_BODY*> m_rigid_bodies;

	// 追加待ちリスト
	std::vector<RIGID_BODY*> m_pending_add;

	// 削除待ちリスト
	std::vector<RIGID_BODY*> m_pending_remove;

	// 次回割り当て剛体ID
	uint32_t m_next_rigid_body_id = 1;

public:
	void Reset();
	const std::vector<RIGID_BODY*>& GetRigidBodies() const { return m_rigid_bodies; }

	void Add(RIGID_BODY* body);
	void Remove(RIGID_BODY* body);

	void Synchronize(CONTACT_MANAGER& contact_manager);
};