//-----------------------------------------------
// TreeGenerator.h
// 制作日：2025/12/17
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "LSystem.h"
#include "renderer/MeshRendererData.h"
#include <vector>

struct TURTLE
{
	XMMATRIX transform;
	XMVECTOR position;
	float branch_length;
	float branch_radius;
};

class TREE_GENERATOR
{
private:
	LSYSTEM& m_lsystem;
	std::vector<MESH_RENDERER_DATA> m_branches;
	std::vector<MESH_RENDERER_DATA> m_leaves;

	void ApplyTransform(MESH_RENDERER_DATA& mesh, const XMMATRIX& transform);

public:
	TREE_GENERATOR(LSYSTEM& lsystem) : m_lsystem(lsystem) {}
	MESH_RENDERER_DATA GenerateTree(int n);
};