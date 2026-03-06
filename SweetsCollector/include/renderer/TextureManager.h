//-----------------------------------------------
// TextureManager.h
// 制作日：2025/06/11
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include <unordered_map>
#include <string>

class TEXTURE_MANAGER
{
private:
	static std::unordered_map<std::string, ID3D11ShaderResourceView*> m_texture_pool;

public:
	static ID3D11ShaderResourceView* Load(const char* name);
	static void ReleaseAll();
};