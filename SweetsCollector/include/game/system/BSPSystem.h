//-----------------------------------------------
// BSPSystem.h
// 制作日：2026/01/04
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <vector>
#include <memory>
#include <random>
#include "math/Vector3.h"

struct WALL_RECT
{
	int x = 0;
	int z = 0;
	int w = 0;
	int h = 0;

	int x1() const { return x; }
	int z1() const { return z; }
	int x2() const { return x + w; }
	int z2() const { return z + h; }
	int cx() const { return x + w / 2; }
	int cz() const { return z + h / 2; }
};

struct WALL_GRID
{
	int w = 0;
	int h = 0;
	std::vector<uint8_t> V;
	std::vector<uint8_t> H;

	explicit WALL_GRID(int grid_w, int grid_h) : w(grid_w), h(grid_h), V((grid_w + 1) * grid_h, 0), H(grid_w * (grid_h + 1), 0){}

	uint8_t& GetV(int x, int z) { return V[z * (w + 1) + x]; }
	uint8_t& GetH(int x, int z) { return H[z * w + x]; }

	uint8_t GetV(int x, int z) const { return V[z * (w + 1) + x]; }
	uint8_t GetH(int x, int z) const { return H[z * w + x]; }

	void ClearAll()
	{
		std::fill(V.begin(), V.end(), 0);
		std::fill(H.begin(), H.end(), 0);
	}

	void AddBoundaryWalls()
	{
		for (int z = 0; z < h; z++)
		{
			GetV(0, z) = 1;
			GetV(w, z) = 1;
		}

		for (int x = 0; x < w; x++)
		{
			GetH(x, 0) = 1;
			GetH(x, h) = 1;
		}
	}
};

struct BSP_NODE
{
	WALL_RECT area;
	std::unique_ptr<BSP_NODE> p_left = nullptr;
	std::unique_ptr<BSP_NODE> p_right = nullptr;

	int rep_x = 0;
	int rep_z = 0;

	bool IsLeaf() const { return p_left == nullptr && p_right == nullptr; }
};

struct BSP_PARAMS
{
	int map_width = 100;
	int map_height = 100;
	int center_size = 24;
	int min_leaf_size = 14;
	int max_depth = 5;

	int door_width = 3;
	int doors_per_split_min = 1;
	int doors_per_split_max = 2;

	int macro_door_per_segment_min = 1;
	int macro_door_per_segment_max = 2;

	uint32_t seed = 69338;
};

struct GENERATED_LAYOUT
{
	WALL_GRID wall_grid;
	WALL_RECT center_area;
	std::vector<WALL_RECT> macro_regions;

	explicit GENERATED_LAYOUT(int map_w, int map_h) : wall_grid(map_w, map_h) {}
};

struct WALL_SEGMENT
{
	bool vertical = false;

	int x0 = 0;
	int z0 = 0;
	int x1 = 0;
	int z1 = 0;

	int length = 0;
};

struct WALL_INSTANCE
{
	float px, py, pz;
	float sx, sy, sz;
	float yaw; // Y回転
};

struct AABB2
{
	float min_x, min_z;
	float max_x, max_z;
};

struct DOOR_SEGMENT
{
	bool vertical = false;
	int line = 0;
	int a0 = 0;
	int a1 = 0;
};

//-----------------------------------------------
// ランダムな整数を生成する
//-----------------------------------------------
static inline int RandomInt(int lo, int hi, std::mt19937& rng)
{
	if (lo > hi)
	{
		std::swap(lo, hi);
	}
	return std::uniform_int_distribution<int>(lo, hi)(rng);
}

//-----------------------------------------------
// ランダムな真偽値を生成する
//-----------------------------------------------
static inline bool RandomBool(std::mt19937& rng)
{
	return (RandomInt(0, 1, rng) == 0);
}

