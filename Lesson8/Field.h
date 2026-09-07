//-----------------------------------------------
// Field.h
// 制作日：2025/08/07
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "GameObject.h"

class Field : public GameObject
{
private:
	static constexpr int MAX_WIDTH = 20; // フィールドの幅
	static constexpr int HEIGHT = 24; // フィールドの高さ

	int m_current_width; // 現在のフィールドの幅

	char m_field[HEIGHT][MAX_WIDTH + 1] =
	{
		"##########          ",
		"#        #          ",
		"#        #          ",
		"#        #          ",
		"#        #          ",
		"#        #          ",
		"#        #          ",
		"#        #          ",
		"#        #          ",
		"#        #          ",
		"#        #          ",
		"#--------#          ",
		"#        #          ",
		"#        #          ",
		"#        #          ",
		"#        #          ",
		"#        #          ",
		"#        #          ",
		"#        #          ",
		"#        #          ",
		"#        #          ",
		"#        #          ",
		"#        #          ",
		"##########          "
	}; // フィールドの2次元配列

public:
	Field() : m_current_width(10) {}; // デフォルトコンストラクタ
	~Field() override = default; // デストラクタ

	void Update() override {} // フィールドの更新処理は特に必要ない
	void Draw() override;

	void SetCurrentWidth(int width)
	{
		if (width < 0 || width > MAX_WIDTH)
		{
			return; // 範囲外の場合は何もしない
		}
		m_current_width = width; // 現在の幅を設定

		for (int y = 0; y < HEIGHT; y++)
		{
			for (int x = 0; x < MAX_WIDTH; x++)
			{
				if (x < m_current_width)
				{
					if (y == 0 || y == HEIGHT - 1) // 上下の行は'#'で埋める
					{
						m_field[y][x] = '#';
					}
					else if (x == 0 || x == m_current_width - 1) // 左右の端も'#'で埋める
					{
						m_field[y][x] = '#';
					}
					else if (y == HEIGHT / 2 - 1 && x >= 1 && x < m_current_width - 1) // 中央部分は'-'で埋める
					{
						m_field[y][x] = '-';
					}
					else
					{
						m_field[y][x] = ' '; // 中央部分は空白で埋める
					}
				}
				else
				{
					m_field[y][x] = ' '; // それ以降は空白で埋める
				}
			}
			m_field[y][MAX_WIDTH] = '\0'; // 行末のヌル文字を設定
		}
	}

	int GetCurrentWidth() const { return m_current_width; } // 現在の幅を取得

	int GetCurrentMaxX() const
	{// 現在の最大X座標を取得
		return static_cast<int>(m_position.x) + m_current_width - 1;
	}
	int GetCurrntMinX() const
	{// 現在の最小X座標を取得
		return static_cast<int>(m_position.x) + 1;
	}
};