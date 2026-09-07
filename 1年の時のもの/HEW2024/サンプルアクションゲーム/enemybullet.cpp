//-------------------------------------------------------------------
// enemybullet.cpp アクションゲームサンプル
// 制作日：2024/02/09 制作者：安田晴人
//-------------------------------------------------------------------
#include "main.h"
#include "enemybullet.h"
#include "bullet.h"
#include "field.h"

#define CONIOEX
#include "conioex.h"

//-------------------------------------------------------------------
// グローバル変数
//-------------------------------------------------------------------
ENEMYBULLET enemybullet[MAX_ENEMYBULLET];		//敵の弾の実体
ENEMYBULLETSOUND eSound;

//-------------------------------------------------------------------
// 敵の弾初期化
//-------------------------------------------------------------------
void InitializeEnemyBullet(void) 
{
	for (int i = 0; i < MAX_ENEMYBULLET; i++)
	{
		enemybullet[i].PosX =
		enemybullet[i].OldPosX = 0.0f;
		enemybullet[i].PosY =
		enemybullet[i].OldPosY = 0.0f;
		enemybullet[i].VelX =
		enemybullet[i].VelY = 0.0f;
		enemybullet[i].ColorNum = 0;
		enemybullet[i].HitWallCnt = 0;

		enemybullet[i].type = ENEMYBULLETTYPE_NORMAL;

		enemybullet[i].FrameCnt = 0;

		enemybullet[i].isUse = false;
	}

	//衝突効果音
	strcpy(eSound.CollisionSound, "MUSICDATA\\bullet.mp3");
	eSound.CollisionSoundHandle = opensound(eSound.CollisionSound);
	if (eSound.CollisionSoundHandle)
	{
		setvolume(eSound.CollisionSoundHandle, 50);
	}

}

//-------------------------------------------------------------------
// 敵の弾更新
//-------------------------------------------------------------------
void UpdateEnemyBullet(void)
{

	for (int i = 0; i < MAX_ENEMYBULLET; i++)
	{
		if (enemybullet[i].isUse)
		{//使用していたら

			//前フレームの座標を格納
			enemybullet[i].OldPosX = enemybullet[i].PosX;
			enemybullet[i].OldPosY = enemybullet[i].PosY;

			//三角関数計算変数初期化
			double cAngle = 0;
			double cRadian = 0;
			double cResult = 0;
			double sAngle = 0;
			double sRadian = 0;
			double sResult = 0;

			switch (enemybullet[i].type)
			{

			case ENEMYBULLETTYPE_NORMAL:
				//ノーマル移動
				enemybullet[i].PosX += enemybullet[i].VelX;
				enemybullet[i].PosY += enemybullet[i].VelY;
				break;

			case ENEMYBULLETTYPE_FAST:
				//早い移動
				enemybullet[i].PosX += enemybullet[i].VelX;
				if (enemybullet[i].VelY > 0)
				{//Y移動地がプラスの時
					enemybullet[i].PosY += enemybullet[i].VelY + ENEMYBULLET_CHANGE_SPEED;
				}

				else
				{
					enemybullet[i].PosY += enemybullet[i].VelY - ENEMYBULLET_CHANGE_SPEED;
				}

				break;

			case ENEMYBULLETTYPE_CURVE:
				//カーブ移動
				cAngle = (double)enemybullet[i].FrameCnt;
				cRadian = cAngle * M_PI / 180;
				cResult = cos(cRadian);

				enemybullet[i].PosX += enemybullet[i].VelX + (float)cResult * 0.5f;
				enemybullet[i].PosY += enemybullet[i].VelY;

				break;

			case ENEMYBULLETTYPE_SPEEDCHANGE:
				//変速移動
				sAngle = (double)enemybullet[i].FrameCnt;
				sRadian = sAngle * M_PI / 180;
				sResult = sin(sRadian);

				enemybullet[i].PosX += enemybullet[i].VelX;
				enemybullet[i].PosY += enemybullet[i].VelY * (float)sResult;
				break;

			default:break;
			}

			//フレーム数増加
			enemybullet[i].FrameCnt += 5;

			if (enemybullet[i].FrameCnt > 180)
			{
				enemybullet[i].FrameCnt = 0;
			}

			if (enemybullet[i].HitWallCnt > MAX_HIT_COUNT)
			{
				//消す
				enemybullet[i].isUse = false;

				//残像の描画位置設定
				gotoxy((int)enemybullet[i].OldPosX, (int)enemybullet[i].OldPosY);

				//残像を消す
				printf("　");
			}

			//上下左右壁
			if (enemybullet[i].PosX <= MIN_ENEMYBULLET_X || enemybullet[i].PosX >= MAX_ENEMYBULLET_X)
			{
				//跳ね返りカウント増加
				enemybullet[i].HitWallCnt++;

				//反射させる
				enemybullet[i].VelX *= -1.0f;

				//1フレーム前の位置に戻す
				enemybullet[i].PosX = enemybullet[i].OldPosX;
			}

			else if (enemybullet[i].PosY <= MIN_ENEMYBULLET_Y || enemybullet[i].PosY >= MAX_ENEMYBULLET_Y)
			{
				//跳ね返りカウント増加
				enemybullet[i].HitWallCnt++;

				//反射させる
				enemybullet[i].VelY *= -1.0f;

				//1フレーム前の位置に戻す
				enemybullet[i].PosY = enemybullet[i].OldPosY;
			}

			//弾と敵の弾の当たり判定処理
			BULLET* pBullet = GetBullet();		//弾実体の先頭アドレスを取得

			for (int j = 0; j < MAX_BULLET; j++)
			{
				if ((pBullet + j)->isUse)
				{//敵の弾もプレイヤーの弾もどちらも使っている場合


					//弾
					int ATop = (int)(pBullet + j)->PosY;
					int ABottom = (int)(pBullet + j)->PosY + (1.0f - 0.1f);
					int ARight = (int)(pBullet + j)->PosX + (BULLET_SIZE_X - 0.1f);
					int ALeft = (int)(pBullet + j)->PosX;

					int ATopOld = (int)(pBullet + j)->OldPosY;
					int ABottomOld = (int)(pBullet + j)->OldPosY + (1.0f - 0.1f);
					int ARightOld = (int)(pBullet + j)->OldPosX + (BULLET_SIZE_X - 0.1f);
					int ALeftOld = (int)(pBullet + j)->OldPosX;


					//敵
					int BTop = (int)enemybullet[i].PosY;
					int BBottom = (int)enemybullet[i].PosY;
					int BRight = (int)enemybullet[i].PosX + (ENEMYBULLET_SIZE - 0.1f);
					int BLeft = (int)enemybullet[i].PosX;

					int BTopOld = (int)enemybullet[i].OldPosY;
					int BBottomOld = (int)enemybullet[i].OldPosY;
					int BRightOld = (int)enemybullet[i].OldPosX + (ENEMYBULLET_SIZE - 0.1f);
					int BLeftOld = (int)enemybullet[i].OldPosX;


					//当たり判定式
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

						if ((int)(pBullet + j)->OldPosY == FIELD_POSY)
						{//消えたところがフィールドに重なるとき
							//残像の描画位置設定
							gotoxy(MIN_FIELD_X, FIELD_POSY);

							//色設定
							textcolor(WHITE);
							//フィールド描画
							printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
							//色をもとに戻す
							textcolor(WHITE);
						}

						//弾を消す
						enemybullet[i].isUse = false;

						//残像の描画位置設定
						gotoxy((int)enemybullet[i].OldPosX, (int)enemybullet[i].OldPosY);

						//残像を消す
						printf(" ");

						if ((int)enemybullet[i].OldPosY == FIELD_POSY)
						{//消えたところがフィールドに重なるとき
							//残像の描画位置設定
							gotoxy(MIN_FIELD_X, FIELD_POSY);

							//色設定
							textcolor(WHITE);
							//フィールド描画
							printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
							//色をもとに戻す
							textcolor(WHITE);
						}

						//衝突音
						playsound(eSound.CollisionSoundHandle, 0);
					}
				}
			}
		}
	}
}

