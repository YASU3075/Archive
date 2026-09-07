//-----------------------------------------------
// GameScene.cpp
// 制作日：2025/08/07
// 制作者：安田晴人
//-----------------------------------------------
#include "GameScene.h"
#include "GameObject.h"
#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "BulletManager.h"
#include "Score.h"
#include "Field.h"
#include "CollisionManager.h"
#include "DrawManager.h"
#include "Manager.h"
#include "Result.h"

//------------------------------------------------
// コンストラクタ
//------------------------------------------------
GameScene::GameScene()
{
	m_time = 0;

	// プレイヤー、敵、スコア、フィールド、アイテム、衝突管理クラスの初期化
	Player* player = AddGameObject<Player>();
	Enemy* enemy = AddGameObject<Enemy>();
	Score* score = AddGameObject<Score>();
	Field* field = AddGameObject<Field>();

	// 描画管理クラスの初期化
	m_draw_manager = std::make_unique<DrawManager>();

	// 各ゲームオブジェクトに描画管理クラスを設定
	player->SetDrawManager(m_draw_manager.get());
	enemy->SetDrawManager(m_draw_manager.get());
	score->SetDrawManager(m_draw_manager.get());
	field->SetDrawManager(m_draw_manager.get());
	
	// 弾の管理クラスと衝突管理クラスの初期化
	m_bullet_manager = std::make_unique<BulletManager>();
	m_collision_manager = std::make_unique<CollisionManager>();

	m_bullet_manager->SetDrawManager(m_draw_manager.get()); // 弾の管理クラスに描画管理クラスを設定

	m_collision_manager->SetScore(score); // 衝突管理クラスにスコアを設定

	// 各ゲームオブジェクトに弾の管理クラスを設定
	player->SetBulletManager(m_bullet_manager.get());
	enemy->SetBulletManager(m_bullet_manager.get());
	m_collision_manager->SetBulletManager(m_bullet_manager.get());

	player->SetPosition(Vector2(10.0f, 22.0f)); // プレイヤーの初期位置
	player->SetSymbol('P'); // プレイヤーのシンボル
	player->SetLayer(1); // プレイヤーのレイヤー
	player->SetField(field); // プレイヤーにフィールドを設定

	enemy->SetPosition(Vector2(10.0f, 2.0f)); // 敵の初期位置
	enemy->SetSymbol('e'); // 敵のシンボル
	enemy->SetLayer(1); // 敵のレイヤー
	enemy->SetField(field); // 敵にフィールドを設定

	score->SetField(field); // スコアにフィールドを設定

	// フィールドの初期化
	field->SetPosition(Vector2(5.0f, 0.0f)); // フィールドの初期位置
	field->SetLayer(0);
}

//------------------------------------------------
// シーンの更新処理
//------------------------------------------------
void GameScene::Update()
{
	int prev_time = static_cast<int>(m_time); // 前回の時間を保存
	m_time += 1.0f / 60.0f;
	int current_time = static_cast<int>(m_time); // 現在の時間を取得

	if (m_time >= m_time_limit) // 時間制限に達した場合
	{
		Score* score = GetGameObject<Score>();
		Manager::GetInstance()->SetNextScene(new Result(score->GetScore())); // 結果シーンに遷移
	}

	if (current_time % 10 == 0 && current_time != prev_time) // 10秒ごとに処理を行う
	{
		Field* field = GetGameObject<Field>();
		field->SetCurrentWidth(field->GetCurrentWidth() + 1); // フィールドの幅を1増やす

		Enemy* enemy = GetGameObject<Enemy>();
		enemy->SetLevel(enemy->GetLevel() + 1); // 敵のレベルを1上げる
	}

	// 各ゲームオブジェクトの更新処理
	for (const auto& game_object : m_game_objects)
	{
		game_object->Update();
	}

	// 弾の管理クラスの更新
	m_bullet_manager->UpdateBullets();
	// 衝突管理クラスの更新
	m_collision_manager->Update();

	for (const auto& game_object : m_game_objects)
	{
		if (game_object->IsDestroyed())
		{
			// 破棄されたゲームオブジェクトを削除
			auto it = std::remove_if(m_game_objects.begin(), m_game_objects.end(),
				[&game_object](const std::unique_ptr<GameObject>& obj) {
					return obj.get() == game_object.get();
				});
			m_game_objects.erase(it, m_game_objects.end());
		}
	}

	// 弾の破棄処理
	m_bullet_manager->DestroyBullets();
}

//------------------------------------------------
// シーンの描画処理
//------------------------------------------------
void GameScene::Draw()
{
	// 各ゲームオブジェクトの描画処理
	for (const auto& game_object : m_game_objects)
	{
		game_object->Draw();
	}

	m_bullet_manager->DrawBullets(); // 弾の描画

	m_draw_manager->Draw(); // 描画管理クラスの描画処理

	// 時間の表示
	Field* field = GetGameObject<Field>();
	printf("\x1b[%d;%dHTime:%d", 3, field->GetCurrentMaxX() + 1, static_cast<int>(m_time_limit - m_time));
}