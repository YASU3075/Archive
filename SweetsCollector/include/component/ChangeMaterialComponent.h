//-----------------------------------------------
// ChangeMaterialComponent.h
// 制作日：2025/09/14
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <unordered_map>
#include <string>
#include "component/Component.h"

class CHANGE_MATERIAL_COMPONENT : public COMPONENT
{
private:
	std::unordered_map<std::string, class MATERIAL> m_material_map;
public:
	void Init() override;
	void Update() override;

	void AddMaterial(const std::string& key, MATERIAL* material);
	void ChangeMaterial(const std::string& key);
	MATERIAL* GetMaterial(const std::string& key);
};