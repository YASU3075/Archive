//-----------------------------------------------
// Shore.cpp
// 制作日：2026/01/28
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "title/object/Shore.h"
#include "component/renderer/MeshRendererComponent.h"
#include "renderer/MeshFactory.h"
#include "renderer/MaterialManager.h"
#include "math/PerlinNoise3D.h"
#include "math/Easing.h"

//-----------------------------------------------
// 岸の高さを取得
//-----------------------------------------------
float SHORE::ShoreHeight(float x, float z, const SHORE_PARAMS& params)
{
	PERLIN_NOISE_3D noise(12345);

	float n = noise.Noise(x * params.terrain_freq, 0.0f, z * params.terrain_freq);
	float terrain = n * params.terrain_amp + z * params.z_slope;

	// 2) 川マスク（中心ほど1、外ほど0）
	float d = fabsf(x - params.river_center_x);
	float edgeDist = d - params.river_half_width;
	float t = 1.0f - std::clamp(edgeDist / std::max(params.shore_blend, 1e-4f), 0.0f, 1.0f);
	float riverMask = EASING::Smooth01(t);

	// 3) 川のくぼみ（中心ほど深い）
	float river = riverMask * params.river_depth;

	// 4) 最終高さ
	return terrain - river;
}

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void SHORE::Init()
{
	m_transform.SetPos(VECTOR3(0.0f, 0.0f, 0.0f));
	m_transform.SetSca(VECTOR3(1.0f, 1.0f, 1.0f));

	SHORE_PARAMS shore_params;
	shore_params.river_center_x = 0.0f;
	shore_params.river_half_width = 4.0f;
	shore_params.shore_blend = 3.0f;
	shore_params.river_depth = 2.0f;
	shore_params.terrain_amp = 1.0f;
	shore_params.terrain_freq = 0.1f;
	shore_params.z_slope = 0.05f;


	MESH_RENDERER_COMPONENT* mesh_renderer = AddComponent<MESH_RENDERER_COMPONENT>();
	MESH_RENDERER_DATA shore_mesh = MESH_FACTORY::CreatePlaneMesh(30.0f, 100.0f, 60, 200);
	// 高さマップの適用
	MESH_FACTORY::ApplyHeight(shore_mesh, [&](float x, float z) {
		return ShoreHeight(x, z, shore_params);
		}, true);
	mesh_renderer->SetMeshData(shore_mesh);
	mesh_renderer->SetMaterial(MATERIAL_MANAGER::LoadFromCustomFile("asset/material/ShoreMaterial.mtl"));
}