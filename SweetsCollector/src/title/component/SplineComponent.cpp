//-----------------------------------------------
// SplineComponent.cpp
// 制作日：2026/01/29
// 制作者：安田晴人
//-----------------------------------------------
#include "title/component/SplineComponent.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void SPLINE_COMPONENT::Init()
{
	ClearControlPoints();
}

//-----------------------------------------------
// 制御点の追加
//-----------------------------------------------
void SPLINE_COMPONENT::AddControlPoint(int index, const VECTOR3& point)
{
	m_control_points.emplace_back(index, point);
}

//-----------------------------------------------
// スプライン曲線上の点を取得
//-----------------------------------------------
VECTOR3 SPLINE_COMPONENT::GetPoint(int current_index, float t) const
{
	if (m_control_points.size() < 2)
	{
		return VECTOR3();
	}

	// 現在の制御点と次の制御点を取得
	const VECTOR3& p0 = m_control_points[current_index % m_control_points.size()].second;
	const VECTOR3& p1 = m_control_points[(current_index + 1) % m_control_points.size()].second;

	// 線形補間
	return p0 * (1.0f - t) + p1 * t;
}

//-----------------------------------------------
// 制御点の数を取得
//-----------------------------------------------
size_t SPLINE_COMPONENT::GetControlPointCount() const
{
	return m_control_points.size();
}

//-----------------------------------------------
// 制御点のクリア
//-----------------------------------------------
void SPLINE_COMPONENT::ClearControlPoints()
{
	m_control_points.clear();
}