// =============================================================================
// main.cpp アクションゲームサンプル
// 制作日：2024/01/16 制作者：安田晴人
// =============================================================================
#include "main.h"
#include "player.h"
#include "field.h"
#include "bullet.h"
#include "enemy.h"


#include "conioex.h"


// *****************************************************************************
// マクロ定義
// *****************************************************************************


// *****************************************************************************
// プロトタイプ宣言
// *****************************************************************************
#ifdef _DEBUG
void DispFPS(void);
#endif

// *****************************************************************************
// グローバル変数
// *****************************************************************************
int CountFps;				// FPSカウンタ

// =============================================================================
//  メイン関数
// =============================================================================
int main(void)
{
	int ExecLastTime;
	int FPSLastTime;
	int CurrentTime;
	int FrameCount;

	// 画面をクリア
	clrscr();

	// カーソル非表示
	setcursortype(NOCURSOR);

	// 分解能を設定
	timeBeginPeriod(1);

	ExecLastTime = 
	FPSLastTime = timeGetTime();
	CurrentTime =
	FrameCount = 0;

	// 初期化処理
	InitializePlayer();
	InitializeField();
	InitializeBullet();
	InitializeEnemy();

	// ゲームメイン処理
	do
	{
		CurrentTime = timeGetTime();
		if((CurrentTime - FPSLastTime) >= 500)	// 0.5秒ごとに実行
		{
			CountFps = FrameCount * 1000 / (CurrentTime - FPSLastTime);
			FPSLastTime = CurrentTime;
			FrameCount = 0;
		}

		if((CurrentTime - ExecLastTime) >= (1000 / 60))	// 60fpsでフレーム管理
		{
			ExecLastTime = CurrentTime;

			// 更新処理
			UpdatePlayer();
			UpdateField();
			UpdateBullet();
			UpdateEnemy();


			// 描画処理
			DrawPlayer();
			DrawField();
			DrawBullet();
			DrawEnemy();


#ifdef _DEBUG
			// FPS表示
			DispFPS();
#endif

			FrameCount++;
		}
	} while(!inport(PK_ESC));

	// 終了処理
	FinalizePlayer();
	FinalizeField();
	FinalizeBullet();
	FinalizeEnemy();

	// 分解能を戻す
	timeEndPeriod(1);

	// 画面をクリア
	clrscr();
	// カーソル表示
	setcursortype(NORMALCURSOR);

	printf("Enterキーで終了します\n");
	rewind(stdin);
	(void)getchar();

	return 0;

}

#ifdef _DEBUG
// =============================================================================
//  FPS表示
// =============================================================================
void DispFPS(void)
{
	// 色設定
	textcolor(WHITE);

	gotoxy(1, 1);
	printf("FPS:%d", CountFps);
}
#endif

//メモ
//
// ★ゲームループ
// ①初期化 Initialize
// 
// １フレーム
// {
// ②更新 Update
// ③描画 Draw
// }
// 
// ④終了 Finalize
// 
// 1.無
// 2.プレイヤー（動く）
// 3.地面（重力、ジャンプ）
// 4.弾
// 5.敵（当たり判定、バウンディングボックス）
// 6.空中床
// 7.アニメーション
// 8.画面遷移（タイトル→ゲーム→リザルト→タイトル…）
// 
// 
// 目に見えるもの→「オブジェクト（obj）」
// objには「実体」があります
// 実体→構造体
// 
// 
// キーの入力について
// ・プレス(press)			→　押しっぱなしでずっと有効
// ・トリガー(trigger)		→　押した１フレームだけ有効　あとは無効
// ・リリース(release)		→　離した１フレームだけ有効
// ・押していない	→　プレスの逆
//

