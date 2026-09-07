//-----------------------------------------------
// ranking.cpp アクションゲームサンプル
// 制作日：2024/02/24 制作者：安田晴人
//-----------------------------------------------
#include "main.h"
#include "ranking.h"
#include "game.h"
#include "player.h"

#define CONIOEX
#include "conioex.h"

//-----------------------------------------------
// グローバル変数
//-----------------------------------------------
RANKING ranking;

//-----------------------------------------------
// ランキング初期化
//-----------------------------------------------
void InitializeRanking() 
{
	//構造体のmapの配列に代入
	strcpy(ranking.map[0], "                                                                               ");
	strcpy(ranking.map[1], "       0                                                               0       ");
	strcpy(ranking.map[2], "     00000          111111111111111111111111111111111111111          00000     ");
	strcpy(ranking.map[3], "    0101110         111111111111111111111111111111111111111         0101110    ");
	strcpy(ranking.map[4], "    1111111         111111111111111111111111111111111111111         1111111    ");
	strcpy(ranking.map[5], "    1111111         111111111111111111111111111111111111111         1111111    ");
	strcpy(ranking.map[6], "     11111          111111111111111111111111111111111111111          11111     ");
	strcpy(ranking.map[7], "       1                                                               1       ");
	strcpy(ranking.map[8], "                                                                               ");
	strcpy(ranking.map[9], "       0                                                               0       ");
	strcpy(ranking.map[10], "     00000         222222222222222222222222222222222222222           00000     ");
	strcpy(ranking.map[11], "    0202220        222222222222222222222222222222222222222          0202220    ");
	strcpy(ranking.map[12], "    2222222        222222222222222222222222222222222222222          2222222    ");
	strcpy(ranking.map[13], "    2222222        222222222222222222222222222222222222222          2222222    ");
	strcpy(ranking.map[14], "     22222         222222222222222222222222222222222222222           22222     ");
	strcpy(ranking.map[15], "       2                                                               2       ");
	strcpy(ranking.map[16], "                                                                               ");
	strcpy(ranking.map[17], "       0                                                               0       ");
	strcpy(ranking.map[18], "     00000         333333333333333333333333333333333333333           00000     ");
	strcpy(ranking.map[19], "    0303330        333333333333333333333333333333333333333          0303330    ");
	strcpy(ranking.map[20], "    3333333        333333333333333333333333333333333333333          3333333    ");
	strcpy(ranking.map[21], "    3333333        333333333333333333333333333333333333333          3333333    ");
	strcpy(ranking.map[22], "     33333         333333333333333333333333333333333333333           33333     ");
	strcpy(ranking.map[23], "       3                                                               3       ");
	strcpy(ranking.map[24], "                                                                               ");

	for (int i = 0; i < 3; i++)
	{
		ranking.Score[i] = 0;
	}

	//ファイルポインタ
	FILE* fp;

	//ファイルを開く
	fp = fopen("DATA\\ranking.txt", "r");

	if (fp != NULL)
	{
		for (int i = 0; i < 3; i++)
		{
			(char)fscanf(fp, "%s", ranking.name[i]);
			(int)fscanf(fp, "%d", &ranking.Score[i]);
		}
		//ファイルを閉じる
		fclose(fp);
	}

	else
	{
		gotoxy(1, 1);
		printf("データ読み込み失敗");
	}

	//プレイヤーのアドレス取得
	PLAYER* pPlayer = GetPlayer();
	//プレイヤーの残りHPによってスコア増やす
	float multi = (float)pPlayer->Hp / 10 + 1.0f;

	//スコア取得
	int Score = GetScore() * multi;

	if (Score > ranking.Score[2])
	{//スコアがランクインしたら

		if (Score > ranking.Score[0])
		{//１位よりスコアが高いとき

			//３位の配列に２位のスコアと名前を代入
			ranking.Score[2] = ranking.Score[1];
			strcpy(ranking.name[2], ranking.name[1]);
			//２位の配列に１位のスコアと名前を代入
			ranking.Score[1] = ranking.Score[0];
			strcpy(ranking.name[1], ranking.name[0]);
			//１位の配列に今回のスコアを代入
			ranking.Score[0] = Score;
			InputPlayerName(1);
		}

		else if (Score > ranking.Score[1])
		{//今回のスコアが２位の時

			//３位の配列に２位のスコアと名前を代入
			ranking.Score[2] = ranking.Score[1];
			strcpy(ranking.name[2], ranking.name[1]);
			//２位の配列に今回のスコアと名前を代入
			ranking.Score[1] = Score;
			InputPlayerName(2);
		}

		else if (Score > ranking.Score[2])
		{//今回のスコアが３位の時

			//３位の配列に今回のスコアと名前を代入
			ranking.Score[2] = Score;
			InputPlayerName(3);
		}


		//ファイルを開く
		fp = fopen("DATA\\ranking.txt", "w");

		if (fp != NULL)
		{
			for (int i = 0; i < 3; i++)
			{
				fprintf(fp, "%s ", ranking.name[i]);
				fprintf(fp, "%d ", ranking.Score[i]);
			}
			//ファイルを閉じる
			fclose(fp);
		}

		else
		{
			gotoxy(1, 1);
			printf("データ書き込み失敗");
		}

	}

	//初期の色で画面描画
	for (int i = 0; i < MAX_RANKING_Y; i++)
	{
		for (int j = 0; j < MAX_RANKING_X; j++)
		{
			if (ranking.map[i][j] == '0')
			{//唐辛子の葉の部分を緑に

				//文字の描画位置設定
				gotoxy(j + 1, i + 1);
				//色設定
				textattr(0x2F);
				//画面表示
				printf(" ");
			}

			else if (ranking.map[i][j] == '1')
			{//１位

				//文字の描画位置設定
				gotoxy(j + 1, i + 1);
				//色設定
				textattr(0xEF);
				//画面表示
				printf(" ");
			}

			else if (ranking.map[i][j] == '2')
			{//２位

				//文字の描画位置設定
				gotoxy(j + 1, i + 1);
				//色設定
				textattr(0x7F);
				//画面表示
				printf(" ");
			}

			if (ranking.map[i][j] == '3')
			{//３位

				//文字の描画位置設定
				gotoxy(j + 1, i + 1);
				//色設定
				textattr(0x6F);
				//画面表示
				printf(" ");
			}

			//色設定をもとに戻す
			textcolor(WHITE);
			textbackground(BLACK);
		}
	}

	
	//色設定
	textattr(0xEF);
	textcolor(BLACK);
	//１位の名前
	gotoxy(6, 5);
	printf("１位");
	gotoxy(70, 5);
	printf("%d", ranking.Score[0]);

	gotoxy(30, 5);
	printf("%s", ranking.name[0]);


	//色設定
	textattr(0x7F);
	textcolor(BLACK);
	//２位の名前
	gotoxy(6, 13);
	printf("２位");
	gotoxy(70, 13);
	printf("%d", ranking.Score[1]);

	gotoxy(30, 13);
	printf("%s", ranking.name[1]);


	//色設定
	textattr(0x6F);
	textcolor(BLACK);
	//３位の名前
	gotoxy(6, 21);
	printf("３位");
	gotoxy(70, 21);
	printf("%d", ranking.Score[2]);

	gotoxy(30, 21);
	printf("%s", ranking.name[2]);

	//色をもとに戻す
	textcolor(WHITE);
	textbackground(BLACK);

	//フレームカウンターの初期化
	ranking.FrameCnt = 0;

	strcpy(ranking.bgm, "MUSICDATA\\ranking_bgm.mp3");
	ranking.SoundHandle = opensound(ranking.bgm);

	if (ranking.SoundHandle)
	{//bgmを開けたら
		playsound(ranking.SoundHandle, 1);
		setvolume(ranking.SoundHandle, 50);
	}

	//ダメージ音
	strcpy(ranking.enter, "MUSICDATA\\enter.mp3");
	ranking.EnterSoundHandle = opensound(ranking.enter);
	if (ranking.EnterSoundHandle)
	{
		setvolume(ranking.EnterSoundHandle, 90);
	}

#ifdef _DEBUG

	/*gotoxy(1, 25);
	printf("ランキング");*/

#endif

}

