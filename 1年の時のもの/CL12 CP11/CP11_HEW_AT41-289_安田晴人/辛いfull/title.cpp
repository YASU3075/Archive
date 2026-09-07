//-----------------------------------------------
// title.cpp アクションゲームサンプル
// 制作日：2024/01/31 制作者：安田晴人
//-----------------------------------------------
#include "main.h"
#include "title.h"

#define CONIOEX
#include "conioex.h"

//-----------------------------------------------
// グローバル変数
//-----------------------------------------------
TITLE title;

//-----------------------------------------------
// タイトル初期化
//-----------------------------------------------
void InitializeTitle() 
{

	//構造体のmapの配列に代入
	strcpy(title.map[0],  "                                                                               ");
	strcpy(title.map[1],  "           11                                                                  ");
	strcpy(title.map[2],  "           11                                                                  ");
	strcpy(title.map[3],  "     11111111111111                                                            ");
	strcpy(title.map[4],  "       11      11                                                              ");
	strcpy(title.map[5],  "        11    11           11                  1111            11   11         ");
	strcpy(title.map[6],  "  11111111111111111111    111         11       11              11   11         ");
	strcpy(title.map[7],  "           11             11          11    11111111           11   11         ");
	strcpy(title.map[8],  "           11             11           11      11    11   11   11   11         ");
	strcpy(title.map[9],  "    1111111111111111      11           11      11    11   11   11   11         ");
	strcpy(title.map[10], "           11             11  1       11       11    11   111  11   11         ");
	strcpy(title.map[11], "           11              1111                11     1111 11  11   11         ");
	strcpy(title.map[12], "                                                                               ");
	strcpy(title.map[13], "           0                  0                  0                  0          ");
	strcpy(title.map[14], "         00000              00000              00000              00000        ");
	strcpy(title.map[15], "       00a0aaa00          00b0bbb00          00c0ccc00          00d0ddd00      ");
	strcpy(title.map[16], "       aaaaaaaaa          bbbbbbbbb          ccccccccc          ddddddddd      ");
	strcpy(title.map[17], "      aaaaaaaaaaa        bbbbbbbbbbb        ccccccccccc        ddddddddddd     ");
	strcpy(title.map[18], "      aaaaaaaaaa         bbbbbbbbbb         cccccccccc         dddddddddd      ");
	strcpy(title.map[19], "       aaaaaaaaa          bbbbbbbbb          ccccccccc          ddddddddd      ");
	strcpy(title.map[20], "       aaaaaaaa           bbbbbbbb           cccccccc           dddddddd       ");
	strcpy(title.map[21], "        aaaaaaa            bbbbbbb            ccccccc            ddddddd       ");
	strcpy(title.map[22], "         aaaaa              bbbbb              ccccc              ddddd        ");
	strcpy(title.map[23], "           a                  b                  c                  d          ");
	strcpy(title.map[24], "                                                                               ");
	

	title.isPushA = false;
	title.isPushB = false;
	title.isPushC = false;
	title.isPushD = false;

	title.isStop = false;

	title.PosX = 1;
	title.OldPosX = 1;
	title.PosY = 1;
	title.OldPosY = 1;

	title.FrameCnt = 0;

	//タイトルの初期表示
	for (int i = 0; i < MAX_TITLE_Y; i++)
	{
		for (int j = 0; j < MAX_TITLE_X; j++)
		{
			

			if (title.map[i][j] == '0')
			{//唐辛子のへたの部分

				//描画位置設定
				gotoxy(j + 1, i + 1);
				//色設定
				textattr(0x2F);
				//画面表示
				printf(" ");
			}

			if (title.map[i][j] == 'a')
			{//ゲームスタートの唐辛子

				//描画位置設定
				gotoxy(j + 1, i + 1);
				//色設定
				textattr(0xCF);
				//画面表示
				printf(" ");
			}

			if (title.map[i][j] == 'b')
			{//チュートリアルの唐辛子

				//描画位置設定
				gotoxy(j + 1, i + 1);
				//色設定
				textattr(0x9F);
				//画面表示
				printf(" ");
			}

			if (title.map[i][j] == 'c')
			{//ランキングの唐辛子

				//描画位置設定
				gotoxy(j + 1, i + 1);
				//色設定
				textattr(0xBF);
				//画面表示
				printf(" ");
			}

			if (title.map[i][j] == 'd')
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

	textattr(0xCF);
	textcolor(WHITE);

	gotoxy(9, 18);
	printf("ゲーム");
	gotoxy(8, 19);
	printf("スタート");

	textattr(0x9F);
	gotoxy(28, 18);
	printf("操作説明");

	textattr(0xBF);
	gotoxy(46, 18);
	printf("ランキング");

	textattr(0xFF);
	textcolor(BLACK);
	gotoxy(67, 18);
	printf("終了");

	textcolor(WHITE);
	textbackground(BLACK);


	strcpy(title.bgm, "MUSICDATA\\title_bgm.mp3");
	title.SoundHandle = opensound(title.bgm);

	if (title.SoundHandle)
	{//bgmを開けたら
		playsound(title.SoundHandle, 1);
		setvolume(title.SoundHandle, 50);
	}
}

//-----------------------------------------------
// タイトル更新
//-----------------------------------------------
void UpdateTitle()
{

	if (inport(PM_LEFT))
	{//左クリック

		int x = inport(PM_CURX);
		int y = inport(PM_CURY);
		
		if (title.map[y - 1][x - 1] == 'a')
		{//ゲームスタート
			SetScene(SCENE_LOAD);
		}

		else if (title.map[y - 1][x - 1] == 'b')
		{//操作説明
			SetScene(SCENE_EXPLANATION);
		}

		else if (title.map[y - 1][x - 1] == 'c')
		{//ランキング
			SetScene(SCENE_RANKING);
		}

		else if (title.map[y - 1][x - 1] == 'd')
		{//終了
			FinishGame();
		}
	}

	//フレームカウンター増加
	title.FrameCnt++;

	if (title.FrameCnt % 3 == 0 && !title.isStop)
	{//3フレームごとに光らせる位置更新

		//3フレーム前の座標取得
		title.OldPosX = title.PosX;
		title.OldPosY = title.PosY;

		title.PosX++;

		if (title.PosX > MAX_TITLE_X + 1)
		{//最大値を超えたら
			title.PosX = 1;
		}

		title.PosY++;

		if (title.PosY > MAX_MOJI_Y)
		{//最大値を超えたら
			title.PosY = 1;
		}
	}

	if (title.FrameCnt > 243 && !title.isStop)
	{//キラキラさせているとき
		title.FrameCnt = 0;
		title.isStop = true;
	}

	else if (title.FrameCnt > 60 && title.isStop)
	{//キラキラを止めているとき
		title.FrameCnt = 0;
		title.isStop = false;
	}


#ifdef _DEBUG

	//シーン切り替え
	//if (inport(PK_1))
	//{
	//	SetScene(SCENE_LOAD);
	//}

#endif

}

//-----------------------------------------------
// タイトル描画
//-----------------------------------------------
void DrawTitle()
{
	//カーソルでタッチしたときの描画処理
	{
		int CursolX = inport(PM_CURX);
		int CursolY = inport(PM_CURY);

		if (title.map[CursolY - 1][CursolX - 1] == 'a' && !title.isPushA)
		{//ゲームスタートの唐辛子にカーソルが合っているとき
			//色を赤に設定
			textattr(0x4F);
			//文字の色を白に
			textcolor(WHITE);

			//画面描画
			gotoxy(10, 16);
			printf(" ");
			gotoxy(12, 16);
			printf("   ");
			gotoxy(8, 17);
			printf("         ");
			gotoxy(7, 18);
			printf("  ゲーム   ");
			gotoxy(7, 19);
			printf(" スタート ");
			gotoxy(8, 20);
			printf("         ");
			gotoxy(8, 21);
			printf("        ");
			gotoxy(9, 22);
			printf("       ");
			gotoxy(10, 23);
			printf("     ");
			gotoxy(12, 24);
			printf(" ");

			//色をもとに戻す
			textbackground(BLACK);

			title.isPushA = true;
		}

		else if (!(title.map[CursolY - 1][CursolX - 1] == 'a') && title.isPushA)
		{//ゲームスタートの唐辛子からカーソルを外した時
			//色を赤に設定
			textattr(0xCF);
			//文字の色を白に
			textcolor(WHITE);

			//画面描画
			gotoxy(10, 16);
			printf(" ");
			gotoxy(12, 16);
			printf("   ");
			gotoxy(8, 17);
			printf("         ");
			gotoxy(7, 18);
			printf("  ゲーム   ");
			gotoxy(7, 19);
			printf(" スタート ");
			gotoxy(8, 20);
			printf("         ");
			gotoxy(8, 21);
			printf("        ");
			gotoxy(9, 22);
			printf("       ");
			gotoxy(10, 23);
			printf("     ");
			gotoxy(12, 24);
			printf(" ");

			//色をもとに戻す
			textbackground(BLACK);

			title.isPushA = false;
		}

		if (title.map[CursolY - 1][CursolX - 1] == 'b' && !title.isPushB)
		{//チュートリアルの唐辛子にカーソルが合ってるとき

			//色を青に設定
			textattr(0x1F);
			//文字の色を白に
			textcolor(WHITE);

			//画面描画
			gotoxy(29, 16);
			printf(" ");
			gotoxy(31, 16);
			printf("   ");
			gotoxy(27, 17);
			printf("         ");
			gotoxy(26, 18);
			printf("  操作説明 ");
			gotoxy(26, 19);
			printf("          ");
			gotoxy(27, 20);
			printf("         ");
			gotoxy(27, 21);
			printf("        ");
			gotoxy(28, 22);
			printf("       ");
			gotoxy(29, 23);
			printf("     ");
			gotoxy(31, 24);
			printf(" ");

			//色をもとに戻す
			textbackground(BLACK);

			title.isPushB = true;
		}

		else if (!(title.map[CursolY - 1][CursolX - 1] == 'b') && title.isPushB)
		{//チュートリアルの唐辛子からカーソルを外した時

			//色を明るい青に設定
			textattr(0x9F);
			//文字の色を白に
			textcolor(WHITE);

			//画面描画
			gotoxy(29, 16);
			printf(" ");
			gotoxy(31, 16);
			printf("   ");
			gotoxy(27, 17);
			printf("         ");
			gotoxy(26, 18);
			printf("  操作説明 ");
			gotoxy(26, 19);
			printf("          ");
			gotoxy(27, 20);
			printf("         ");
			gotoxy(27, 21);
			printf("        ");
			gotoxy(28, 22);
			printf("       ");
			gotoxy(29, 23);
			printf("     ");
			gotoxy(31, 24);
			printf(" ");

			//色をもとに戻す
			textbackground(BLACK);

			title.isPushB = false;
		}

		if (title.map[CursolY - 1][CursolX - 1] == 'c' && !title.isPushC)
		{//ランキングの唐辛子にカーソルが合ってるとき

			//色をシアンに設定
			textattr(0x3F);
			//文字の色を白に
			textcolor(WHITE);

			//画面描画
			gotoxy(48, 16);
			printf(" ");
			gotoxy(50, 16);
			printf("   ");
			gotoxy(46, 17);
			printf("         ");
			gotoxy(45, 18);
			printf(" ランキング");
			gotoxy(45, 19);
			printf("          ");
			gotoxy(46, 20);
			printf("         ");
			gotoxy(46, 21);
			printf("        ");
			gotoxy(47, 22);
			printf("       ");
			gotoxy(48, 23);
			printf("     ");
			gotoxy(50, 24);
			printf(" ");

			//色をもとに戻す
			textbackground(BLACK);

			title.isPushC = true;
		}

		else if (!(title.map[CursolY - 1][CursolX - 1] == 'c') && title.isPushC)
		{//ランキングの唐辛子からカーソルを外した時

			//色を明るいシアンに設定
			textattr(0xBF);
			//文字の色を白に
			textcolor(WHITE);

			//画面描画
			gotoxy(48, 16);
			printf(" ");
			gotoxy(50, 16);
			printf("   ");
			gotoxy(46, 17);
			printf("         ");
			gotoxy(45, 18);
			printf(" ランキング");
			gotoxy(45, 19);
			printf("          ");
			gotoxy(46, 20);
			printf("         ");
			gotoxy(46, 21);
			printf("        ");
			gotoxy(47, 22);
			printf("       ");
			gotoxy(48, 23);
			printf("     ");
			gotoxy(50, 24);
			printf(" ");

			//色をもとに戻す
			textbackground(BLACK);

			title.isPushC = false;
		}

		if (title.map[CursolY - 1][CursolX - 1] == 'd' && !title.isPushD)
		{//終了の唐辛子にカーソルが合ってるとき

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
			printf("   終了    ");
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

			title.isPushD = true;
		}

		else if (!(title.map[CursolY - 1][CursolX - 1] == 'd') && title.isPushD)
		{//終了の唐辛子からカーソルを外した時

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
			printf("   終了    ");
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

			title.isPushD = false;
		}
	}

	
	if (title.PosX != title.OldPosX || title.PosY != title.OldPosY)
	{//光らせる位置が動いていたら
		for (int i = 0; i < title.PosX; i++)
		{
			if (title.map[i][title.OldPosX - i] == '1')
			{//３フレーム前の光らせる位置が文字だったら
				//描画位置設定
				gotoxy(title.OldPosX - i + 1, i + 1);
				//色設定
				textattr(0x4F);
				//画面描画
				printf(" ");

				//色をもとに戻す
				textbackground(BLACK);
				textcolor(WHITE);
			}

			if (title.map[i][title.OldPosX - i + 1] == '1')
			{//３フレーム前の光らせる位置の１つ右が文字だったら
				//描画位置設定
				gotoxy(title.OldPosX - i + 2, i + 1);
				//色設定
				textattr(0x4F);
				//画面描画
				printf(" ");

				//色をもとに戻す
				textbackground(BLACK);
				textcolor(WHITE);
			}

			if (title.map[i][title.PosX - i] == '1')
			{//光らせる位置が文字だったら
				//描画位置設定
				gotoxy(title.PosX - i + 1, i + 1);
				//色設定
				textattr(0xCF);
				//画面描画
				printf(" ");

				//色をもとに戻す
				textbackground(BLACK);
				textcolor(WHITE);
			}

			if (title.map[i][title.PosX - i + 1] == '1')
			{//光らせる位置の1つ右が文字だったら
				//描画位置設定
				gotoxy(title.PosX - i + 2, i + 1);
				//色設定
				textattr(0xCF);
				//画面描画
				printf(" ");

				//色をもとに戻す
				textbackground(BLACK);
				textcolor(WHITE);
			}


		}
	}

#ifdef _DEBUG

	/*gotoxy(1, 25);
	printf("タイトル");*/

#endif 
}

//-----------------------------------------------
// タイトル終了
//-----------------------------------------------
void FinalizeTitle()
{
	closesound(title.SoundHandle);
}
