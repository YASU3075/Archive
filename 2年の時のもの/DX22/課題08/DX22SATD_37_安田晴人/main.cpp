
//ウィンドウの表示

#include "main.h"
#include "renderer.h"

#include	"keyboard.h"
#include "mouse.h"
#include "Sprite3D.h"
#include "SceneManager.h"
#include "sprite.h"
#include "Easing.h"

//#include	<windows.h>

//===================================
// ライブラリのリンク
//===================================
#pragma	comment (lib, "d3d11.lib")
#pragma	comment (lib, "d3dcompiler.lib")
#pragma	comment (lib, "winmm.lib")
#pragma	comment (lib, "dxguid.lib")
#pragma	comment (lib, "dinput8.lib")

//=================================
//マクロ定義
//=================================
#define		CLASS_NAME		"DX22 Window"
#define		WINDOW_CAPTION	"DX22 3Dの中の人"

//===================================
//プロトタイプ宣言
//===================================
//コールバック関数＝＞他人が呼び出してくれる関数
LRESULT	CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


//==================================
//グローバル変数
//==================================
#ifdef _DEBUG	//デバッグモード時のみ変数を作る
int	g_CountFPS;							//FPSカウンター
char g_DebugStr[2048] = WINDOW_CAPTION;	//表示文字列設定
#endif

//=====================================
//メイン関数
//======================================
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance, LPSTR lpCmd, int nCmdShow)
{
	//乱数初期化
	srand((UINT)timeGetTime());
	//フレームレート計測用変数
	DWORD	dwExecLastTime;
	DWORD	dwFPSLastTime;
	DWORD	dwCurrentTime;
	DWORD	dwFrameCount;
	//COMコンポーネントの準備（機能を部品化して外部のプログラムから共有利用する仕組み）
	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

	//ウィンドウクラスの登録（ウィンドウの仕様的な物を決めてWindowsへセットする）
	WNDCLASS	wc;	//構造体を準備
	ZeroMemory(&wc, sizeof(WNDCLASS));//内容を０で初期化
	wc.lpfnWndProc = WndProc;	//コールバック関数のポインター
	wc.lpszClassName = CLASS_NAME;	//この仕様書の名前
	wc.hInstance = hInstance;	//このアプリケーションのこと
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);//カーソルの種類
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);//ウィンドウの背景色
	RegisterClass(&wc);	//構造体をWindowsへセット


	//ウィンドウサイズの調整
	//             左上　　右下
	RECT	rc = { 0, 0, 1280, 720 };//横1280 縦720
	//描画領域が1280X720になるようにサイズを調整する
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME|WS_MAXIMIZEBOX|WS_MINIMIZEBOX), FALSE);

	//ウィンドウの作成
	HWND	hWnd = CreateWindow(
		CLASS_NAME,	//作りたいウィンドウ
		WINDOW_CAPTION,	//ウィンドウに表示するタイトル
		WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX),	//標準的な形状のウィンドウ サイズ変更禁止
		CW_USEDEFAULT,		//デフォルト設定でおまかせ
		CW_USEDEFAULT,
		rc.right - rc.left,//CW_USEDEFAULT,//ウィンドウの幅
		rc.bottom - rc.top,//CW_USEDEFAULT,//ウィンドウの高さ
		NULL,
		NULL,
		hInstance,		//アプリケーションのハンドル
		NULL
	);

	MAIN_MANAGER* p_main_manager = new MAIN_MANAGER();		//メインクラス

	//初期化処理
	if (FAILED(p_main_manager->Init(hInstance, hWnd, true)))
	{
		return -1;//初期化失敗
	}

	//作成したウィンドウを表示する
	ShowWindow(hWnd, nCmdShow);//引数に従って表示、または非表示
	//ウィンドウの内容を強制表示
	UpdateWindow(hWnd);

	//メッセージループ
	MSG	msg;
	ZeroMemory(&msg, sizeof(MSG));//メッセージ構造体を作成して初期化
	//フレームレート計測初期化
	timeBeginPeriod(1);	//タイマーの分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();//現在のタイマー値
	dwCurrentTime = dwFrameCount = 0;

	//終了メッセージが来るまでループする
	//ゲームループ
	while (1)
	{	//メッセージの有無をチェック
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{ //Windowsからメッセージが来た！
			if (msg.message == WM_QUIT)//完全終了しましたメッセージ
			{
				break;	//whileループからぬける
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);	//WndProcが呼び出される
			}
	
		}
		else //Windowsからメッセージが来ていない
		{
			dwCurrentTime = timeGetTime();//現在のタイマー値を取得
			if ((dwCurrentTime - dwFPSLastTime) >= 1000)//1秒経過したか
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;	//現在のタイマー値を保存
				dwFrameCount = 0;				//フレームカウントをクリア
			}
			if ((dwCurrentTime - dwExecLastTime) >= ((float)1000 / 60)) // 1/60秒経過したか
			{
				dwExecLastTime = dwCurrentTime;	//現在の時間を保存
#ifdef _DEBUG
				wsprintf(g_DebugStr, WINDOW_CAPTION);//配列にキャプション文字列を格納
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
				SetWindowText(hWnd, g_DebugStr);//キャプション部分の書き換え
#endif

				p_main_manager->Update();	//更新処理
				p_main_manager->Draw();		//描画処理

				keycopy();

				dwFrameCount++;	//フレームカウントを進める
			}

		}
	}//while

	//終了処理
	p_main_manager->Uninit();

	//終了する
	return (int)msg.wParam;

}

