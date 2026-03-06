//-----------------------------------------------
// SweetsShootComponent.h
// 制作日：2026/01/04
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "game/SweetsData.h"
#include "component/Component.h"

class SWEETS_SHOOT_COMPONENT : public COMPONENT
{
private:
	class SWEETS_MANAGEMENT_COMPONENT* m_p_sweets_management = nullptr; // お菓子管理コンポーネントへのポインタ

	const float m_shoot_power = 10.0f; // お菓子を撃つ力
	const float m_shoot_offset_length = 5.0f; // お菓子を撃つオフセットの長さ

	float m_shoot_timer = 0.0f; // お菓子を撃つタイマー
	const float m_shoot_cooldown = 1.0f; // お菓子を撃つクールダウン時間
public:
	// 初期化処理
	void Init() override;
	// 更新処理
	void Update() override;
	// お菓子を撃つ
	void ShootSweets(SWEETS_DATA& data);
};
