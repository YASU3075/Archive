//-----------------------------------------------
// Box.cpp
// 制作日：2024/12/10
// 制作者：安田晴人
//-----------------------------------------------
#include	"Box.h"
#include "model.h"
#include "Sprite3D.h"

void BOX::Init()
{
	//テクスチャの読み込み
	TexMetadata		metadata;
	ScratchImage	image;

	LoadFromWICFile(L"asset\\texture\\mapchip.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_texture);
	assert(m_texture);
}

void BOX::Uninit()
{
	if (m_texture != NULL)
	{
		m_texture->Release();
		m_texture = NULL;
	}
}

void BOX::Update()
{

}

void BOX::Draw()
{
	BOX_TEXTURE tex;
	tex.m_anim_pattern[0] = 1;
	tex.m_anim_pattern[1] = 4;
	tex.m_anim_pattern[2] = 9;
	tex.m_anim_pattern[3] = 11;
	tex.m_anim_pattern[4] = 20;
	tex.m_anim_pattern[5] = 30;

	GetDeviceContext()->PSSetShaderResources(0, 1, &m_texture);

	DrawSpriteBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 8, &tex);
}

