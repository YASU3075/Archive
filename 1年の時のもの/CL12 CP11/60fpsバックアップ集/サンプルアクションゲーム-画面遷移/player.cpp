//-----------------------------------------------
// player.cpp アクションゲームサンプル
// 制作日：2024/01/16 制作者：安田晴人
//-----------------------------------------------
#include "main.h"
#include "block.h"
#include "player.h"
#include "field.h"
#include "bullet.h"


#define CONIOEX
#include "conioex.h"

//-----------------------------------------------
// グローバル変数
//-----------------------------------------------
PLAYER player;		//プレイヤーの実体
int charge;

//-----------------------------------------------
// プレイヤー初期化
//-----------------------------------------------
void InitializePlayer(void) 
{
	player.PosX = 
	player.OldPosX = INITIAL_PLAYER_X;
	player.PosY = 
	player.OldPosY = INITIAL_PLAYER_Y;

	player.VelY = 0.0f;

	player.isJump = true;
	player.isTrigger = false;
	player.isRelease = true;
	charge = 0;

	player.isCharge = false;

}

//-----------------------------------------------
// プレイヤー更新
//-----------------------------------------------
void UpdatePlayer(void)
{
	//前フレームの座標を格納
	player.OldPosX = player.PosX;
	player.OldPosY = player.PosY;

	//移動処理
	{

		//横移動処理
		player.PosX += player.VelX;
		
		//壁判定
		{
			if (player.PosX >= MAX_PLAYER_X)
			{//画面右端
				player.PosX = MAX_PLAYER_X;
			}

			if (player.PosX <= MIN_PLAYER_X)
			{//画面左端
				player.PosX = MIN_PLAYER_X;
			}
		}

		//重力
		player.PosY += player.VelY;
		player.VelY += GRAVITY_FORCE;

		if (inport(PK_D))
		{//Dキー（右入力）
			player.PosX += PLAYER_X_SPEED;
		}

		if (inport(PK_A))
		{//Aキー（左入力）
			player.PosX -= PLAYER_X_SPEED;
		}

		if (inport(PK_W))
		{//Wキー入力（上入力）

			player.PosY -= PLAYER_Y_SPEED;

			if (player.PosY <= MIN_PLAYER_Y)
			{//画面上
				player.PosY = MIN_PLAYER_Y;
			}

		}

		if (inport(PK_S))
		{//Sキー入力（下入力）

			player.PosY += PLAYER_Y_SPEED;

			if (player.PosY >= MAX_PLAYER_Y)
			{//画面下
				player.PosY = MAX_PLAYER_Y;
			}

		}

		if (inport(PK_SP))
		{//スペース入力（ジャンプ）

			if (!player.isJump)
			{
				player.VelY -= JUMP_FORCE;

				if (player.PosY <= MIN_PLAYER_Y)
				{//画面上
					player.PosY = MIN_PLAYER_Y;
				}

				player.isJump = true;
			}

		}

	}

	//プレイヤーとブロックの当たり判定
	BLOCK* pBlock = GetBlock();		//ブロックの実体アドレスを取得

	//乗ってるブロックのアドレスを初期化する
	player.pPlayerOnBlock = NULL;

	for (int i = 0; i < MAX_BLOCK; i++)
	{
		if ((pBlock + i)->isUse)
		{

			

			//プレイヤー
			int ATop = (int)player.PosY;
			int ABottom = (int)player.PosY + (PLAYER_SIZE - 0.1f);
			int ARight = (int)player.PosX + (PLAYER_SIZE - 0.1f);
			int ALeft = (int)player.PosX;

			int ATopOld = (int)player.OldPosY;
			int ABottomOld = (int)player.OldPosY + (PLAYER_SIZE - 0.1f);
			int ARightOld = (int)player.OldPosX + (PLAYER_SIZE - 0.1f);
			int ALeftOld = (int)player.OldPosX;


			//ブロック
			int BTop = (int)(pBlock + i)->PosY;
			int BBottom = (int)(pBlock + i)->PosY + (BLOCK_SIZE - 1.0f);
			int BRight = (int)(pBlock + i)->PosX + ((pBlock + i)->Size) - 1;
			int BLeft = (int)(pBlock + i)->PosX;

			int BTopOld = (int)(pBlock + i)->OldPosY;
			int BBottomOld = (int)(pBlock + i)->OldPosY +(BLOCK_SIZE - 1.0f);
			int BRightOld = (int)(pBlock + i)->OldPosX + ((pBlock + i)->Size) - 1;
			int BLeftOld = (int)(pBlock + i)->OldPosX;

			//プレイヤーが上から下に通り抜けたかどうか
			if (ABottomOld < BTop && ABottom >= BTop && ARight >= BLeft && ALeft <= BRight)
			{//ブロックの上に乗ってる
				player.PosY = (pBlock + i)->PosY - 1.0f;
				player.VelY = 0.0f;
				player.isJump = false;

			}

			//プレイヤーが下から上に通り抜けたかどうか
			if (ATopOld > BBottom && ATop <= BTop && ARight >= BLeft && ALeft <= BRight)
			{
				player.PosY = (pBlock + i)->PosY + 1.0f;
				player.VelY = 0.0f;
			}
			

			//REVERSEタイプの時
			if ((pBlock + i)->type == BLOCKTYPE_REVERSE)
			{
				//プレイヤーの1つ下にブロックがあるかどうか
				if ((int)(player.PosY + 1.0f) == (int)(pBlock + i)->PosY)
				{
					//乗ってるブロックのアドレスを取得
					player.pPlayerOnBlock = (pBlock + i);

					//乗ってるブロックのスピード分だけプレイヤーのスピードに代入
					player.VelX = (pBlock + i)->VelX;
				}

				else
				{
					player.VelX = 0.0f;
				}
			}

			//NORMALタイプの時
			else if ((pBlock + i)->type == BLOCKTYPE_NORMAL)
			{
				//プレイヤーの1つ下にブロックがあるかどうか
				if ((int)(player.PosY + 1.0f) == (int)(pBlock + i)->PosY)
				{
					player.VelX = 0.0f;
				}
			}

			

		}
	}

	//地面に立つ
	if (player.PosY >= FIELD_POSY)
	{
		player.PosY = FIELD_POSY - 1.0f;
		player.VelY = 0.0f;
		player.isJump = false;
	}

	if (inport(PK_B))
	{//Bを押しているとき

		//Press処理
		player.isRelease = false;
		charge++;

		if (charge >= 60)
		{
			player.isCharge = true;
		}

		if (!player.isTrigger)
		{//Trigger処理
			SetBullet(player.PosX + 1.0f, player.PosY, 1.1f, 0.0f);
			player.isTrigger = true;
		}
	}

	else
	{//Bを押していないとき
		player.isTrigger = false;

		if (!player.isRelease)
		{//Release処理

			if (charge >= 60)
			{//チャージショット
				SetBullet(player.PosX + 1.0f, player.PosY, 3.0f, 0.0f);					//右
				SetBullet(player.PosX, player.PosY - 1.0f, 0.0f, -1.5f);				//上
				SetBullet(player.PosX - 1.0f, player.PosY, -3.0f, 0.0f);				//左
				SetBullet(player.PosX - 1.0f, player.PosY -1.0f, -3.0f, -1.5f);			//左上
				SetBullet(player.PosX + 1.0f, player.PosY - 1.0f, 3.0f, -1.5f);			//右上

				player.isCharge = false;
			}
			player.isRelease = true;
			charge = 0;
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

	if (player.isCharge)
	{//チャージできているとき
		textcolor(RED);
	}

	//プレイヤー実体の描画位置設定
	gotoxy((int)player.PosX, (int)player.PosY);

	//画面表示
	printf("*");

	//色をもとに戻す
	textcolor(WHITE);

#ifdef _DEBUG
	gotoxy(1, 2);
	printf("PosX:%0.1f", player.PosX);
	gotoxy(1, 3);
	printf("PosY:%0.1f", player.PosY);
	


#endif

}

//-----------------------------------------------
// プレイヤー終了
//-----------------------------------------------
void FinalizePlayer(void)
{
	//２年生になってテクスチャーとかモデルとかを読み込んだ時に解放するのに使う
	//今のところはあまり使わないが癖づけておく


}

