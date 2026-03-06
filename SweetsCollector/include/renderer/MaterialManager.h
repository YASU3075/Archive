//-----------------------------------------------
// MaterialManager.h
// 制作日：2025/07/24
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <unordered_map>
#include <string>



class MATERIAL;
class MATERIAL_MANAGER
{
private:
	static std::unordered_map<std::string, MATERIAL*> m_material_pool;

public:

	static MATERIAL_MANAGER& GetInstance()
	{
		static MATERIAL_MANAGER instance;
		return instance;
	}
	static MATERIAL* LoadFromCustomFile(const char* name);
	static void ReleaseAll();
};