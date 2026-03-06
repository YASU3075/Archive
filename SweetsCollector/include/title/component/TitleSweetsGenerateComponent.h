//-----------------------------------------------
// TitleSweetsGenerateComponent.h
// 制作日：2026/01/29
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "component/Component.h"

class TITLE_SWEETS_GENERATE_COMPONENT : public COMPONENT
{
private:
	const int MAX_SWEETS_NUM = 30;	//最大スイーツ数
	int m_current_sweets_num = 0;	//現在のスイーツ数
	float m_generate_interval = 2.0f;	//スイーツ生成間隔
	float m_timer = 0.0f;				//タイマー

	const float X_POS_RANGE = 3.0f;	//X座標範囲
public:
	void Init() override;
	void Update() override;
};