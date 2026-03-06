//-----------------------------------------------
// PhaseManagerComponent.h
// 制作日：2025/09/15
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "component/Component.h"

class PHASE_MANAGER_COMPONENT : public COMPONENT
{
public:
	enum class PHASE
	{
		PHASE_ONE,
		PHASE_TWO,
		PHASE_THREE,
	};

private:
	PHASE m_current_phase = PHASE::PHASE_ONE;
	float m_phase_duration = 30.0f; // 各フェーズの持続時間（秒）
	float m_time_in_current_phase = 0.0f; // 現在のフェーズにいる時間

public:
	// 初期化
	void Init() override;
	// 更新
	void Update() override;
	PHASE GetCurrentPhase() const { return m_current_phase; }
	void SetPhaseDuration(float duration) { m_phase_duration = duration; }
};
