//-----------------------------------------------
// ChangeMaterialComponent.cpp
// 制作日：2025/09/14
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "entity/GameObject.h"

#include "component/ChangeMaterialComponent.h"

#include "component/renderer/ModelRendererComponent.h"
#include "component/renderer/SpriteRendererComponent.h"

#include "renderer/MaterialManager.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void CHANGE_MATERIAL_COMPONENT::Init()
{
	m_material_map.clear();
}

void CHANGE_MATERIAL_COMPONENT::Update()
{
}

//-----------------------------------------------
// マテリアルの追加
//-----------------------------------------------
void CHANGE_MATERIAL_COMPONENT::AddMaterial(const std::string& key, MATERIAL* material)
{
	if (!material)
	{
		return;
	}
	m_material_map[key] = *material;
}

//-----------------------------------------------
// マテリアルの変更
//-----------------------------------------------
void CHANGE_MATERIAL_COMPONENT::ChangeMaterial(const std::string& key)
{
	auto it = m_material_map.find(key);
	if (it == m_material_map.end())
	{
		return;
	}
	auto mesh_renderer = m_p_owner->GetComponent<MODEL_RENDERER_COMPONENT>();
	if (mesh_renderer)
	{
		mesh_renderer->SetMaterial(&it->second);
		return;
	}
	auto sprite_renderer = m_p_owner->GetComponent<SPRITE_RENDERER_COMPONENT>();
	if (sprite_renderer)
	{
		sprite_renderer->SetMaterial(&it->second);
		return;
	}
}

//-----------------------------------------------
// マテリアルの取得
//-----------------------------------------------
MATERIAL* CHANGE_MATERIAL_COMPONENT::GetMaterial(const std::string& key)
{
	auto it = m_material_map.find(key);
	if (it == m_material_map.end())
	{
		return nullptr;
	}
	return &it->second;
}