//-----------------------------------------------
// ランキング更新
//-----------------------------------------------
void UpdateRanking()
{

	//フレーム数増加
	ranking.FrameCnt++;
	if (ranking.FrameCnt > 120)
	{
		ranking.FrameCnt = 0;
	}


	if (inport(PK_ENTER))
	{
		//エンター音
		playsound(ranking.EnterSoundHandle, 0);
		Sleep(300);
		SetScene(SCENE_TITLE);
	}

#ifdef _DEBUG

	////シーン切り替え
	//if (inport(PK_5))
	//{
	//	SetScene(SCENE_TITLE);
	//}

#endif


}

//-----------------------------------------------
// ランキング描画
//-----------------------------------------------
void DrawRanking()
{
	if (ranking.FrameCnt % 120 == 0)
	{
		gotoxy(30, 24);
		printf("　　　　　　　　　　　　");
	}

	else if (ranking.FrameCnt % 60 == 0)
	{
		gotoxy(30, 24);
		printf("エンターキーでタイトルへ");
	}

}

//-----------------------------------------------
// ランキング終了
//-----------------------------------------------
void FinalizeRanking()
{
	int Score = GetScore();
	if (Score > 0)
	{
		//スコアを0にする
		ScoreAdd(-Score / 100);
	}

	closesound(ranking.SoundHandle);
	closesound(ranking.EnterSoundHandle);
}


