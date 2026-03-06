//-----------------------------------------------
// SweetsSpawnComponent.h
// 制作日：2026/01/11
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <cstdint>
#include <random>
#include "game/system/BSPSystem.h"
#include "component/Component.h"

struct SWEETS_DATA;

class SWEETS_SPAWN_COMPONENT : public COMPONENT
{
private:
	int m_max_per_region = 5;		//1領域あたりの最大スポーン数
	float m_spawn_interval = 3.0f;	//スポーン間隔
	int m_spawn_per_tick = 1;		//1ティックあたりのスポーン数

	float m_sweets_radius = 1.0f;		//スポーンするお菓子の半径
	float m_sweets_spacing = 1.0f;		//スポーンするお菓子の間隔
	int m_tries_per_sweets = 8;			//お菓子1つあたりの三角形数

	uint32_t m_seed_salt = 0xA53C9E1u;

	class GENERATE_MAP_COMPONENT* m_p_generate_mamodel_renderer = nullptr;
	std::mt19937 m_rng;
	double m_timer = 0.0;

	std::vector<AABB2> m_wall_aabbs;
	std::vector<AABB2> m_door_aabbs;
	bool m_has_forbidden = false;

	std::vector<class SWEETS*> m_spawned_sweets;

	void CacheMapRefs();
	void BuildFobiddenAABBs();
	void TickSpawn();
	void CleanupSweets();
	VECTOR3 WorldToLocal(const VECTOR3& world_pos);
	int FindRegionIndex(const std::vector<WALL_RECT>& regions, const float x, float z);
	bool CanSpawn(float x, float z) const;
	void TrySpawnInRegion(const WALL_RECT& region, int region_index);

public:
	void Init() override;
	void Update() override;
};