//-----------------------------------------------
// DX21:初回授業
// 制作日：2024/04/22
// 製作者：安田晴人
//-----------------------------------------------
#include <Windows.h>

//エントリーポイント
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstanse, LPSTR lpCmdLine, int nCmdShow)
{

	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstanse);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	UINT ret[3] = {};

	ret[0] = MessageBox(NULL, "はいを押せ！", "選択", MB_YESNO);

	if (ret[0] == IDYES)
	{
		MessageBox(NULL, "君は素直な人だね！", "正直者", MB_OK);
	}

	else
	{
		MessageBox(NULL, "お前はなんて天邪鬼なやつなんだ", "愚か者", MB_ICONWARNING);
	}

	ret[1] = MessageBox(NULL, "いいえを押せ！", "指示に従うな", MB_YESNO);

	if (ret[1] == IDYES)
	{
		MessageBox(NULL, "よく惑わされなかったな", "用心深い", MB_OK);
	}

	else
	{
		MessageBox(NULL, "タイトルをちゃんと読もう…", "せっかち", MB_ICONEXCLAMATION);
	}

	return 0;

}