static inline std::vector<int> PickDoorStarts1D(std::mt19937& rng, int seg_len, int door_width, int door_count, int margin)
{
	std::vector<int> starts;

	if (seg_len <= 0)
	{
		return starts;
	}
	if (door_width <= 0)
	{
		return starts;
	}

	int min_start = margin;
	int max_start = seg_len - door_width - margin;

	if (max_start < min_start)
	{
		return starts;
	}

	const int min_gap = door_width + 3;

	for (int i = 0; i < 200 && static_cast<int>(starts.size()) < door_count; i++)
	{
		int s = RandomInt(min_start, max_start, rng);
		bool ok = true;
		for (int prev : starts)
		{
			if (std::abs(prev - s) < min_gap)
			{
				ok = false;
				break;
			}
		}

		if (ok)
		{
			starts.push_back(s);
		}
	}

	std::sort(starts.begin(), starts.end());
	return starts;
}

static inline void AddVerticalWallWithDoors(WALL_GRID& g, int x, int z_start, int z_end, int door_width, int door_count, std::mt19937& rng)
{
	if (x < 0 || x > g.w)
	{
		return;
	}
	z_start = std::max(0, z_start);
	z_end = std::min(g.h, z_end);
	int len = z_end - z_start;

	if (len <= 0)
	{
		return;
	}

	for (int i = z_start; i < z_end; i++)
	{
		g.GetV(x, i) = 1;
	}

	int margin = 2;
	auto starts = PickDoorStarts1D(rng, len, door_width, door_count, margin);
	for (int s : starts)
	{
		for (int dz = 0; dz < door_width; dz++)
		{
			int z = z_start + s + dz;
			if (z >= z_start && z < z_end)
			{
				g.GetV(x, z) = 0;
			}
		}
	}
}

//-----------------------------------------------
// 水平な壁を追加し、ドアを開ける
//-----------------------------------------------
static inline void AddHorizontalWallWithDoors(WALL_GRID& g, int z, int x_start, int x_end, int door_width, int door_count, std::mt19937& rng)
{
	if (z < 0 || z > g.h)
	{
		return;
	}
	x_start = std::max(0, x_start);
	x_end = std::min(g.w, x_end);
	int len = x_end - x_start;
	if (len <= 0)
	{
		return;
	}
	for (int i = x_start; i < x_end; i++)
	{
		g.GetH(i, z) = 1;
	}
	int margin = 2;
	auto starts = PickDoorStarts1D(rng, len, door_width, door_count, margin);
	for (int s : starts)
	{
		for (int dx = 0; dx < door_width; dx++)
		{
			int x = x_start + s + dx;
			if (x >= x_start && x < x_end)
			{
				g.GetH(x, z) = 0;
			}
		}
	}
}

//-----------------------------------------------
// BSPノードを分割する
//-----------------------------------------------
static inline bool SplitNode(BSP_NODE& n, const BSP_PARAMS& params, bool& out_vertical, int& out_cut, std::mt19937& rng)
{
	if (n.area.w < params.min_leaf_size * 2 && n.area.h < params.min_leaf_size * 2)
	{
		return false;
	}

	float ratio = static_cast<float>(n.area.w) / static_cast<float>(n.area.h);
	bool vertical;
	if (ratio > 1.25f)
	{
		vertical = true;
	}
	else if (ratio < 0.8f)
	{
		vertical = false;
	}
	else
	{
		vertical = RandomBool(rng);
	}

	if (vertical)
	{
		if (n.area.w < params.min_leaf_size * 2)
		{
			return false;
		}

		int min_cut = n.area.x + params.min_leaf_size;
		int max_cut = n.area.x + n.area.w - params.min_leaf_size;

		if (max_cut <= min_cut)
		{
			return false;
		}

		int cut = RandomInt(min_cut, max_cut, rng);

		WALL_RECT left{ n.area.x, n.area.z, cut - n.area.x, n.area.h };
		WALL_RECT right{ cut, n.area.z, n.area.x + n.area.w - cut, n.area.h };

		n.p_left = std::make_unique<BSP_NODE>();
		n.p_left->area = left;
		n.p_right = std::make_unique<BSP_NODE>();
		n.p_right->area = right;

		out_vertical = true;
		out_cut = cut;
		return true;
	}
	else
	{
		if (n.area.h < params.min_leaf_size * 2)
		{
			return false;
		}
		int min_cut = n.area.z + params.min_leaf_size;
		int max_cut = n.area.z + n.area.h - params.min_leaf_size;
		if (max_cut <= min_cut)
		{
			return false;
		}
		int cut = RandomInt(min_cut, max_cut, rng);
		WALL_RECT top{ n.area.x, n.area.z, n.area.w, cut - n.area.z };
		WALL_RECT bottom{ n.area.x, cut, n.area.w, n.area.z + n.area.h - cut };
		n.p_left = std::make_unique<BSP_NODE>();
		n.p_left->area = top;
		n.p_right = std::make_unique<BSP_NODE>();
		n.p_right->area = bottom;
		out_vertical = false;
		out_cut = cut;
		return true;
	}
}

