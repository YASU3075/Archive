//-----------------------------------------------
// Shore.h
// 制作日：2026/01/28
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "entity/GameObject.h"

struct SHORE_PARAMS
{
	float river_center_x = 0.0f;
    float river_half_width = 1.0f;
	float shore_blend = 0.5f;
	float river_depth = 0.5f;

	float terrain_amp = 1.0f;
	float terrain_freq = 1.0f;
	float z_slope = 0.1f;
};

class SHORE : public GAME_OBJECT
{
private:
	float ShoreHeight(float x, float z, const SHORE_PARAMS& params);
public:
	void Init() override;
};