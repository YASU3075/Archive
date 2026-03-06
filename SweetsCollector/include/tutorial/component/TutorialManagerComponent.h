//-----------------------------------------------
// TutorialManagerComponent.h
// 制作日：2026/02/11
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "component/Component.h"
#include "tutorial/TutorialState.h"

class TUTORIAL_UI_MANAGER_COMPONENT;

class TUTORIAL_MANAGER_COMPONENT : public COMPONENT
{
private:
	TUTORIAL_UI_MANAGER_COMPONENT* m_tutorial_ui_manager_component = nullptr;
	TUTORIAL_STATE m_state = TUTORIAL_STATE::MOVE;

public:
	void Init() override;

	void EnterState(TUTORIAL_STATE state);
	void OnMoveCompleted();
	void OnWeightCompleted();
	void OnShootToGoalCompleted();

	void SetState(TUTORIAL_STATE state);
	TUTORIAL_STATE GetState() const { return m_state; }
};