//-----------------------------------------------
// BSPノードを分割し、壁を追加していく（再帰）
//-----------------------------------------------
static inline void BuildBSPAndWalls(BSP_NODE& n, WALL_GRID& g, const BSP_PARAMS& params, int depth, std::mt19937& rng)
{
	n.rep_x = n.area.cx();
	n.rep_z = n.area.cz();

	if (depth >= params.max_depth)
	{
		return;
	}

	bool vertical = false;
	int cut = 0;
	if (!SplitNode(n, params, vertical, cut, rng))
	{
		return;
	}

	int door_count = RandomInt(params.doors_per_split_min, params.doors_per_split_max, rng);

	if (vertical)
	{
		AddVerticalWallWithDoors(g, cut, n.area.z, n.area.z + n.area.h, params.door_width, door_count, rng);
	}
	else
	{
		AddHorizontalWallWithDoors(g, cut, n.area.x, n.area.x + n.area.w, params.door_width, door_count, rng);
	}

	BuildBSPAndWalls(*n.p_left, g, params, depth + 1, rng);
	BuildBSPAndWalls(*n.p_right, g, params, depth + 1, rng);
}

//-----------------------------------------------
// BSPで壁レイアウトを生成
//-----------------------------------------------
static inline GENERATED_LAYOUT GenerateBSPWalls(const BSP_PARAMS& params)
{
	if (params.map_width <= 2 || params.map_height <= 2)
	{
		throw std::invalid_argument("map too small");
	}
	if (params.center_size <= 2)
	{
		throw std::invalid_argument("center size too small");
	}
	if (params.center_size >= params.map_width || params.center_size >= params.map_height)
	{
		throw std::invalid_argument("center size too large");
	}

	GENERATED_LAYOUT out(params.map_width, params.map_height);
	out.wall_grid.ClearAll();
	out.wall_grid.AddBoundaryWalls();

	std::mt19937 rng(params.seed);

	int cx = (params.map_width - params.center_size) / 2;
	int cz = (params.map_height - params.center_size) / 2;
	WALL_RECT center{ cx, cz, params.center_size, params.center_size };
	out.center_area = center;

	int x1 = center.x1();
	int z1 = center.z1();
	int x2 = center.x2();
	int z2 = center.z2();

	{
		auto MacroDoorCount = [&]() {return RandomInt(params.macro_door_per_segment_min, params.macro_door_per_segment_max, rng); };

		AddVerticalWallWithDoors(out.wall_grid, x1, 0, z1, params.door_width, MacroDoorCount(), rng);
		AddVerticalWallWithDoors(out.wall_grid, x1, z1, z2, params.door_width, MacroDoorCount(), rng);
		AddVerticalWallWithDoors(out.wall_grid, x1, z2, params.map_height, params.door_width, MacroDoorCount(), rng);

		AddVerticalWallWithDoors(out.wall_grid, x2, 0, z1, params.door_width, MacroDoorCount(), rng);
		AddVerticalWallWithDoors(out.wall_grid, x2, z1, z2, params.door_width, MacroDoorCount(), rng);
		AddVerticalWallWithDoors(out.wall_grid, x2, z2, params.map_height, params.door_width, MacroDoorCount(), rng);

		AddHorizontalWallWithDoors(out.wall_grid, z1, 0, x1, params.door_width, MacroDoorCount(), rng);
		AddHorizontalWallWithDoors(out.wall_grid, z1, x1, x2, params.door_width, MacroDoorCount(), rng);
		AddHorizontalWallWithDoors(out.wall_grid, z1, x2, params.map_width, params.door_width, MacroDoorCount(), rng);

		AddHorizontalWallWithDoors(out.wall_grid, z2, 0, x1, params.door_width, MacroDoorCount(), rng);
		AddHorizontalWallWithDoors(out.wall_grid, z2, x1, x2, params.door_width, MacroDoorCount(), rng);
		AddHorizontalWallWithDoors(out.wall_grid, z2, x2, params.map_width, params.door_width, MacroDoorCount(), rng);
	}

	int x0 = 0, x3 = params.map_width;
	int z0 = 0, z3 = params.map_height;

	WALL_RECT regions[8] = 
	{
		{ x0, z0, x1 - x0, z1 - z0 },
		{ x1, z0, x2 - x1, z1 - z0 },
		{ x2, z0, x3 - x2, z1 - z0 },
		{ x0, z1, x1 - x0, z2 - z1 },
		{ x2, z1, x3 - x2, z2 - z1 },
		{ x0, z2, x1 - x0, z3 - z2 },
		{ x1, z2, x2 - x1, z3 - z2 },
		{ x2, z2, x3 - x2, z3 - z2 },
	};

	out.macro_regions.clear();
	out.macro_regions.reserve(8);

	for (const WALL_RECT& r : regions)
	{
		if (r.w <= params.min_leaf_size || r.h <= params.min_leaf_size)
		{
			continue;
		}

		out.macro_regions.push_back(r);

		BSP_NODE root;
		root.area = r;
		BuildBSPAndWalls(root, out.wall_grid, params, 0, rng);
	}

	return out;
}

