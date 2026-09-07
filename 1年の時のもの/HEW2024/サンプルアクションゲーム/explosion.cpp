//-------------------------------------------------------------------
// explosion.cpp アクションゲームサンプル
// 制作日：2024/01/30 制作者：安田晴人
//-------------------------------------------------------------------
#include "main.h"
#include "explosion.h"
#include "field.h"

#define CONIOEX
#include "conioex.h"

//-------------------------------------------------------------------
// グローバル変数
//-------------------------------------------------------------------
EXPLOSION explosion[MAX_EXPLOSION];		//爆発アニメーションの実体

//-------------------------------------------------------------------
// 爆発アニメーション初期化
//-------------------------------------------------------------------
void InitializeExplosion(void) 
{
	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		explosion[i].PosX =
		explosion[i].PosY = 0.0f;

		explosion[i].frameCnt = 0;

		explosion[i].isUse = false;
	}
}

//-------------------------------------------------------------------
// 爆発アニメーション更新
//-------------------------------------------------------------------
void UpdateExplosion(void)
{
	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		if (explosion[i].isUse)
		{//使用していたら
			explosion[i].frameCnt++;

			if (explosion[i].frameCnt > 25)
			{//25フレーム経ったら
				explosion[i].isUse = false;
			}

		}
	}
}

//-------------------------------------------------------------------
// 爆発アニメーション描画
//-------------------------------------------------------------------
void DrawExplosion(void)
{

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{

		if (explosion[i].isUse)
		{//使用していたら
		
			//色設定
			textcolor(YELLOW);

			//爆発アニメーション実体の描画位置設定
			gotoxy((int)explosion[i].PosX, (int)explosion[i].PosY);

			//画面表示
			if (explosion[i].frameCnt < 5)
			{
				printf("●");
			}

			else if (explosion[i].frameCnt < 10)
			{
				printf("〇");
			}

			else if (explosion[i].frameCnt < 15)
			{
				printf("※");
			}

			else if (explosion[i].frameCnt < 20)
			{
				printf("・");
			}

			else if (explosion[i].frameCnt < 25)
			{
				printf("　");
			}

			//色をもとに戻す
			textcolor(WHITE);
		}

	}
}

//-------------------------------------------------------------------
// 爆発アニメーション終了
//-------------------------------------------------------------------
void FinalizeExplosion(void)
{
	//２年生になってテクスチャーとかモデルとかを読み込んだ時に解放するのに使う
	//今のところはあまり使わないが癖づけておく
}

//-------------------------------------------------------------------
// 爆発アニメーションのセッター
//-------------------------------------------------------------------
void SetExplosion(float px, float py)
{

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		if (!explosion[i].isUse)
		{
			explosion[i].PosX = px;
			explosion[i].PosY = py;

			explosion[i].frameCnt = 0;

			explosion[i].isUse = true;
			break;

		}
	}

}

//-------------------------------------------------------------------
// 爆発アニメーションのゲッター（実体の先頭アドレスを取得する関数）
//-------------------------------------------------------------------
EXPLOSION* GetExplosion(void)
{
	return explosion;
}


