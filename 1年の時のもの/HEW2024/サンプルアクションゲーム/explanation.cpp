//-----------------------------------------------
// explanation.cpp アクション説明サンプル
// 制作日：2024/02/26 制作者：安田晴人
//-----------------------------------------------
#include "main.h"
#include "explanation.h"
#include "player.h"
#include "field.h"
#include "bat.h"

#define CONIOEX
#include "conioex.h"

//-----------------------------------------------
// グローバル変数
//-----------------------------------------------
EXPLANATION expla;

//-----------------------------------------------
// 説明シーン初期化
//-----------------------------------------------
void InitializeExplanation() 
{
	expla.isPushW = false;
	expla.isPushA = false;
	expla.isPushS = false;
	expla.isPushD = false;
	expla.isLeftClick = false;
	expla.isRightClick = false;

	expla.FrameCnt = 0;
	expla.State = 0;
	expla.OldState = 0;

	expla.row = 0;
	expla.col = 0;

	InitializePlayer();
	InitializeBat();
	InitializeField();

	//唐辛子君描画
	char map[15][80] =
	{
		"                                   2        2        2        2                ",
		"           0                     22 22    22 22    22 22    22 22              ",
		"          000                   22   22  22   22  22   22  22   22             ",
		"       000000000           222222     2222     2222     2222     2222222       ",
		"     0000a0aaa0000        222                                         22       ",
		"     aaaaaaaaaaaaa        44                                          33       ",
		"    aaaa  aaa  aaaa       44                                         333       ",
		"    aaaaaaaaaaaaaa       44                                          33        ",
		"     aa  aaaaa  aa       44                                          33        ",
		"     aaa       aa  11   44                                           33        ",
		"      aaaaaaaaaaa  1111 44                                          33         ",
		"       aaaaaaaaa    11144                                           33         ",
		"         aaaaa        11444                                        333         ",
		"          aaa           444444444444444444444444444444444444433333333          ",
		"          aa                                                                   ",
	};


	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			if (map[i][j] == '0')
			{//唐辛子の葉の部分

				gotoxy(j + 1, i + 1);
				textattr(0x2F);
				printf(" ");
			}

			if (map[i][j] == 'a')
			{//唐辛子の実の部分
				gotoxy(j + 1, i + 1);
				textattr(0x4F);
				printf(" ");
			}

			if (map[i][j] == '1')
			{//吹き出しの部分
				gotoxy(j + 1, i + 1);
				textattr(0xCF);
				printf(" ");
			}

			if (map[i][j] == '2')
			{//吹き出しの部分
				gotoxy(j + 1, i + 1);
				textattr(0xBF);
				printf(" ");
			}

			if (map[i][j] == '3')
			{//吹き出しの部分
				gotoxy(j + 1, i + 1);
				textattr(0x9F);
				printf(" ");
			}

			if (map[i][j] == '4')
			{//吹き出しの部分
				gotoxy(j + 1, i + 1);
				textattr(0x4F);
				printf(" ");
			}
			//色をもとに戻す
			textbackground(BLACK);
			textcolor(WHITE);
		}
	}

	//色設定
	textcolor(RED);

	//1文目
	strcpy(expla.text[0][0], "まずはキャラクターを動かそう！");
	strcpy(expla.text[0][1], "WASDキーで動けるよ！！");

	//2文目
	strcpy(expla.text[1][0], "流石！！");
	strcpy(expla.text[1][1], "この調子で次はバットを振ろう！！");
	strcpy(expla.text[1][2], "マウスの左、右クリックで振れるよ！！");
	strcpy(expla.text[1][3], "バットは敵の弾を跳ね返すのに使うよ！");

	//3文目
	strcpy(expla.text[2][0], "すごい！すごい！！");
	strcpy(expla.text[2][1], "これでもう操作はばっちりだね！！");
	strcpy(expla.text[2][2], "タイトルからゲームをプレイして");
	strcpy(expla.text[2][3], "世界を辛い物でいっぱいにしよう！！");

	//色をもとに戻す
	textcolor(WHITE);

	//テキスト表示音
	strcpy(expla.sound, "MUSICDATA\\text.mp3");
	expla.SoundHandle = opensound(expla.sound);
	if (expla.SoundHandle)
	{
		setvolume(expla.SoundHandle, 50);
		playsound(expla.SoundHandle, 1);
	}

}

