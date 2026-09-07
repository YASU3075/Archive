//-----------------------------------------------
// Map.h
// 制作日：2025/01/05
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include "renderer.h"

//-----------------------------------------------
// マクロ定義
//-----------------------------------------------
#define MAP_SIZE_X		(20)
#define MAP_SIZE_Z		(20)

class MAP
{
private:
	char m_map_chip[MAP_SIZE_Z][MAP_SIZE_X + 1] =
	{
		"nnnnnnnnnnnnnnnnnnnn",
		"nnnnnnnnnnnnnnnnnnnn",
		"nnnnnnnnnnnnnnnnnnnn",
		"nnnnnnnnnnnnnnnnnnnn",
		"nnnnnnnnnnnnnnnnnnnn",
		"nnnnnnnnnnnnnnnnnnnn",
		"nnnnnnnnnnnnnnnnnnnn",
		"nnnnnnnnnnnnnnnnnnnn",
		"nnnnnnnnnnnnnnnnnnnn",
		"nnnnnnnnnnnnnnnnnnnn",
		"nnnnnnnnnnnnnnnnnnnn",
		"nnnnnnnnnnnnnnnnnnnn",
		"nnnnnnnnnnnnnnnnnnnn",
		"nnnnnnnnnnnnnnnnnnnn",
		"nnnnnnnnnnnnnnnnnnnn",
		"nnnnnnnnnnnnnnnnnnnn",
		"nnnnnnnnnnnnnnnnnnnn",
		"nnnnnnnnnnnnnnnnnnnn",
		"nnnnnnnnnnnnnnnnnnnn",
		"nnnnnnnnnnnnnnnnnnnn",
	};

	ID3D11ShaderResourceView* m_texture;

public:
	MAP() {}
	~MAP() {}

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
};