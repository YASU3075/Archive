//-----------------------------------------------
// Easing.cpp
// 制作日：2025/01/18
//-----------------------------------------------
#include "Easing.h"
#include "sprite.h"

//-----------------------------------------------
// マクロ定義
//-----------------------------------------------
#define BEIZER_NUM			20

//-----------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------
float TimeCheck(float time, float speed, int num);

//-----------------------------------------------
// グローバル変数
//-----------------------------------------------
static EASING Easing[BEIZER_NUM];

// 初期化	main.cppに突っ込む
void InitEasing(void)
{
	for (int i = 0; i < BEIZER_NUM; i++)
	{
		Easing[i].posi = {};
		Easing[i].time = 0;
		Easing[i].isReset = true;
	}
}

// 各Updataに突っ込む



/*========================================================
// ゲッター
========================================================*/
EASING GetEasing(int num)
{
	return Easing[num];
}

/*========================================================
// 現在地計算
* p0 始点
* p1 支点1
* p2 支点2
* p3 終点
* num 番号
========================================================*/
void RecalBezier(XMFLOAT2 p0, XMFLOAT2 p1, XMFLOAT2 p2, XMFLOAT2 p3, int num)
{
	XMFLOAT2 P;			// 点P
	float t = Easing[num].time;

	float i = 1 - t;	// 1 - t

	P.x = pow(i, 3) * p0.x + 3 * t * pow(i, 2) * p1.x + 3 * pow(t, 2) * i * p2.x + pow(t, 3) * p3.x;
	P.y = pow(i, 3) * p0.y + 3 * t * pow(i, 2) * p1.y + 3 * pow(t, 2) * i * p2.y + pow(t, 3) * p3.y;

	Easing[num].posi = P;
}

/*========================================================
// 斜め計算
* p0 始点
* p1 終点
* num 番号
========================================================*/
XMFLOAT2 DiagonalEasing(XMFLOAT2 p0, XMFLOAT2 p1, int num)
{
	if (Easing[num].isReset)
	{
		Easing[num].time = 0.0f;
		Easing[num].isReset = false;
	}

	XMFLOAT2 P;			// 点P

	float saX, saY;
	saX = p1.x - p0.x;
	saY = p1.y - p0.y;

	P.x = Easing[num].posi.y * saX + p0.x;
	P.y = Easing[num].posi.y * saY + p0.y;
	
	return P;
}

/*========================================================
// 3次ベジェ値 https://cubic-bezier.com/#.19,.85,.89,.13
* p0	※始点
* p1	※終点
* p2	ピンクの点
* p3	青の点
* 速度
* ループ
* 番号
========================================================*/
void Bezier(float speed, int num, XMFLOAT2 p2, XMFLOAT2 p3)
{
	RecalBezier(XMFLOAT2(0, 0), p2, p3, XMFLOAT2(1, 1), num);

	Easing[num].time = TimeCheck(Easing[num].time, speed, num);
}

/*========================================================
// Easing functions
* p0 始点
* p1 終点
* speed 速さ 0.0～1.0
* loop 繰り返し
* num 時間の番号
========================================================*/
void easeInElastic(float speed, int num)
{
	Easing[num].time = TimeCheck(Easing[num].time, speed, num);

	float c4 = (2 * 3.14f) / 3;
	Easing[num].posi.y = -powf(2, 10 * Easing[num].time - 10) * sinf((Easing[num].time * 10 - 10.75) * c4);
}
void easeOutElastic(float speed, int num)
{
	Easing[num].time = TimeCheck(Easing[num].time, speed, num);

	float c4 = (2 * 3.14f) / 3;
	Easing[num].posi.y = powf(2, -10 * Easing[num].time) * sinf((Easing[num].time * 10 - 0.75) * c4) + 1;
}
void easeInOutElastic(float speed, int num)
{
	Easing[num].time = TimeCheck(Easing[num].time, speed, num);

	float c5 = (2 * 3.14f) / 4.5;

	if (Easing[num].time < 0.5f)
		Easing[num].posi.y = -(powf(2, 20 * Easing[num].time - 10) * sinf((20 * Easing[num].time - 11.125) * c5)) / 2;
	else
		Easing[num].posi.y = (powf(2, -20 * Easing[num].time + 10) * sinf((20 * Easing[num].time - 11.125) * c5)) / 2 + 1;
}
void easeInBounce(float speed, int num)
{
	Easing[num].time = TimeCheck(Easing[num].time, speed, num);

	float time = 1 - Easing[num].time;

	float n1 = 7.5625f;
	float d1 = 2.75f;

	if (time < 1.0f / d1) {
		Easing[num].posi.y = 1 - (n1 * time * time);
	}
	else if (time < 2.0f / d1) {
		Easing[num].posi.y = 1 - (n1 * (time -= 1.5f / d1) * time + 0.75f);
	}
	else if (time < 2.5f / d1) {
		Easing[num].posi.y = 1 - (n1 * (time -= 2.25f / d1) * time + 0.9375f);
	}
	else {
		Easing[num].posi.y = 1 - (n1 * (time -= 2.625f / d1) * time + 0.984375f);
	}
}
void easeOutBounce(float speed, int num)
{
	Easing[num].time = TimeCheck(Easing[num].time, speed, num);

	float time = Easing[num].time;

	float n1 = 7.5625f;
	float d1 = 2.75f;

	if (time < 1.0f / d1) {
		Easing[num].posi.y = n1 * time * time;
	}
	else if (time < 2.0f / d1) {
		Easing[num].posi.y = n1 * (time -= 1.5f / d1) * time + 0.75f;
	}
	else if (time < 2.5f / d1) {
		Easing[num].posi.y = n1 * (time -= 2.25f / d1) * time + 0.9375f;
	}
	else {
		Easing[num].posi.y = n1 * (time -= 2.625f / d1) * time + 0.984375f;
	}
}
void easeInOutBounce(float speed, int num)
{
	Easing[num].time = TimeCheck(Easing[num].time, speed, num);

	float time;
	float n1 = 7.5625f;
	float d1 = 2.75f;

	if (Easing[num].time < 0.5)
		time = 1 - 2 * Easing[num].time;
	else
		time = 2 * Easing[num].time - 1;

	if (time < 1.0f / d1) {
		Easing[num].posi.y = n1 * time * time;
	}
	else if (time < 2.0f / d1) {
		Easing[num].posi.y = n1 * (time -= 1.5f / d1) * time + 0.75f;
	}
	else if (time < 2.5f / d1) {
		Easing[num].posi.y = n1 * (time -= 2.25f / d1) * time + 0.9375f;
	}
	else {
		Easing[num].posi.y = n1 * (time -= 2.625f / d1) * time + 0.984375f;
	}

	if (Easing[num].time < 0.5)
		Easing[num].posi.y = (1 - Easing[num].posi.y) / 2;
	else
		Easing[num].posi.y = (1 + Easing[num].posi.y) / 2;
}



// ループするか、リセットかけるか確認
float TimeCheck(float time, float speed, int num)
{
	time += speed;
	if (1.0f + 60 * speed < time)
	{
		Easing[num].isReset = true;
		return 0;
	}
	return time;
}

// 手動
void TimeReset(int num)
{
	Easing[num].time = 0.0f;
}