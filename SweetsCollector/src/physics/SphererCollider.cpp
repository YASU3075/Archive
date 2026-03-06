//-----------------------------------------------
// SphererCollider.cpp
// 制作日：2025/07/09
// 制作者：安田晴人
//-----------------------------------------------
#include "physics/BoxCollider3D.h"
#include "Main.h"
#include "scene/Manager.h"
#include "renderer/Renderer.h"
#include "physics/AABB3D.h"
#include "physics/SphereCollider.h"
#include "physics/Physics3D.h"
#include "physics/PhysicsDebugDraw3D.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void SPHERE_COLLIDER::Init()
{

}

float SPHERE_COLLIDER::GetWorldRadius() const
{
	VECTOR3 scale = m_p_owner->GetTransform()->GetSca();
	float max_scale = std::max(scale.x, std::max(scale.y, scale.z));
	return max_scale;
}

//-----------------------------------------------
// ワールド中心の取得
//-----------------------------------------------
VECTOR3 SPHERE_COLLIDER::GetWorldCenter() const
{
	return m_p_owner->GetTransform()->GetPos();
}

//-----------------------------------------------
// AABBの取得
//-----------------------------------------------
void SPHERE_COLLIDER::ComputeAABB(VECTOR3& out_min, VECTOR3& out_max) const
{
	float world_radius = GetWorldRadius();
	VECTOR3 center = GetWorldCenter();
	out_min = center - VECTOR3(world_radius, world_radius, world_radius);
	out_max = center + VECTOR3(world_radius, world_radius, world_radius);
}

//-----------------------------------------------
// NArrowPhase用のサポートポイントの取得
//-----------------------------------------------
VECTOR3 SPHERE_COLLIDER::Support(const VECTOR3& dir) const
{
	VECTOR3 norm_dir = dir.GetNormalized();
	return GetWorldCenter() + norm_dir * GetWorldRadius();
}

//-----------------------------------------------
// レイキャスト処理
//-----------------------------------------------
bool SPHERE_COLLIDER::Raycast(const RAY& ray, RAY_CAST& hit_info, float max_distance) const
{
	VECTOR3 center = GetWorldCenter();
	float radius = GetWorldRadius();

	VECTOR3 oc = ray.origin - center;

	float a = VECTOR3::Dot(ray.direction, ray.direction);
	float b = 2.0f * VECTOR3::Dot(oc, ray.direction);
	float c = VECTOR3::Dot(oc, oc) - radius * radius;

	float discriment = b * b - 4.0f * a * c;
	if (discriment < 0.0f)
	{
		return false;
	}

	float sqrt_d = std::sqrt(discriment);
	float t1 = (-b - sqrt_d) / (2.0f * a);
	float t2 = (-b + sqrt_d) / (2.0f * a);

	float t = (t1 >= 0.0f) ? t1 : t2;
	if (t < 0.0f || t > max_distance)
	{
		return false;
	}

	hit_info.distance = t;
	hit_info.point = ray.origin + ray.direction * t;
	hit_info.normal = (hit_info.point - center).GetNormalized();
	hit_info.collider = const_cast<SPHERE_COLLIDER*>(this);

	return true;
}

//-----------------------------------------------
// デバッグ描画
//-----------------------------------------------
void SPHERE_COLLIDER::DebugDraw() const
{
	PHYSICS_DEBUG_DRAW_3D::AddSphere(GetWorldCenter(), GetWorldRadius(), 0xff00ffff);
}