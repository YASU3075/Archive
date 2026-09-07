//-----------------------------------------------
// countdown.cpp アクションカウントダウンサンプル
// 制作日：2024/01/31 制作者：安田晴人
//-----------------------------------------------
#include "main.h"
#include "countdown.h"

#define CONIOEX
#include "conioex.h"

//-----------------------------------------------
// グローバル変数
//-----------------------------------------------
COUNTDOWN countdown;

//-----------------------------------------------
// カウントダウンシーン初期化
//-----------------------------------------------
void InitializeCountdown() 
{
	countdown.FrameCnt = 0;

	strcpy(countdown.map[0],  "                                                                               ");
	strcpy(countdown.map[1],  "                                                                               ");
	strcpy(countdown.map[2],  "                           ■■■■■■■■■■                                ");
	strcpy(countdown.map[3],  "                           ■■■■■■■■■■■                              ");
	strcpy(countdown.map[4],  "                                               ■■                            ");
	strcpy(countdown.map[5],  "                                               ■■                            ");
	strcpy(countdown.map[6],  "                                               ■■                            ");
	strcpy(countdown.map[7],  "                                               ■■                            ");
	strcpy(countdown.map[8],  "                                               ■■                            ");
	strcpy(countdown.map[9],  "                           ■■■■■■■■■■■                              ");
	strcpy(countdown.map[10], "                           ■■■■■■■■■■■                              ");
	strcpy(countdown.map[11], "                                               ■■                            ");
	strcpy(countdown.map[12], "                                               ■■                            ");
	strcpy(countdown.map[13], "                                               ■■                            ");
	strcpy(countdown.map[14], "                                               ■■                            ");
	strcpy(countdown.map[15], "                                               ■■                            ");
	strcpy(countdown.map[16], "                           ■■■■■■■■■■■■                            ");
	strcpy(countdown.map[17], "                           ■■■■■■■■■■■                              ");
	strcpy(countdown.map[18], "                                                                               ");
	strcpy(countdown.map[19], "                                                                               ");
	
	//カウント
	strcpy(countdown.CountSound, "MUSICDATA\\countdown.mp3");
	countdown.CountSoundHandle = opensound(countdown.CountSound);
	if (countdown.CountSoundHandle)
	{
		setvolume(countdown.CountSoundHandle, 70);
	}

	//スタート
	strcpy(countdown.StartSound, "MUSICDATA\\start.mp3");
	countdown.StartSoundHandle = opensound(countdown.StartSound);
	if (countdown.StartSoundHandle)
	{
		setvolume(countdown.StartSoundHandle, 80);
	}

	//カウント音
	playsound(countdown.CountSoundHandle, 0);
}

