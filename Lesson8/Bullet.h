//-----------------------------------------------
// Bullet.h
// 制作日：2025/08/05
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "GameObject.h"
#include "Vector2.h"

class Bullet : public GameObject
{
private:
	Vector2 m_velocity; // 弾の速度
	float m_speed; // 弾の移動速度
	bool m_is_player_bullet; // プレイヤーの弾かどうか
public:
	Bullet() : GameObject(), m_velocity(0, 0), m_speed(0.01f), m_is_player_bullet(true) {} // デフォルトコンストラクタ
	Bullet(const Vector2& position, char symbol, const Vector2& velocity, float speed = 1.0f, bool is_player = true)
		: GameObject(position, symbol), m_velocity(velocity), m_speed(speed), m_is_player_bullet(is_player) {} // コンストラクタ

	virtual ~Bullet() override {} // デストラクタ

	// 弾の速度を設定
	void SetVelocity(const Vector2& velocity) { m_velocity = velocity; }

	void SetSpeed(float speed) { m_speed = speed; } // 弾の速度を設定

	void SetIsPlayerBullet(bool is_player) { m_is_player_bullet = is_player; } // プレイヤーの弾かどうかを設定

	// 弾の速度を取得
	Vector2 GetVelocity() const { return m_velocity; }

	bool IsPlayerBullet() const { return m_is_player_bullet; } // プレイヤーの弾かどうかを取得

	// 弾の更新処理
	void Update() override;

	void Draw() override;
};