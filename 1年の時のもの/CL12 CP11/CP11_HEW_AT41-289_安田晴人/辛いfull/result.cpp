//-----------------------------------------------
// result.cpp アクションゲームサンプル
// 制作日：2024/01/31 制作者：安田晴人
//-----------------------------------------------
#include "main.h"
#include "game.h"
#include "result.h"
#include "player.h"

#define CONIOEX
#include "conioex.h"

//-----------------------------------------------
// グローバル変数
//-----------------------------------------------
RESULT result;

char num[10][9][10 + 1] = {};


//-----------------------------------------------
// リザルト初期化
//-----------------------------------------------
void InitializeResult() 
{
	result.FrameCnt = 0;
	result.isPushD = false;

	//構造体のmapの配列に代入
	strcpy(result.map[0], "                                                                               ");
	strcpy(result.map[1], "                                                                               ");
	strcpy(result.map[2], "                                                                               ");
	strcpy(result.map[3], "                                                                               ");
	strcpy(result.map[4], "                                                                               ");
	strcpy(result.map[5], "                                                                               ");
	strcpy(result.map[6], "                                                                               ");
	strcpy(result.map[7], "                                                                               ");
	strcpy(result.map[8], "                                                                               ");
	strcpy(result.map[9], "                                                                               ");
	strcpy(result.map[10], "                                                                               ");
	strcpy(result.map[11], "                                                                               ");
	strcpy(result.map[12], "                                                                               ");
	strcpy(result.map[13], "          000                                                       0          ");
	strcpy(result.map[14], "       000000000                                                  00000        ");
	strcpy(result.map[15], "     0000a0aaa0000                             55555            00d0ddd00      ");
	strcpy(result.map[16], "     aaaaaaaaaaaaa                    44444444455555555         ddddddddd      ");
	strcpy(result.map[17], "    aaaa  aaa  aaaa         22222233333444444555555555         ddddddddddd     ");
	strcpy(result.map[18], "    aaaaaaaaaaaaaa       1111222333333333444445555555555       dddddddddd      ");
	strcpy(result.map[19], "     aa  aaaaa  aa    111112222222223333444444444555555555      ddddddddd      ");
	strcpy(result.map[20], "     aaa       aa        111222223333344444445555555555         dddddddd       ");
	strcpy(result.map[21], "      aaaaaaaaaaa            222223333334444555555555            ddddddd       ");
	strcpy(result.map[22], "       aaaaaaaaa                      4444444455555555            ddddd        ");
	strcpy(result.map[23], "         aaaaa                                  55555               d          ");
	strcpy(result.map[24], "                                                                               ");

	//リザルトの初期表示
	for (int i = 0; i < 25; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			if (result.map[i][j] == '0')
			{//唐辛子のへたの部分

				//描画位置設定
				gotoxy(j + 1, i + 1);
				//色設定
				textattr(0x2F);
				//画面表示
				printf(" ");
			}

			if (result.map[i][j] == 'a')
			{//ゲームスタートの唐辛子

				//描画位置設定
				gotoxy(j + 1, i + 1);
				//色設定
				textattr(0xCF);
				//画面表示
				printf(" ");
			}

			if (result.map[i][j] == 'd')
			{//終了の唐辛子

				//描画位置設定
				gotoxy(j + 1, i + 1);
				//色設定
				textattr(0xFF);
				//画面表示
				printf(" ");
			}

			//色設定を元に戻す
			textbackground(BLACK);
			textcolor(WHITE);
		}
	}

	textattr(0xFF);
	textcolor(BLACK);
	gotoxy(65, 18);
	printf("ランキング");

	textcolor(WHITE);
	textbackground(BLACK);

	//番号
	{
		//0
		strcpy(num[0][0], "■■■■■");
		strcpy(num[0][1], "■      ■");
		strcpy(num[0][2], "■      ■");
		strcpy(num[0][3], "■      ■");
		strcpy(num[0][4], "■      ■");
		strcpy(num[0][5], "■      ■");
		strcpy(num[0][6], "■      ■");
		strcpy(num[0][7], "■      ■");
		strcpy(num[0][8], "■■■■■");


		//1
		strcpy(num[1][0], "   ■■  ");
		strcpy(num[1][1], " ■■■  ");
		strcpy(num[1][2], "■ ■■  ");
		strcpy(num[1][3], "   ■■   ");
		strcpy(num[1][4], "   ■■   ");
		strcpy(num[1][5], "   ■■   ");
		strcpy(num[1][6], "   ■■   ");
		strcpy(num[1][7], "   ■■   ");
		strcpy(num[1][8], "■■■■■");

		//2
		strcpy(num[2][0], "■■■■■");
		strcpy(num[2][1], "        ■");
		strcpy(num[2][2], "        ■");
		strcpy(num[2][3], "        ■");
		strcpy(num[2][4], "■■■■■");
		strcpy(num[2][5], "■        ");
		strcpy(num[2][6], "■        ");
		strcpy(num[2][7], "■        ");
		strcpy(num[2][8], "■■■■■");


		//3
		strcpy(num[3][0], "■■■■■");
		strcpy(num[3][1], "        ■");
		strcpy(num[3][2], "        ■");
		strcpy(num[3][3], "        ■");
		strcpy(num[3][4], "■■■■■");
		strcpy(num[3][5], "        ■");
		strcpy(num[3][6], "        ■");
		strcpy(num[3][7], "        ■");
		strcpy(num[3][8], "■■■■■");

		//4
		strcpy(num[4][0], "■      ■");
		strcpy(num[4][1], "■      ■");
		strcpy(num[4][2], "■      ■");
		strcpy(num[4][3], "■      ■");
		strcpy(num[4][4], "■■■■■");
		strcpy(num[4][5], "        ■");
		strcpy(num[4][6], "        ■");
		strcpy(num[4][7], "        ■");
		strcpy(num[4][8], "        ■");

		//5
		strcpy(num[5][0], "■■■■■");
		strcpy(num[5][1], "■        ");
		strcpy(num[5][2], "■        ");
		strcpy(num[5][3], "■        ");
		strcpy(num[5][4], "■■■■■");
		strcpy(num[5][5], "        ■");
		strcpy(num[5][6], "        ■");
		strcpy(num[5][7], "        ■");
		strcpy(num[5][8], "■■■■■");

		//6
		strcpy(num[6][0], "■■■■■");
		strcpy(num[6][1], "■        ");
		strcpy(num[6][2], "■        ");
		strcpy(num[6][3], "■        ");
		strcpy(num[6][4], "■■■■■");
		strcpy(num[6][5], "■      ■");
		strcpy(num[6][6], "■      ■");
		strcpy(num[6][7], "■      ■");
		strcpy(num[6][8], "■■■■■");

		//7
		strcpy(num[7][0], "■■■■■");
		strcpy(num[7][1], "■      ■");
		strcpy(num[7][2], "■      ■");
		strcpy(num[7][3], "■      ■");
		strcpy(num[7][4], "        ■");
		strcpy(num[7][5], "        ■");
		strcpy(num[7][6], "        ■");
		strcpy(num[7][7], "        ■");
		strcpy(num[7][8], "        ■");

		//8
		strcpy(num[8][0], "■■■■■");
		strcpy(num[8][1], "■      ■");
		strcpy(num[8][2], "■      ■");
		strcpy(num[8][3], "■      ■");
		strcpy(num[8][4], "■■■■■");
		strcpy(num[8][5], "■      ■");
		strcpy(num[8][6], "■      ■");
		strcpy(num[8][7], "■      ■");
		strcpy(num[8][8], "■■■■■");

		//9
		strcpy(num[9][0], "■■■■■");
		strcpy(num[9][1], "■      ■");
		strcpy(num[9][2], "■      ■");
		strcpy(num[9][3], "■      ■");
		strcpy(num[9][4], "■■■■■");
		strcpy(num[9][5], "        ■");
		strcpy(num[9][6], "        ■");
		strcpy(num[9][7], "        ■");
		strcpy(num[9][8], "■■■■■");
	}

	//プレイヤーのアドレス取得
	PLAYER* pPlayer = GetPlayer();
	//プレイヤーの残りHPによってスコア増やす
	float multi = (float)pPlayer->Hp / 10 + 1.0f;

	//スコア表示
	result.Scovill = GetScore() * multi;
	
	int digit = 10;

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (result.Scovill % digit == j)
			{//スコアの位がｊと同じとき
				//色設定
				textcolor((rand() % 14) + 1);

				for (int k = 0; k < 9; k++)
				{
					//描画位置設定
					gotoxy(80 - ((i + 1) * 15), k + 3);
					//画面描画
					printf("%s", num[j][k]);
				}
				//色を元に戻す
				textcolor(WHITE);
				break;
			}
		}
		//桁数移動
		result.Scovill /= 10;

		if (result.Scovill <= 0)
		{
			break;
		}

	}

	//色設定
	textcolor(RED);

	result.Scovill = GetScore() * multi;

	if (result.Scovill <= 1000)
	{//パプリカ
		gotoxy(4, 1);
		printf("君のスコヴィル値はパプリカ級！");
		gotoxy(4, 2);
		printf("まだまだ甘いね！");
	}

	else if (result.Scovill <= 1500)
	{//タバスコ
		gotoxy(4, 1);
		printf("君のスコヴィル値はタバスコ級！");
		gotoxy(4, 2);
		printf("もう少し頑張ろう！");
	}

	else if (result.Scovill <= 2000)
	{//ハラペーニョ
		gotoxy(4, 1);
		printf("君のスコヴィル値はハラペーニョ級！");
		gotoxy(4, 2);
		printf("まだまだ上を目指そう！");
	}

	else if (result.Scovill <= 3000)
	{//島唐辛子
		gotoxy(4, 1);
		printf("君のスコヴィル値は島唐辛子級！");
		gotoxy(4, 2);
		printf("君もだいぶ辛くなってきたね！");
	}

	else if (result.Scovill <= 4500)
	{//鷹の爪
		gotoxy(4, 1);
		printf("君のスコヴィル値は鷹の爪級！");
		gotoxy(4, 2);
		printf("流石の辛さだね！！");
	}

	else if (result.Scovill <= 6000)
	{//ハバネロ
		gotoxy(4, 1);
		printf("君のスコヴィル値はハバネロ級！");
		gotoxy(4, 2);
		printf("もう君の辛さに追いつける者はいないね！！");
	}

	else
	{//キャロライナリーパー
		gotoxy(4, 1);
		printf("君のスコヴィル値はキャロライナリーパー級！");
		gotoxy(4, 2);
		printf("もう君は世界一の唐辛子だ！！");
	}

	//色を元に戻す
	textcolor(WHITE);

	//ぱっぱらー
	strcpy(result.bgm, "MUSICDATA\\result.mp3");
	result.SoundHandle = opensound(result.bgm);

	if (result.SoundHandle)
	{//bgmを開けたら
		setvolume(result.SoundHandle, 70);
		playsound(result.SoundHandle, 0);
	}

	//拍手
	strcpy(result.clap, "MUSICDATA\\hakusyu.mp3");
	result.ClapSoundHandle = opensound(result.clap);

	if (result.ClapSoundHandle)
	{//bgmを開けたら
		setvolume(result.ClapSoundHandle, 60);
	}

}

