//-----------------------------------------------
// SplineComponent.h
// 制作日：2026/01/29
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <vector>
#include <utility>
#include "math/Vector3.h"
#include "component/Component.h"

class SPLINE_COMPONENT : public COMPONENT
{
private:
	std::vector<std::pair<int, VECTOR3>> m_control_points; // 制御点のリスト
public:
	void Init() override;

	// 制御点の追加
	void AddControlPoint(int index, const VECTOR3& point);

	// スプライン曲線上の点を取得
	VECTOR3 GetPoint(int current_index, float t) const;

	// 制御点の数を取得
	size_t GetControlPointCount() const;

	// 制御点のクリア
	void ClearControlPoints();
};