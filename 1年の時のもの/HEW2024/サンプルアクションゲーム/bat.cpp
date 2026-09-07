//-------------------------------------------------------------------
// bat.cpp アクションゲームサンプル
// 制作日：2024/02/16 制作者：安田晴人
//-------------------------------------------------------------------
#include "main.h"
#include "player.h"
#include "bat.h"
#include "bullet.h"
#include "enemybullet.h"
#include "field.h"

#define CONIOEX
#include "conioex.h"

//-------------------------------------------------------------------
// グローバル変数
//-------------------------------------------------------------------
BAT bat;		//バットの実体

//-------------------------------------------------------------------
// バット初期化
//-------------------------------------------------------------------
void InitializeBat(void)
{

	bat.PosX =
	bat.OldPosX = INITIAL_PLAYER_X;
	bat.PosY =
	bat.OldPosY = INITIAL_PLAYER_Y;
	bat.VelX =
	bat.VelY = 0.0f;

	bat.LeftFrameCnt = 0;

	bat.isLeftTriger = false;
	bat.isLeftUse = false;

	bat.isRightTriger = false;
	bat.isRightUse = false;
	
	//反射音音
	strcpy(bat.sound, "MUSICDATA\\hansya.mp3");
	bat.SoundHandle = opensound(bat.sound);
	if (bat.SoundHandle)
	{
		setvolume(bat.SoundHandle, 60);
	}

}

