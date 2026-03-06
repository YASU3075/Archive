//-----------------------------------------------
// SplineMoveComponent.h
// 制作日：2026/01/29
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "component/Component.h"

class SPLINE_MOVE_COMPONENT : public COMPONENT
{
private:
	class SPLINE_COMPONENT* m_p_spline = nullptr;	// スプラインコンポーネント
	float m_progress = 0.0f;						// スプライン上の進行度
	float m_speed = 0.1f;							// 移動速度
	int m_current_index = 0;						// 現在の制御点インデックス

public:
	void Init() override;
	void Update() override;

	void SetSplineComponent(class SPLINE_COMPONENT* p_spline) { m_p_spline = p_spline; }
	void SetSpeed(float speed) { m_speed = speed; }
};