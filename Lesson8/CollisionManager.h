//-----------------------------------------------
// CollisionManager.h
// 制作日：2025/08/07
// 制作者：安田晴人
//-----------------------------------------------
#pragma once


class CollisionManager
{
private:
	class BulletManager* m_bullet_manager; // 弾の管理クラスへのポインタ
	class Score* m_score;

	// 弾同士の衝突判定
	void CheckBulletCollisions();
public:
	CollisionManager() : m_bullet_manager(nullptr), m_score(nullptr) {} // コンストラクタ

	void Update();

	void SetBulletManager(class BulletManager* bullet_manager) { m_bullet_manager = bullet_manager; } // 弾の管理クラスを設定
	void SetScore(class Score* score) { m_score = score; }
	
};