//-----------------------------------------------
// player.cpp アクションゲームサンプル
// 制作日：2024/01/16 制作者：安田晴人
//-----------------------------------------------
#include "main.h"
#include "player.h"

#define CONIOEX
#include "conioex.h"

//-----------------------------------------------
// グローバル変数
//-----------------------------------------------
PLAYER player;		//プレイヤーの実体

//-----------------------------------------------
// プレイヤー初期化
//-----------------------------------------------
void InitializePlayer(void) 
{
	player.PosX = 
	player.OldPosX = INITIAL_PLAYER_X;
	player.PosY = 
	player.OldPosY = INITIAL_PLAYER_Y;
}

//-----------------------------------------------
// プレイヤー更新
//-----------------------------------------------
void UpdatePlayer(void)
{
	//前フレームの座標を格納
	player.OldPosX = player.PosX;
	player.OldPosY = player.PosY;


	if (inport(PK_D))
	{//Dキー（右入力）
		player.PosX += PLAYER_X_SPEED;

		if (player.PosX >= MAX_PLAYER_X)
		{//画面右端
			player.PosX = MAX_PLAYER_X;
		}

	}

	if (inport(PK_A))
	{//Aキー（左入力）

		player.PosX -= PLAYER_X_SPEED;

		if (player.PosX <= MIN_PLAYER_X)
		{//画面左端
			player.PosX = MIN_PLAYER_X;
		}

	}

	if (inport(PK_W))
	{//Wキー入力

		player.PosY -= PLAYER_Y_SPEED;

		if (player.PosY <= MIN_PLAYER_Y)
		{//画面上
			player.PosY = MIN_PLAYER_Y;
		}

	}

	if (inport(PK_S))
	{//Sキー入力

		player.PosY += PLAYER_Y_SPEED;

		if (player.PosY >= MAX_PLAYER_Y)
		{//画面下
			player.PosY = MAX_PLAYER_Y;
		}

	}

}

//-----------------------------------------------
// プレイヤー描画
//-----------------------------------------------
void DrawPlayer(void)
{
	//もし描画位置が動いていたら残像処理
	if ((int)player.OldPosX != (int)player.PosX || (int)player.OldPosY != (int)player.PosY)
	{
		//残像の描画位置設定
		gotoxy((int)player.OldPosX, (int)player.OldPosY);

		//残像を消す
		printf(" ");
	}

	//色設定
	textcolor(GREEN);

	//プレイヤー実体の描画位置設定
	gotoxy((int)player.PosX, (int)player.PosY);

	//画面表示
	printf("*");

	//色をもとに戻す
	textcolor(WHITE);
}

//-----------------------------------------------
// プレイヤー終了
//-----------------------------------------------
void FinalizePlayer(void)
{
	//２年生になってテクスチャーとかモデルとかを読み込んだ時に解放するのに使う
	//今のところはあまり使わないが癖づけておく


}

