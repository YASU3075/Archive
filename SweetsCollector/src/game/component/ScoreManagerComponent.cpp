//-----------------------------------------------
// ScoreManagerComponent.cpp
// 制作日：2026/01/01
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "game/component/ScoreManagerComponent.h"

//------------------------------------------------
// 初期化処理
//------------------------------------------------
void SCORE_MANAGER_COMPONENT::Init()
{
	m_score = 0;
	m_current_display_score = 0;
}

//------------------------------------------------
// 更新処理
//------------------------------------------------
void SCORE_MANAGER_COMPONENT::Update()
{
	// スコアの表示を滑らかに更新
	if (m_current_display_score < m_score)
	{
		m_current_display_score += (m_score - m_current_display_score) * 0.01f + 0.9f;
		if (m_current_display_score > m_score)
		{
			m_current_display_score = m_score;
		}
	}
	else if (m_current_display_score > m_score)
	{
		m_current_display_score -= (m_current_display_score - m_score) * 0.01f + 0.9f;
		if (m_current_display_score < m_score)
		{
			m_current_display_score = m_score;
		}
	}
	
}