//-----------------------------------------------
// SweetsSpawnComponent.cpp
// 制作日：2026/01/11
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "EngineTime.h"
#include "game/component/SweetsSpawnComponent.h"
#include "game/component/GenerateMapComponent.h"

#include "game/component/SweetsFloatRotateComponent.h"
#include "component/renderer/ModelRendererComponent.h"
#include "game/component/SweetsComponent.h"
#include "game/SweetsData.h"
#include "game/object/Sweets.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void SWEETS_SPAWN_COMPONENT::Init()
{
	m_p_generate_mamodel_renderer = GetOwner()->GetComponent<GENERATE_MAP_COMPONENT>();
	if (!m_p_generate_mamodel_renderer)
	{
		assert(false && "SweetsSpawnComponent::Init() - GenerateMapComponent not found on owner.");
		return;
	}
	m_timer = 0.0;
	m_rng.seed(static_cast<uint32_t>(m_seed_salt) + static_cast<uint32_t>(TIME::FrameCount()));
	CacheMapRefs();
	BuildFobiddenAABBs();
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void SWEETS_SPAWN_COMPONENT::Update()
{
	if (!m_p_generate_mamodel_renderer)
	{
		CacheMapRefs();
		return;
	}
	
	if (!m_has_forbidden)
	{
		BuildFobiddenAABBs();
	}

	m_timer += TIME::DeltaTime();

	if (m_timer < m_spawn_interval)
	{
		return;
	}
	m_timer = 0.0;

	// 領域ごとに不足分を見て補充
	TickSpawn();
}

//-----------------------------------------------
// マップの参照をキャッシュ
//-----------------------------------------------
void SWEETS_SPAWN_COMPONENT::CacheMapRefs()
{
	m_p_generate_mamodel_renderer = m_p_owner->GetComponent<GENERATE_MAP_COMPONENT>();
	if (m_p_generate_mamodel_renderer)
	{
		m_rng.seed(m_seed_salt);
	}
}

//-----------------------------------------------
// 禁止エリアAABBの構築
//-----------------------------------------------
void SWEETS_SPAWN_COMPONENT::BuildFobiddenAABBs()
{
	if (!m_p_generate_mamodel_renderer)
	{// マップ生成コンポーネントが無い
		return;
	}
	
	// 壁セグメントからAABBを構築
	const GENERATED_LAYOUT& layout = m_p_generate_mamodel_renderer->GetLayout();
	const std::vector<WALL_SEGMENT>& segments = m_p_generate_mamodel_renderer->GetWallSegments();

	m_wall_aabbs.clear();
	m_wall_aabbs.reserve(segments.size());

	// 壁セグメントをAABBに変換して格納
	for (const WALL_SEGMENT& s : segments)
	{
		WALL_INSTANCE w = ToInstance(s, m_p_generate_mamodel_renderer->GetCellSize(), 1.0f, m_p_generate_mamodel_renderer->GetWallThickness());
		m_wall_aabbs.push_back(WallInstanceToAABB2(w));
	}

	// ドアセグメントを抽出してAABBに変換
	auto doors = ExtractDoorsFromWallGrid(layout.wall_grid);
	m_door_aabbs.clear();
	m_door_aabbs.reserve(doors.size());
	for (const DOOR_SEGMENT& d : doors)
	{
		m_door_aabbs.push_back(DoorToAABB2(d, m_p_generate_mamodel_renderer->GetCellSize(), m_p_generate_mamodel_renderer->GetWallThickness()));
	}

	// フラグを立てる
	m_has_forbidden = true;
}

//-----------------------------------------------
// スポーン処理
//-----------------------------------------------
void SWEETS_SPAWN_COMPONENT::TickSpawn()
{
	// 各regionごとのスポーン数カウント
	const auto& regions = m_p_generate_mamodel_renderer->GetLayout().macro_regions;

	std::vector<int> count(regions.size(), 0);

	CleanupSweets();

	// 今いるSWEETSを各regionにカウント（位置から判定）
	for (auto* p : m_spawned_sweets)
	{
		// ワールド座標をローカル座標に変換
		VECTOR3 pos = p->GetTransform()->GetPos();
		VECTOR3 local = WorldToLocal(pos);

		// 位置から領域インデックスを取得
		int idx = FindRegionIndex(regions, local.x, local.z);
		if (idx >= 0)
		{
			count[idx]++;
		}
	}

	// 各領域ごとに不足分をスポーン
	for (int i = 0; i < (int)regions.size(); ++i)
	{
		int need = m_max_per_region - count[i];
		if (need <= 0) continue;

		int spawnN = std::min(need, m_spawn_per_tick);
		for (int k = 0; k < spawnN; ++k)
		{
			TrySpawnInRegion(regions[i], i);
		}
	}
}

//-----------------------------------------------
// ワールド座標をローカル座標に変換
//-----------------------------------------------
VECTOR3 SWEETS_SPAWN_COMPONENT::WorldToLocal(const VECTOR3& world_pos)
{
	VECTOR3 owner_pos = m_p_owner->GetTransform()->GetPos();
	VECTOR3 off = m_p_generate_mamodel_renderer->GetMapOriginOffset();
	return VECTOR3(world_pos.x - owner_pos.x - off.x, 0.0f, world_pos.z - owner_pos.z - off.z);
}

//-----------------------------------------------
// 位置から領域インデックスを取得
//-----------------------------------------------
int SWEETS_SPAWN_COMPONENT::FindRegionIndex(const std::vector<WALL_RECT>& regions, const float x, float z)
{
	for (int i = 0; i < (int)regions.size(); ++i)
	{
		const auto& r = regions[i];

		float min_x = r.x * m_p_generate_mamodel_renderer->GetCellSize();
		float max_x = (r.x + r.w) * m_p_generate_mamodel_renderer->GetCellSize();
		float min_z = r.z * m_p_generate_mamodel_renderer->GetCellSize();
		float max_z = (r.z + r.h) * m_p_generate_mamodel_renderer->GetCellSize();

		if (x >= min_x && x < max_x && z >= min_z && z < max_z)
		{
			return i;
		}
	}
	return -1;
}

//-----------------------------------------------
// 指定位置がスポーン可能か
//-----------------------------------------------
bool SWEETS_SPAWN_COMPONENT::CanSpawn(float x, float z) const
{
	// 壁AABBと接触していないか
	for (const auto& aabb : m_wall_aabbs)
	{
		if (HitAABB2_Expanded(x, z, m_sweets_radius + m_sweets_spacing, aabb))
		{
			return false;
		}
	}
	// ドアAABBと接触していないか
	for (const auto& aabb : m_door_aabbs)
	{
		if (HitAABB2_Expanded(x, z, m_sweets_radius + m_sweets_spacing, aabb))
		{
			return false;
		}
	}
	return true;
}

//-----------------------------------------------
// 領域内でスポーンを試みる
//-----------------------------------------------
void SWEETS_SPAWN_COMPONENT::TrySpawnInRegion(const WALL_RECT& region, int region_index)
{
	// 領域のワールド座標範囲を取得
	float cell = m_p_generate_mamodel_renderer->GetCellSize();
	float margin = m_sweets_radius + m_p_generate_mamodel_renderer->GetWallThickness() * 0.5f + 0.1f;

	float min_x = (region.x * cell) + margin;
	float max_x = ((region.x + region.w) * cell) - margin;
	float min_z = (region.z * cell) + margin;
	float max_z = ((region.z + region.h) * cell) - margin;
	if (max_x <= min_x || max_z <= min_z) return;

	// ランダム関数
	auto RandFloat = [&](float lo, float hi) {
		return std::uniform_real_distribution<float>(lo, hi)(m_rng);
		};

	// 試行
	for (int t = 0; t < m_tries_per_sweets; ++t)
	{
		float x = RandFloat(min_x, max_x);
		float z = RandFloat(min_z, max_z);

		if (!CanSpawn(x, z))
		{// スポーン不可
			continue;
		}

		// 生成
		SWEETS* p = Manager::GetScene()->AddGameObject<SWEETS>(LAYER::LAYER_GAME_OBJECT);
		SWEETS_FLOAT_ROTATE_COMPONENT* component = p->AddComponent<SWEETS_FLOAT_ROTATE_COMPONENT>();
		component->SetFloatingSpeed(0.2f);
		component->SetRotateSpeed(30.0f);
		component->SetFloatingRange(0.1f);

		MODEL_RENDERER_COMPONENT* p_model = p->GetComponent<MODEL_RENDERER_COMPONENT>();
		
		int type = std::uniform_int_distribution<int>(1, (int)SWEETS_TYPE::SWEETS_TYPE_MAX - 1)(m_rng);
		SWEETS_TYPE sweets_type = static_cast<SWEETS_TYPE>(type);
		p->SetDataByType(sweets_type);

		component->SetOriginalY(p_model->GetLocalPos().y);

		VECTOR3 owner_pos = m_p_owner->GetTransform()->GetPos();
		VECTOR3 off = m_p_generate_mamodel_renderer->GetMapOriginOffset();

		const float offset_y = 3.0f;
		float y = owner_pos.y;
		p->GetTransform()->SetPos(VECTOR3(owner_pos.x + off.x + x, y + offset_y, owner_pos.z + off.z + z));

		m_spawned_sweets.push_back(p);
		return;
	}
}

//-----------------------------------------------
// スポーン済みお菓子のクリーンアップ
//-----------------------------------------------
void SWEETS_SPAWN_COMPONENT::CleanupSweets()
{
	auto it = m_spawned_sweets.begin();
	while (it != m_spawned_sweets.end())
	{
		SWEETS* p = *it;
		if (p->GetDestroy())
		{
			it = m_spawned_sweets.erase(it);
		}
		else
		{
			++it;
		}
	}
}