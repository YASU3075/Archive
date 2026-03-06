//-----------------------------------------------
// TutorialWeightUIComponent.cpp
// 制作日：206/02/12
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "input/Input.h"
#include "input/GamePad.h"
#include "tutorial/component/TutorialWeightUIComponent.h"

#include "tutorial/object/TutorialManager.h"
#include "tutorial/component/TutorialManagerComponent.h"
#include "EngineTime.h"

//------------------------------------------------
// 初期化処理
//------------------------------------------------
void TUTORIAL_WEIGHT_UI_COMPONENT::Init()
{
	m_timer = 0.0f;
}

//------------------------------------------------
// 更新処理
//------------------------------------------------
void TUTORIAL_WEIGHT_UI_COMPONENT::Update()
{
	if (Input::GetKeyTrigger(VK_RETURN) || INPUT_SYSTEM::Instance().GetGamePad(0).Pressed(PAD_BUTTON::A))
	{
		// チュートリアルマネージャーを取得
		TUTORIAL_MANAGER_COMPONENT* tutorial_manager = Manager::GetScene()->GetGameObject<TUTORIAL_MANAGER>()->GetComponent<TUTORIAL_MANAGER_COMPONENT>();
		if (tutorial_manager)
		{
			tutorial_manager->OnWeightCompleted();
		}
	}
}