//-----------------------------------------------
// Enemy.h
// 制作日：2025/08/06
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "GameObject.h"
#include "Vector2.h"


class Enemy : public GameObject
{
private:
	int m_level; // 敵のレベル
	float m_cool_time; // クールタイム
	float m_speed;
	class BulletManager* m_p_bullet_manager;
	class Field* m_p_field; // 前方宣言
public:
	Enemy() : m_level(1), m_cool_time(0.0f), m_speed(1.0f), m_p_bullet_manager(nullptr), m_p_field(nullptr) {}; // デフォルトコンストラクタ
	Enemy(const Vector2& position, char symbol, int level = 1, float cool_time = 1.0f)
		: GameObject(position, symbol), m_level(level), m_cool_time(cool_time), m_p_bullet_manager(nullptr) {} // コンストラクタ

	virtual ~Enemy() override {} // デストラクタ

	void SetBulletManager(BulletManager* bullet_manager) { m_p_bullet_manager = bullet_manager; } // 弾の管理クラスを設定

	// 更新処理
	virtual void Update() override;

	// 描画処理
	void Draw() override;

	void SetLevel(int level) 
	{ 
		m_level = level; 
		if (m_level > 10) // レベルが10を超える場合は最大値を10に制限
		{
			m_level = 10;
		}
	}

	// 敵のレベルを取得
	int GetLevel() const { return m_level; }

	void SetField(Field* field) { m_p_field = field; } // フィールドを設定
};