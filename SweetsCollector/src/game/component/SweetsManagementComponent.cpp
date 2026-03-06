//-----------------------------------------------
// SweetsManagementComponent.cpp
// 制作日：2025/12/28
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "scene/Manager.h"
#include "renderer/Renderer.h"
#include "game/component/SweetsManagementComponent.h"
#include "component/MovePlayer.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void SWEETS_MANAGEMENT_COMPONENT::Init()
{
	m_p_move_player = GetOwner()->GetComponent<MOVE_PLAYER>();
}

//-----------------------------------------------
// お菓子を追加する
//-----------------------------------------------
bool SWEETS_MANAGEMENT_COMPONENT::AddSweets(const SWEETS_DATA& data)
{
	if (m_current_sweets_weight + data.weight > MAX_SWEETS_WEIGHT)
	{
		// 最大重量を超える場合、追加できない
		return false;
	}
	m_sweets_queue.push(data);
	m_current_sweets_weight += data.weight;

	m_p_move_player->SetSpeed(m_p_move_player->GetMaxSpeed() * (static_cast<float>(MAX_SWEETS_WEIGHT - m_current_sweets_weight) / static_cast<float>(MAX_SWEETS_WEIGHT)));

	return true;
}

//-----------------------------------------------
// 先頭のお菓子を取り出す
//-----------------------------------------------
SWEETS_DATA SWEETS_MANAGEMENT_COMPONENT::PopSweets()
{
	if (m_sweets_queue.empty())
	{
		return SWEETS_DATA{};
	}
	SWEETS_DATA data = m_sweets_queue.front();
	m_sweets_queue.pop();

	m_current_sweets_weight -= data.weight;
	m_p_move_player->SetSpeed(m_p_move_player->GetMaxSpeed() * (static_cast<float>(MAX_SWEETS_WEIGHT - m_current_sweets_weight) / static_cast<float>(MAX_SWEETS_WEIGHT)));

	return data;
}