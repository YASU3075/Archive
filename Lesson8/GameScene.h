//-----------------------------------------------
// GameScene.h
// 制作日：2025/08/05
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <vector>
#include <memory>

#include "Scene.h"
#include "GameObject.h"
#include "DrawManager.h"
#include "CollisionManager.h"
#include "BulletManager.h"

class GameScene : public Scene
{
private:
	std::vector<std::unique_ptr<GameObject>> m_game_objects;

	std::unique_ptr<BulletManager> m_bullet_manager; // 弾の管理クラス
	std::unique_ptr<CollisionManager> m_collision_manager; // 衝突管理クラス
	std::unique_ptr<DrawManager> m_draw_manager; // 描画管理クラス

	float m_time; // 時間管理用変数
	const float m_time_limit = 60.0f; // ゲームの時間制限

public:
	GameScene();

	virtual ~GameScene() override = default; // デストラクタ

	// シーンの更新処理
	virtual void Update() override;

	// シーンの描画処理
	virtual void Draw() override;

	// ゲームオブジェクトの追加
	template<typename T>
	T* AddGameObject()
	{
		T* game_object = new T();
		m_game_objects.emplace_back(game_object);

		return game_object;
	}

	template<typename T>
	T* GetGameObject()
	{
		for (const auto& game_object : m_game_objects)
		{
			if (dynamic_cast<T*>(game_object.get()))
			{
				return dynamic_cast<T*>(game_object.get());
			}
		}
		return nullptr;
	}
};