//-----------------------------------------------
// リザルト更新
//-----------------------------------------------
void UpdateResult()
{

	if (inport(PM_LEFT))
	{//左クリック押したとき

		//マウスの座標取得
		int x = inport(PM_CURX);
		int y = inport(PM_CURY);


		if (result.map[y - 1][x - 1] == 'd')
		{//左クリックしたところがタイトルの唐辛子だったら

			//シーン切り替え
			SetScene(SCENE_RANKING);
		}

	}
	


	//フレームカウンター増加
	result.FrameCnt++;

	if (result.FrameCnt > 100)
	{
		result.FrameCnt = 0;
		//拍手
		playsound(result.ClapSoundHandle, 0);
	}

#ifdef _DEBUG

	//シーン切り替え
	/*if (inport(PK_4))
	{
		SetScene(SCENE_TITLE);
	}*/

#endif 

}

//-----------------------------------------------
// リザルト描画
//-----------------------------------------------
void DrawResult()
{
	if (result.FrameCnt % 5 == 0)
	{
		if (result.FrameCnt / 5 == 1)
		{//炎の1段階目表示
			for (int i = 0; i < 25; i++)
			{
				for (int j = 0; j < 80; j++)
				{
					if (result.map[i][j] == '1')
					{
						//描画位置設定
						gotoxy(j + 1, i + 1);
						//色設定
						textattr(0x4F);
						//画面表示
						printf(" ");
					}

					//色設定を元に戻す
					textbackground(BLACK);
					textcolor(WHITE);
				}
			}
		}

		else if (result.FrameCnt / 5 == 2)
		{//炎の2段階目表示
			for (int i = 0; i < 25; i++)
			{
				for (int j = 0; j < 80; j++)
				{
					if (result.map[i][j] == '2')
					{
						//描画位置設定
						gotoxy(j + 1, i + 1);
						//色設定
						textattr(0xCF);
						//画面表示
						printf(" ");
					}

					//色設定を元に戻す
					textbackground(BLACK);
					textcolor(WHITE);
				}
			}
		}

		else if (result.FrameCnt / 5 == 3)
		{//炎の3段階目表示
			for (int i = 0; i < 25; i++)
			{
				for (int j = 0; j < 80; j++)
				{
					if (result.map[i][j] == '3')
					{
						//描画位置設定
						gotoxy(j + 1, i + 1);
						//色設定
						textattr(0xBF);
						//画面表示
						printf(" ");
					}

					//色設定を元に戻す
					textbackground(BLACK);
					textcolor(WHITE);
				}
			}
		}

		else if (result.FrameCnt / 5 == 4)
		{//炎の4段階目表示
			for (int i = 0; i < 25; i++)
			{
				for (int j = 0; j < 80; j++)
				{
					if (result.map[i][j] == '4')
					{
						//描画位置設定
						gotoxy(j + 1, i + 1);
						//色設定
						textattr(0x3F);
						//画面表示
						printf(" ");
					}

					//色設定を元に戻す
					textbackground(BLACK);
					textcolor(WHITE);
				}
			}
		}

		else if (result.FrameCnt / 5 == 5)
		{//炎の5段階目表示
			for (int i = 0; i < 25; i++)
			{
				for (int j = 0; j < 80; j++)
				{
					if (result.map[i][j] == '5')
					{
						//描画位置設定
						gotoxy(j + 1, i + 1);
						//色設定
						textattr(0x9F);
						//画面表示
						printf(" ");
					}

					//色設定を元に戻す
					textbackground(BLACK);
					textcolor(WHITE);
				}
			}
		}

		else if (result.FrameCnt / 5 == 6)
		{//炎の5段階目消去
			for (int i = 0; i < 25; i++)
			{
				for (int j = 0; j < 80; j++)
				{
					if (result.map[i][j] == '5')
					{
						//描画位置設定
						gotoxy(j + 1, i + 1);
						//色設定
						textbackground(BLACK);
						//画面表示
						printf(" ");
					}

					//色設定を元に戻す
					textbackground(BLACK);
					textcolor(WHITE);
				}
			}
			}

		else if (result.FrameCnt / 5 == 7)
		{//炎の4段階目消去
			for (int i = 0; i < 25; i++)
			{
				for (int j = 0; j < 80; j++)
				{
					if (result.map[i][j] == '4')
					{
						//描画位置設定
						gotoxy(j + 1, i + 1);
						//色設定
						textbackground(BLACK);
						//画面表示
						printf(" ");
					}

					//色設定を元に戻す
					textbackground(BLACK);
					textcolor(WHITE);
				}
			}
			}

		else if (result.FrameCnt / 5 == 8)
		{//炎の3段階目消去
			for (int i = 0; i < 25; i++)
			{
				for (int j = 0; j < 80; j++)
				{
					if (result.map[i][j] == '3')
					{
						//描画位置設定
						gotoxy(j + 1, i + 1);
						//色設定
						textbackground(BLACK);
						//画面表示
						printf(" ");
					}

					//色設定を元に戻す
					textbackground(BLACK);
					textcolor(WHITE);
				}
			}
			}

		else if (result.FrameCnt / 5 == 9)
		{//炎の2段階目消去
			for (int i = 0; i < 25; i++)
			{
				for (int j = 0; j < 80; j++)
				{
					if (result.map[i][j] == '2')
					{
						//描画位置設定
						gotoxy(j + 1, i + 1);
						//色設定
						textbackground(BLACK);
						//画面表示
						printf(" ");
					}

					//色設定を元に戻す
					textbackground(BLACK);
					textcolor(WHITE);
				}
			}
			}

		else if (result.FrameCnt / 5 == 10)
		{//炎の1段階目消去
			for (int i = 0; i < 25; i++)
			{
				for (int j = 0; j < 80; j++)
				{
					if (result.map[i][j] == '1')
					{
						//描画位置設定
						gotoxy(j + 1, i + 1);
						//色設定
						textbackground(BLACK);
						//画面表示
						printf(" ");
					}

					//色設定を元に戻す
					textbackground(BLACK);
					textcolor(WHITE);
				}
			}
		}

	}

	//マウスの座標取得
	int x = inport(PM_CURX);
	int y = inport(PM_CURY);

	if (result.map[y - 1][x - 1] == 'd' && !result.isPushD)
	{//マウスカーソルがタイトルの唐辛子だったら

		//色を暗い灰色に設定
		textattr(0x8F);
		//文字の色を黒に
		textcolor(BLACK);
		
		//画面描画
		gotoxy(67, 16);
		printf(" ");
		gotoxy(69, 16);
		printf("   ");
		gotoxy(65, 17);
		printf("         ");
		gotoxy(64, 18);
		printf(" ランキング");
		gotoxy(64, 19);
		printf("          ");
		gotoxy(65, 20);
		printf("         ");
		gotoxy(65, 21);
		printf("        ");
		gotoxy(66, 22);
		printf("       ");
		gotoxy(67, 23);
		printf("     ");
		gotoxy(69, 24);
		printf(" ");


		//色設定を元に戻す
		textbackground(BLACK);
		textcolor(WHITE);

		result.isPushD = true;
	}

	else if (!(result.map[y - 1][x - 1] == 'd') && result.isPushD)
	{

		//色を白に設定
		textattr(0xFF);
		//文字の色を黒に
		textcolor(BLACK);

		//画面描画
		gotoxy(67, 16);
		printf(" ");
		gotoxy(69, 16);
		printf("   ");
		gotoxy(65, 17);
		printf("         ");
		gotoxy(64, 18);
		printf(" ランキング");
		gotoxy(64, 19);
		printf("          ");
		gotoxy(65, 20);
		printf("         ");
		gotoxy(65, 21);
		printf("        ");
		gotoxy(66, 22);
		printf("       ");
		gotoxy(67, 23);
		printf("     ");
		gotoxy(69, 24);
		printf(" ");

		//色をもとに戻す
		textcolor(WHITE);
		textbackground(BLACK);

		result.isPushD = false;
	}

	


#ifdef _DEBUG

	/*gotoxy(1, 25);
	printf("リザルト");*/

#endif
}

//-----------------------------------------------
// リザルト終了
//-----------------------------------------------
void FinalizeResult()
{
	closesound(result.ClapSoundHandle);
	closesound(result.SoundHandle);
}
