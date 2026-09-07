// =============================================================================
// main.cpp アクションゲームサンプル
// 制作日：2024/01/16 制作者：安田晴人
// =============================================================================
#include "main.h"
#include "title.h"
#include "load.h"
#include "game.h"
#include "result.h"
#include "ranking.h"
#include "explanation.h"
#include "countdown.h"



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
void WindowConsoleInitialize(void);

// *****************************************************************************
// グローバル変数
// *****************************************************************************
int CountFps;						// FPSカウンタ
SCENE Scene = SCENE_TITLE;			//シーン管理用
bool isFinish;						//終了判定用

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

	DWORD g_dwMode_default;    // コンソールのデフォルト状態を保持する変数

	// コンソールでマウスの情報を取得するために下記が必要
	const HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);

	GetConsoleMode(hStdin, &g_dwMode_default);    // 終了時にデフォルト状態に戻すため、デフォルトの情報を保持
	SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

	// カーソル非表示
	setcursortype(NOCURSOR);


	// 分解能を設定
	timeBeginPeriod(1);

	ExecLastTime = 
	FPSLastTime = timeGetTime();
	CurrentTime =
	FrameCount = 0;

	//画面サイズ設定
	WindowConsoleInitialize();

	//時間で乱数のシード作成
	srand((unsigned int)time(0));

	// 初期化処理
	switch (Scene)
	{

	case SCENE_TITLE:
		InitializeTitle();
		break;

	case SCENE_LOAD:
		InitializeLoad();
		break;

	case SCENE_GAME:
		InitializeGame();
		break;
		
	case SCENE_RESULT:
		InitializeResult();
		break;

	case SCENE_RANKING:
		InitializeRanking();
		break;

	case SCENE_EXPLANATION:
		InitializeExplanation();
		break;

	case SCENE_COUNTDOWN:
		InitializeCountdown();
		break;

	default:break;
	}

	isFinish = false;
	

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
			switch (Scene)
			{

			case SCENE_TITLE:
				UpdateTitle();
				break;

			case SCENE_LOAD:
				UpdateLoad();
				break;

			case SCENE_GAME:
				UpdateGame();
				break;

			case SCENE_RESULT:
				UpdateResult();
				break;

			case SCENE_RANKING:
				UpdateRanking();
				break;

			case SCENE_EXPLANATION:
				UpdateExplanation();
				break;

			case SCENE_COUNTDOWN:
				UpdateCountdown();
				break;

			default:break;
			}


			// 描画処理
			switch (Scene)
			{

			case SCENE_TITLE:
				DrawTitle();
				break;

			case SCENE_LOAD:
				DrawLoad();
				break;

			case SCENE_GAME:
				DrawGame();
				break;

			case SCENE_RESULT:
				DrawResult();
				break;

			case SCENE_RANKING:
				DrawRanking();
				break;

			case SCENE_EXPLANATION:
				DrawExplanation();
				break;

			case SCENE_COUNTDOWN:
				DrawCountdown();
				break;

			default:break;
			}

#ifdef _DEBUG
			// FPS表示
			//DispFPS();
#endif

			FrameCount++;
		}
	} while(!isFinish);

	// 終了処理
	switch (Scene)
	{

	case SCENE_TITLE:
		FinalizeTitle();
		break;

	case SCENE_LOAD:
		FinalizeLoad();
		break;

	case SCENE_GAME:
		FinalizeGame();
		break;

	case SCENE_RESULT:
		FinalizeResult();
		break;

	case SCENE_RANKING:
		FinalizeRanking();
		break;

	case SCENE_EXPLANATION:
		FinalizeExplanation();
		break;

	case SCENE_COUNTDOWN:
		FinalizeCountdown();
		break;

	default:break;
	}

	// マウス入力可否を元に戻す
	SetConsoleMode(hStdin, g_dwMode_default);

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

//-----------------------------------------------
// シーンのセッター
//-----------------------------------------------
void SetScene(SCENE s)
{

	// 終了処理
	switch (Scene)
	{

	case SCENE_TITLE:
		FinalizeTitle();
		break;

	case SCENE_LOAD:
		FinalizeLoad();
		break;

	case SCENE_GAME:
		FinalizeGame();
		break;

	case SCENE_RESULT:
		FinalizeResult();
		break;

	case SCENE_RANKING:
		FinalizeRanking();
		break;

	case SCENE_EXPLANATION:
		FinalizeExplanation();
		break;

	case SCENE_COUNTDOWN:
		FinalizeCountdown();
		break;

	default:break;
	}

	//シーンのセット
	Scene = s;

	// 画面をクリア
	clrscr();

	// 初期化処理
	switch (Scene)
	{

	case SCENE_TITLE:
		InitializeTitle();
		break;

	case SCENE_LOAD:
		InitializeLoad();
		break;

	case SCENE_GAME:
		InitializeGame();
		break;

	case SCENE_RESULT:
		InitializeResult();
		break;

	case SCENE_RANKING:
		InitializeRanking();
		break;

	case SCENE_EXPLANATION:
		InitializeExplanation();
		break;

	case SCENE_COUNTDOWN:
		InitializeCountdown();
		break;

	default:break;
	}

}

//-----------------------------------------------
// ゲーム終了
//-----------------------------------------------
void FinishGame()
{
	isFinish = true;
}

#ifdef _DEBUG
// =============================================================================
//  FPS表示
// =============================================================================
//void DispFPS(void)
//{
//	// 色設定
//	textcolor(WHITE);
//
//	gotoxy(1, 1);
//	printf("FPS:%d", CountFps);
//}
#endif

// =============================================================================
// ウィンドウ設定の初期化（HEW用設定）
// =============================================================================
void WindowConsoleInitialize(void)
{
	// Set console window position
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos(consoleWindow, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	// Set console window size
	SMALL_RECT windowSize = { 0, 0, 80, 25 };
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);

	// Set console buffer size
	// ウィンドウサイズは80, 25で固定すること
	COORD coord;
	coord.X = 80;
	coord.Y = 25;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coord);

	// Set console font style and size
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;

	// ↓フォントサイズのみ変更可↓
	cfi.dwFontSize.X = 18;                   // Width of each character in the font
	cfi.dwFontSize.Y = 36;                   // Height
	// ↑フォントサイズのみ変更可↑
	
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"MS Gothic");       // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}
