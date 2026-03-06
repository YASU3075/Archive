//-----------------------------------------------
// ScoreManagerComponent.h
// 制作日：2025/12/28
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "component/Component.h"

class SCORE_MANAGER_COMPONENT : public COMPONENT
{
private:
	int m_score = 0; // スコア
	float m_current_display_score = 0; // 現在表示されているスコア
public:
	void Init() override;
	void Update() override;

	// スコアを加算する
	void AddScore(int points) { m_score += points; }
	// スコアを取得する
	int GetScore() const { return m_score; }

	int GetCurrentDisplayScore() const { return m_current_display_score; }
};