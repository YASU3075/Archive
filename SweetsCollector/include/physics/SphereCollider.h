//-----------------------------------------------
// SphereCollider.h
// 制作日：2025/07/07
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "Collider3D.h"
#include "math/Vector3.h"

class SPHERE_COLLIDER : public COLLIDER_3D
{
private:
	float m_radius = 1.0f;	// 半径
public:
	SPHERE_COLLIDER() 
	{
		m_collider_type = COLLIDER_TYPE_3D::SPHERE;
	}

	// 初期化
	void Init() override;

	void SetRadius(float radius) { m_radius = radius; }
	float GetRadius() const { return m_radius; }

	float GetWorldRadius() const;

	virtual VECTOR3 GetWorldCenter() const override;
	virtual void ComputeAABB(VECTOR3& out_min, VECTOR3& out_max) const override;
	virtual VECTOR3 Support(const VECTOR3& dir) const override;
	virtual bool Raycast(const RAY& ray, RAY_CAST& hit_info, float max_distance) const override;

	virtual void DebugDraw() const override;
};