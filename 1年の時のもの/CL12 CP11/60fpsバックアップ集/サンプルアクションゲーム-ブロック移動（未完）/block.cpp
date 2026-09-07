//-----------------------------------------------
// block.cpp アクションゲームサンプル
// 制作日：2024/01/24 制作者：安田晴人
//-----------------------------------------------
#include "main.h"
#include "block.h"
#include "field.h"
#include "bullet.h"

#define CONIOEX
#include "conioex.h"

//-----------------------------------------------
// グローバル変数
//-----------------------------------------------
BLOCK block[MAX_BLOCK];		//ブロックの実体

//-----------------------------------------------
// ブロック初期化
//-----------------------------------------------
void InitializeBlock(void) 
{
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		block[i].PosX =
		block[i].OldPosX = 0.0f;
		block[i].PosY =
		block[i].OldPosY = 0.0f;
		block[i].VelX = 0.0f;
	
		block[i].Size = 0;

		block[i].type = BLOCKTYPE_NORMAL;
		block[i].frameCnt = 0;

		block[i].isUse = false;
	}

	//ブロックの設置
	SetBlock(30.0f, 26.0f, 5, -0.05f, BLOCKTYPE_REVERSE);

}

//-----------------------------------------------
// ブロック更新
//-----------------------------------------------
void UpdateBlock(void)
{
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		if (block[i].isUse)
		{//使用していたら

			//前フレームの座標を格納
			block[i].OldPosX = block[i].PosX;
			block[i].OldPosY = block[i].PosY;

			switch (block[i].type)
			{
			case BLOCKTYPE_NORMAL:
				break;

			case BLOCKTYPE_REVERSE:
				//移動
				block[i].PosX += block[i].VelX;

				block[i].frameCnt++;

				if (block[i].frameCnt >= 60)
				{//６０フレーム経ったら
					//反転
					block[i].VelX *= -1;

					//リセット
					block[i].frameCnt = 0;
				}


				break;

			default:break;
			}

			//壁判定
			if (block[i].PosX <= MIN_BLOCK_X || block[i].PosX >= MAX_BLOCK_X || block[i].PosY <= MIN_BLOCK_Y || block[i].PosY >= MAX_BLOCK_Y)
			{
				//消す
				block[i].isUse = false;

				//残像の描画位置設定
				gotoxy((int)block[i].OldPosX, (int)block[i].OldPosY);

				//残像を消す
				printf(" ");

			}

		

			//弾とブロックの当たり判定処理
			BULLET* pBullet = GetBullet();		//弾実体の先頭アドレスを取得

			for (int j = 0; j < MAX_BULLET; j++)
			{
				if ((pBullet + j)->isUse)
				{//ブロックも弾もどちらも使っている場合
					
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


					//ブロック
					int BTop = (int)block[i].PosY;
					int BBottom = (int)block[i].PosY + (BLOCK_SIZE - 0.1f);
					int BRight = (int)block[i].PosX + (block[i].Size - 1);
					int BLeft = (int)block[i].PosX;

					int BTopOld = (int)block[i].OldPosY;
					int BBottomOld = (int)block[i].OldPosY + (BLOCK_SIZE - 0.1f);
					int BRightOld = (int)block[i].OldPosX + (block[i].Size - 1);
					int BLeftOld = (int)block[i].OldPosX;



					if (
						(ATop <= BBottom && ABottom >= BTop && ARight >= BLeft && ALeft <= BRight) ||		//重なったとき
						(ARightOld < BLeft && ALeft > BRight && ATop <= BBottom && ABottom >= BTop) ||		//左から右すり抜けたとき
						(ALeftOld > BRight && ARight <= BLeft && ATop <= BBottom && ABottom >= BTop) ||		//右から左すり抜けたとき
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

					}

					
				}
			}
		}
	}
}

//-----------------------------------------------
// ブロック描画
//-----------------------------------------------
void DrawBlock(void)
{
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		if (block[i].isUse)
		{//使用していたら

			//もし描画位置が動いていたら残像処理
			if ((int)block[i].OldPosX != (int)block[i].PosX || (int)block[i].OldPosY != (int)block[i].PosY)
			{
				//残像の描画位置設定
				gotoxy((int)block[i].OldPosX, (int)block[i].OldPosY);

				//残像を消す
				printf(" ");
			}

			//色設定
			textcolor(MAGENTA);

			//ブロック実体の描画位置設定
			gotoxy((int)block[i].PosX, (int)block[i].PosY);

			char buff[128] = {};		//表示用バッファ
			for (int j = 0; j < block[i].Size; j++)
			{
				strcat(buff, "#");
			}

			//画面表示
			printf("%s", buff);

			char MirageBuff[128] = {};	//残像用バッファ
			for (int j = 0; j < block[i].Size; j++)
			{
				strcat(MirageBuff, " ");
			}

			//画面表示
			printf("%s", MirageBuff);

			//色をもとに戻す
			textcolor(WHITE);
		}
	}
}

//-----------------------------------------------
// ブロック終了
//-----------------------------------------------
void FinalizeBlock(void)
{
	//２年生になってテクスチャーとかモデルとかを読み込んだ時に解放するのに使う
	//今のところはあまり使わないが癖づけておく
}

//-----------------------------------------------
// ブロックのセッター
//-----------------------------------------------
void SetBlock(float px, float py, int s, float vx, BLOCKTYPE t)
{
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		if (!block[i].isUse)
		{
			block[i].PosX = px;
			block[i].OldPosX = px;
			block[i].PosY = py;
			block[i].OldPosY = py;

			block[i].VelX = vx;

			block[i].Size = s;

			block[i].type = t;
			block[i].frameCnt = 0;

			block[i].isUse = true;
			break;

		}
	}
}

//-------------------------------------------------------------------
// ブロックのゲッター（実体の先頭アドレスを取得する関数）
//-------------------------------------------------------------------
BLOCK* GetBlock(void)
{
	return block;
}

