//--
// ---------------------------------------------
// Collider3D.h
// 制作日：2025/07/07
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "component/Component.h"
#include "math/Vector3.h"
#include "Ray.h"

enum class COLLIDER_TYPE_3D
{
    NONE = 0,
    SPHERE,
    BOX,
    CAPSULE,
    MESH,
};

class COLLIDER_3D : public COMPONENT
{
protected:
	bool m_is_trigger = false;                                  // トリガーフラグ
	COLLIDER_TYPE_3D m_collider_type = COLLIDER_TYPE_3D::NONE;  // コライダーのタイプ

public:
    COLLIDER_3D() = default;
    virtual ~COLLIDER_3D() = default;

	COLLIDER_TYPE_3D GetColliderType() const { return m_collider_type; }
	
	void SetIsTrigger(bool is_trigger) { m_is_trigger = is_trigger; }
	bool IsTrigger() const { return m_is_trigger; }

	virtual VECTOR3 GetWorldCenter() const = 0;
	virtual void ComputeAABB(VECTOR3& out_min, VECTOR3& out_max) const = 0;
	virtual VECTOR3 Support(const VECTOR3& dir) const = 0;
	virtual bool Raycast(const RAY& ray, RAY_CAST& hit_info, float max_distance) const = 0;

	virtual void DebugDraw() const = 0;
};