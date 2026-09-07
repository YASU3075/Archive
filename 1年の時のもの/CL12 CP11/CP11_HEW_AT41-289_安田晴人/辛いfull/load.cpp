//-----------------------------------------------
// load.cpp アクションゲームサンプル
// 制作日：2024/02/13 制作者：安田晴人
//-----------------------------------------------
#include "main.h"
#include "load.h"

#define CONIOEX
#include "conioex.h"

//-----------------------------------------------
// グローバル変数
//-----------------------------------------------
LOAD load;

//-----------------------------------------------
// ロード初期化
//-----------------------------------------------
void InitializeLoad() 
{
	//構造体のmapの配列に代入
	strcpy(load.map[0], "                                                                               ");
	strcpy(load.map[1], "                                                                               ");
	strcpy(load.map[2], "        1111111111111                     33   3  3           4                ");
	strcpy(load.map[3], "        11         11                     33    3  3          4                ");
	strcpy(load.map[4], "        11         11                     333            44444444444           ");
	strcpy(load.map[5], "        11         11   222222222222222   33333          4    4    4           ");
	strcpy(load.map[6], "        11         11                     33  333        44444444444           ");
	strcpy(load.map[7], "        11         11                     33                  4                ");
	strcpy(load.map[8], "        1111111111111                     33                  4                ");
	strcpy(load.map[9], "                                                                               ");
	strcpy(load.map[10], "                                                                               ");
	strcpy(load.map[11], "                                                                               ");
	strcpy(load.map[12], "      0          0          0          0          0          0          0      ");
	strcpy(load.map[13], "      0          0          0          0          0          0          0      ");
	strcpy(load.map[14], "    00000      00000      00000      00000      00000      00000      00000    ");
	strcpy(load.map[15], "   0a0aaa0    0b0bbb0    0c0ccc0    0d0ddd0    0e0eee0    0f0fff0    0g0ggg0   ");
	strcpy(load.map[16], "   aaaaaaa    bbbbbbb    ccccccc    ddddddd    eeeeeee    fffffff    ggggggg   ");
	strcpy(load.map[17], "    aaaaaa     bbbbbb     cccccc     dddddd     eeeeee     ffffff     gggggg   ");
	strcpy(load.map[18], "    aaaaa      bbbbb      ccccc      ddddd      eeeee      fffff      ggggg    ");
	strcpy(load.map[19], "     aaaa       bbbb       cccc       dddd       eeee       ffff       gggg    ");
	strcpy(load.map[20], "     aaa        bbb        ccc        ddd        eee        fff        ggg     ");
	strcpy(load.map[21], "      aa         bb         cc         dd         ee         ff         gg     ");
	strcpy(load.map[22], "     aa         bb         cc         dd         ee         ff         gg      ");
	strcpy(load.map[23], "                                                                               ");
	strcpy(load.map[24], "                                                                               ");


	//初期の色で画面描画
	for (int i = 0; i < MAX_LOAD_Y; i++)
	{
		for (int j = 0; j < MAX_LOAD_X; j++)
		{
			if (load.map[i][j] == '1' || load.map[i][j] == '2' || load.map[i][j] == '3' || load.map[i][j] == '4' || load.map[i][j] == 'W')
			{//ロード中の文字を白に

				//文字の描画位置設定
				gotoxy(j + 1, i + 1);
				//色設定
				textattr(0xFF);
				//画面表示
				printf(" ");
			}

			if (load.map[i][j] == '0')
			{//唐辛子の葉の部分を緑に

				//文字の描画位置設定
				gotoxy(j + 1, i + 1);
				//色設定
				textattr(0x2F);
				//画面表示
				printf(" ");
			}

			if (
				load.map[i][j] == 'a' || load.map[i][j] == 'b' || load.map[i][j] == 'c' || load.map[i][j] == 'd' ||
				load.map[i][j] == 'e' || load.map[i][j] == 'f' || load.map[i][j] == 'g'
				)
			{//唐辛子実の部分を白

				//文字の描画位置設定
				gotoxy(j + 1, i + 1);
				//色設定
				textattr(0xFF);
				//画面表示
				printf(" ");
			}

			//色設定をもとに戻す
			textcolor(WHITE);
			textbackground(BLACK);
		}
	}

	//フレームカウンターの初期化
	load.FrameCnt = 0;

	//インターバルを乱数で設定
	load.interval = rand() % 4 + 1;

#ifdef _DEBUG

	/*gotoxy(1, 25);
	printf("ロード");*/

#endif

}

//-----------------------------------------------
// ロード更新
//-----------------------------------------------
void UpdateLoad()
{

	//フレーム数増加
	load.FrameCnt++;

	//シーン切り替え
	if (load.FrameCnt > LOAD_SECOND * (MAX_CHIRI - 1) + LOAD_SECOND / load.interval)
	{//ロードが終わったら
		SetScene(SCENE_COUNTDOWN);
	}

#ifdef _DEBUG

	//シーン切り替え
	/*if (inport(PK_2))
	{
		SetScene(SCENE_COUNTDOWN);
	}*/

#endif


}

