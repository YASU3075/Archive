//-----------------------------------------------
// BoxCollider3D.cpp
// 制作日：2025/07/09
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "scene/Manager.h"
#include "renderer/Renderer.h"
#include "entity/GameObject.h"
#include "physics/AABB3D.h"
#include "math/Matrix3x3.h"
#include "physics/Physics3D.h"
#include "physics/BoxCollider3D.h"
#include "physics/PhysicsDebugDraw3D.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void BOX_COLLIDER_3D::Init()
{

}

//-----------------------------------------------
// ワールドサイズの取得
//-----------------------------------------------
VECTOR3 BOX_COLLIDER_3D::GetWorldSize() const
{
	return m_p_owner->GetTransform()->GetSca();
}

//-----------------------------------------------
// ワールド頂点の取得
//-----------------------------------------------
void BOX_COLLIDER_3D::GetWorldVertices(std::vector<VECTOR3>& out_vertices) const
{
	out_vertices.clear();
	out_vertices.reserve(8);
	VECTOR3 half_size = GetWorldSize() * 0.5f;
	VECTOR3 local_vertices[8] = {
		{-half_size.x, -half_size.y, -half_size.z},
		{ half_size.x, -half_size.y, -half_size.z},
		{ -half_size.x,  half_size.y, -half_size.z},
		{ half_size.x,  half_size.y, -half_size.z},
		{-half_size.x, -half_size.y,  half_size.z},
		{ half_size.x, -half_size.y,  half_size.z},
		{ -half_size.x,  half_size.y,  half_size.z},
		{ half_size.x,  half_size.y,  half_size.z}
	};

	MATRIX4X4 world_matrix = m_p_owner->GetTransform()->GetWorldMatrix();
	
	TRANSFORM* t = m_p_owner->GetTransform();
	MATRIX3x3 rot = t->GetRotationMatrix();
	VECTOR3 center = t->GetPos();

	for (int i = 0; i < 8; ++i)
	{
		//out_vertices.push_back(world_matrix.MultiplyPoint(local_vertices[i]));
		out_vertices.push_back(rot.MultiplyVector(local_vertices[i]) + center);
	}
}

//-----------------------------------------------
// ワールド中心の取得
//-----------------------------------------------
VECTOR3 BOX_COLLIDER_3D::GetWorldCenter() const
{
	return m_p_owner->GetTransform()->GetPos();
}

//-----------------------------------------------
// AABBの取得
//-----------------------------------------------
void BOX_COLLIDER_3D::ComputeAABB(VECTOR3& out_min, VECTOR3& out_max) const
{
	std::vector<VECTOR3> vertices;
	GetWorldVertices(vertices);

	if (vertices.empty())
	{
		return;
	}

	out_min = VECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	out_max = VECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	
	for (const auto& v : vertices)
	{
		out_min.x = std::min(out_min.x, v.x);
		out_max.x = std::max(out_max.x, v.x);

		out_min.y = std::min(out_min.y, v.y);
		out_max.y = std::max(out_max.y, v.y);
		
		out_min.z = std::min(out_min.z, v.z);
		out_max.z = std::max(out_max.z, v.z);
	}
}

//-----------------------------------------------
// NarrowPhase用のサポートポイントの取得
//-----------------------------------------------
VECTOR3 BOX_COLLIDER_3D::Support(const VECTOR3& dir) const
{
	std::vector<VECTOR3> vertices;
	GetWorldVertices(vertices);

	float max_dot = -FLT_MAX;
	VECTOR3 best;

	for (auto&v : vertices)
	{
		float dot = VECTOR3::Dot(v, dir);
		if (dot > max_dot)
		{
			max_dot = dot;
			best = v;
		}
	}

	return best;
}

