/*==============================================================================

   頂点管理[polygon.h]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once


#include "main.h"
#include "renderer.h"
#include "polygon.h"//<<<<<<<<<<<<<<<<<<<<<<<NEW

//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// 構造体定義
//*****************************************************************************
class Object2D
{
public:
	XMFLOAT3	position;
	XMFLOAT4	color;
	XMFLOAT2	size;
	XMFLOAT2	scale;
	float		rotate;
	ID3D11ShaderResourceView* texID;

};
class Object3D
{
public:
	XMFLOAT3	position;
	XMFLOAT3	velocity;
	XMFLOAT3	scale;
	XMFLOAT3	rotate;
	ID3D11ShaderResourceView* texID;
	XMMATRIX	World;

};


class Camera
{
public:
	XMFLOAT3	position;
	XMFLOAT3	Atposition;
	XMFLOAT3	Upvector;
	float		fov;
	float		nearclip;
	float		farclip;
};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPolygon(void);
void UninitPolygon(void);
void UpdatePolygon(void);
void DrawPolygon(void);

