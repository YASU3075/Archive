/*==============================================================================

   レンダリング管理[renderer.h]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once

#include "main.h"

//*********************************************************
// 構造体
//*********************************************************

// 頂点構造体
struct VERTEX_3D
{
	XMFLOAT3 Position;	//頂点座標　XMFLOAT3 ＝　float x,y,z
	XMFLOAT3 Normal;	//法線ベクトル 
	XMFLOAT4 Diffuse;	//色  XMFLOAT4 = float x,y,z,w
	XMFLOAT2 TexCoord;	//テクスチャ座標 XMFLOAT2 = float x,y
};


// マテリアル構造体
struct MATERIAL
{
	XMFLOAT4	Ambient;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Specular;
	XMFLOAT4	Emission;
	float		Shininess;
	float		Dummy[3];//16byte境界用
};

//ライト構造体
struct LIGHT
{
	XMFLOAT4 Direction;		//光のベクトル
	XMFLOAT4 Diffuse;		//光の色
	XMFLOAT4 Ambient;		//環境光
	BOOL Enable;			//ライティングスイッチ
	float Dummy[3];
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitRenderer(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void UninitRenderer(void);

void Clear(void);
void Present(void);

ID3D11Device *GetDevice( void );
ID3D11DeviceContext *GetDeviceContext( void );

void SetDepthEnable( bool Enable );

void SetWorldViewProjection2D(void);
void SetWorldViewProjection3D(void);


void SetWorldMatrix(XMMATRIX WorldMatrix );
void SetViewMatrix(XMMATRIX ViewMatrix );
void SetProjectionMatrix(XMMATRIX ProjectionMatrix );


void SetMaterial( MATERIAL Material );
void SetLight(LIGHT light);