//-----------------------------------------------
// レイキャスト処理
//-----------------------------------------------
bool BOX_COLLIDER_3D::Raycast(const RAY& ray, RAY_CAST& hit_info, float max_distance) const
{
	TRANSFORM* transform = m_p_owner->GetTransform();
	VECTOR3 center = transform->GetPos();
	MATRIX3x3 rot = transform->GetRotationMatrix();

	// ローカル空間に変換
	VECTOR3 ray_origin_local = rot.Transpose().MultiplyVector(ray.origin - center);
	VECTOR3 ray_dir_local = rot.Transpose().MultiplyVector(ray.direction);

	VECTOR3 half_size = GetWorldSize() * 0.5f;
	VECTOR3 min = -half_size;
	VECTOR3 max = half_size;

	float t_min = 0.0f;
	float t_max = max_distance;
	int hit_axis = -1;
	int sign = 0;

	if (fabs(ray_dir_local.x) < 1e-6f)
	{
		if (ray_origin_local.x < min.x || ray_origin_local.x > max.x)
		{
			return false;
		}
	}
	else
	{
		float inv_d = 1.0f / ray_dir_local.x;
		float t0 = (min.x - ray_origin_local.x) * inv_d;
		float t1 = (max.x - ray_origin_local.x) * inv_d;
		int s = 1;
		if (t0 > t1)
		{
			std::swap(t0, t1);
			s = -1;
		}
		if (t0 > t_min)
		{
			t_min = t0;
			hit_axis = 0;
			sign = s;
		}
		t_max = std::min(t_max, t1);
		if (t_max < t_min)
		{
			return false;
		}
	}

	if (fabs(ray_dir_local.y) < 1e-6f)
	{
		if (ray_origin_local.y < min.y || ray_origin_local.y > max.y)
		{
			return false;
		}
	}
	else
	{
		float inv_d = 1.0f / ray_dir_local.y;
		float t0 = (min.y - ray_origin_local.y) * inv_d;
		float t1 = (max.y - ray_origin_local.y) * inv_d;
		int s = 1;
		if (t0 > t1)
		{
			std::swap(t0, t1);
			s = -1; 
		}
		if (t0 > t_min)
		{
			t_min = t0; hit_axis = 1; sign = s;
		}
		t_max = std::min(t_max, t1);
		if (t_max < t_min)
		{
			return false;
		}
	}

	if (fabs(ray_dir_local.z) < 1e-6f)
	{
		if (ray_origin_local.z < min.z || ray_origin_local.z > max.z)
		{
			return false;
		}
	}
	else
	{
		float inv_d = 1.0f / ray_dir_local.z;
		float t0 = (min.z - ray_origin_local.z) * inv_d;
		float t1 = (max.z - ray_origin_local.z) * inv_d;
		int s = 1;
		if (t0 > t1)
		{
			std::swap(t0, t1); s = -1;
		}
		if (t0 > t_min)
		{
			t_min = t0; hit_axis = 2; sign = s;
		}
		t_max = std::min(t_max, t1);
		if (t_max < t_min)
		{
			return false;
		}
	}

	// 衝突点をワールドに
	hit_info.distance = t_min;
	hit_info.point = ray.origin + ray.direction * t_min;
	hit_info.collider = const_cast<BOX_COLLIDER_3D*>(this);

	// 法線をローカルで決定
	VECTOR3 normal_local(0.0f, 0.0f, 0.0f);
	if (hit_axis == 0)
	{
		normal_local = VECTOR3(static_cast<float>(sign), 0.0f, 0.0f);
	}
	else if (hit_axis == 1)
	{
		normal_local = VECTOR3(0.0f, static_cast<float>(sign), 0.0f);
	}
	else if (hit_axis == 2)
	{
		normal_local = VECTOR3(0.0f, 0.0f, static_cast<float>(sign));
	}

	// ワールドに変換
	hit_info.normal = rot.MultiplyVector(normal_local);

	return true;
}

//-----------------------------------------------
// デバッグ描画
//-----------------------------------------------
void BOX_COLLIDER_3D::DebugDraw() const
{
	const TRANSFORM* t = m_p_owner->GetTransform();
	MATRIX3x3 rot = t->GetRotationMatrix();

	VECTOR3 center = t->GetPos();
	VECTOR3 half_size = GetWorldSize() * 0.5f;

	PHYSICS_DEBUG_DRAW_3D::AddOBB(center, rot.GetColumn(0), rot.GetColumn(1), rot.GetColumn(2), half_size, 0xff00ffff);
}
