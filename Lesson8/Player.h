//-----------------------------------------------
// Player.h
// 制作日：2025/08/05
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "GameObject.h"
#include "Vector2.h"

class Player : public GameObject
{
private:
	class BulletManager* m_p_bullet_manager; // 前方宣言
	Vector2 m_velocity; // プレイヤーの速度
	float m_speed; // プレイヤーの移動速度
	class Field* m_p_field; // 前方宣言
public:
	Player() : m_p_bullet_manager(nullptr), m_speed(1.0f), m_p_field(nullptr) {}; // デフォルトコンストラクタ
	Player(const Vector2& position, char symbol, float speed = 1.0f) : GameObject(position, symbol), m_speed(speed), m_velocity(0, 0), m_p_bullet_manager(nullptr) {} // コンストラクタ

	virtual ~Player() override {} // デストラクタ

	// プレイヤーの速度を設定
	void SetVelocity(const Vector2& velocity) { m_velocity = velocity; }

	// プレイヤーの速度を取得
	Vector2 GetVelocity() const { return m_velocity; }

	// プレイヤーの更新処理
	void Update() override;

	// プレイヤーの描画処理
	void Draw() override;

	void SetBulletManager(BulletManager* bullet_manager) { m_p_bullet_manager = bullet_manager; } // 弾の管理クラスを設定
	void SetField(Field* field) { m_p_field = field; } // フィールドを設定
};