//-----------------------------------------------
// 壁グリッドから壁セグメントをマージして取得
//-----------------------------------------------
static inline std::vector<WALL_SEGMENT> BuildMergedWallSegments(const WALL_GRID& g)
{
	std::vector<WALL_SEGMENT> segs;
	segs.reserve((g.w + 1) * g.h + g.w * (g.h + 1));

	for (int x = 0; x <= g.w; ++x)
	{
		int z = 0;
		while (z < g.h)
		{
			if (g.GetV(x, z) == 0) { ++z; continue; }

			int zStart = z;
			while (z < g.h && g.GetV(x, z) == 1) ++z;
			int zEnd = z;

			WALL_SEGMENT s{};
			s.vertical = true;
			s.x0 = x;
			s.z0 = zStart;
			s.x1 = x;
			s.z1 = zEnd;
			s.length = (zEnd - zStart);
			segs.push_back(s);
		}
	}

	for (int z = 0; z <= g.h; ++z)
	{
		int x = 0;
		while (x < g.w)
		{
			if (g.GetH(x, z) == 0) { ++x; continue; }

			int xStart = x;
			while (x < g.w && g.GetH(x, z) == 1) ++x;
			int xEnd = x;

			WALL_SEGMENT s{};
			s.vertical = false;
			s.x0 = xStart;
			s.z0 = z;
			s.x1 = xEnd;
			s.z1 = z;
			s.length = (xEnd - xStart);
			segs.push_back(s);
		}
	}

	return segs;
}

//-----------------------------------------------
// 壁セグメントを壁インスタンスに変換
//-----------------------------------------------
static inline WALL_INSTANCE ToInstance(const WALL_SEGMENT& s, float cell, float wallH, float thick)
{
	WALL_INSTANCE w{};
	w.py = wallH * 0.5f;
	w.sy = wallH;

	const float endInset = thick * 0.5f; // まずこれ。効きすぎなら 0.25f に

	if (s.vertical)
	{
		float x = s.x0 * cell;
		float z0 = s.z0 * cell;
		float z1 = s.z1 * cell;

		// 両端を内側へ
		z0 += endInset;
		z1 -= endInset;

		float len = (z1 - z0);
		if (len < 0.01f) len = 0.01f;

		w.sx = thick;
		w.sz = len;
		w.px = x;
		w.pz = (z0 + z1) * 0.5f;
		w.yaw = 0.0f;
	}
	else
	{
		float z = s.z0 * cell;
		float x0 = s.x0 * cell;
		float x1 = s.x1 * cell;

		x0 += endInset;
		x1 -= endInset;

		float len = (x1 - x0);
		if (len < 0.01f) len = 0.01f;

		w.sx = len;
		w.sz = thick;
		w.px = (x0 + x1) * 0.5f;
		w.pz = z;
		w.yaw = 0.0f;
	}

	return w;
}

