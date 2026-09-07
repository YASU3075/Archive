//-----------------------------------------------
// enemy.cpp アクションゲームサンプル
// 制作日：2024/01/22 制作者：安田晴人
//-----------------------------------------------
#include "main.h"
#include "enemy.h"
#include "field.h"
#include "bullet.h"

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

		enemy[i].type = ENEMYTYPE_NORMAL;
		enemy[i].frameCnt = 0;

		enemy[i].isUse = false;
	}

	//敵の設置
	SetEnemy(40.0f, FIELD_POSY - 1.0f, 0.0f, -0.05f, ENEMYTYPE_REVERSE);
	SetEnemy(4.0f, FIELD_POSY - 1.0f, 0.0f, 0.0f);
	SetEnemy(20.0f, 15.0f, 0.0f, 0.0f);
	

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

			switch (enemy[i].type)
			{
			case ENEMYTYPE_NORMAL:
				break;

			case ENEMYTYPE_REVERSE:
				//移動
				enemy[i].PosX += enemy[i].VelX;
				enemy[i].PosY += enemy[i].VelY;

				enemy[i].frameCnt++;

				if (enemy[i].frameCnt >= 60)
				{//６０フレーム経ったら
					//反転
					enemy[i].VelX *= -1;
					enemy[i].VelY *= -1;

					//リセット
					enemy[i].frameCnt = 0;
				}


				break;

			default:break;

			}
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

			//弾と敵の当たり判定処理
			BULLET* pBullet = GetBullet();		//弾実体の先頭アドレスを取得

			for (int j = 0; j < MAX_BULLET; j++)
			{
				if ((pBullet + j)->isUse)
				{//敵も弾もどちらも使っている場合
					
					//当たり判定式
					
					//弾
					int ATop = (int)(pBullet + j)->PosY;
					int ABottom = (int)(pBullet + j)->PosY + (BULLET_SIZE - 0.1f);
					int ARight = (int)(pBullet + j)->PosX + (BULLET_SIZE - 0.1f);
					int ALeft = (int)(pBullet + j)->PosX;

					int ATopOld = (int)(pBullet + j)->OldPosY;
					int ABottomOld = (int)(pBullet + j)->OldPosY + (BULLET_SIZE - 0.1f);
					int ARightOld = (int)(pBullet + j)->OldPosX + (BULLET_SIZE - 0.1f);
					int ALeftOld = (int)(pBullet + j)->OldPosX;


					//敵
					int BTop = (int)enemy[i].PosY;
					int BBottom = (int)enemy[i].PosY + (ENEMY_SIZE - 0.1f);
					int BRight = (int)enemy[i].PosX + (ENEMY_SIZE - 0.1f);
					int BLeft = (int)enemy[i].PosX;

					int BTopOld = (int)enemy[i].OldPosY;
					int BBottomOld = (int)enemy[i].OldPosY + (ENEMY_SIZE - 0.1f);
					int BRightOld = (int)enemy[i].OldPosX + (ENEMY_SIZE - 0.1f);
					int BLeftOld = (int)enemy[i].OldPosX;



					if (
						(ATop <= BBottom && ABottom >= BTop && ARight >= BLeft && ALeft <= BRight) ||		//重なったとき
						(ARightOld < BLeft && ARight >= BLeft && ATop <= BBottom && ABottom >= BTop) ||		//左から右すり抜けたとき
						(ALeftOld > BRight && ALeft <= BRight && ATop <= BBottom && ABottom >= BTop) ||		//右から左すり抜けたとき
						(ARight >= BLeft && ALeft <= BRight && ABottomOld < BTop && ATop > BBottom) ||	//上から下すり抜けたとき
						(ARight >= BLeft && ALeft <= BRight && ATopOld > BBottom && ABottom < BTop)		//下から上すり抜けたとき
						)
					{

						//弾を消す
						(pBullet + j)->isUse = false;

						//残像の描画位置設定
						gotoxy((int)(pBullet + j)->OldPosX, (int)(pBullet + j)->OldPosY);

						//残像を消す
						printf(" ");

						//敵を消す
						enemy[i].isUse = false;

						//残像の描画位置設定
						gotoxy((int)enemy[i].OldPosX, (int)enemy[i].OldPosY);

						//残像を消す
						printf(" ");

					}

					
				}
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
void SetEnemy(float px, float py, float vx, float vy, ENEMYTYPE t)
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

			enemy[i].type = t;
			enemy[i].frameCnt = 0;

			enemy[i].isUse = true;
			break;

		}
	}
}


//メモ
//バウンディングサークル
// 二つの円の半径の和と二つの円の中心の距離の差を比べて半径の和の方が大きいときぶつかっている
// 斜めの関係にある時は三平方の定理で距離を取るが二乗が入ると処理が重い
// 画面端の敵に当たらないときがある
//

