//-----------------------------------------------
// TimerManagetComponent.cpp
// 制作日：2026/01/10
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "game/component/TimerManagerComponent.h"
#include "EngineTime.h"
#include "game/object/TimeUpUI.h"
#include "game/component/TimeUpUIComponent.h"
#include "object/Player.h"
#include "component/MovePlayer.h"

//------------------------------------------------
// 初期化処理
//------------------------------------------------
void TIMER_MANAGER_COMPONENT::Init()
{
	m_elapsed_time = 0.0;
	m_time_limit = 99.0;
}

//------------------------------------------------
// 更新処理
//------------------------------------------------
void TIMER_MANAGER_COMPONENT::Update()
{
	// 経過時間を更新
	m_elapsed_time += TIME::DeltaTime();

	if (m_elapsed_time >= m_time_limit)
	{
		m_elapsed_time = m_time_limit;

		// タイムアップ処理
		TIME_UP_UI* p_time_up_ui = Manager::GetScene()->GetGameObject<TIME_UP_UI>();
		if (p_time_up_ui)
		{
			TIME_UP_UI_COMPONENT* p_time_up_ui_component = p_time_up_ui->GetComponent<TIME_UP_UI_COMPONENT>();
			if (p_time_up_ui_component)
			{
				p_time_up_ui_component->SetActive(true);
			}
		}

		// キー移動制御
		PLAYER* p_player = Manager::GetScene()->GetGameObject<PLAYER>();
		if (p_player)
		{
			MOVE_PLAYER* p_move_player = p_player->GetComponent<MOVE_PLAYER>();
			if (p_move_player)
			{
				p_move_player->SetActive(false);
			}
		}
	}
}