//-----------------------------------------------
// CapsuleCollider3D.h
// 制作日：2025/09/25
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "math/Vector3.h"
#include "Collider3D.h"

class CAPSULE_COLLIDER_3D : public COLLIDER_3D
{
private:
	float m_radius = 0.5f; // 半径
	float m_height = 2.0f; // 高さ（円柱部分の高さ）
public:
	CAPSULE_COLLIDER_3D()
	{
		m_collider_type = COLLIDER_TYPE_3D::CAPSULE;
	}
	// 初期化
	void Init() override;

	void SetRadius(float radius) { m_radius = radius; }
	float GetRadius() const { return m_radius; }
	
	void SetHeight(float height) { m_height = height; }
	float GetHeight() const { return m_height; }
	
	virtual VECTOR3 GetWorldCenter() const override;
	virtual void ComputeAABB(VECTOR3& out_min, VECTOR3& out_max) const override;
	virtual VECTOR3 Support(const VECTOR3& dir) const override;
	virtual bool Raycast(const RAY& ray, RAY_CAST& hit_info, float max_distance) const override;
	
	virtual void DebugDraw() const override;
};