//-----------------------------------------------
// 名前のカーソル入力
//-----------------------------------------------
void InputPlayerName(int rank)
{
	//変数宣言
	int cursol_y = 3;	//カーソルのy座標初期値
	int cursol_x = 13;	//カーソルのx座標初期値
	int ok = 0;			//エンターキー判定
	int right = 0;		//右矢印判定
	int left = 0;		//左矢印判定
	int up = 0;			//上矢印判定
	int down = 0;		//下矢印判定
	int W = 0;
	int A = 0;
	int S = 0;
	int D = 0;
	int mode = 1;		//ひらがなカタカナの判定用（左から順に１，２）
	int old_x = 0;		//一個前のカーソルのX座標
	int old_y = 0;		//一個前のカーソルのY座標
	int keikoku = 0;	//操作できないものが入力されたとき用
	int moji = 0;		//文字数

	bool isPushKST = false;		//最後の文字がかさた行かどうか
	bool isPushH = false;		//最後の文字がは行かどうか

	char name[32 + 1] = {};

	char hiragana[5][40] =
	{
		"あ　か　さ　た　な　は　ま　や　ら　わ",
		"い　き　し　ち　に　ひ　み　ゆ　り　を",
		"う　く　す　つ　ぬ　ふ　む　よ　る　ん",
		"え　け　せ　て　ね　へ　め　！　れ　ー",
		"お　こ　そ　と　の　ほ　も　？　ろ　・",
	};

	char katakana[5][40] =
	{
		"ア　カ　サ　タ　ナ　ハ　マ　ヤ　ラ　ワ",
		"イ　キ　シ　チ　ニ　ヒ　ミ　ユ　リ　ヲ",
		"ウ　ク　ス　ツ　ヌ　フ　ム　ヨ　ル　ン",
		"エ　ケ　セ　テ　ネ　ヘ　メ　！　レ　ー",
		"オ　コ　ソ　ト　ノ　ホ　モ　？　ロ　・",
	};

	while (1)
	{

		gotoxy(1, 1);
		printf("NEW　RECORD！！");
		gotoxy(20, 1);
		printf("名前を入力してください");

		//枠表示
		textbackground(WHITE);
		gotoxy(10, 2);
		printf("　　　　　　　　　　　　　　　　　　　　　　　　　");

		for (int i = 0; i < 7; i++)
		{
			gotoxy(10, i + 3);
			printf("　");
			gotoxy(58, i + 3);
			printf("　");
		}

		gotoxy(10, 9);
		printf("　　　　　　　　　　　　　　　　　　　　　　　　　");

		textbackground(BLACK);

		//scanf対策
		reinport();

		if (old_x == 37 && old_y == 8)
		{//前回一字削除だった時
			if (moji > 0)
			{
				moji--;
			}
		}

		else if (old_x == 49 && old_y == 8)
		{//前回全削除のとき
			moji = 0;
		}


		if (mode == 1)
		{
			for (int i = 0; i < 5; i++)
			{//ひらがな表示
				gotoxy(11, i + 3);
				printf("%s", &hiragana[i]);
			}

			gotoxy(52, 3);
			printf("カナ");
		}

		else
		{
			for (int i = 0; i < 5; i++)
			{//カタカナ表示
				gotoxy(11, i + 3);
				printf("%s", &katakana[i]);
			}
			gotoxy(52, 3);
			printf("ひら");
		}

		gotoxy(11, 8);
		printf("〝　゜　小文字　　一字削除　　全部削除　 決定");


		//警告文表示
		gotoxy(11, 10);

		if (keikoku == 1)
		{//濁点をつけられないとき
			printf("この文字に濁点はつけられないよ");
		}

		else if (keikoku == 2)
		{//半濁点をつけられないとき
			printf("この文字に半濁点はつけられないよ");
		}

		else if (keikoku == 3)
		{//小文字にできないとき
			printf("この文字は小文字にできないよ");
		}

		keikoku = 0;

		gotoxy(11, 15);
		printf("・WASDか←↑↓→でカーソルを移動してEnterで決定！\n");
		gotoxy(11, 16);
		printf("・名前は最大１２文字！\n");
		gotoxy(11, 17);
		printf("・最後に決定を押して完了！！");

		//現在の名前表示
		textcolor(GREEN);
		gotoxy(11, 12);
		for (int i = 0; i < 24; i++)
		{
			printf("%c", name[i]);
			if (name[i] == '\0')
			{
				break;
			}

		}
		textcolor(WHITE);

		gotoxy(cursol_x, cursol_y);
		printf("←");

		//カーソル移動
		while (1)
		{//エンターキーを押すまで

			ok = inport(PK_ENTER);
			right = inport(PK_RIGHT);
			left = inport(PK_LEFT);
			down = inport(PK_DOWN);
			up = inport(PK_UP);
			W = inport(PK_W);
			A = inport(PK_A);
			S = inport(PK_S);
			D = inport(PK_D);

			if (ok != 0)
			{
				break;
			}

			else
			{
				if (down != 0 || S != 0)
				{//下矢印入力

					//前回の矢印を消す
					gotoxy(cursol_x, cursol_y);
					printf("　");

					//矢印表示
					cursol_y++;

					if (cursol_x == 56 && cursol_y != 8)
					{//一番右のモード切替の時
						cursol_y += 4;
					}

					if (cursol_y == 8)
					{//特殊入力の場所に行くとき

						if (cursol_x == 21)
						{//小文字
							cursol_x = 25;
						}

						else if (cursol_x == 29 || cursol_x == 33)
						{//一字削除
							cursol_x = 37;
						}

						else if (cursol_x == 41 || cursol_x == 45)
						{//全部削除
							cursol_x = 49;
						}

					}

					else if (cursol_y > 8)
					{//一番下の時にさらに下矢印を押した場合一番上に戻す
						cursol_y = 3;
					}
					gotoxy(cursol_x, cursol_y);
					printf("←");


					//連続で入力されてしまうのでスリープ関数をはさむ
					Sleep(200);
				}

				else if (up != 0 || W != 0)
				{//上矢印入力

					//前回の矢印を消す
					gotoxy(cursol_x, cursol_y);
					printf("　");

					//矢印表示
					cursol_y--;

					if (cursol_x == 56 && cursol_y == 7)
					{//一番右のモード切替の時
						cursol_y = 3;
					}

					if (cursol_y < 3)
					{//一番上の時にさらに上矢印を押した場合一番下に戻す
						cursol_y = 8;
					}

					if (cursol_y == 8)
					{//特殊入力の場所に行くとき

						if (cursol_x == 21)
						{//小文字
							cursol_x = 25;
						}

						else if (cursol_x == 29 || cursol_x == 33)
						{//一字削除
							cursol_x = 37;
						}

						else if (cursol_x == 41 || cursol_x == 45)
						{//全部削除
							cursol_x = 49;
						}

					}

					gotoxy(cursol_x, cursol_y);
					printf("←");


					//連続で入力されてしまうのでスリープ関数をはさむ
					Sleep(200);
				}

				else if (right != 0 || D != 0)
				{//右矢印入力

					//前回の矢印を消す
					gotoxy(cursol_x, cursol_y);
					printf("　");

					//矢印表示
					cursol_x += 4;

					if (cursol_y == 8)
					{//一番下の特殊入力の時

						if (cursol_x == 21)
						{//小文字
							cursol_x = 25;
						}

						else if (cursol_x == 29 || cursol_x == 33)
						{//一字削除
							cursol_x = 37;
						}

						else if (cursol_x == 41 || cursol_x == 45)
						{//全部削除
							cursol_x = 49;
						}
					}

					if (cursol_x > 50 && cursol_x < 56)
					{//モード切替に行くか行かないか

						if (cursol_y == 3 || cursol_y == 8)
						{//モード切替があるところ
							cursol_x = 56;
						}

						else
						{//一番右のときにさらに右を押したら一番左に戻す
							cursol_x = 13;
						}

					}

					else if (cursol_x > 56)
					{//一番右のときにさらに右を押したら一番左に戻す
						cursol_x = 13;
					}

					gotoxy(cursol_x, cursol_y);
					printf("←");

					//連続で入力されてしまうのでスリープ関数をはさむ
					Sleep(200);

				}

				else if (left != 0 || A != 0)
				{//左矢印入力

					//前回の矢印を消す
					gotoxy(cursol_x, cursol_y);
					printf("　");

					if (cursol_x == 56)
					{//一番右のモード切替から左に行くとき
						cursol_x -= 3;
					}

					//矢印表示
					cursol_x -= 4;

					if (cursol_y == 8)
					{//一番下の特殊入力の時

						if (cursol_x == 21)
						{//小文字
							cursol_x = 17;
						}

						else if (cursol_x == 29 || cursol_x == 33)
						{//一字削除
							cursol_x = 25;
						}

						else if (cursol_x == 41 || cursol_x == 45)
						{//全部削除
							cursol_x = 37;
						}
					}


					if (cursol_x < 13)
					{//一番左のときにさらに左を押したら一番右に戻す
						if (cursol_y == 3 || cursol_y == 8)
						{
							cursol_x = 56;
						}
						else
						{
							cursol_x = 49;
						}
					}

					gotoxy(cursol_x, cursol_y);
					printf("←");

					//連続で入力されてしまうのでスリープ関数をはさむ
					Sleep(200);

				}

			}
		}

		if (cursol_x == 56 && cursol_y == 3)
		{//ひらカナ変換の時
			if (mode == 1)
			{//カタカナに変える
				mode = 2;
			}

			else
			{//ひらがなに変える
				mode = 1;
			}

			moji--;
		}

		else if (cursol_y == 8)
		{//特殊入力の時

			if (cursol_x == 13)
			{//濁点のとき
				if (isPushKST || isPushH)
				{
					if (((old_x >= 17 && old_x <= 25) || old_x == 33) && old_y != 8)
					{//濁点をつけると文字コードでプラス１される
						name[(moji - 1) * 2 + 1] += 1;
					}

					else if (old_x == 17 && old_y == 8)
					{//半濁点から濁点
						name[(moji - 1) * 2 + 1] -= 1;
					}
				}
				else
				{//濁点をつけられない文字の時
					keikoku = 1;
				}

				moji--;
			}

			else if (cursol_x == 17)
			{//半濁点のとき
				if (isPushH)
				{
					if (old_x == 33)
					{//半濁点をつけると文字コードでプラス2される
						name[(moji - 1) * 2 + 1] += 2;
					}

					else if (old_x == 13 && old_y == 8)
					{//濁点から半濁点
						name[(moji - 1) * 2 + 1] += 1;
					}
				
				}

				else
				{//半濁点をつけられない文字の時
					keikoku = 2;
				}

				moji--;

			}

			else if (cursol_x == 25)
			{//小文字のとき

				if ((old_x == 25 && old_y == 5) || (old_x == 41 && old_y < 6))
				{//つ、や、ゆ、よのとき
					name[(moji - 1) * 2 + 1] -= 1;
				}

				else
				{
					keikoku = 3;
				}

				moji--;

			}

			else if (cursol_x == 37)
			{//一字削除の時

				if (moji != 0)
				{//１文字でも入力していたら
					if (name[(moji - 1) * 2] != '\0')
					{
						name[(moji - 1) * 2] = {};
						name[(moji - 1) * 2 + 1] = {};
						isPushKST = false;
						isPushH = false;
					}
				}
				moji--;
			}

			else if (cursol_x == 49)
			{//全部削除のとき
				if (name[(moji - 1) * 2] != '\0')
				{
					for (int i = 0; i < 24; i++)
					{
						name[i] = {};
					}
				}
					moji--;
					isPushKST = false;
					isPushH = false;
				
			}

			else if (cursol_x == 56)
			{//決定
				if (moji > 0)
				{
					textcolor(YELLOW);
					gotoxy(11, 13);
					printf("けってい！！\n");
					textcolor(WHITE);
					break;
				}

				moji--;

			}
		}

		else
		{//文字入力の時

			if (cursol_x >= 17 && cursol_x <= 25 && cursol_y < 8)
			{//かさた行の時
				isPushKST = true;
				isPushH = false;
			}

			else if (cursol_x == 33 && cursol_y < 8)
			{//は行の時
				isPushKST = false;
				isPushH = true;
			}

			else
			{
				isPushKST = false;
				isPushH = false;
			}

			if (mode == 1)
			{//ひらがな
				name[moji * 2] = hiragana[cursol_y - 3][cursol_x - 13];
				name[moji * 2 + 1] = hiragana[cursol_y - 3][cursol_x - 12];
			}
			else if (mode == 2)
			{//カタカナ
				name[moji * 2] = katakana[cursol_y - 3][cursol_x - 13];
				name[moji * 2 + 1] = katakana[cursol_y - 3][cursol_x - 12];
			}

		}

		if (keikoku == 0)
		{
			//濁点、半濁点、小文字できるか判定用
			old_x = cursol_x;
			old_y = cursol_y;
		}
		//画面全消し
		clrscr();

		moji++;

		if (moji > 11)
		{//文字数が最大値を超えないように
			moji = 11;
		}

	}

	//入力した名前を構造体にコピー
	strcpy(ranking.name[rank - 1], name);

	//キー情報リセット
	reinport();

	//画面全消し
	clrscr();

};

