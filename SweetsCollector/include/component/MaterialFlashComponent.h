//-----------------------------------------------
// MaterialFlashComponent.h
// 制作日：2025/09/14
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "component/Component.h"

class MATERIAL_FLASH_COMPONENT : public COMPONENT
{
private:
	float m_add_frame = 0.1f;
	float m_frame = 0.0f;
	float m_flash_time = 0.0f;
	class MATERIAL* m_p_material = nullptr;
public:
	void Init() override;
	void Update() override;

	void SetFlashTime(float time) { m_flash_time = time; }
};
