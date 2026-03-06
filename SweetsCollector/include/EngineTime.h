//------------------------------------------------
// EngineTime.h
// 制作日：2026/01/10
// 制作者：安田晴人
//------------------------------------------------
#pragma once
#include <cstdint>

class TIME
{
private:
	static int64_t m_frequency;			// 高精度パフォーマンスカウンタの周波数
	static int64_t m_previous_time;		// 前回のフレームのカウンタ値

	static double m_unscaled_delta_time;		// 前フレームからの経過時間（スケール適用前）
	static double m_scaled_delta_time;			// 前フレームからの経過時間（スケール適用後）

	static double m_unscaled_time;				// 起動からの累積時間（スケール適用前）
	static double m_scaled_time;				// 起動からの累積時間（スケール適用後）

	static double m_time_scale;					// 時間スケール

	static unsigned long long m_frame_count;	// フレームカウント

public:
	static void Init();
	static void Tick();
	
	static float DeltaTime();				// スケール適用後のデルタタイム（秒）
	static float UnscaledDeltaTime();		// スケール適用前のデルタタイム（秒）
	static float TimeSinceStartup();		// スケール適用後の累積時間（秒）
	static float UnscaledTime();			// スケール適用前の累積時間（秒）

	static void SetTimeScale(double scale);
	static double GetTimeScale();
	static unsigned long long FrameCount();
};