static inline VECTOR3 CalcRectCenterWorld(const WALL_RECT& r, float cell)
{
	return VECTOR3(
		(r.x + r.w * 0.5f) * cell,
		0.0f,
		(r.z + r.h * 0.5f) * cell
	);
}

static inline VECTOR3 CalcRectSizeWorld(const WALL_RECT& r, float cell)
{
	return VECTOR3(
		r.w * cell,
		0.0f,
		r.h * cell
	);
}

static inline bool HitAABB2_Expanded(float px, float pz, float r, const AABB2& b)
{
	return (px >= b.min_x - r && px <= b.max_x + r &&
		pz >= b.min_z - r && pz <= b.max_z + r);
}

static inline std::vector<DOOR_SEGMENT> ExtractDoorsFromWallGrid(const WALL_GRID& g)
{
	std::vector<DOOR_SEGMENT> doors;

	// ---- V: x固定、z方向 ----
	for (int x = 0; x <= g.w; ++x)
	{
		int z = 0;
		while (z < g.h)
		{
			// 0区間を探す
			if (g.GetV(x, z) != 0) { ++z; continue; }

			int z_start = z;
			while (z < g.h && g.GetV(x, z) == 0) ++z;
			int z_end = z; // [zStart, zEnd)

			// “前後が1に挟まれてる”ならドアっぽい
			bool bounded =
				(z_start - 1 >= 0 && z_end < g.h &&
					g.GetV(x, z_start - 1) == 1 &&
					g.GetV(x, z_end) == 1);

			if (bounded)
			{
				DOOR_SEGMENT d{};
				d.vertical = true;
				d.line = x;
				d.a0 = z_start;
				d.a1 = z_end;
				doors.push_back(d);
			}
		}
	}

	// ---- H: z固定、x方向 ----
	for (int z = 0; z <= g.h; ++z)
	{
		int x = 0;
		while (x < g.w)
		{
			if (g.GetH(x, z) != 0) { ++x; continue; }

			int x_start = x;
			while (x < g.w && g.GetH(x, z) == 0) ++x;
			int x_end = x;

			bool bounded =
				(x_start - 1 >= 0 && x_end < g.w &&
					g.GetH(x_start - 1, z) == 1 &&
					g.GetH(x_end, z) == 1);

			if (bounded)
			{
				DOOR_SEGMENT d{};
				d.vertical = false;
				d.line = z;
				d.a0 = x_start;
				d.a1 = x_end;
				doors.push_back(d);
			}
		}
	}

	return doors;
}

static inline AABB2 WallInstanceToAABB2(const WALL_INSTANCE& w)
{
	AABB2 b{};
	b.min_x = w.px - w.sx * 0.5f;
	b.max_x = w.px + w.sx * 0.5f;
	b.min_z = w.pz - w.sz * 0.5f;
	b.max_z = w.pz + w.sz * 0.5f;
	return b;
}

static inline AABB2 DoorToAABB2(const DOOR_SEGMENT& d, float cell, float thick)
{
	// ドア開口部を「そこにも置かない」ためのAABB（壁厚み範囲に設定）
	AABB2 b{};
	if (d.vertical)
	{
		float x = d.line * cell;
		float z0 = d.a0 * cell;
		float z1 = d.a1 * cell;
		b.min_x = x - thick * 0.5f;
		b.max_x = x + thick * 0.5f;
		b.min_z = z0;
		b.max_z = z1;
	}
	else
	{
		float z = d.line * cell;
		float x0 = d.a0 * cell;
		float x1 = d.a1 * cell;
		b.min_x = x0;
		b.max_x = x1;
		b.min_z = z - thick * 0.5f;
		b.max_z = z + thick * 0.5f;
	}
	return b;
}