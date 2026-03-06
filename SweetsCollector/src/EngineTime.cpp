//-----------------------------------------------
// EngineTime.cpp
// 制作日：2026/01/10
// 制作者：安田晴人
//-----------------------------------------------
#include <windows.h>

#include "EngineTime.h"

int64_t TIME::m_frequency = {};
int64_t TIME::m_previous_time = {};

double TIME::m_unscaled_delta_time = 0.0;
double TIME::m_scaled_delta_time = 0.0;

double TIME::m_unscaled_time = 0.0;
double TIME::m_scaled_time = 0.0;

double TIME::m_time_scale = 1.0;

unsigned long long TIME::m_frame_count = 0;


//------------------------------------------------
// 初期化処理
//------------------------------------------------
void TIME::Init()
{
	LARGE_INTEGER freq{};
	QueryPerformanceFrequency(&freq);
	m_frequency = freq.QuadPart;

	LARGE_INTEGER now{};
	QueryPerformanceCounter(&now);
	m_previous_time = now.QuadPart;

	m_unscaled_delta_time = 0.0;
	m_scaled_delta_time = 0.0;
	m_unscaled_time = 0.0;
	m_scaled_time = 0.0;
	m_time_scale = 1.0;
	m_frame_count = 0;
}

//------------------------------------------------
// フレーム更新処理
//------------------------------------------------
void TIME::Tick()
{
	LARGE_INTEGER now{};
	QueryPerformanceCounter(&now);

	const int64_t diff = now.QuadPart - m_previous_time;
	m_previous_time = now.QuadPart;

	double dt = (m_frequency > 0) ? (static_cast<double>(diff) / static_cast<double>(m_frequency)) : 0.0;

	// スパイク対策
	if (dt > 0.1) dt = 0.1;
	if (dt < 0.0) dt = 0.0;

	m_unscaled_delta_time = dt;
	m_scaled_delta_time = dt * m_time_scale;

	m_unscaled_time += m_unscaled_delta_time;
	m_scaled_time += m_scaled_delta_time;

	m_frame_count++;
}

//------------------------------------------------
// スケール適用後のデルタタイム（秒）取得
//------------------------------------------------
float TIME::DeltaTime()
{
	return static_cast<float>(m_scaled_delta_time);
}

//------------------------------------------------
// スケール適用前のデルタタイム（秒）取得
//------------------------------------------------
float TIME::UnscaledDeltaTime()
{
	return static_cast<float>(m_unscaled_delta_time);
}

//------------------------------------------------
// スケール適用後の累積時間（秒）取得
//------------------------------------------------
float TIME::TimeSinceStartup()
{
	return static_cast<float>(m_scaled_time);
}

//------------------------------------------------
// スケール適用前の累積時間（秒）取得
//------------------------------------------------
float TIME::UnscaledTime()
{
	return static_cast<float>(m_unscaled_time);
}

//------------------------------------------------
// 時間スケール取得
//------------------------------------------------
double TIME::GetTimeScale()
{
	return m_time_scale;
}

//------------------------------------------------
// 時間スケール設定
//------------------------------------------------
void TIME::SetTimeScale(double scale)
{
	m_time_scale = 0.0 > scale ? 0.0 : scale;
}

//------------------------------------------------
// フレームカウント取得
//------------------------------------------------
unsigned long long TIME::FrameCount()
{
	return m_frame_count;
}