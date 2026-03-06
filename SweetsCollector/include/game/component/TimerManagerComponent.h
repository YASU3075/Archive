//-----------------------------------------------
// TimerManagerComponent.h
// 制作日：2026/01/10
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "component/Component.h"

class TIMER_MANAGER_COMPONENT : public COMPONENT
{
private:
	double m_elapsed_time = 0.0; // 経過時間
	double m_time_limit = 1.0;   // タイムリミット
public:
	void Init() override;
	void Update() override;
	// タイムリミットを設定する
	void SetTimeLimit(double time_limit) { m_time_limit = time_limit; }
	// 経過時間を取得する
	double GetElapsedTime() const { return m_elapsed_time; }
	// タイムリミットを取得する
	double GetTimeLimit() const { return m_time_limit; }
	// タイムアップしているか確認する
	bool IsTimeUp() const { return m_elapsed_time >= m_time_limit; }
};
