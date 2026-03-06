//-----------------------------------------------
// TutorialManagerComponent.cpp
// 制作日：2026/02/11
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "tutorial/object/TutorialUIManager.h"
#include "tutorial/component/TutorialManagerComponent.h"
#include "tutorial/component/TutorialUIManagerComponent.h"
#include "component/UIAnimatorComponent.h"
#include "component/renderer/SpriteRendererComponent.h"
#include "renderer/Material.h"
#include "renderer/MaterialManager.h"
#include "object/Fade.h"
#include "component/SimpleFadeComponent.h"
#include "EngineTime.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void TUTORIAL_MANAGER_COMPONENT::Init()
{
	m_tutorial_ui_manager_component = Manager::GetScene()->GetGameObject<TUTORIAL_UI_MANAGER>()->GetComponent<TUTORIAL_UI_MANAGER_COMPONENT>();

	if (m_tutorial_ui_manager_component == nullptr)
	{// nullチェック
		assert(false);
		return;
	}

	// 最初のチュートリアルを表示
	EnterState(m_state);
}

//-----------------------------------------------
// ステート遷移処理
//-----------------------------------------------
void TUTORIAL_MANAGER_COMPONENT::EnterState(TUTORIAL_STATE state)
{
	m_state = state;
	switch (m_state)
	{
	case TUTORIAL_STATE::MOVE:
		m_tutorial_ui_manager_component->ShowStep(TUTORIAL_STATE::MOVE);
		break;
	case TUTORIAL_STATE::WEIGHT:
		m_tutorial_ui_manager_component->ShowStep(TUTORIAL_STATE::WEIGHT);
		break;
	case TUTORIAL_STATE::SHOOT_TO_GOAL:
		m_tutorial_ui_manager_component->ShowStep(TUTORIAL_STATE::SHOOT_TO_GOAL);
		break;
	default:
		assert(false);
		break;
	}
}

//-----------------------------------------------
// 移動完了時のコールバック
//-----------------------------------------------
void TUTORIAL_MANAGER_COMPONENT::OnMoveCompleted()
{
	if (m_state != TUTORIAL_STATE::MOVE)
	{
		return;
	}

	m_tutorial_ui_manager_component->HideStep();
	EnterState(TUTORIAL_STATE::WEIGHT);
}

//-----------------------------------------------
// 重さ説明完了時のコールバック
//-----------------------------------------------
void TUTORIAL_MANAGER_COMPONENT::OnWeightCompleted()
{
	if (m_state != TUTORIAL_STATE::WEIGHT)
	{
		return;
	}
	m_tutorial_ui_manager_component->HideStep();
	EnterState(TUTORIAL_STATE::SHOOT_TO_GOAL);
}

//-----------------------------------------------
// ゴールシュート完了時のコールバック
//-----------------------------------------------
void TUTORIAL_MANAGER_COMPONENT::OnShootToGoalCompleted()
{
	if (m_state != TUTORIAL_STATE::SHOOT_TO_GOAL)
	{
		return;
	}
	m_tutorial_ui_manager_component->HideStep();

	Manager::GetScene()->GetGameObject<FADE>()->GetComponent<SIMPLE_FADE_COMPONENT>()->StartFadeOut();
}