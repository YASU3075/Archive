//-----------------------------------------------
// MeshFactory.h
// 制作日：2025/11/30
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <functional>
#include "renderer/MeshRendererData.h"

class MESH_FACTORY
{
public:
	static MESH_RENDERER_DATA CreateCubeMesh(float width, float height, float depth);
	static MESH_RENDERER_DATA CreateSphereMesh(float radius, int slices, int stacks);
	static MESH_RENDERER_DATA CreateCylinderMesh(float radius, float height, int segments);
	static MESH_RENDERER_DATA CreatePlaneMesh(float width, float depth, int x_segments, int z_segments);

	static void RecalculateNormals(MESH_RENDERER_DATA& mesh);
	static void ApplyHeight(MESH_RENDERER_DATA& mesh, const std::function<float(float x, float z)>& height_func, bool recalc_normals = true);
};