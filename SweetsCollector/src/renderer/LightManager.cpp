//-----------------------------------------------
// LightManager.cpp
// 制作日：2025/11/19
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/LightManager.h"
#include "component/renderer/LightComponent.h"

//-----------------------------------------------
// LIGHT生成
//-----------------------------------------------
LIGHT LIGHT_MANAGER::MakeLightFromComponent(const LIGHT_COMPONENT& comp)
{
	LIGHT light{};
	const LIGHT_DESC& desc = comp.GetLightDesc();

	light.Enable = desc.enable ? TRUE : FALSE;

	switch (desc.kind)
	{
	case LIGHT_KIND::DIRECTIONAL:
		light.Type = static_cast<UINT>(LIGHT_TYPE::DIRECTIONAL);
		break;
	case LIGHT_KIND::POINT:
		light.Type = static_cast<UINT>(LIGHT_TYPE::POINT);
		break;
	case LIGHT_KIND::SPOT:
		light.Type = static_cast<UINT>(LIGHT_TYPE::SPOT);
		break;
	default:
		break;
	}

	VECTOR3 pos = comp.GetWorldPosition();
	VECTOR3 dir = comp.GetWorldDirection().GetNormalized();

	light.Position = XMFLOAT4(pos.x, pos.y, pos.z, 1.0f);
	light.Direction = XMFLOAT4(dir.x, dir.y, dir.z, 0.0f);

	light.Diffuse = XMFLOAT4(
		desc.color.x * desc.intensity,
		desc.color.y * desc.intensity,
		desc.color.z * desc.intensity,
		1.0f
	);

	light.Ambient = XMFLOAT4(
		desc.ambient.x,
		desc.ambient.y,
		desc.ambient.z,
		1.0f
	);

	light.Range = desc.range;
	light.SpotInner = std::cos(desc.spot_inner_angle);
	light.SpotOuter = std::cos(desc.spot_outer_angle);

	return light;
}

//-----------------------------------------------
// ライト登録
//-----------------------------------------------
void LIGHT_MANAGER::Register(LIGHT_COMPONENT* light)
{
	for (auto& registered_light : m_lights)
	{
		if (registered_light == light)
		{
			// すでに登録されている
			return;
		}
	}

	m_lights.push_back(light);
}

//-----------------------------------------------
// ライト登録解除
//-----------------------------------------------
void LIGHT_MANAGER::Unregister(LIGHT_COMPONENT* light)
{
	for (auto it = m_lights.begin(); it != m_lights.end(); ++it)
	{
		if (*it == light)
		{
			m_lights.erase(it);
			return;
		}
	}
}

//-----------------------------------------------
// ライト収集
//-----------------------------------------------
void LIGHT_MANAGER::GatherLights(const VECTOR3& pos, LIGHT* out_lights, UINT& out_count, UINT max_lights)
{
	struct Candidate
	{
		LIGHT_COMPONENT* light_component;
		float distance_sq;
	};

	std::vector<LIGHT_COMPONENT*> directional_lights;
	std::vector<Candidate> others;

	// ライトを分類
	for (auto& light_comp : m_lights)
	{
		const LIGHT_DESC& desc = light_comp->GetLightDesc();
		if (!desc.enable)
		{
			continue;
		}
		if (desc.kind == LIGHT_KIND::DIRECTIONAL)
		{
			directional_lights.push_back(light_comp);
		}
		else
		{
			VECTOR3 light_pos = light_comp->GetWorldPosition();
			float dist_sq = (light_pos - pos).LengthSquared();
			others.push_back({ light_comp, dist_sq });
		}
	}

	// 距離でソート
	std::sort(others.begin(), others.end(), [](const Candidate& a, const Candidate& b) {
		return a.distance_sq < b.distance_sq;
		});

	out_count = 0;

	// Directionalライトを追加
	for (auto& dir_light_comp : directional_lights)
	{
		if (out_count >= max_lights)
		{
			break;
		}

		out_lights[out_count++] = MakeLightFromComponent(*dir_light_comp);
	}

	// 残りのライトを追加
	for (auto& candidate : others)
	{
		if (out_count >= max_lights)
		{
			break;
		}
		out_lights[out_count++] = MakeLightFromComponent(*candidate.light_component);
	}

}