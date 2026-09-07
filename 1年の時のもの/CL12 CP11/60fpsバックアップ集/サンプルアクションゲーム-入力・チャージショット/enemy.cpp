//-----------------------------------------------
// enemy.cpp アクションゲームサンプル
// 制作日：2024/01/22 制作者：安田晴人
//-----------------------------------------------
#include "main.h"
#include "enemy.h"
#include "field.h"

#define CONIOEX
#include "conioex.h"

//-----------------------------------------------
// グローバル変数
//-----------------------------------------------
ENEMY enemy[MAX_ENEMY];		//敵の実体

//-----------------------------------------------
// 敵初期化
//-----------------------------------------------
void InitializeEnemy(void) 
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		enemy[i].PosX =
		enemy[i].OldPosX = 0.0f;
		enemy[i].PosY =
		enemy[i].OldPosY = 0.0f;
		enemy[i].VelX =
		enemy[i].VelY = 0.0f;

		enemy[i].isUse = false;
	}

	//敵の設置
	SetEnemy(40.0f, FIELD_POSY - 1.0f, 0.0f, 0.0f);

}

//-----------------------------------------------
// 敵更新
//-----------------------------------------------
void UpdateEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy[i].isUse)
		{//使用していたら

			//前フレームの座標を格納
			enemy[i].OldPosX = enemy[i].PosX;
			enemy[i].OldPosY = enemy[i].PosY;

			//移動
			enemy[i].PosX += enemy[i].VelX;
			enemy[i].PosY += enemy[i].VelY;

			//左右壁判定
			if (enemy[i].PosX <= MIN_ENEMY_X || enemy[i].PosX >= MAX_ENEMY_X )
			{
				//消す
				enemy[i].isUse = false;

				//残像の描画位置設定
				gotoxy((int)enemy[i].OldPosX, (int)enemy[i].OldPosY);

				//残像を消す
				printf(" ");

			}

			//上下壁判定
			if (enemy[i].PosY <= MIN_ENEMY_Y || enemy[i].PosY >= MAX_ENEMY_Y)
			{
				//消す
				enemy[i].isUse = false;

				//残像の描画位置設定
				gotoxy((int)enemy[i].OldPosX, (int)enemy[i].OldPosY);

				//残像を消す
				printf(" ");

			}

		}
	}
}

//-----------------------------------------------
// 敵描画
//-----------------------------------------------
void DrawEnemy(void)
{

	for (int i = 0; i < MAX_ENEMY; i++)
	{

		if (enemy[i].isUse)
		{//使用していたら

		//もし描画位置が動いていたら残像処理
			if ((int)enemy[i].OldPosX != (int)enemy[i].PosX || (int)enemy[i].OldPosY != (int)enemy[i].PosY)
			{
				//残像の描画位置設定
				gotoxy((int)enemy[i].OldPosX, (int)enemy[i].OldPosY);

				//残像を消す
				printf(" ");
			}

			//色設定
			textcolor(BLUE);

			//敵実体の描画位置設定
			gotoxy((int)enemy[i].PosX, (int)enemy[i].PosY);

			//画面表示
			printf("E");

			//色をもとに戻す
			textcolor(WHITE);
		}

	}
}

//-----------------------------------------------
// 敵終了
//-----------------------------------------------
void FinalizeEnemy(void)
{
	//２年生になってテクスチャーとかモデルとかを読み込んだ時に解放するのに使う
	//今のところはあまり使わないが癖づけておく


}


//-----------------------------------------------
// 敵のセッター
//-----------------------------------------------
void SetEnemy(float px, float py, float vx, float vy)
{

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (!enemy[i].isUse)
		{
			enemy[i].PosX = px;
			enemy[i].OldPosX = px;
			enemy[i].PosY = py;
			enemy[i].OldPosY = py;

			enemy[i].VelX = vx;
			enemy[i].VelY = vy;

			enemy[i].isUse = true;
			break;

		}
	}

}
