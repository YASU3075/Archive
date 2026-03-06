//-----------------------------------------------
// LightDesc.h
// 制作日：2025/11/19
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include "Main.h"


enum class LIGHT_KIND
{
	DIRECTIONAL,
	POINT,
	SPOT,
};

struct LIGHT_DESC
{
	LIGHT_KIND kind = LIGHT_KIND::DIRECTIONAL;

	XMFLOAT3 color = { 1.0f, 1.0f, 1.0f };
	float intensity = 1.0f;

	XMFLOAT3 ambient = { 0.1f, 0.1f, 0.1f };
	float range = 10.0f; // POINTとSPOT用

	float spot_inner_angle = XM_PIDIV4; // ラジアン
	float spot_outer_angle = XM_PIDIV2; // ラジアン
	bool enable = true;
	bool dummy[3] = { false, false, false };
};