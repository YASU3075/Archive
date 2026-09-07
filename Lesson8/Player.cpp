//-----------------------------------------------
// Player.cpp
// 制作日：2025/08/06
// 制作者：安田晴人
//-----------------------------------------------
#include <conio.h>
#include "Player.h"
#include "BulletManager.h"
#include "DrawManager.h"
#include "Field.h"

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void Player::Update()
{
	m_last_position = m_position; // 前回の位置を保存

	int i = 0; // キー入力があるか確認
	if (_kbhit())
	{
		i = _getch(); // キー入力を取得
	}
	else
	{
		return; // キー入力がない場合は何もしない
	}
	switch (i)
	{
	case 'a':
	case '1':
		m_velocity = Vector2(-m_speed, 0); // 左
		break;
	case 'd':
	case '3':
		m_velocity = Vector2(m_speed, 0); // 右
		break;
	case ' ':
	{
		m_p_bullet_manager->CreateBullet(m_position + Vector2(0, -1), '+', Vector2(0, -1.0f), 0.1f); // 弾を発射 (上方向に速度1.0fで発射)
	}
	default:
		break;
	}

	m_position += m_velocity; // 位置を更新

	if (m_position.x < m_p_field->GetCurrntMinX() || m_position.x >= m_p_field->GetCurrentMaxX())
	{
		m_position.x = m_last_position.x; // 画面外に出た場合は前回の位置に戻す
	}

	m_velocity = Vector2(0, 0); // 速度をリセット
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void Player::Draw()
{
	m_p_draw_manager->Reserve(static_cast<int>(m_position.x), static_cast<int>(m_position.y), m_symbol, m_layer);

	if (static_cast<int>(m_position.x) == static_cast<int>(m_last_position.x) &&
		static_cast<int>(m_position.y) == static_cast<int>(m_last_position.y))
	{
		return; // 位置が変わっていない場合は何もしない
	}

	m_p_draw_manager->Reserve(static_cast<int>(m_last_position.x), static_cast<int>(m_last_position.y), ' ', m_layer);
}

