//-----------------------------------------------
// MeshRendererData.h
// 制作日：2025/11/30
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <vector>
#include "renderer/Renderer.h"

struct MESH_RENDERER_DATA
{
	std::vector<VERTEX_3D> vertices;
	std::vector<UINT> indices;
};