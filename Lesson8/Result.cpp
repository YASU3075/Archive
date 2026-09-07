//-----------------------------------------------
// Result.cpp
// 制作日：2025/08/07
// 制作者：安田晴人
//-----------------------------------------------
#include <stdio.h>
#include <conio.h>
#include "Result.h"
#include "Manager.h"
#include "Title.h"

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void Result::Update()
{
	m_time += 1.0f / 60.0f;
	if (_kbhit())
	{
		int i = _getch();
		if (i == '\r' || i == 'q')
		{
			if (i == '\r') 
			{
				Manager::GetInstance()->SetNextScene(new Title()); // タイトルシーンへ遷移
			}
			else if (i == 'q') // qキーが押された場合
			{
				Manager::GetInstance()->SetQuit(true);
			}
		}
	}
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void Result::Draw()
{
	printf("\x1b[%d;%dHゲーム終了!", 5, 5);
	printf("\x1b[%d;%dH結果:%d", 6, 5, m_score);
	printf("\x1b[%d;%dHEnterキーでタイトルへ戻る", 8, 5);
	printf("\x1b[%d;%dHqキーでゲームを終了", 9, 5);
}