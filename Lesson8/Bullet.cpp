//-----------------------------------------------
// Bullet.cpp
// 制作日：2025/08/07
// 制作者：安田晴人
//-----------------------------------------------
#include "Bullet.h"
#include "DrawManager.h"

void Bullet::Update()
{
	m_last_position = m_position; // 前回の位置を保存
	m_position += (m_velocity * m_speed); // 新しい位置を計算
	if (m_position.y < 12 && m_is_player_bullet) // プレイヤーの弾が画面上部に到達した場合
	{
		m_position.y = 12; // 画面上部に固定
	}
	else if (m_position.y >= 24 && !m_is_player_bullet) // 敵の弾が画面下部に到達した場合
	{
		Destroy();
	}

	if (m_position.x < 0 || m_position.y < 0 || m_position.x >= 80 || m_position.y >= 24) // 画面外に出た場合
	{
		Destroy(); // 弾を破棄
	}
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void Bullet::Draw()
{
	m_p_draw_manager->Reserve(static_cast<int>(m_position.x), static_cast<int>(m_position.y), m_symbol, m_layer);

	if (static_cast<int>(m_position.x) == static_cast<int>(m_last_position.x) &&
		static_cast<int>(m_position.y) == static_cast<int>(m_last_position.y))
	{
		return; // 位置が変わっていない場合は何もしない
	}

	m_p_draw_manager->Reserve(static_cast<int>(m_last_position.x), static_cast<int>(m_last_position.y), ' ', m_layer);
}