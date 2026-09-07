//-----------------------------------------------
// Score.cpp
// 制作日：2025/08/07
// 制作者：安田晴人
//-----------------------------------------------
#include "Score.h"
#include "Field.h"

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void Score::Draw()
{
	printf("\x1b[%d;%dHScore:%d ", Y_POS, m_field->GetCurrentMaxX() + 1, m_display_score); // スコアを画面の左上に表示

	for (const auto& score_pair : m_score_by_y) // スコアの履歴を描画
	{
		printf("\x1b[%d;%dH<=%d", score_pair.first + 1, m_field->GetCurrentMaxX() + 1, score_pair.second);
	}
}
