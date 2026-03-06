//-----------------------------------------------
// BoxCollider3D.h
// 制作日：2025/07/09
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <vector>
#include "math/Vector3.h"
#include "Collider3D.h"

class BOX_COLLIDER_3D : public COLLIDER_3D
{
private:
public:
	BOX_COLLIDER_3D()
	{
		m_collider_type = COLLIDER_TYPE_3D::BOX;
	}

	// 初期化
	void Init() override;

	VECTOR3 GetWorldSize() const;

	void GetWorldVertices(std::vector<VECTOR3>& out_vertices) const;

	virtual VECTOR3 GetWorldCenter() const override;
	virtual void ComputeAABB(VECTOR3& out_min, VECTOR3& out_max) const override;
	virtual VECTOR3 Support(const VECTOR3& dir) const override;
	virtual bool Raycast(const RAY& ray, RAY_CAST& hit_info, float max_distance) const override;

	virtual void DebugDraw() const override;
};