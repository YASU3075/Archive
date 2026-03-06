//-----------------------------------------------
// PhysicsDebugDraw3D.cpp
// 制作日：2026/01/16
// 制作者：安田晴人
//-----------------------------------------------
#include "physics/PhysicsDebugDraw3D.h"
#include "Main.h"
#include "renderer/Renderer.h"

static std::vector<DEBUG_LINE> g_lines;

XMFLOAT4 ColorU32ToXMFLOAT4(const unsigned int color)
{
	float a = ((color >> 24) & 0xFF) / 255.0f;
	float r = ((color >> 16) & 0xFF) / 255.0f;
	float g = ((color >> 8) & 0xFF) / 255.0f;
	float b = ((color >> 0) & 0xFF) / 255.0f;
	return XMFLOAT4(r, g, b, a);
}

void PHYSICS_DEBUG_DRAW_3D::CLEAR()
{
	g_lines.clear();
}

void PHYSICS_DEBUG_DRAW_3D::AddLine(const VECTOR3& from, const VECTOR3& to, const unsigned int color)
{
	DEBUG_LINE line;
	line.a = from;
	line.b = to;
	line.color = ColorU32ToXMFLOAT4(color);
	g_lines.push_back(line);
}

static void AddEdge(int i, int j, const VECTOR3 v[8], unsigned int color)
{
	PHYSICS_DEBUG_DRAW_3D::AddLine(v[i], v[j], color);
}

void PHYSICS_DEBUG_DRAW_3D::AddAABB(const VECTOR3& min, const VECTOR3& max, const unsigned int color)
{
	VECTOR3 v[8] =
	{
		VECTOR3(min.x, min.y, min.z),
		VECTOR3(max.x, min.y, min.z),
		VECTOR3(max.x, max.y, min.z),
		VECTOR3(min.x, max.y, min.z),
		VECTOR3(min.x, min.y, max.z),
		VECTOR3(max.x, min.y, max.z),
		VECTOR3(max.x, max.y, max.z),
		VECTOR3(min.x, max.y, max.z),
	};
	AddEdge(0, 1, v, color);
	AddEdge(1, 2, v, color);
	AddEdge(2, 3, v, color);
	AddEdge(3, 0, v, color);
	AddEdge(4, 5, v, color);
	AddEdge(5, 6, v, color);
	AddEdge(6, 7, v, color);
	AddEdge(7, 4, v, color);
	AddEdge(0, 4, v, color);
	AddEdge(1, 5, v, color);
	AddEdge(2, 6, v, color);
	AddEdge(3, 7, v, color);
}

void PHYSICS_DEBUG_DRAW_3D::AddOBB(const VECTOR3& center, const VECTOR3& axis_x, const VECTOR3& axis_y, const VECTOR3& axis_z, const VECTOR3& half_size, const unsigned int color)
{
	VECTOR3 v[8] =
	{
		center + (axis_x * half_size.x * -1.0f) + (axis_y * half_size.y * -1.0f) + (axis_z * half_size.z * -1.0f),
		center + ( axis_x * half_size.x) + (axis_y * half_size.y * -1.0f) + (axis_z * half_size.z * -1.0f),
		center + ( axis_x * half_size.x) + ( axis_y * half_size.y) + (axis_z * half_size.z * -1.0f),
		center + (axis_x * half_size.x * -1.0f) + ( axis_y * half_size.y) + (axis_z * half_size.z * -1.0f),
		center + (axis_x * half_size.x * -1.0f) + (axis_y * half_size.y * -1.0f) + ( axis_z * half_size.z),
		center + ( axis_x * half_size.x) + (axis_y * half_size.y * -1.0f) + ( axis_z * half_size.z),
		center + ( axis_x * half_size.x) + ( axis_y * half_size.y) + ( axis_z * half_size.z),
		center + (axis_x * half_size.x * -1.0f) + ( axis_y * half_size.y) + ( axis_z * half_size.z),
	};
	AddEdge(0, 1, v, color);
	AddEdge(1, 2, v, color);
	AddEdge(2, 3, v, color);
	AddEdge(3, 0, v, color);
	AddEdge(4, 5, v, color);
	AddEdge(5, 6, v, color);
	AddEdge(6, 7, v, color);
	AddEdge(7, 4, v, color);
	AddEdge(0, 4, v, color);
	AddEdge(1, 5, v, color);
	AddEdge(2, 6, v, color);
	AddEdge(3, 7, v, color);
}

void PHYSICS_DEBUG_DRAW_3D::AddSphere(const VECTOR3& center, float radius, const unsigned int color, int segments)
{
	auto AddCircle = [&](const VECTOR3& u, const VECTOR3& v)
		{
			VECTOR3 prev_point = center + u * radius;
			for (int i = 1; i <= segments; i++)
			{
				float theta = (float)i / (float)segments * XM_2PI;
				VECTOR3 next_point = center + (u * cosf(theta) + v * sinf(theta)) * radius;
				AddLine(prev_point, next_point, color);
				prev_point = next_point;
			}
		};

	AddCircle(VECTOR3(1.0f, 0.0f, 0.0f), VECTOR3(0.0f, 1.0f, 0.0f));
	AddCircle(VECTOR3(1.0f, 0.0f, 0.0f), VECTOR3(0.0f, 0.0f, 1.0f));
	AddCircle(VECTOR3(0.0f, 1.0f, 0.0f), VECTOR3(0.0f, 0.0f, 1.0f));
}

void PHYSICS_DEBUG_DRAW_3D::Flush()
{
	if (g_lines.size() > 0)
	{
		Renderer::DrawDebugLines(g_lines.data(), g_lines.size(), true);
	}
	g_lines.clear();
}
