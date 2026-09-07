//-----------------------------------------------
// Box.h
// 制作日：2024/12/10
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

//-----------------------------------------------
// マクロ定義
//-----------------------------------------------
#define		BOX_MAX		(100)		//BOXの数
#define BOX_WIDTH	(2.0f)
#define BOX_HEIGHT	(2.0f)


//-----------------------------------------------
// 列挙体宣言
//-----------------------------------------------
enum BOX_TYPE
{
	BOX_TYPE_NORMAL = 0,
	BOX_TYPE_FLOOR,			//床
	BOX_TYPE_WALL,			//壁
};

#include "GameObject3D.h"

class BOX : public GAME_OBJECT_3D
{
private:
	BOX_TYPE m_type;
	ID3D11ShaderResourceView* m_texture;

public:
	void Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	//セッター
	void SetType(BOX_TYPE type) { m_type = type; };

	//ゲッター
	BOX_TYPE GetType(void) { return m_type; };
};



void	InitBox();
void	UninitBox();
void	UpdateBox();
void	DrawBox();


