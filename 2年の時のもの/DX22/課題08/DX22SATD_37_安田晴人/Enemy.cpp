//-----------------------------------------------
// Enemy.cpp
// 制作日：2025/01/08
// 制作者：安田晴人
//-----------------------------------------------
#include "Enemy.h"
#include "Sprite3D.h"
#include "Game.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void ENEMY::Init(void)
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
void ENEMY::Uninit(void)
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
void ENEMY::Update(void)
{
	GAME_OBJECT_3D_CONTAINER objects =  m_p_game->GetGameObjectContainer(m_collider_index);
	for (int i = 0; i < objects.GetCount(); i++)
	{
		if (objects.GetGameObject(i)->GetTag() == "BULLET")
		{
			objects.GetGameObject(i)->SetDestroy(true);
			m_destroy = true;
			break;
		}
	}
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void ENEMY::Draw(void)
{
	BOX_TEXTURE tex;
	tex.m_anim_pattern[0] = 2;
	tex.m_anim_pattern[1] = 2;
	tex.m_anim_pattern[2] = 2;
	tex.m_anim_pattern[3] = 2;
	tex.m_anim_pattern[4] = 2;
	tex.m_anim_pattern[5] = 2;

	GetDeviceContext()->PSSetShaderResources(0, 1, &m_texture);
	DrawSpriteBox(m_pos, m_rot, XMFLOAT3(m_sca.x, m_sca.y * 1.5f, m_sca.z), m_color, 8, 8, &tex);
}