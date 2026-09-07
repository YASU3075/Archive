//-----------------------------------------------
// Title.cpp
// 制作日：2025/08/07
// 制作者：安田晴人
//-----------------------------------------------
#include <conio.h>
#include <stdio.h>
#include "Title.h"
#include "GameScene.h"
#include "Manager.h"

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void Title::Update()
{
	m_time += 1.0f / 60.0f;

	if (_kbhit())
	{
		int i = _getch();
		if (i == '\r')
		{
			Manager::GetInstance()->SetNextScene(new GameScene()); // 次のシーンを設定
		}
		else if (i == 'q')
		{
			Manager::GetInstance()->SetQuit(true); // ゲームを終了
		}
		else
		{
			return; // 他のキーは無視
		}
	}
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void Title::Draw()
{
	printf("\x1b[%d;%dHチキチキシューティングゲーム！", 4, 5);
	printf("\x1b[%d;%dHゲームの説明:", 7, 5);
	printf("\x1b[%d;%dH敵の撃ってくる弾をぎりぎりまでひきつけて弾を当ててスコアを稼ごう！", 8, 5);
	printf("\x1b[%d;%dH操作方法：A,1キーで左移動　D,3キーで右移動　スペースキーで弾を発射", 9, 5);
	printf("\x1b[%d;%dH目指せ!!ハイスコア!!", 11, 5);

	printf("\x1b[%d;%dHEnterキーでゲームスタート", 15, 5);

	printf("\x1b[%d;%dHqキーでゲームを終了", 17, 5);
}

