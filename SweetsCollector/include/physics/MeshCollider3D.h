//-----------------------------------------------
// MeshCollider3D.h
// 制作日：2025/09/25
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <vector>
#include "math/Vector3.h"
#include "Collider3D.h"

class MESH_COLLIDER_3D : public COLLIDER_3D
{
private:
	std::vector<VECTOR3> m_vertices;	// 頂点リスト
	std::vector<unsigned int> m_indices;			// インデックスリスト

public:
	MESH_COLLIDER_3D()
	{
		m_collider_type = COLLIDER_TYPE_3D::MESH;
	}

	void Init() override;

	void SetMesh(const std::vector<VECTOR3>& vertices, const std::vector<unsigned int>& indices)
	{
		m_vertices = vertices;
		m_indices = indices;
	}

	virtual VECTOR3 GetWorldCenter() const override;
	virtual void ComputeAABB(VECTOR3& out_min, VECTOR3& out_max) const override;
	virtual VECTOR3 Support(const VECTOR3& dir) const override;
	virtual bool Raycast(const RAY& ray, RAY_CAST& hit_info, float max_distance) const override;

	virtual void DebugDraw() const override;
};