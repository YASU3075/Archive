//-------------------------------------------------------------------
// bullet.cpp アクションゲームサンプル
// 制作日：2024/01/17 制作者：安田晴人
//-------------------------------------------------------------------
#include "main.h"
#include "bullet.h"
#include "field.h"

#define CONIOEX
#include "conioex.h"

//-------------------------------------------------------------------
// グローバル変数
//-------------------------------------------------------------------
BULLET bullet[MAX_BULLET];		//弾の実体

//-------------------------------------------------------------------
// 弾初期化
//-------------------------------------------------------------------
void InitializeBullet(void) 
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		bullet[i].PosX =
		bullet[i].OldPosX = 0.0f;
		bullet[i].PosY =
		bullet[i].OldPosY = 0.0f;
		bullet[i].VelX =
		bullet[i].VelY = 0.0f;

		bullet[i].isUse = false;
	}
}

//-------------------------------------------------------------------
// 弾更新
//-------------------------------------------------------------------
void UpdateBullet(void)
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (bullet[i].isUse)
		{//使用していたら

			//前フレームの座標を格納
			bullet[i].OldPosX = bullet[i].PosX;
			bullet[i].OldPosY = bullet[i].PosY;

			//移動
			bullet[i].PosX += bullet[i].VelX;
			bullet[i].PosY += bullet[i].VelY;

			//上下左右壁
			if (bullet[i].PosX <= MIN_BULLET_X || bullet[i].PosX >= MAX_BULLET_X || bullet[i].PosY <= MIN_BULLET_Y || bullet[i].PosY >= MAX_BULLET_Y)
			{
				
				//消す
				bullet[i].isUse = false;

				//残像の描画位置設定
				gotoxy((int)bullet[i].OldPosX, (int)bullet[i].OldPosY);

				//残像を消す
				printf(" ");

			}
		}
	}
}

//-------------------------------------------------------------------
// 弾描画
//-------------------------------------------------------------------
void DrawBullet(void)
{

	for (int i = 0; i < MAX_BULLET; i++)
	{

		if (bullet[i].isUse)
		{//使用していたら

		//もし描画位置が動いていたら残像処理
			if ((int)bullet[i].OldPosX != (int)bullet[i].PosX || (int)bullet[i].OldPosY != (int)bullet[i].PosY)
			{
				//残像の描画位置設定
				gotoxy((int)bullet[i].OldPosX, (int)bullet[i].OldPosY);

				//残像を消す
				printf(" ");
			}

			//色設定
			textcolor(RED);

			//弾実体の描画位置設定
			gotoxy((int)bullet[i].PosX, (int)bullet[i].PosY);

			//画面表示
			printf("=");

			//色をもとに戻す
			textcolor(WHITE);
		}

	}
}

//-------------------------------------------------------------------
// 弾終了
//-------------------------------------------------------------------
void FinalizeBullet(void)
{
	//２年生になってテクスチャーとかモデルとかを読み込んだ時に解放するのに使う
	//今のところはあまり使わないが癖づけておく
}

//-------------------------------------------------------------------
// 弾のセッター
//-------------------------------------------------------------------
void SetBullet(float px, float py, float vx, float vy)
{

	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (!bullet[i].isUse)
		{
			bullet[i].PosX = px;
			bullet[i].OldPosX = px;
			bullet[i].PosY = py;
			bullet[i].OldPosY = py;

			bullet[i].VelX = vx;
			bullet[i].VelY = vy;

			bullet[i].isUse = true;
			break;

		}
	}

}

//-------------------------------------------------------------------
// 弾のゲッター（実体の先頭アドレスを取得する関数）
//-------------------------------------------------------------------
BULLET* GetBullet(void)
{
	return bullet;
}
