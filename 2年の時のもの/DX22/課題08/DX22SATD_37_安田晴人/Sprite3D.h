//-----------------------------------------------
// Sprite3D.h
// 制作日：2024/11/20
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include "main.h"
#include "renderer.h"
#include "model.h"

class BOX_TEXTURE
{
public:
	int m_anim_pattern[6];
};


//-----------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------
void InitSprite3D(void);
void UninitSprite3D(void);
void DrawSpriteModel(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale, XMFLOAT4 color, MODEL* model);
void DrawSpriteBox(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale, XMFLOAT4 color, int xnum, int ynum, BOX_TEXTURE* p_box_texture);
void SetVertex(void);