//-------------------------------------------------------------------
// バット更新
//-------------------------------------------------------------------
void UpdateBat(void)
{
	if (inport(PM_LEFT))
	{//左クリック
		if (!bat.isLeftTriger && !bat.isRightUse)
		{//トリガー処理
			bat.isLeftTriger = true;
			bat.isLeftUse = true;
		}
	}
	else
	{//左クリックしてないとき
		bat.isLeftTriger = false;
	}

	if (inport(PM_RIGHT))
	{//右クリック
		if (!bat.isRightTriger && !bat.isLeftUse)
		{//トリガー処理
			bat.isRightTriger = true;
			bat.isRightUse = true;
		}
	}

	else
	{//右クリックしてないとき
		bat.isRightTriger = false;
	}

	if (bat.isLeftUse)
	{//バットを使用していたら

		//フレームカウンター増加
		bat.LeftFrameCnt++;

		if (bat.LeftFrameCnt % 3 == 0)
		{
			//3フレーム前の座標取得
			bat.OldPosX = bat.PosX;
			bat.OldPosY = bat.PosY;

			//3フレームごとにプレイヤー構造体のアドレス取得
			PLAYER* pPlayer = GetPlayer();

			//3フレームごとにプレイヤーの位置取得
			bat.PosX = pPlayer->PosX;
			bat.PosY = pPlayer->PosY;
		}


		//敵の弾の先頭アドレス取得
		ENEMYBULLET* pEnemyBullet = GetEnemyBullet();

		for (int i = 0; i < MAX_ENEMYBULLET; i++)
		{

			if ((pEnemyBullet + i)->isUse)
			{//敵の弾が使用していたら

				//弾
				int ATop = (int)(pEnemyBullet + i)->PosY;
				int ABottom = (int)(pEnemyBullet + i)->PosY;
				int ARight = (int)(pEnemyBullet + i)->PosX + (ENEMYBULLET_SIZE - 0.1f);
				int ALeft = (int)(pEnemyBullet + i)->PosX;

				int ATopOld = (int)(pEnemyBullet + i)->OldPosY;
				int ABottomOld = (int)(pEnemyBullet + i)->OldPosY;
				int ARightOld = (int)(pEnemyBullet + i)->OldPosX + (ENEMYBULLET_SIZE - 0.1f);
				int ALeftOld = (int)(pEnemyBullet + i)->OldPosX;


				if (bat.LeftFrameCnt < 3)
				{//左
					int BTopOld = (int)bat.OldPosY;
					int BBottomOld = (int)bat.OldPosY;
					int BRightOld = (int)bat.OldPosX;
					int BLeftOld = (int)bat.OldPosX - 6;

					int BTop = (int)bat.PosY;
					int BBottom = (int)bat.PosY;
					int BRight = (int)bat.PosX;
					int BLeft = (int)bat.PosX - 6;

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
						(pEnemyBullet + i)->isUse = false;

						//残像の描画位置設定
						gotoxy((int)(pEnemyBullet + i)->OldPosX, (int)(pEnemyBullet + i)->OldPosY);

						//残像を消す
						printf(" ");

						//Y方向のスピードだけ－にして弾を発射
						SetBullet((pEnemyBullet + i)->PosX, (pEnemyBullet + i)->PosY, (pEnemyBullet + i)->VelX, -(pEnemyBullet + i)->VelY);

						//反射音
						playsound(bat.SoundHandle, 0);
					}

				}

				else if (bat.LeftFrameCnt < 6)
				{//左上
					int BTopOld = (int)bat.OldPosY;
					int BBottomOld = (int)bat.OldPosY;
					int BRightOld = (int)bat.OldPosX;
					int BLeftOld = (int)bat.OldPosX - 6;

					int BTop = (int)bat.PosY - 3;
					int BBottom = (int)bat.PosY;
					int BRight = (int)bat.PosX;
					int BLeft = (int)bat.PosX - 5;

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
						(pEnemyBullet + i)->isUse = false;

						//残像の描画位置設定
						gotoxy((int)(pEnemyBullet + i)->OldPosX, (int)(pEnemyBullet + i)->OldPosY);

						//残像を消す
						printf(" ");

						if ((pEnemyBullet + i)->VelX < 0)
						{//左向きに弾が動いていたら
							//X方向とY方向のスピードを－にして弾を発射
							SetBullet((pEnemyBullet + i)->PosX, (pEnemyBullet + i)->PosY, -(pEnemyBullet + i)->VelX, -(pEnemyBullet + i)->VelY);
						}

						else
						{//右向きに弾が動いていたら
							//Y方向のスピードを－にして弾を発射
							SetBullet((pEnemyBullet + i)->PosX, (pEnemyBullet + i)->PosY, (pEnemyBullet + i)->VelX, -(pEnemyBullet + i)->VelY);
						}

						//反射音
						playsound(bat.SoundHandle, 0);
					}

				}

				else if (bat.LeftFrameCnt < 9)
				{//真上

					int BTopOld = (int)bat.OldPosY - 3;
					int BBottomOld = (int)bat.OldPosY;
					int BRightOld = (int)bat.OldPosX;
					int BLeftOld = (int)bat.OldPosX - 5;

					int BTop = (int)bat.PosY - 3;
					int BBottom = (int)bat.PosY;
					int BRight = (int)bat.PosX + (BAT_SIZE_X - 0.1f);
					int BLeft = (int)bat.PosX;

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
						(pEnemyBullet + i)->isUse = false;

						//残像の描画位置設定
						gotoxy((int)(pEnemyBullet + i)->OldPosX, (int)(pEnemyBullet + i)->OldPosY);

						//残像を消す
						printf(" ");

						//X方向のスピードを０に、Y方向のスピードを－にして弾を発射
						SetBullet((pEnemyBullet + i)->PosX, (pEnemyBullet + i)->PosY - 1.0f, 0.0f, -(pEnemyBullet + i)->VelY);

						//反射音
						playsound(bat.SoundHandle, 0);
					}

				}

				else if (bat.LeftFrameCnt < 12)
				{//残像処理のとき

					int BTopOld = (int)bat.OldPosY - 3;
					int BBottomOld = (int)bat.OldPosY;
					int BRightOld = (int)bat.OldPosX;
					int BLeftOld = (int)bat.OldPosX;

					int BTop = (int)bat.PosY - 3;
					int BBottom = (int)bat.PosY;
					int BRight = (int)bat.PosX + (BAT_SIZE_X - 0.1f);
					int BLeft = (int)bat.PosX;

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
						(pEnemyBullet + i)->isUse = false;

						//残像の描画位置設定
						gotoxy((int)(pEnemyBullet + i)->OldPosX, (int)(pEnemyBullet + i)->OldPosY);

						//残像を消す
						printf(" ");

						//X方向のスピードを０に、Y方向のスピードを－にして弾を発射
						SetBullet((pEnemyBullet + i)->PosX, (pEnemyBullet + i)->PosY - 1.0f, 0.0f, -(pEnemyBullet + i)->VelY);

						//反射音
						playsound(bat.SoundHandle, 0);
					}

				}

				if (
					((int)pEnemyBullet[i].OldPosX >= MIN_FIELD_X - 1 && (int)pEnemyBullet[i].OldPosX <= MAX_FIELD_X + 1) &&
					((int)pEnemyBullet[i].OldPosY == FIELD_POSY)
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

		}

		if (bat.LeftFrameCnt > 12)
		{
			bat.LeftFrameCnt = 0;
			bat.isLeftUse = false;
		}

	}

	if (bat.isRightUse)
	{
		//フレームカウンター増加
		bat.RightFrameCnt++;

		if (bat.RightFrameCnt % 3 == 0)
		{
			//3フレーム前の座標取得
			bat.OldPosX = bat.PosX;
			bat.OldPosY = bat.PosY;

			//3フレームごとにプレイヤー構造体のアドレス取得
			PLAYER* pPlayer = GetPlayer();

			//3フレームごとにプレイヤーの位置取得
			bat.PosX = pPlayer->PosX;
			bat.PosY = pPlayer->PosY;
		}


		//敵の弾の先頭アドレス取得
		ENEMYBULLET* pEnemyBullet = GetEnemyBullet();

		for (int i = 0; i < MAX_ENEMYBULLET; i++)
		{

			if ((pEnemyBullet + i)->isUse)
			{//敵の弾が使用していたら

				//弾
				int ATop = (int)(pEnemyBullet + i)->PosY;
				int ABottom = (int)(pEnemyBullet + i)->PosY;
				int ARight = (int)(pEnemyBullet + i)->PosX + (ENEMYBULLET_SIZE - 0.1f);
				int ALeft = (int)(pEnemyBullet + i)->PosX;

				int ATopOld = (int)(pEnemyBullet + i)->OldPosY;
				int ABottomOld = (int)(pEnemyBullet + i)->OldPosY;
				int ARightOld = (int)(pEnemyBullet + i)->OldPosX + (ENEMYBULLET_SIZE - 0.1f);
				int ALeftOld = (int)(pEnemyBullet + i)->OldPosX;


				if (bat.RightFrameCnt < 3)
				{//右
					int BTopOld = (int)bat.OldPosY;
					int BBottomOld = (int)bat.OldPosY;
					int BRightOld = (int)bat.OldPosX + 6;
					int BLeftOld = (int)bat.OldPosX;

					int BTop = (int)bat.PosY;
					int BBottom = (int)bat.PosY;
					int BRight = (int)bat.PosX + 6;
					int BLeft = (int)bat.PosX;

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
						(pEnemyBullet + i)->isUse = false;

						//残像の描画位置設定
						gotoxy((int)(pEnemyBullet + i)->OldPosX, (int)(pEnemyBullet + i)->OldPosY);

						//残像を消す
						printf(" ");

						//Y方向のスピードだけ－にして弾を発射
						SetBullet((pEnemyBullet + i)->PosX, (pEnemyBullet + i)->PosY, (pEnemyBullet + i)->VelX, -(pEnemyBullet + i)->VelY);

						//反射音
						playsound(bat.SoundHandle, 0);

					}

				}

				else if (bat.RightFrameCnt < 6)
				{//右上
					int BTopOld = (int)bat.OldPosY;
					int BBottomOld = (int)bat.OldPosY;
					int BRightOld = (int)bat.OldPosX + 6;
					int BLeftOld = (int)bat.OldPosX;

					int BTop = (int)bat.PosY - 3;
					int BBottom = (int)bat.PosY;
					int BRight = (int)bat.PosX + 6;
					int BLeft = (int)bat.PosX;

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
						(pEnemyBullet + i)->isUse = false;

						//残像の描画位置設定
						gotoxy((int)(pEnemyBullet + i)->OldPosX, (int)(pEnemyBullet + i)->OldPosY);

						//残像を消す
						printf(" ");

						if ((pEnemyBullet + i)->VelX > 0)
						{//右向きに弾が動いていたら
							//X方向とY方向のスピードを－にして弾を発射
							SetBullet((pEnemyBullet + i)->PosX, (pEnemyBullet + i)->PosY, -(pEnemyBullet + i)->VelX, -(pEnemyBullet + i)->VelY);
						}

						else
						{//左向きに弾が動いていたら
							//Y方向のスピードを－にして弾を発射
							SetBullet((pEnemyBullet + i)->PosX, (pEnemyBullet + i)->PosY, (pEnemyBullet + i)->VelX, -(pEnemyBullet + i)->VelY);
						}

						//反射音
						playsound(bat.SoundHandle, 0);
					}

				}

				else if (bat.RightFrameCnt < 9)
				{//真上

					int BTopOld = (int)bat.OldPosY - 3;
					int BBottomOld = (int)bat.OldPosY;
					int BRightOld = (int)bat.OldPosX + 6;
					int BLeftOld = (int)bat.OldPosX;

					int BTop = (int)bat.PosY - 3;
					int BBottom = (int)bat.PosY;
					int BRight = (int)bat.PosX + (BAT_SIZE_X - 0.1f);
					int BLeft = (int)bat.PosX;

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
						(pEnemyBullet + i)->isUse = false;

						//残像の描画位置設定
						gotoxy((int)(pEnemyBullet + i)->OldPosX, (int)(pEnemyBullet + i)->OldPosY);

						//残像を消す
						printf(" ");

						//X方向のスピードを０に、Y方向のスピードを－にして弾を発射
						SetBullet((pEnemyBullet + i)->PosX, (pEnemyBullet + i)->PosY - 1.0f, 0.0f, -(pEnemyBullet + i)->VelY);

						//反射音
						playsound(bat.SoundHandle, 0);
					}

				}

				else if (bat.RightFrameCnt < 12)
				{//残像処理のとき

					int BTopOld = (int)bat.OldPosY - 3;
					int BBottomOld = (int)bat.OldPosY;
					int BRightOld = (int)bat.OldPosX;
					int BLeftOld = (int)bat.OldPosX;

					int BTop = (int)bat.PosY - 3;
					int BBottom = (int)bat.PosY;
					int BRight = (int)bat.PosX + (BAT_SIZE_X - 0.1f);
					int BLeft = (int)bat.PosX;

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
						(pEnemyBullet + i)->isUse = false;

						//残像の描画位置設定
						gotoxy((int)(pEnemyBullet + i)->OldPosX, (int)(pEnemyBullet + i)->OldPosY);

						//残像を消す
						printf(" ");

						//X方向のスピードを０に、Y方向のスピードを－にして弾を発射
						SetBullet((pEnemyBullet + i)->PosX, (pEnemyBullet + i)->PosY - 1.0f, 0.0f, -(pEnemyBullet + i)->VelY);

						//反射音
						playsound(bat.SoundHandle, 0);
					}
				}

				if (
					((int)pEnemyBullet[i].OldPosX >= MIN_FIELD_X - 1 && (int)pEnemyBullet[i].OldPosX <= MAX_FIELD_X + 1) &&
					((int)pEnemyBullet[i].OldPosY == FIELD_POSY)
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

		}
		if (bat.RightFrameCnt > 12)
		{
			bat.RightFrameCnt = 0;
			bat.isRightUse = false;
		}
	}

}

