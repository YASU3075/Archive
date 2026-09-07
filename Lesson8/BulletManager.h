//-----------------------------------------------
// BulletManager.h
// 制作日：2025/08/05
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <vector>
#include "PoolAllocator.h"
#include "Bullet.h"

class BulletManager
{
private:
	static constexpr size_t MAX_BULLETS = 100; // 最大弾数
	PoolAllocator<Bullet, MAX_BULLETS> m_bullet_pool; // 弾のプールアロケータ
	class DrawManager* m_p_draw_manager; // 前方宣言
public:
	BulletManager() : m_bullet_pool(), m_p_draw_manager(nullptr) {} // コンストラクタ

	~BulletManager() = default;

	// 弾を生成
	Bullet* CreateBullet(const Vector2& position, char symbol, const Vector2& velocity, float speed = 1.0f, bool is_player = true)
	{
		Bullet* bullet = m_bullet_pool.Alloc();
		if (bullet)
		{
			bullet->SetPosition(position);
			bullet->SetSymbol(symbol);
			bullet->SetVelocity(velocity);
			bullet->SetSpeed(speed); // 弾の速度を設定
			bullet->SetIsPlayerBullet(is_player); // プレイヤーの弾かどうかを設定
			bullet->SetDrawManager(m_p_draw_manager); // 描画マネージャーを設定
		}
		return bullet;
	}

	// 弾を解放
	void ReleaseBullet(Bullet* bullet)
	{
		m_bullet_pool.Free(bullet);
	}

	// 弾の更新処理
	void UpdateBullets()
	{
		m_bullet_pool.ForEach([](Bullet& bullet)
		{
			bullet.Update(); // 弾の更新処理
		});
	}

	void DestroyBullets()
	{
		std::vector<Bullet*> bullets_to_destroy; // 破棄する弾のリスト

		m_bullet_pool.ForEach([&bullets_to_destroy](Bullet& bullet)
			{
				if (bullet.IsDestroyed()) // 弾が破棄されている場合
				{
					bullets_to_destroy.push_back(&bullet); // 破棄する弾のリストに追加
				}
			});

		for (Bullet* bullet : bullets_to_destroy)
		{
			printf("\x1b[%d;%dH ", static_cast<int>(bullet->GetPosition().y), static_cast<int>(bullet->GetPosition().x)); // 前の位置を空白で消す
			m_bullet_pool.Free(bullet); // 弾を解放
		}
	}

	// 弾の描画処理
	void DrawBullets()
	{
		m_bullet_pool.ForEach([](Bullet& bullet)
			{
				bullet.Draw(); // 弾の描画処理
			});
	}

	void SetDrawManager(class DrawManager* draw_manager)
	{
		m_p_draw_manager = draw_manager;
	}

	// 弾のプールを取得
	PoolAllocator<Bullet, MAX_BULLETS>& GetBulletPool() { return m_bullet_pool; }
};