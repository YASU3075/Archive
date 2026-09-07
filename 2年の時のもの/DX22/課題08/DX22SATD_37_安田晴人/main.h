/*==============================================================================

   共通ヘッダー [main.h]
                                                         Author : 
                                                         Date   : 
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once

#define NOMINMAX

#pragma warning(push)
#pragma warning(disable:4005)

#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止
#include <stdio.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#define DIRECTINPUT_VERSION 0x0800		// 警告対処
#include "dinput.h"
#include "mmsystem.h"

#pragma warning(pop)

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH	(1920)				// ウインドウの幅
#define SCREEN_HEIGHT	(1080)				// ウインドウの高さ


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************



class MAIN_MANAGER
{
private:
    class SCENE_MANAGER* m_p_scene_manager = nullptr;

public:
    MAIN_MANAGER() {}         //コンストラクタ
    ~MAIN_MANAGER() {}        //デストラクタ

    //初期化関数
    HRESULT	Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
    //終了処理
    void	Uninit(void);
    //更新処理
    void	Update(void);
    //描画処理
    void	Draw(void);
};


#include <DirectXMath.h>
using namespace DirectX;

#include "DirectXTex.h"//<<<<

#if _DEBUG
#pragma comment(lib, "DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex_Release.lib")
#endif
