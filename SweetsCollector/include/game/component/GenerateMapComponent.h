//-----------------------------------------------
// GenerateMapComponent.h
// 制作日：2026/01/05
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <memory>
#include "game/system/BSPSystem.h"
#include "component/Component.h"

class GENERATE_MAP_COMPONENT : public COMPONENT
{
private:
	BSP_PARAMS m_params;

	float m_cell_size = 2.0f;
	float m_wall_height = 3.0f;
	float m_wall_thickness = 1.0f;

	int m_max_merged_wall_length = 0;
	bool m_unit_cube_long_axis_x = true;

	std::vector<class WALL*> m_spawned_walls;
	std::vector<class FIELD*> m_spawned_floors;

	std::unique_ptr<GENERATED_LAYOUT> m_layout;
	std::vector<WALL_SEGMENT> m_wall_segments;

	void SpawnWallsFromSegments(const std::vector<WALL_SEGMENT>& segments);
	void SpawnFloorsFromLayout(const GENERATED_LAYOUT& layout);

public:
	void Init() override;

	void Generate(uint32_t seed);
	void ClearSpawnedWalls();
	void ClearSpawnedFields();

	VECTOR3 GetMapWorldSize() const;

	const GENERATED_LAYOUT& GetLayout() const { return *m_layout; }
	const std::vector<WALL_SEGMENT>& GetWallSegments() const { return m_wall_segments; }

	float GetCellSize() const { return m_cell_size; }
	float GetWallThickness() const { return m_wall_thickness; }
	VECTOR3 GetMapOriginOffset() const;
};
