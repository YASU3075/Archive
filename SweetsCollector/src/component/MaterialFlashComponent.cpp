//-----------------------------------------------
// MaterialFlashComponent.cpp
// 制作日：2025/09/14
// 制作者：安田晴人
//-----------------------------------------------

#include "Main.h"
#include "scene/Manager.h"
#include "renderer/Renderer.h"
#include "entity/GameObject.h"

#include "component/renderer/SpriteRendererComponent.h"
#include "component/MaterialFlashComponent.h"
#include "renderer/Material.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void MATERIAL_FLASH_COMPONENT::Init()
{
	m_p_material = m_p_owner->GetComponent<RENDERER_COMPONENT>()->GetMaterial();
	m_frame = 0.0f;
	m_flash_time = 1.0f;
	m_add_frame = 0.1f;
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void MATERIAL_FLASH_COMPONENT::Update()
{
	m_frame += m_add_frame;
	if (m_frame > m_flash_time)
	{
		m_frame = m_flash_time;
		m_add_frame *= -1.0f;
	}
	else if (m_frame < 0.0f)
	{
		m_frame = 0.0f;
		m_add_frame *= -1.0f;
	}
	m_p_material->m_material_data.m_diffuse.w = m_frame / m_flash_time;
}

