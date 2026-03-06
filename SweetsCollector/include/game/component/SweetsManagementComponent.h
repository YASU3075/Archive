//-----------------------------------------------
// SweetsManagementComponent.h
// 制作日：2025/12/28
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <queue>
#include "game/SweetsData.h"
#include "component/Component.h"

class MOVE_PLAYER;

class SWEETS_MANAGEMENT_COMPONENT : public COMPONENT
{
private:
	const int MAX_SWEETS_WEIGHT = 100; // 最大お菓子重量
	int m_current_sweets_weight = 0; // 現在のお菓子重量
	std::queue<SWEETS_DATA> m_sweets_queue; // お菓子キュー

	MOVE_PLAYER* m_p_move_player = nullptr;
public:
	// 初期化処理
	void Init() override;

	// お菓子を追加する
	bool AddSweets(const SWEETS_DATA& data);

	// お菓子を取り出す
	const std::queue<SWEETS_DATA>& GetSweetsQueue() const { return m_sweets_queue; }

	// 先頭のお菓子を取り出す
	SWEETS_DATA PopSweets();

	// 現在のお菓子重量を取得する
	int GetCurrentSweetsWeight() const { return m_current_sweets_weight; }
};