//-----------------------------------------------
// ResultSweetsSpawnComponent.h
// 制作日：2026/02/07
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "component/Component.h"

class RESULT_SWEETS_SPAWN_COMPONENT : public COMPONENT
{
private:
	float m_spawn_timer = 0.0f;
	float m_spawn_interval = 0.5f;
	int m_max_sweets = 20;
	int m_current_sweets = 0;
public:
	void Init() override;
	void Update() override;
	void SetSpawnInterval(float interval) { m_spawn_interval = interval; }
	void SetMaxSweets(int max_sweets) { m_max_sweets = max_sweets; }
};