//-----------------------------------------------
// Enemy.cpp
// 制作日：2025/08/06
// 制作者：安田晴人
//-----------------------------------------------
#include "Enemy.h"
#include "BulletManager.h"
#include "DrawManager.h"
#include "Field.h"

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void Enemy::Update()
{
	m_last_position = m_position; // 前回の位置を保存

	m_position.x += m_speed * m_level * 0.1f; // 敵のレベルに応じて横方向に移動 (レベルが高いほど速く移動)
	if (m_position.x <= m_p_field->GetCurrntMinX() || m_position.x > m_p_field->GetCurrentMaxX())
	{
		m_position.x = m_last_position.x; // 画面外に出た場合は前回の位置に戻す
		m_speed = -m_speed; // 画面の端に到達したら移動方向を反転
	}

	

	// 敵のクールタイムを減少
	if (m_cool_time > 0.0f)
	{
		m_cool_time -= 0.1f; // クールタイムを減少 (ここでは仮に0.1秒ごとに減少)
	}
	else
	{
		// クールタイムが終了したら弾を発射
		if (m_p_bullet_manager)
		{
			m_p_bullet_manager->CreateBullet(m_position + Vector2(0, 1), '*', Vector2(0, 1.0f), 0.1f, false); // 弾を発射 (下方向に速度1.0fで発射)
			m_cool_time = 11.0f - static_cast<float>(m_level); // クールタイムをリセット
		}
	}
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void Enemy::Draw()
{
	m_p_draw_manager->Reserve(static_cast<int>(m_position.x), static_cast<int>(m_position.y), m_symbol, m_layer);

	if (static_cast<int>(m_position.x) == static_cast<int>(m_last_position.x) &&
		static_cast<int>(m_position.y) == static_cast<int>(m_last_position.y))
	{
		return; // 位置が変わっていない場合は何もしない
	}

	m_p_draw_manager->Reserve(static_cast<int>(m_last_position.x), static_cast<int>(m_last_position.y), ' ', m_layer);
}