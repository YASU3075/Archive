//-----------------------------------------------
// ContactManager.h
// 制作日：2025/07/20
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <vector>
#include "math/Vector3.h"
#include "physics/PhysicsContact3D.h"


class RIGID_BODY;

class CONTACT_MANAGER
{
private:
	std::vector<CONTACT_3D> m_contacts;
	std::vector<std::pair<RIGID_BODY*, RIGID_BODY*>> m_potential_pairs;

	int m_dbg_frame_id = 0;
	int m_dbg_substep_id = 0;
	int m_dbg_step = 0;
	int m_dbg_solver_iter = 0;
private:
	
	void SortPair(RIGID_BODY*& a, RIGID_BODY*& b, bool& reversed);
	bool ComputeIsTrigger(RIGID_BODY* a, RIGID_BODY* b);

public:
	CONTACT_MANAGER() = default;
	~CONTACT_MANAGER() = default;

	void BeginSolverIteration(int iter) { m_dbg_solver_iter = iter; }
	int DbgSolverIter() const { return m_dbg_solver_iter; }

	void SetStepInfo(int frame_id, int substep_id, int step)
	{
		m_dbg_frame_id = frame_id;
		m_dbg_substep_id = substep_id;
		m_dbg_step = step;
	}

	int DbgFrame() const { return m_dbg_frame_id; }
	int DbgSub() const { return m_dbg_substep_id; }
	int DbgSteps() const { return m_dbg_step; }

	void BeginSubstep();

	// フレーム開始・終了処理
	void BeginFrame();
	void EndFrame();

	// 全コンタクトをクリア
	void ClearContacts();

	// コンタクトの取得
	const std::vector<CONTACT_3D>& GetContacts() const { return m_contacts; }

	// 潜在的な衝突ペアのクリア
	void ClearPotentialPairs() { m_potential_pairs.clear(); }

	// 潜在的な衝突ペアの追加
	void AddPotentialPair(RIGID_BODY* a, RIGID_BODY* b) { m_potential_pairs.emplace_back(a, b); }

	// 潜在的な衝突ペアの取得
	const std::vector<std::pair<RIGID_BODY*, RIGID_BODY*>>& GetPotentialPairs() const { return m_potential_pairs; }

	// 剛体に関するコンタクトの削除
	void RemoveContactsForRigidBody(RIGID_BODY* rb);

	// ウォームスタート
	void WarmStart();

	// 剛体が他の剛体と接触しているかどうか
	bool IsBodyTouching(RIGID_BODY* rb) const;

	std::vector<CONTACT_3D>& GetContacts() { return m_contacts; }
};
