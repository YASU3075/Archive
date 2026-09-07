//-----------------------------------------------
// Field.cpp
// 制作日：2025/08/07
// 制作者：安田晴人
//-----------------------------------------------
#include "Field.h"
#include "DrawManager.h"

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void Field::Draw()
{
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < MAX_WIDTH; x++)
		{
			m_p_draw_manager->Reserve(static_cast<int>(m_position.x) + x, static_cast<int>(m_position.y) + y, m_field[y][x], m_layer);
		}
	}
}
