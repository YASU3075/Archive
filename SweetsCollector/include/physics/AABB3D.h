//-----------------------------------------------
// AABB3D.h
// 制作日：2025/07/09
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "math/Vector3.h"

class AABB_3D
{
public:
	VECTOR3 m_min = { 0.0f, 0.0f, 0.0f };
	VECTOR3 m_max = { 0.0f, 0.0f, 0.0f };

	AABB_3D() = default;
	AABB_3D(const VECTOR3& min, const VECTOR3& max) : m_min(min), m_max(max) {}

	bool Overlaps(const AABB_3D& other) const
	{//重なってるか判
		return !(m_max.x < other.m_min.x || m_min.x > other.m_max.x ||
			m_max.y < other.m_min.y || m_min.y > other.m_max.y ||
			m_max.z < other.m_min.z || m_min.z > other.m_max.z);
	}

	VECTOR3 GetCenter()
	{//中心
		return (m_max + m_min) * 0.5f;
	}

	VECTOR3 GetScale()
	{//サイズ
		return m_max - m_min;
	}

	void Expand(float margin)
	{//拡大・縮小
		m_min -= VECTOR3(margin, margin, margin);
		m_max += VECTOR3(margin, margin, margin);
	}

	bool ContainPoint(const VECTOR3& point) const
	{// 点がAABB内に含まれているか判定
		return (point.x >= m_min.x && point.x <= m_max.x &&
			point.y >= m_min.y && point.y <= m_max.y &&
			point.z >= m_min.z && point.z <= m_max.z);
	}

};