//-----------------------------------------------
// 説明シーン更新
//-----------------------------------------------
void UpdateExplanation()
{
	UpdatePlayer();
	UpdateBat();
	UpdateField();

	//1フレーム前の状態を取得
	expla.OldState = expla.State;

	if (expla.State == 0)
	{//初期状態

		expla.FrameCnt++;

		if (expla.FrameCnt % 9 == 0)
		{//9フレームごとに
			if (expla.col < 4)
			{//テキストの行数以内の時
				if (expla.text[0][expla.col][expla.row] != '\0')
				{//文字が終わるまで
					gotoxy(30 + expla.row, 7 + expla.col);
					printf("%c%c", expla.text[0][expla.col][expla.row], expla.text[0][expla.col][expla.row + 1]);

					expla.row += 2;
				}

				else
				{//文字が終わったら行をひとつずらす
					expla.row = 0;
					expla.col++;
				}
			}
		}


		if (inport(PK_W))
		{
			expla.isPushW = true;
		}

		if (inport(PK_A))
		{
			expla.isPushA = true;
		}

		if (inport(PK_S))
		{
			expla.isPushS = true;
		}

		if (inport(PK_D))
		{
			expla.isPushD = true;
		}

		if (expla.isPushW && expla.isPushA && expla.isPushS && expla.isPushD)
		{//WASDキーすべて押したら状態をずらして他の数値をリセット
			expla.State = 1;
			expla.FrameCnt = 0;
			expla.col = 0;
			expla.row = 0;
		}

	}

	if (expla.State == 1)
	{//移動し終わった

		expla.FrameCnt++;

		if (expla.FrameCnt % 9 == 0)
		{//9フレームごとに
			if (expla.col < 4)
			{//テキストの行数以内の時
				if (expla.text[1][expla.col][expla.row] != '\0')
				{//文字が終わるまで
					gotoxy(30 + expla.row, 7 + expla.col);
					printf("%c%c", expla.text[1][expla.col][expla.row], expla.text[1][expla.col][expla.row + 1]);

					expla.row += 2;
				}

				else
				{//文字が終わったら行をひとつずらす
					expla.row = 0;
					expla.col++;
				}
			}
		}

		if (inport(PM_LEFT))
		{
			expla.isLeftClick = true;
		}

		if (inport(PM_RIGHT))
		{
			expla.isRightClick = true;
		}

		if (expla.isLeftClick && expla.isRightClick)
		{//マウスの左、右クリックしたら状態をずらして他の数値をリセット
			expla.State = 2;
			expla.FrameCnt = 0;
			expla.col = 0;
			expla.row = 0;
		}
	}

	if (expla.State == 2)
	{//説明終了
		expla.FrameCnt++;

		if (expla.FrameCnt % 9 == 0)
		{//9フレームごとに
			if (expla.col < 5)
			{//テキストの行数以内の時
				if (expla.text[2][expla.col][expla.row] != '\0')
				{//文字が終わるまで
					gotoxy(30 + expla.row, 7 + expla.col);
					printf("%c%c", expla.text[2][expla.col][expla.row], expla.text[2][expla.col][expla.row + 1]);

					expla.row += 2;
				}

				else
				{//文字が終わったら行をひとつずらす
					expla.row = 0;
					expla.col++;
				}
			}


			else
			{//文字をすべて表示したら状態をずらしてフレーム数をリセット
				expla.State = 3;
				expla.FrameCnt = 0;
			}
		}
	}

	if (expla.State == 3)
	{
		expla.FrameCnt++;

		if (expla.FrameCnt > 120)
		{//文字が出きって120フレーム経ったら
			SetScene(SCENE_TITLE);
		}
	}


#ifdef _DEBUG
	////シーン切り替え
	//if (inport(PK_6))
	//{
	//	SetScene(SCENE_TITLE);
	//}
#endif 

}

//-----------------------------------------------
// 説明シーン描画
//-----------------------------------------------
void DrawExplanation()
{
	DrawPlayer();
	DrawBat();
	DrawField();

	if (expla.OldState != expla.State)
	{//状態が変わってたら

		if (expla.State == 1)
		{

			gotoxy(30, 7);
			printf("　　　　　　　　　　　　　　　");
			gotoxy(30, 8);
			printf("　　　　　　　　　　　　　");

		}

		if (expla.State == 2)
		{

			gotoxy(30, 7);
			printf("　　　　");
			gotoxy(30, 8);
			printf("　　　　　　　　　　　　　　　　");
			gotoxy(30, 9);
			printf("　　　　　　　　　　　　　　　　　　　　");
			gotoxy(30, 10);
			printf("　　　　　　　　　　　　　　　　　　　　");

		}

	}

#ifdef _DEBUG
	/*gotoxy(10, 1);
	printf("操作説明");*/
#endif

}

//-----------------------------------------------
// 説明シーン終了
//-----------------------------------------------
void FinalizeExplanation()
{
	FinalizePlayer();
	FinalizeBat();
	FinalizeField();

	closesound(expla.SoundHandle);

}
