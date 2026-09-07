//-----------------------------------------------
// game.cpp アクションゲームサンプル
// 制作日：2024/01/31 制作者：安田晴人
//-----------------------------------------------
#include "game.h"
#include "block.h"
#include "player.h"
#include "field.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"

//-----------------------------------------------
// ゲームシーン初期化
//-----------------------------------------------
void InitializeGame() 
{
	InitializePlayer();
	InitializeField();
	InitializeBullet();
	InitializeEnemy();
	InitializeBlock();
	InitializeExplosion();
}

//-----------------------------------------------
// ゲームシーン更新
//-----------------------------------------------
void UpdateGame()
{
	UpdatePlayer();
	UpdateField();
	UpdateBullet();
	UpdateEnemy();
	UpdateBlock();
	UpdateExplosion();
}

//-----------------------------------------------
// ゲームシーン描画
//-----------------------------------------------
void DrawGame()
{
	DrawPlayer();
	DrawField();
	DrawBullet();
	DrawEnemy();
	DrawBlock();
	DrawExplosion();
}

//-----------------------------------------------
// ゲームシーン終了
//-----------------------------------------------
void FinalizeGame()
{
	FinalizePlayer();
	FinalizeField();
	FinalizeBullet();
	FinalizeEnemy();
	FinalizeBlock();
	FinalizeExplosion();
}
