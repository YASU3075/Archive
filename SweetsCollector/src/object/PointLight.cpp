//-----------------------------------------------
// PointLight.cpp
// 制作日：2025/11/19
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "object/PointLight.h"
#include "component/renderer/LightComponent.h"

#include "renderer/LightManager.h"

//-----------------------------------------------
// 初期化
//-----------------------------------------------
void POINT_LIGHT::Init()
{
	m_range = 20.0f;

	// ライトコンポーネント追加
	auto light_component = AddComponent<LIGHT_COMPONENT>();

	LIGHT_DESC light_desc = {};
	light_desc.enable = true;
	light_desc.color = m_color;
	light_desc.kind = LIGHT_KIND::POINT;
	light_desc.range = m_range;
	light_desc.intensity = m_intensity;

	light_component->SetLightDesc(light_desc);

	// ライトマネージャーに登録
	LIGHT_MANAGER::GetInstance().Register(light_component);
}

void POINT_LIGHT::SetColor(const XMFLOAT3& color)
{
	m_color = color;

	auto light_component = GetComponent<LIGHT_COMPONENT>();
	if (light_component)
	{
		LIGHT_DESC light_desc = light_component->GetLightDesc();
		light_desc.color = m_color;
		light_component->SetLightDesc(light_desc);
	}
}
