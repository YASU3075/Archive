//-----------------------------------------------
// LightManager.h
// 制作日：2025/11/19
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <vector>
#include "renderer/Renderer.h"
#include "math/Vector3.h"


class LIGHT_COMPONENT;
class LIGHT_MANAGER
{
private:
	std::vector<LIGHT_COMPONENT*> m_lights;
	LIGHT MakeLightFromComponent(const LIGHT_COMPONENT& comp);

public:
	static LIGHT_MANAGER& GetInstance()
	{
		static LIGHT_MANAGER instance;
		return instance;
	}

	void Register(LIGHT_COMPONENT* light);
	void Unregister(LIGHT_COMPONENT* light);

	void GatherLights(const VECTOR3& pos, LIGHT* out_lights, UINT& out_count, UINT max_lights = 4);

};
