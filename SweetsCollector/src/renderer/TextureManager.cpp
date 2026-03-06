//-----------------------------------------------
// TextureManager.cpp
// 制作日：2025/06/11
// 制作者：安田晴人
//-----------------------------------------------
#include "main.h"
#include "renderer/Renderer.h"
#include "renderer/TextureManager.h"

std::unordered_map<std::string, ID3D11ShaderResourceView*> TEXTURE_MANAGER::m_texture_pool;

//-----------------------------------------------
// テクスチャのロード
//-----------------------------------------------
ID3D11ShaderResourceView* TEXTURE_MANAGER::Load(const char* name)
{
	if (m_texture_pool.count(name) > 0)
	{
		return m_texture_pool[name];
	}

	wchar_t w_file_name[512];
	mbstowcs(w_file_name, name, strlen(name) + 1);

	//無かったらテクスチャを作成して返す
	ID3D11ShaderResourceView* ret = nullptr;
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(w_file_name, WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &ret);
	assert(ret);

	m_texture_pool[name] = ret;

	return ret;
}

//-----------------------------------------------
// ロードしたテクスチャの解放
//-----------------------------------------------
void TEXTURE_MANAGER::ReleaseAll()
{
	for (auto& texture : m_texture_pool)
	{
		if (texture.second)
		{
			texture.second->Release();
			texture.second = nullptr;
		}
	}
	m_texture_pool.clear();
}