//-----------------------------------------------
// ロード描画
//-----------------------------------------------
void DrawLoad()
{
	//乱数で16進数の色の数値を決定(黒以外)
	int ColorNum = (rand() % 15 + 1) * 16 + 15;
	
	if (load.FrameCnt % LOAD_SECOND == 0)
	{//フレーム数がロードにかかるフレーム数の倍数の時
		for (int i = 0; i < MAX_LOAD_Y; i++)
		{
			for (int j = 0; j < MAX_LOAD_X; j++)
			{
				if (load.map[i][j] == '1')
				{//文字を乱数で決めて色に

					//文字の描画位置設定
					gotoxy(j + 1, i + 1);
					//色設定
					textattr(ColorNum);
					//画面表示
					printf(" ");
				}

				else if (load.map[i][j] == '2')
				{//文字を乱数で決めて色に

				//文字の描画位置設定
					gotoxy(j + 1, i + 1);
					//色設定
					textattr(ColorNum + 32);
					//画面表示
					printf(" ");
				}

				else if (load.map[i][j] == '3')
				{//文字を乱数で決めて色に

				//文字の描画位置設定
					gotoxy(j + 1, i + 1);
					//色設定
					textattr(ColorNum + 16);
					//画面表示
					printf(" ");
				}

				else if (load.map[i][j] == '4')
				{//文字を乱数で決めて色に

				//文字の描画位置設定
					gotoxy(j + 1, i + 1);
					//色設定
					textattr(ColorNum + 48);
					//画面表示
					printf(" ");
				}

				//色設定をもとに戻す
				textcolor(WHITE);
				textbackground(BLACK);
			}
		}
	}

	for (int i = 0; i < MAX_CHIRI; i++)
	{
		if (load.FrameCnt == LOAD_SECOND * i + LOAD_SECOND / load.interval)
		{//唐辛子の実を描画するとき
			for (int j = 0; j < MAX_LOAD_Y; j++)
			{
				for (int k = 0; k < MAX_LOAD_X; k++)
				{
					if (ColorNum == 0x2F)
					{//唐辛子の葉の部分と同じ緑色の時は明るい緑にする
						ColorNum = 0xAF;
					}

					else if (ColorNum == 0xFF)
					{//唐辛子の実が初期の白と同じとき赤にする
						ColorNum = 0x4F;
					}

					if (load.map[j][k] == 'a' && load.FrameCnt == LOAD_SECOND * 0 + LOAD_SECOND / load.interval)
					{//唐辛子の実の部分を乱数で決めた色に
						//文字の描画位置設定
						gotoxy(k + 1, j + 1);
						//色設定
						textattr(ColorNum);
						//画面表示
						printf(" ");
					}

					else if (load.map[j][k] == 'b' && load.FrameCnt == LOAD_SECOND * 1 + LOAD_SECOND / load.interval)
					{//唐辛子の実の部分を乱数で決めた色に
						//文字の描画位置設定
						gotoxy(k + 1, j + 1);
						//色設定
						textattr(ColorNum);
						//画面表示
						printf(" ");
					}

					else if (load.map[j][k] == 'c' && load.FrameCnt == LOAD_SECOND * 2 + LOAD_SECOND / load.interval)
					{//唐辛子の実の部分を乱数で決めた色に
						//文字の描画位置設定
						gotoxy(k + 1, j + 1);
						//色設定
						textattr(ColorNum);
						//画面表示
						printf(" ");
					}

					else if (load.map[j][k] == 'd' && load.FrameCnt == LOAD_SECOND * 3 + LOAD_SECOND / load.interval)
					{//唐辛子の実の部分を乱数で決めた色に
						//文字の描画位置設定
						gotoxy(k + 1, j + 1);
						//色設定
						textattr(ColorNum);
						//画面表示
						printf(" ");
					}

					else if (load.map[j][k] == 'e' && load.FrameCnt == LOAD_SECOND * 4 + LOAD_SECOND / load.interval)
					{//唐辛子の実の部分を乱数で決めた色に
						//文字の描画位置設定
						gotoxy(k + 1, j + 1);
						//色設定
						textattr(ColorNum);
						//画面表示
						printf(" ");
					}

					else if (load.map[j][k] == 'f' && load.FrameCnt == LOAD_SECOND * 5 + LOAD_SECOND / load.interval)
					{//唐辛子の実の部分を乱数で決めた色に
						//文字の描画位置設定
						gotoxy(k + 1, j + 1);
						//色設定
						textattr(ColorNum);
						//画面表示
						printf(" ");
					}

					else if (load.map[j][k] == 'g' && load.FrameCnt == LOAD_SECOND * 6 + LOAD_SECOND / load.interval)
					{//唐辛子の実の部分を乱数で決めた色に
						//文字の描画位置設定
						gotoxy(k + 1, j + 1);
						//色設定
						textattr(ColorNum);
						//画面表示
						printf(" ");
					}

					//色設定をもとに戻す
					textcolor(WHITE);
					textbackground(BLACK);
				}
			}
		}
	}
}

//-----------------------------------------------
// ロード終了
//-----------------------------------------------
void FinalizeLoad()
{
	
}
