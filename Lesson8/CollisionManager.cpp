//-----------------------------------------------
// CollisionManager.cpp
// 制作日：2025/08/07
// 制作者：安田晴人
//-----------------------------------------------
#include <vector>
#include <mutex>
#include "CollisionManager.h"
#include "BulletManager.h"
#include "PoolAllocator.h"
#include "Score.h"

//------------------------------------------------
// 弾同士の衝突判定
//------------------------------------------------
void CollisionManager::CheckBulletCollisions()
{
	if (!m_bullet_manager)
	{// 弾の管理クラスが設定されていない場合は何もしない
		return;
	}

	PoolAllocator<Bullet, 100>& bullet_pool = m_bullet_manager->GetBulletPool(); // 弾のプールを取得

	std::vector<Bullet*> player_bullets; // プレイヤーの弾のリスト
	std::vector<Bullet*> enemy_bullets; // 敵の弾のリスト

	bullet_pool.ForEach([&player_bullets, &enemy_bullets](Bullet& bullet)
		{
			if (bullet.IsPlayerBullet()) // プレイヤーの弾の場合
			{
				player_bullets.push_back(&bullet);
			}
			else // 敵の弾の場合
			{
				enemy_bullets.push_back(&bullet);
			}
		});

	if (player_bullets.empty() || enemy_bullets.empty())
	{// プレイヤーの弾または敵の弾が存在しない場合は何もしない
		return;
	}

	std::mutex mutex;
	std::vector<std::pair<Bullet*, Bullet*>> collision_pairs; // 衝突した弾のペアを格納するリスト

	auto check_range = [&](size_t start, size_t end)
		{
			for (size_t i = start; i < end; i++)
			{
				Bullet* p_bullet = player_bullets[i];
				Vector2 p_pos = p_bullet->GetPosition();
				Vector2 p_last_pos = p_bullet->GetLastPosition();

				for (Bullet* e_bullet : enemy_bullets)
				{
					Vector2 e_pos = e_bullet->GetPosition();
					Vector2 e_last_pos = e_bullet->GetLastPosition();

					// プレイヤーの弾と敵の弾の衝突判定
					if (((static_cast<int>(p_pos.y) == static_cast<int>(e_pos.y) || static_cast<int>(p_last_pos.y) == static_cast<int>(e_pos.y)) ||
						static_cast<int>(p_pos.y) == static_cast<int>(e_last_pos.y) || static_cast<int>(p_last_pos.y) == static_cast<int>(e_last_pos.y) ||
						((p_pos.y >= e_pos.y && p_pos.y <= e_last_pos.y) || (p_last_pos.y >= e_pos.y && p_last_pos.y <= e_last_pos.y) ||
							(e_pos.y >= p_pos.y && e_pos.y <= p_last_pos.y) || (e_last_pos.y >= p_pos.y && e_last_pos.y <= p_last_pos.y))) &&
						(static_cast<int>(p_pos.x) == static_cast<int>(e_pos.x) || static_cast<int>(p_last_pos.x) == static_cast<int>(e_pos.x)))
					{
						std::lock_guard<std::mutex> lock(mutex);
						collision_pairs.emplace_back(p_bullet, e_bullet); // 衝突した弾のペアを追加
						break;
					}
				}
			}
		};

	const size_t thread_count = std::min<size_t>(std::thread::hardware_concurrency(), player_bullets.size()); // 利用可能なスレッド数を取得
	std::vector<std::thread> threads; // スレッドのリスト
	size_t block_size = (player_bullets.size() + thread_count - 1) / thread_count; // 各スレッドが処理するブロックサイズ

	for (size_t i = 0; i < thread_count; i++)
	{
		size_t start = i * block_size;
		size_t end = std::min(start + block_size, player_bullets.size());
		if (start < end) // 有効な範囲であればスレッドを作成
		{
			threads.emplace_back(check_range, start, end);
		}
	}

	for (auto& thread : threads)
	{
		thread.join(); // 全てのスレッドが終了するまで待機
	}

	for (auto& pair : collision_pairs)
	{
		Bullet* p_bullet = pair.first; // プレイヤーの弾
		Bullet* e_bullet = pair.second; // 敵の弾

		p_bullet->Destroy(); // プレイヤーの弾を破棄
		e_bullet->Destroy(); // 敵の弾も破棄
		m_score->AddScoreByY(static_cast<int>(p_bullet->GetPosition().y)); // スコアを更新
	}
}

//------------------------------------------------
// 更新処理
//------------------------------------------------
void CollisionManager::Update()
{
	CheckBulletCollisions(); // 衝突判定の更新
}

