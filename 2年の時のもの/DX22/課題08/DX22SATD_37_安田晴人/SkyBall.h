//-----------------------------------------------
// SkyBall.h
// 制作日：2025/02/19
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "main.h"
#include "renderer.h"

class CAMERA;

class SKY_BALL_OBJECT
{
public:
	CAMERA* p_camera;

	bool use;
	XMFLOAT3 pos;
	XMFLOAT3 sca;
	XMFLOAT3 rot;
};

void InitSkyBall();
void UninitSkyBall();
void UpdateSkyBall();
void DrawSkyBall();
SKY_BALL_OBJECT* GetSkyBall();
