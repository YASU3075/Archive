//-----------------------------------------------
// Score.h
// 制作日：2025/08/07
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <stdio.h>
#include <vector>

#include "GameObject.h"


class Score : public GameObject
{
private:
	const int X_POS = 20; // スコア表示のX座標
	const int Y_POS = 5; // スコア表示のY座標
	int m_score; // スコア
	int m_display_score; // 表示するスコア
	class Field* m_field; // フィールドへのポインタ

	const std::vector<std::pair<int, int>> m_score_by_y
	{
		{12, 0},
		{13, 5},
		{14, 10},
		{15, 20},
		{16, 30},
		{17, 50},
		{18, 100},
		{19, 500},
		{20, -100},
	}; // スコアの履歴

public:
	Score() : m_score(0), m_display_score(0), m_field(nullptr) {} // コンストラクタ

	// スコアを取得
	int GetScore() const { return m_score; }

	// スコアを加算
	void AddScoreByY(int y)
	{
		for (const auto& score_pair : m_score_by_y)
		{
			if (score_pair.first == y)
			{
				if (y == 20) // y座標が20の場合はスコアを減算
				{
					m_score += score_pair.second; // y座標に対応するスコアを減算
				}
				else // その他のy座標の場合はスコアを加算
				{
					m_score += score_pair.second; // y座標に対応するスコアを加算
				}
				if (m_score < 0) // スコアが負の値にならないように制限
				{
					m_score = 0;
				}
				return;
			}
		}
	}

	// スコアをリセット
	void Reset()
	{
		m_score = 0;
	}

	void Update()
	{
		if (m_display_score != m_score)
		{
			if (m_display_score < m_score)
			{
				m_display_score += 5; // スコアを5ずつ増やす
				if (m_display_score > m_score)
				{
					m_display_score = m_score; // 最大スコアを超えないようにする
				}
			}
			else if (m_display_score > m_score)
			{
				m_display_score -= 5; // スコアを5ずつ減らす
				if (m_display_score < m_score)
				{
					m_display_score = m_score; // 最小スコアを超えないようにする
				}
			}
		}
	}

	// スコアを描画
	void Draw() override;

	void SetField(Field* field)
	{
		m_field = field; // フィールドを設定
	}
};
