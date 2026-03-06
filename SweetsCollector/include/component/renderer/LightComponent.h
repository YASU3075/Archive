//-----------------------------------------------
// LightComponent.h
// 制作日：2025/11/19
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "renderer/LightDesc.h"
#include "component/Component.h"

class LIGHT_COMPONENT : public COMPONENT
{
private:
	LIGHT_DESC m_light_desc;

public:

	void Uninit() override;

	void SetLightDesc(const LIGHT_DESC& desc) { m_light_desc = desc; }
	const LIGHT_DESC& GetLightDesc() const { return m_light_desc; }

	VECTOR3 GetWorldPosition() const;
	VECTOR3 GetWorldDirection() const;
};
