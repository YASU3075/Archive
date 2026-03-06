//-----------------------------------------------
// GenerateMapComponent.cpp
// 制作日：2026/01/05
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "game/component/GenerateMapComponent.h"

#include "object/Field.h"
#include "game/object/Wall.h"

//------------------------------------------------
// 初期化処理
//------------------------------------------------
void GENERATE_MAP_COMPONENT::Init()
{
	Generate(m_params.seed);
}

//------------------------------------------------
// マップ生成処理
//------------------------------------------------
void GENERATE_MAP_COMPONENT::Generate(uint32_t seed)
{
	// BSPパラメータのシードを設定
	m_params.seed = seed;

	// 既存の床をクリア
	ClearSpawnedFields();

	// 既存の壁をクリア
	ClearSpawnedWalls();

	// BSPで壁レイアウトを生成
	m_layout = std::make_unique<GENERATED_LAYOUT>(GenerateBSPWalls(m_params));

	// 床オブジェクトを生成
	SpawnFloorsFromLayout(*m_layout);

	// 壁セグメントをマージして取得
	std::vector<WALL_SEGMENT> segments = BuildMergedWallSegments(m_layout->wall_grid);

	// 壁オブジェクトを生成
	SpawnWallsFromSegments(segments);
}

//------------------------------------------------
// 生成した壁オブジェクトを全て破棄
//------------------------------------------------
void GENERATE_MAP_COMPONENT::ClearSpawnedWalls()
{
	for (WALL* p_wall : m_spawned_walls)
	{
		if (p_wall)
		{
			p_wall->Destroy();
		}
	}
	m_spawned_walls.clear();
}

void GENERATE_MAP_COMPONENT::ClearSpawnedFields()
{
	for (FIELD* p_field : m_spawned_floors)
	{
		if (p_field)
		{
			p_field->Destroy();
		}
	}
	m_spawned_floors.clear();
}

//------------------------------------------------
// マップのワールドサイズを取得
//------------------------------------------------
VECTOR3 GENERATE_MAP_COMPONENT::GetMapWorldSize() const
{
	return VECTOR3(
		m_params.map_width * m_cell_size,
		0.0f,
		m_params.map_height * m_cell_size
	);
}

//------------------------------------------------
// マップの原点オフセットを取得
//------------------------------------------------
VECTOR3 GENERATE_MAP_COMPONENT::GetMapOriginOffset() const
{
	return VECTOR3(
		-m_params.map_width * m_cell_size * 0.5f,
		0.0f,
		-m_params.map_height * m_cell_size * 0.5f
	);
}

//------------------------------------------------
// 壁セグメントから壁オブジェクトを生成
//------------------------------------------------
void GENERATE_MAP_COMPONENT::SpawnWallsFromSegments(const std::vector<WALL_SEGMENT>& segments)
{
	VECTOR3 owner_pos = m_p_owner->GetTransform()->GetPos();

	VECTOR3 mapOriginOffset(
		-m_params.map_width * m_cell_size * 0.5f,
		0.0f,
		-m_params.map_height * m_cell_size * 0.5f
	);

	for (const auto& s : segments)
	{
		WALL_INSTANCE instance = ToInstance(s, m_cell_size, m_wall_height, m_wall_thickness);

		WALL* p_wall = Manager::GetScene()->AddGameObject<WALL>(LAYER::LAYER_GAME_OBJECT);

		VECTOR3 pos = owner_pos + mapOriginOffset + VECTOR3(instance.px, instance.py, instance.pz);
		p_wall->GetTransform()->SetPos(pos);

		p_wall->GetTransform()->SetSca(VECTOR3(instance.sx, instance.sy, instance.sz));
		p_wall->GetTransform()->SetRot(QUATERNION::FromEuler(VECTOR3(0.0f, instance.yaw * XM_PI / 180.0f, 0.0f)));
	}
}

//------------------------------------------------
// レイアウト情報から床オブジェクトを生成
//------------------------------------------------
void GENERATE_MAP_COMPONENT::SpawnFloorsFromLayout(const GENERATED_LAYOUT& layout)
{
	VECTOR3 owner_pos = m_p_owner->GetTransform()->GetPos();

	// マップを owner 中心に置くためのオフセット
	VECTOR3 mapOriginOffset(
		-m_params.map_width * m_cell_size * 0.5f,
		0.0f,
		-m_params.map_height * m_cell_size * 0.5f
	);

	auto SpawnOneFloor = [&](const WALL_RECT& r)
		{
			// サイズが小さすぎる領域はスキップ
			if (r.w <= 0 || r.h <= 0) return;

			VECTOR3 center = CalcRectCenterWorld(r, m_cell_size);
			VECTOR3 size = CalcRectSizeWorld(r, m_cell_size);

			FIELD* p_floor = Manager::GetScene()->AddGameObject<FIELD>(LAYER::LAYER_GAME_OBJECT);

			// Fieldの中心が原点（中心ピボット）前提。もし違えば後で補正。
			// 床のYは必要に応じて調整して（壁と干渉しない高さ）
			float floorY = owner_pos.y; // 例：0.0f固定でもOK
			p_floor->GetTransform()->SetPos(owner_pos + mapOriginOffset + VECTOR3(center.x, floorY, center.z));

			// 厚みは Field 側が持ってるなら触らない
			// unit cube 的にスケールで床を作るなら、Yは 1 のままでも薄くしてもOK
			p_floor->GetTransform()->SetSca(VECTOR3(size.x, 1.0f, size.z));

			m_spawned_floors.push_back(p_floor);
		};

	// 1) 中央部屋
	SpawnOneFloor(layout.center_area);

	// 2) 外側8領域
	for (const auto& r : layout.macro_regions)
	{
		SpawnOneFloor(r);
	}
}