//-------------------------------------------------------------------
// バット描画
//-------------------------------------------------------------------
void DrawBat(void)
{
	if (bat.isLeftUse)
	{//左バットを使用していたら
		textcolor(RED);

		if (bat.LeftFrameCnt < 3)
		{//左
			if ((int)bat.PosX - 6 > 1)
			{//画面にすべて収まる
				gotoxy((int)bat.PosX - 6, (int)bat.PosY);
				printf("■■■");
			}

			else if ((int)bat.PosX - 4 > 1)
			{//２個までなら画面に収まる
				gotoxy((int)bat.PosX - 4, (int)bat.PosY);
				printf("■■");
			}

			else if ((int)bat.PosX - 2 > 1)
			{//１個なら画面に収まる
				gotoxy((int)bat.PosX - 2, (int)bat.PosY);
				printf("■");
			}

		}

		else if (bat.LeftFrameCnt < 6)
		{//左上
			
			if ((int)bat.OldPosX - 6 > 1)
			{//画面にすべて収まる
				gotoxy((int)bat.OldPosX - 6, (int)bat.OldPosY);
				printf("　　　");
			}

			else if ((int)bat.OldPosX - 4 > 1)
			{//２個までなら画面に収まる
				gotoxy((int)bat.OldPosX - 4, (int)bat.OldPosY);
				printf("　　");
			}

			else if ((int)bat.OldPosX - 2 > 1)
			{//１個なら画面に収まる
				gotoxy((int)bat.OldPosX - 2, (int)bat.OldPosY);
				printf("　");
			}
			
			if ((int)bat.PosX - 5 > 1)
			{//画面に３つ収まる
				gotoxy((int)bat.PosX - 5, (int)bat.PosY - 3);
				printf("■");
			}

			if ((int)bat.PosX - 3 > 1)
			{//画面に２つ以上収まる
				gotoxy((int)bat.PosX - 3, (int)bat.PosY - 2);
				printf("■");
			}

			if ((int)bat.PosX - 1 > 1)
			{//画面に１つ以上収まる
				gotoxy((int)bat.PosX - 1, (int)bat.PosY - 1);
				printf("■");
			}
			
		}

		else if (bat.LeftFrameCnt < 9)
		{//真上

			if ((int)bat.OldPosX - 5 > 1)
			{//画面に３つ収まる
				gotoxy((int)bat.OldPosX - 5, (int)bat.OldPosY - 3);
				printf("　");

				if ((int)bat.OldPosY - 3 == FIELD_POSY)
				{//フィールドに重なるとき
					textcolor(WHITE);
					gotoxy(MIN_FIELD_X, FIELD_POSY);
					printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
					textcolor(RED);
				}

			}

			if ((int)bat.OldPosX - 3 > 1)
			{//画面に２つ以上収まる
				gotoxy((int)bat.OldPosX - 3, (int)bat.OldPosY - 2);
				printf("　");

				if ((int)bat.OldPosY - 2 == FIELD_POSY)
				{//フィールドに重なるとき
					textcolor(WHITE);
					gotoxy(MIN_FIELD_X, FIELD_POSY);
					printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
					textcolor(RED);
				}

			}

			if ((int)bat.OldPosX - 1 > 1)
			{//画面に１つ以上収まる
				gotoxy((int)bat.OldPosX - 1, (int)bat.OldPosY - 1);
				printf("　");

				if ((int)bat.OldPosY - 1 == FIELD_POSY)
				{//フィールドに重なるとき
					textcolor(WHITE);
					gotoxy(MIN_FIELD_X, FIELD_POSY);
					printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
					textcolor(RED);
				}

			}

			gotoxy(bat.PosX, bat.PosY - 3);
			printf("■");
			gotoxy(bat.PosX, bat.PosY - 2);
			printf("■");
			gotoxy(bat.PosX, bat.PosY - 1);
			printf("■");

		}

		else if (bat.LeftFrameCnt < 12)
		{//残像消し
			gotoxy(bat.OldPosX, bat.OldPosY - 3);
			printf("　");

			if ((int)bat.OldPosY - 3 == FIELD_POSY)
			{//フィールドに重なるとき
				textcolor(WHITE);
				gotoxy(MIN_FIELD_X, FIELD_POSY);
				printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
				textcolor(RED);
			}

			gotoxy(bat.OldPosX, bat.OldPosY - 2);
			printf("　");

			if ((int)bat.OldPosY - 2 == FIELD_POSY)
			{//フィールドに重なるとき
				textcolor(WHITE);
				gotoxy(MIN_FIELD_X, FIELD_POSY);
				printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
				textcolor(RED);
			}

			gotoxy(bat.OldPosX, bat.OldPosY - 1);
			printf("　");

			if ((int)bat.OldPosY - 1 == FIELD_POSY)
			{//フィールドに重なるとき
				textcolor(WHITE);
				gotoxy(MIN_FIELD_X, FIELD_POSY);
				printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
				textcolor(RED);
			}

		}
		
		textcolor(WHITE);
	}

	if (bat.isRightUse)
	{//右バットを使用していたら
		textcolor(RED);

		if (bat.RightFrameCnt < 3)
		{//右

			if ((int)bat.PosX + 7 < 80)
			{//画面に3つ収まる
				gotoxy((int)bat.PosX + 1, (int)bat.PosY);
				printf("■■■");
			}

			else if ((int)bat.PosX + 5 < 80)
			{//画面に２つ収まる
				gotoxy((int)bat.PosX + 1, (int)bat.PosY);
				printf("■■");
			}

			else if ((int)bat.PosX + 3 < 80)
			{//画面に１つ収まる
				gotoxy((int)bat.PosX + 1, (int)bat.PosY);
				printf("■");
			}

		}

		else if (bat.RightFrameCnt < 6)
		{//右上
			//残像処理
			if ((int)bat.OldPosX + 7 < 80)
			{//３つ入ってる
				gotoxy((int)bat.OldPosX + 1, (int)bat.OldPosY);
				printf("　　　");
			}

			else if ((int)bat.OldPosX + 5 < 80)
			{//２つ入ってる
				gotoxy((int)bat.OldPosX + 1, (int)bat.OldPosY);
				printf("　　");
			}

			else if ((int)bat.OldPosX + 3 < 80)
			{//１つ入ってる
				gotoxy((int)bat.OldPosX + 1, (int)bat.OldPosY);
				printf("　");
			}

			if (bat.PosX + 5 < 80)
			{
				gotoxy(bat.PosX + 5, bat.PosY - 3);
				printf("■");
			}

			if (bat.PosX + 3 < 80)
			{
				gotoxy(bat.PosX + 3, bat.PosY - 2);
				printf("■");
			}

			if (bat.PosX + 1 < 80)
			{
				gotoxy(bat.PosX + 1, bat.PosY - 1);
				printf("■");
			}
		}

		else if (bat.RightFrameCnt < 9)
		{//真上

			//残像処理
			if (bat.OldPosX + 5 < 80)
			{
				gotoxy(bat.OldPosX + 5, bat.OldPosY - 3);
				printf("　");

				if ((int)bat.OldPosY - 3 == FIELD_POSY)
				{//フィールドに重なるとき
					textcolor(WHITE);
					gotoxy(MIN_FIELD_X, FIELD_POSY);
					printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
					textcolor(RED);
				}
			}

			if (bat.OldPosX + 3 < 80)
			{
				gotoxy(bat.OldPosX + 3, bat.OldPosY - 2);
				printf("　");

				if ((int)bat.OldPosY - 2 == FIELD_POSY)
				{//フィールドに重なるとき
					textcolor(WHITE);
					gotoxy(MIN_FIELD_X, FIELD_POSY);
					printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
					textcolor(RED);
				}
			}

			if (bat.PosX + 1 < 80)
			{
				gotoxy(bat.OldPosX + 1, bat.OldPosY - 1);
				printf("　");

				if ((int)bat.OldPosY - 1 == FIELD_POSY)
				{//フィールドに重なるとき
					textcolor(WHITE);
					gotoxy(MIN_FIELD_X, FIELD_POSY);
					printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
					textcolor(RED);
				}
			}

			gotoxy(bat.PosX, bat.PosY - 3);
			printf("■");
			gotoxy(bat.PosX, bat.PosY - 2);
			printf("■");
			gotoxy(bat.PosX, bat.PosY - 1);
			printf("■");

		}

		else if (bat.RightFrameCnt < 12)
		{
			gotoxy(bat.OldPosX, bat.OldPosY - 3);
			printf("　");

			if ((int)bat.OldPosY - 3 == FIELD_POSY)
			{//フィールドに重なるとき
				textcolor(WHITE);
				gotoxy(MIN_FIELD_X, FIELD_POSY);
				printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
				textcolor(RED);
			}

			gotoxy(bat.OldPosX, bat.OldPosY - 2);
			printf("　");

			if ((int)bat.OldPosY - 2 == FIELD_POSY)
			{//フィールドに重なるとき
				textcolor(WHITE);
				gotoxy(MIN_FIELD_X, FIELD_POSY);
				printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
				textcolor(RED);
			}

			gotoxy(bat.OldPosX, bat.OldPosY - 1);
			printf("　");

			if ((int)bat.OldPosY - 1 == FIELD_POSY)
			{//フィールドに重なるとき
				textcolor(WHITE);
				gotoxy(MIN_FIELD_X, FIELD_POSY);
				printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
				textcolor(RED);
			}
		}

		textcolor(WHITE);
	}

}

//-------------------------------------------------------------------
// バット終了
//-------------------------------------------------------------------
void FinalizeBat(void)
{
	closesound(bat.SoundHandle);
}

//-------------------------------------------------------------------
// バットのゲッター
//-------------------------------------------------------------------
BAT* GetBat(void)
{
	return &bat;
}
