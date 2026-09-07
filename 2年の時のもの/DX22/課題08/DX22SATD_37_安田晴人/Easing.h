//-----------------------------------------------
// Easing.h
//-----------------------------------------------

#pragma once

#include "renderer.h"

struct EASING
{
	XMFLOAT2 posi;
	float time;

	bool isReset;
};

//-----------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------
void InitEasing(void);

EASING GetEasing(int num);

void Bezier(float speed, int num, XMFLOAT2 p2, XMFLOAT2 p3);

XMFLOAT2 DiagonalEasing(XMFLOAT2 p0, XMFLOAT2 p1, int num);





/*XMFLOAT2 easeInElastic(XMFLOAT2 p0, XMFLOAT2 p1, float speed,  int num);
XMFLOAT2 easeOutElastic(XMFLOAT2 p0, XMFLOAT2 p1, float speed,  int num);
XMFLOAT2 easeInOutElastic(XMFLOAT2 p0, XMFLOAT2 p1, float speed,  int num);
XMFLOAT2 easeInBounce(XMFLOAT2 p0, XMFLOAT2 p1, float speed,  int num);
XMFLOAT2 easeOutBounce(XMFLOAT2 p0, XMFLOAT2 p1, float speed,  int num);
XMFLOAT2 easeInOutBounce(XMFLOAT2 p0, XMFLOAT2 p1, float speed,  int num);*/

void easeInElastic(float speed, int num);
void easeOutElastic(float speed, int num);
void easeInOutElastic(float speed, int num);
void easeInBounce(float speed, int num);
void easeOutBounce(float speed, int num);
void easeInOutBounce(float speed, int num);

// タイマーリセット	手動
void TimeReset(int num);