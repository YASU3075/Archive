//-----------------------------------------------
// GoalComponent.h
// 制作日：2025/12/28
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "component/Component.h"
#include "physics/ICollisionListener.h"

class GOAL_COMPONENT : public COMPONENT, public I_COLLISION_LISTENER
{
private:
	class SCORE_MANAGER_COMPONENT* m_p_score_manager = nullptr; // スコアマネージャー
public:
	// 初期化処理
	void Init() override;
	// 衝突開始時の処理
	virtual void OnCollisionEnter(COLLIDER_3D* other) override;
};