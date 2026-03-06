//-----------------------------------------------
// DirectionalLight.cpp
// 制作日：2025/11/27
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "object/DirectionalLight.h"

#include "component/renderer/LightComponent.h"
#include "renderer/LightManager.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void DIRECTIONAL_LIGHT::Init()
{
	// ライトコンポーネント追加
	auto light_component = AddComponent<LIGHT_COMPONENT>();
	LIGHT_DESC light_desc = {};
	light_desc.enable = true;
	light_desc.color = XMFLOAT3(1.0f, 1.0f, 1.0f);
	light_desc.kind = LIGHT_KIND::DIRECTIONAL;
	light_desc.intensity = m_intensity;
	light_component->SetLightDesc(light_desc);
	// ライトマネージャーに登録
	LIGHT_MANAGER::GetInstance().Register(light_component);
}