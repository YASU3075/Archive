//-----------------------------------------------
// MaterialManager.cpp
// 制作日：2025/07/24
// 制作者：安田晴人
//-----------------------------------------------
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Main.h"
#include "renderer/Renderer.h"
#include "renderer/MaterialManager.h"
#include "renderer/Material.h"
#include "renderer/TextureManager.h"

using KVMap = std::unordered_map<std::string, std::string>;

std::unordered_map<std::string, MATERIAL*> MATERIAL_MANAGER::m_material_pool;

//-----------------------------------------------
// 文字列の前後の空白を削除
//-----------------------------------------------
static inline std::string Trim(const std::string& str)
{
	//空白文字
	auto ws = " \t\n\r\f\v";

	//先頭の空白でない文字位置を検索
	size_t l = str.find_first_not_of(ws);
	if (l == std::string::npos)
	{//空文字列の場合
		return "";
	}

	//末尾の空白でない文字位置を検索
	size_t r = str.find_last_not_of(ws);
	return str.substr(l, r - l + 1);
}

//-----------------------------------------------
// マテリアルファイルをパース
//-----------------------------------------------
static inline KVMap ParseMaterialFile(const char* fileName)
{
	KVMap kv_map;
	std::ifstream file(fileName);
	if (!file.is_open())
	{// ファイルが開けなかった場合は空のマップを返す
		return kv_map;
	}

	std::string line;
	while (std::getline(file, line))
	{
		line = Trim(line); // 前後の空白を削除
		if (line.empty() || line[0] == '#')
		{// 空行またはコメント行はスキップ
			continue;
		}
		size_t pos = line.find('=');
		if (pos != std::string::npos)
		{
			std::string key = Trim(line.substr(0, pos));
			std::string value = Trim(line.substr(pos + 1));
			kv_map[key] = value;
		}
	}

	file.close();
	return kv_map;
}

//-----------------------------------------------
// 型変換(XMFLOAT4)
//-----------------------------------------------
static inline XMFLOAT4 ToXMFLOAT4(const std::string& str)
{
	XMFLOAT4 result{ 0.0f, 0.0f, 0.0f, 1.0f };
	std::istringstream ss(str);
	char delimiter;
	ss >> result.x >> delimiter >> result.y >> delimiter >> result.z >> delimiter >> result.w;
	return result;
}

//-----------------------------------------------
// 型変換（XMFLOAT2）
//-----------------------------------------------
static inline XMFLOAT2 ToXMFLOAT2(const std::string& str)
{
	XMFLOAT2 result{ 0.0f, 0.0f };
	std::istringstream ss(str);
	char delimiter;
	ss >> result.x >> delimiter >> result.y;
	return result;
}

//-----------------------------------------------
// 型変換(bool)
//-----------------------------------------------
static inline bool ToBool(const std::string& str)
{
	return (str == "true" || str == "1");
}

//-----------------------------------------------
// 型変換(string)
//-----------------------------------------------
static inline std::string ToString(const std::string& str)
{
	return str; // 文字列はそのまま返す
}

//-----------------------------------------------
// ロード
//-----------------------------------------------
MATERIAL* MATERIAL_MANAGER::LoadFromCustomFile(const char* name)
{
	auto it = m_material_pool.find(name);
	if (it != m_material_pool.end())
	{// 既にロード済み
		return it->second;
	}

	// パーサーでキー取得
	auto kv = ParseMaterialFile(name);

	auto mat = new MATERIAL();
	mat->CreateShaders(kv["vertex_shader"].c_str(), kv["pixel_shader"].c_str());
	mat->m_material_data.m_ambient = ToXMFLOAT4(kv["ambient"]);
	mat->m_material_data.m_diffuse = ToXMFLOAT4(kv["diffuse"]);
	mat->m_material_data.m_specular = ToXMFLOAT4(kv["specular"]);
	mat->m_material_data.m_emission = ToXMFLOAT4(kv["emission"]);
	mat->m_material_data.m_shininess = std::stof(kv["shininess"]);
	mat->m_material_data.m_texture_enable = ToBool(kv["texture_enable"]);
	mat->m_material_data.m_uv_scale = ToXMFLOAT2(kv["uv_scale"]);
	mat->m_material_data.m_uv_offset = ToXMFLOAT2(kv["uv_offset"]);

	if (mat->m_material_data.m_texture_enable)
	{
		mat->m_texture = TEXTURE_MANAGER::Load(kv["texture"].c_str());
	}

	m_material_pool[name] = mat;
	return mat;
}

//-----------------------------------------------
// 解放
//-----------------------------------------------

void MATERIAL_MANAGER::ReleaseAll()
{
	for (auto& pair : m_material_pool)
	{
		delete pair.second; // マテリアルの解放
	}
	m_material_pool.clear(); // マテリアルプールをクリア
}
