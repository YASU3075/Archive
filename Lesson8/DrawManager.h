//-----------------------------------------------
// DrawManager.h
// 制作日：2025/08/07
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <stdio.h>

struct DrawCell
{
	char c = ' ';
	int layer = 0;
	bool dirty = true;
};

class DrawManager
{
private:
	static constexpr int WIDTH = 80; // 描画領域の幅
	static constexpr int HEIGHT = 24; // 描画領域の高さ

	DrawCell m_current[HEIGHT][WIDTH]; // 現在の描画内容
	DrawCell m_previous[HEIGHT][WIDTH]; // 前回の描画内容
public:
	DrawManager()
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				m_current[y][x].c = ' '; // 初期化
				m_current[y][x].layer = 0; // レイヤー初期化
				m_current[y][x].dirty = true; // 描画が必要であることを示す
				m_previous[y][x] = m_current[y][x]; // 前回の内容も初期化
			}
		}
	}

	~DrawManager() = default; // デストラクタ

	void Reserve(int x, int y, char c, int layer = 0)
	{
		x -= 1; // 1オリジンから0オリジンに変換

		if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		{// 範囲外チェック
			return;
		}
		if (m_current[y][x].c == c )
		{
			return; // 既に同じ内容が予約されている場合は何もしない
		}
		if (m_current[y][x].layer > layer)
		{// 既に予約されている内容のレイヤーが現在のレイヤーよりも高い場合は何もしない
			return;
		}
		m_current[y][x].c = c; // 描画内容を設定
		m_current[y][x].layer = layer; // レイヤーを設定
		m_current[y][x].dirty = true; // 描画が必要であることを示す
	}
	
	void Draw()
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				if (m_current[y][x].dirty) // 描画が必要なセルのみ描画
				{
					if (m_current[y][x].c != m_previous[y][x].c) // 前回の内容と異なる場合のみ描画
					{
						printf("\x1b[%d;%dH%c", y + 1, x + 1, m_current[y][x].c);
						m_previous[y][x] = m_current[y][x]; // 前回の内容を更新
					}
					m_current[y][x].dirty = false; // 描画後はdirtyフラグをリセット
				}
			}
		}
	}
};