//=========================================
//ウィンドウプロシージャ
// メッセージループ内で呼び出される
//=========================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
		case WM_MOUSEHOVER:
		case WM_MOUSEHWHEEL:
		case WM_ACTIVATEAPP:
			Mouse_ProcessMessage(uMsg, wParam, lParam);
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			Keyboard_ProcessMessage(uMsg, wParam, lParam);
			break;
		case WM_KEYDOWN:	//キーが押された
			if (wParam == VK_ESCAPE)//押されたのはESCキー
			{
				//ウィンドウを閉じたいリクエストをWindowsに送る
				SendMessage(hWnd, WM_CLOSE, 0, 0);
			}
			Keyboard_ProcessMessage(uMsg, wParam, lParam);
			break;

		

		case WM_CLOSE:	//ウィンドウを閉じなさい命令				
			if (
				MessageBox(hWnd, "本当に終了してよろしいですか？",
					"確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK
				)
			{//OKが押されたとき
				DestroyWindow(hWnd);//終了する手続きをWindowsへリクエスト
			}
			else
			{
				return 0;	//やっぱり終わらない
			}

			break;
		case WM_DESTROY:	//終了してOKですよ
			PostQuitMessage(0);		//自分のメッセージに０を送る
			break;
		

	}

	//必用の無いメッセージは適当に処理させて終了
	return DefWindowProc(hWnd, uMsg, wParam, lParam);

}

//==================================
//初期化
//==================================
HRESULT	MAIN_MANAGER::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	Mouse_Initialize(hWnd);
	Mouse_SetMode(Mouse_PositionMode::MOUSE_POSITION_MODE_RELATIVE);
	Mouse_ResetScrollWheelValue();


	//DirectX関連の初期化
	InitRenderer(hInstance, hWnd, bWindow);
	
	InitSprite();
	InitSprite3D();

	//キー入力初期化
	Keyboard_Initialize();
	
	InitEasing();
	

	m_p_scene_manager = new SCENE_MANAGER();
	m_p_scene_manager->Init();

	return	S_OK;
}

//====================================
//	終了処理
//====================================
void MAIN_MANAGER::Uninit(void)
{
	Mouse_Finalize();
	m_p_scene_manager->Uninit();

	UninitSprite3D();
	UninitSprite();
	//DirectX関連の終了処理
	UninitRenderer();
}

//===================================
//更新処理
//====================================
void MAIN_MANAGER::Update(void)
{
	m_p_scene_manager->Update();
}

//==================================
//描画処理
//==================================
void MAIN_MANAGER::Draw(void)
{
	//バックバッファのクリア
	Clear();

	m_p_scene_manager->Draw();

	//バックバッファをフロントバッファへコピー
	Present();

}