//-----------------------------------------------
// DX21:ウィドウの作成
// 制作日：2024/04/24
// 製作者：安田晴人
//-----------------------------------------------
#include <windows.h>

//-----------------------------------------------
// 定数定義
//-----------------------------------------------
#define CLASS_NAME			"GameWindow"
#define WINDOW_CAPTION		"無名のタイトル"

//-----------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//-----------------------------------------------
// メイン関数
//-----------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//ウィンドウクラスの登録（ウィンドウを作るための設定資料集）
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_HAND);	//ウィンドウに表示するカーソルの形
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);	//ウィンドウの色
	RegisterClass(&wc);

	//ウィンドウの作成
	HWND hWnd = CreateWindow(
		CLASS_NAME,
		WINDOW_CAPTION,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,	//x
		CW_USEDEFAULT,	//y
		CW_USEDEFAULT,	//width
		CW_USEDEFAULT,	//height
		NULL,
		NULL,
		hInstance,
		NULL
	);

	//ウィンドウ表示
	ShowWindow(hWnd, nCmdShow);

	//メッセージループ
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//終了
	return (int)msg.wParam;
}

//-----------------------------------------------
// プロシージャ
//-----------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)		//ユーザーが何かしたときにウィンドウズ側で呼び出してくれる関数
{

	switch (uMsg)
	{
	case WM_KEYDOWN: //キー押下
		if (wParam == VK_ESCAPE)
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}

		if (wParam == VK_SPACE)
		{
			MessageBox(hWnd, "なんで押した？", "キー入力検知", MB_OK);
		}
		break;

	case WM_CLOSE: //ウィンドウを閉じる
		if (MessageBox(hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DestroyWindow(hWnd);
		}
		return 0;

	case WM_DESTROY: //アプリケーション終了
		PostQuitMessage(0);
		return 0;
	};

	//その他のメッセージはwindowsに任せる
	return DefWindowProc(hWnd, uMsg, wParam, lParam);

}

//メモ
//
// ウィンドウプロシージャの仕組みがないと×ボタンを押してもウィンドウを閉じてくれない
// イベントドリブン：ユーザーが何かしたとき初めて呼ばれる処理
// 
//
