//-----------------------------------------------
// game.cpp アクションゲームサンプル
// 制作日：2024/01/31 制作者：安田晴人
//-----------------------------------------------
#include "main.h"
#include "game.h"
#include "player.h"
#include "field.h"
#include "bullet.h"
#include "bat.h"
#include "enemybullet.h"
#include "enemy.h"
#include "explosion.h"

#define CONIOEX
#include "conioex.h"

//-----------------------------------------------
// グローバル変数
//-----------------------------------------------
GAME game;

//-----------------------------------------------
// ゲームシーン初期化
//-----------------------------------------------
void InitializeGame() 
{
	game.Score = 0;
	game.OldScore = 0;
	
	//色設定
	textbackground(RED);


	//描画位置設定
	gotoxy(60, 25);
	//画面描画
	printf("スコヴィル値：%d", game.OldScore);
	//色をもとに戻す
	textbackground(BLACK);

	InitializePlayer();
	InitializeBullet();
	InitializeBat();
	InitializeField();
	InitializeEnemy();
	InitializeExplosion();
	InitializeEnemyBullet();

	strcpy(game.bgm, "MUSICDATA\\battle_bgm.mp3");
	game.SoundHandle = opensound(game.bgm);

	if (game.SoundHandle)
	{//bgmを開けたら
		playsound(game.SoundHandle, 1);
		setvolume(game.SoundHandle, 50);
	}
	
}

//-----------------------------------------------
// ゲームシーン更新
//-----------------------------------------------
void UpdateGame()
{	
	UpdatePlayer();
	UpdateBullet();
	UpdateBat();
	UpdateField();
	UpdateEnemy();
	UpdateExplosion();
	UpdateEnemyBullet();
	

#ifdef _DEBUG
	//シーン切り替え
	/*if (inport(PK_3))
	{
		SetScene(SCENE_RESULT);
	}*/
#endif 

}

//-----------------------------------------------
// ゲームシーン描画
//-----------------------------------------------
void DrawGame()
{
	DrawPlayer();
	DrawBullet();
	DrawBat();
	DrawField();
	DrawEnemy();
	DrawExplosion();
	DrawEnemyBullet();

	//スコア表示
	{
		if (game.Score != game.OldScore)
		{
			//1フレームにつき10増やす
			game.OldScore += 10;

			//色設定
			textbackground(RED);
			//描画位置設定
			gotoxy(60, 25);
			//画面描画
			printf("スコヴィル値：%d", game.OldScore);
			//色をもとに戻す
			textbackground(BLACK);
		}
	}
	

#ifdef _DEBUG
	/*gotoxy(10, 1);
	printf("SCORE: %d", game.OldScore);*/
#endif

}

//-----------------------------------------------
// ゲームシーン終了
//-----------------------------------------------
void FinalizeGame()
{
	FinalizePlayer();
	FinalizeBullet();
	FinalizeBat();
	FinalizeField();
	FinalizeEnemy();
	FinalizeExplosion();
	FinalizeEnemyBullet();

	closesound(game.SoundHandle);

}

//-----------------------------------------------
// 敵のキルカウント増加
//-----------------------------------------------
void ScoreAdd(int score)
{
	game.Score += score * 100;
}

//-----------------------------------------------
// スコアのゲッター
//-----------------------------------------------
int GetScore(void)
{
	return game.Score;
}