//-----------------------------------------------
// カウントダウンシーン更新
//-----------------------------------------------
void UpdateCountdown()
{
	if (countdown.FrameCnt <= 240)
	{
		//フレーム数増加
		countdown.FrameCnt++;

		if (countdown.FrameCnt == 60)
		{
			strcpy(countdown.map[0],  "                                                                               ");
			strcpy(countdown.map[1],  "                                                                               ");
			strcpy(countdown.map[2],  "                             ■■■■■■■■■                                ");
			strcpy(countdown.map[3],  "                           ■■■■■■■■■■■                              ");
			strcpy(countdown.map[4],  "                           ■■                ■■                            ");
			strcpy(countdown.map[5],  "                           ■■                ■■                            ");
			strcpy(countdown.map[6],  "                                               ■■                            ");
			strcpy(countdown.map[7],  "                                              ■■                             ");
			strcpy(countdown.map[8],  "                                          ■■■                               ");
			strcpy(countdown.map[9],  "                                        ■■■                                 ");
			strcpy(countdown.map[10], "                                      ■■■                                   ");
			strcpy(countdown.map[11], "                                   ■■■                                      ");
			strcpy(countdown.map[12], "                                   ■■                                        ");
			strcpy(countdown.map[13], "                               ■■■                                          ");
			strcpy(countdown.map[14], "                             ■■■                                            ");
			strcpy(countdown.map[15], "                            ■■                                               ");
			strcpy(countdown.map[16], "                           ■■■■■■■■■■■■                            ");
			strcpy(countdown.map[17], "                           ■■■■■■■■■■■■                            ");
			strcpy(countdown.map[18], "                                                                               ");
			strcpy(countdown.map[19], "                                                                               ");

			//カウント音
			playsound(countdown.CountSoundHandle, 0);
		}

		else if (countdown.FrameCnt == 120)
		{
			strcpy(countdown.map[0],  "                                                                               ");
			strcpy(countdown.map[1],  "                                                                               ");
			strcpy(countdown.map[2],  "                                   ■■■■                                    ");
			strcpy(countdown.map[3],  "                                 ■■■■■                                    ");
			strcpy(countdown.map[4],  "                               ■■  ■■■                                    ");
			strcpy(countdown.map[5],  "                            ■■■   ■■■                                    ");
			strcpy(countdown.map[6],  "                                     ■■■                                    ");
			strcpy(countdown.map[7],  "                                     ■■■                                    ");
			strcpy(countdown.map[8],  "                                     ■■■                                    ");
			strcpy(countdown.map[9],  "                                     ■■■                                    ");
			strcpy(countdown.map[10], "                                     ■■■                                    ");
			strcpy(countdown.map[11], "                                     ■■■                                    ");
			strcpy(countdown.map[12], "                                     ■■■                                    ");
			strcpy(countdown.map[13], "                                     ■■■                                    ");
			strcpy(countdown.map[14], "                                     ■■■                                    ");
			strcpy(countdown.map[15], "                                     ■■■                                    ");
			strcpy(countdown.map[16], "                           ■■■■■■■■■■■■■                          ");
			strcpy(countdown.map[17], "                           ■■■■■■■■■■■■■                          ");
			strcpy(countdown.map[18], "                                                                               ");
			strcpy(countdown.map[19], "                                                                               ");

			//カウント音
			playsound(countdown.CountSoundHandle, 0);
		}

		else if (countdown.FrameCnt == 180)
		{
			strcpy(countdown.map[0],  "                                                                               ");
			strcpy(countdown.map[1],  "                                                                               ");
			strcpy(countdown.map[2],  "           ■■■■■■■■■                   ■■■■■■■■■             ");
			strcpy(countdown.map[3],  "         ■■■■■■■■■■■               ■■■■■■■■■■■           ");
			strcpy(countdown.map[4],  "       ■■■■■■■■■■■■■           ■■                  ■■         ");
			strcpy(countdown.map[5],  "       ■■                  ■■           ■■                  ■■         ");
			strcpy(countdown.map[6],  "      ■■                                 ■■                    ■■        ");
			strcpy(countdown.map[7],  "      ■■                                 ■■                    ■■        ");
			strcpy(countdown.map[8],  "     ■■                                 ■■                      ■■       ");
			strcpy(countdown.map[9],  "     ■■                                 ■■                      ■■       ");
			strcpy(countdown.map[10], "     ■■            ■■■■■■         ■■                      ■■       ");
			strcpy(countdown.map[11], "     ■■            ■■■■■■         ■■                      ■■       ");
			strcpy(countdown.map[12], "     ■■                    ■■         ■■                      ■■       ");
			strcpy(countdown.map[13], "     ■■                    ■■          ■■                    ■■        ");
			strcpy(countdown.map[14], "     ■■■                  ■■          ■■                    ■■        ");
			strcpy(countdown.map[15], "       ■■                  ■■           ■■                  ■■         ");
			strcpy(countdown.map[16], "       ■■■■■■■■■■■■■           ■■                  ■■         ");
			strcpy(countdown.map[17], "         ■■■■■■■■■■■               ■■■■■■■■■■■           ");
			strcpy(countdown.map[18], "           ■■■■■■■■■                   ■■■■■■■■■             ");
			strcpy(countdown.map[19], "                                                                               ");

			//カウント音
			playsound(countdown.StartSoundHandle, 0);
		}

		else if (countdown.FrameCnt == 240)
		{
			strcpy(countdown.map[0], "                                                                               ");
			strcpy(countdown.map[1], "                                                                               ");
			strcpy(countdown.map[2], "                                                                               ");
			strcpy(countdown.map[3], "                                                                               ");
			strcpy(countdown.map[4], "                                                                               ");
			strcpy(countdown.map[5], "                                                                               ");
			strcpy(countdown.map[6], "                                                                               ");
			strcpy(countdown.map[7], "                                                                               ");
			strcpy(countdown.map[8], "                                                                               ");
			strcpy(countdown.map[9], "                                                                               ");
			strcpy(countdown.map[10], "                                                                               ");
			strcpy(countdown.map[11], "                                                                               ");
			strcpy(countdown.map[12], "                                                                               ");
			strcpy(countdown.map[13], "                                                                               ");
			strcpy(countdown.map[14], "                                                                               ");
			strcpy(countdown.map[15], "                                                                               ");
			strcpy(countdown.map[16], "                                                                               ");
			strcpy(countdown.map[17], "                                                                               ");
			strcpy(countdown.map[18], "                                                                               ");
			strcpy(countdown.map[19], "                                                                               ");
		}

	}

	else
	{
		SetScene(SCENE_GAME);
	}

}

//-----------------------------------------------
// カウントダウンシーン描画
//-----------------------------------------------
void DrawCountdown()
{

	if (countdown.FrameCnt == 1)
	{//3
		textcolor(RED);
		for (int i = 0; i < 20; i++)
		{
			gotoxy(1, i + 1);
			printf("%s", countdown.map[i]);
		}
		textcolor(WHITE);
	}

	else if (countdown.FrameCnt == 60)
	{//2
		textcolor(RED);
		for (int i = 0; i < 20; i++)
		{
			gotoxy(1, i + 1);
			printf("%s", countdown.map[i]);
		}
		textcolor(WHITE);
	}

	else if (countdown.FrameCnt == 120)
	{//1
		textcolor(RED);
		for (int i = 0; i < 20; i++)
		{
			gotoxy(1, i + 1);
			printf("%s", countdown.map[i]);
		}
		textcolor(WHITE);
	}

	else if (countdown.FrameCnt == 180)
	{//GO
		textcolor(RED);
		for (int i = 0; i < 20; i++)
		{
			gotoxy(1, i + 1);
			printf("%s", countdown.map[i]);
		}
		textcolor(WHITE);
	}

	else if (countdown.FrameCnt == 240)
	{//消す
		textcolor(RED);
		for (int i = 0; i < 20; i++)
		{
			gotoxy(1, i + 1);
			printf("%s", countdown.map[i]);
		}
		textcolor(WHITE);
	}

	

}

//-----------------------------------------------
// カウントダウンシーン終了
//-----------------------------------------------
void FinalizeCountdown()
{
	closesound(countdown.CountSoundHandle);
	closesound(countdown.StartSoundHandle);
}