//-------------------------------------------------------------------
// 敵の弾描画
//-------------------------------------------------------------------
void DrawEnemyBullet(void)
{

	for (int i = 0; i < MAX_ENEMYBULLET; i++)
	{

		if (enemybullet[i].isUse)
		{//使用していたら

			//もし描画位置が動いていたら残像処理
			if ((int)enemybullet[i].OldPosX != (int)enemybullet[i].PosX || (int)enemybullet[i].OldPosY != (int)enemybullet[i].PosY)
			{
				//残像の描画位置設定
				gotoxy((int)enemybullet[i].OldPosX, (int)enemybullet[i].OldPosY);
				//残像を消す
				printf("　");

				//フィールドと重なった時
				{
					if (
						((int)enemybullet[i].OldPosX >= MIN_FIELD_X - 1 && (int)enemybullet[i].OldPosX <= MAX_FIELD_X + 1) &&
						((int)enemybullet[i].OldPosY == FIELD_POSY)
						)
					{//下の辺の時
						//残像の描画位置設定
						gotoxy(MIN_FIELD_X, FIELD_POSY);

						//色設定
						textcolor(WHITE);
						//フィールド描画
						printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
						//色をもとに戻す
						textcolor(WHITE);
					}

				}

				//色設定
				textcolor(enemybullet[i].ColorNum - enemybullet[i].HitWallCnt);

				if (enemybullet[i].ColorNum - enemybullet[i].HitWallCnt == BLACK)
				{
					enemybullet[i].ColorNum = rand() % 14 + 1;
					textcolor(enemybullet[i].ColorNum);
				}

				//敵の弾実体の描画位置設定
				gotoxy((int)enemybullet[i].PosX, (int)enemybullet[i].PosY);

				//画面表示
				printf("甘");

				//色をもとに戻す
				textcolor(WHITE);
				

			}

		}
	}
}

//-------------------------------------------------------------------
// 敵の弾終了
//-------------------------------------------------------------------
void FinalizeEnemyBullet(void)
{
	closesound(eSound.CollisionSoundHandle);
}

//-------------------------------------------------------------------
// 敵の弾のセッター
//-------------------------------------------------------------------
void SetEnemyBullet(float px, float py, float vx, float vy, int color, ENEMYBULLETTYPE t)
{

	for (int i = 0; i < MAX_ENEMYBULLET; i++)
	{
		if (!enemybullet[i].isUse)
		{
			enemybullet[i].PosX = px;
			enemybullet[i].OldPosX = px;
			enemybullet[i].PosY = py;
			enemybullet[i].OldPosY = py;

			enemybullet[i].VelX = vx;
			enemybullet[i].VelY = vy;

			enemybullet[i].ColorNum = color;

			enemybullet[i].HitWallCnt = 0;

			enemybullet[i].type = t;

			enemybullet[i].FrameCnt = 0;

			enemybullet[i].isUse = true;
			break;

		}
	}

}

//-------------------------------------------------------------------
// 敵の弾のゲッター（実体の先頭アドレスを取得する関数）
//-------------------------------------------------------------------
ENEMYBULLET* GetEnemyBullet(void)
{
	return enemybullet;
}


