//-----------------------------------------------
// Map.cpp
// 制作日：2025/01/05
// 制作者：安田晴人
//-----------------------------------------------
#include "Map.h"
#include "Box.h"
#include "Sprite3D.h"


//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void MAP::Init(void)
{
	//テクスチャの読み込み
	TexMetadata		metadata;
	ScratchImage	image;

	LoadFromWICFile(L"asset\\texture\\mapchip.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_texture);
	assert(m_texture);
}

//-----------------------------------------------
// 終了処理
//-----------------------------------------------
void MAP::Uninit(void)
{
	if (m_texture != NULL)
	{
		m_texture->Release();
		m_texture = NULL;
	}
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void MAP::Update(void)
{

}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void MAP::Draw(void)
{
	BOX_TEXTURE tex;

	for (int z = 0; z < MAP_SIZE_Z; z++)
	{
		for (int x = 0; x < MAP_SIZE_X; x++)
		{
			if (m_map_chip[z][x] == 'n')
			{//ノーマル
				tex.m_anim_pattern[0] = 7;
				tex.m_anim_pattern[1] = 7;
				tex.m_anim_pattern[2] = 7;
				tex.m_anim_pattern[3] = 7;
				tex.m_anim_pattern[4] = 7;
				tex.m_anim_pattern[5] = 7;

				GetDeviceContext()->PSSetShaderResources(0, 1, &m_texture);

				DrawSpriteBox(XMFLOAT3(BOX_WIDTH * x, 0.0f, BOX_HEIGHT * z), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(BOX_WIDTH, BOX_WIDTH, BOX_HEIGHT), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 8, &tex);
			}
		}
	}
}
