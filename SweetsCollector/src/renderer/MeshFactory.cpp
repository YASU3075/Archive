//-----------------------------------------------
// MeshFactory.cpp
// 制作日：2025/11/30
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/MeshFactory.h"

//-----------------------------------------------
// キューブメッシュの作成
//-----------------------------------------------
MESH_RENDERER_DATA MESH_FACTORY::CreateCubeMesh(float width, float height, float depth)
{
	MESH_RENDERER_DATA mesh;

	const float w = width * 0.5f;
	const float h = height * 0.5f;
	const float d = depth * 0.5f;

	// uv座標の定義
	const XMFLOAT2 uv[4] =
	{
		{0.0f, 1.0f}, // 左下
		{1.0f, 1.0f}, // 右下
		{1.0f, 0.0f}, // 右上
		{0.0f, 0.0f}, // 左上
	};

	// 面を追加するラムダ関数
	auto addFace = [&](XMFLOAT3 p0, XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 p3, XMFLOAT3 n)
		{
			const uint32_t base = static_cast<uint32_t>(mesh.vertices.size());

			// 頂点の追加
			VERTEX_3D v[4]{};
			v[0].Position = p0; v[0].TexCoord = uv[0];
			v[1].Position = p1; v[1].TexCoord = uv[1];
			v[2].Position = p2; v[2].TexCoord = uv[2];
			v[3].Position = p3; v[3].TexCoord = uv[3];

			for (int i = 0; i < 4; ++i)
			{// 法線と拡散反射色の設定
				v[i].Normal = n;
				v[i].Diffuse = XMFLOAT4(1, 1, 1, 1);
				mesh.vertices.push_back(v[i]);
			}

			// インデックスの追加
			mesh.indices.push_back(base + 0);
			mesh.indices.push_back(base + 1);
			mesh.indices.push_back(base + 2);

			mesh.indices.push_back(base + 0);
			mesh.indices.push_back(base + 2);
			mesh.indices.push_back(base + 3);
		};


	// Back
	addFace({ -w,-h, d }, { w,-h, d }, { w, h, d }, { -w, h, d }, { 0,0,-1 });
	// Front
	addFace({ w,-h,-d }, { -w,-h,-d }, { -w, h,-d }, { w, h,-d }, { 0,0,1 });

	// Top
	addFace({ -w, h, d }, { w, h, d }, { w, h,-d }, { -w, h,-d }, { 0,1,0 });
	// Bottom
	addFace({ -w,-h,-d }, { w,-h,-d }, { w,-h, d }, { -w,-h, d }, { 0,-1,0 });

	// Left
	addFace({ w,-h, d }, { w,-h,-d }, { w, h,-d }, { w, h, d }, { -1,0,0 });
	// Right
	addFace({ -w,-h,-d }, { -w,-h, d }, { -w, h, d }, { -w, h,-d }, { 1,0,0 });

	return mesh;
}

//-----------------------------------------------
// 球メッシュの作成
//-----------------------------------------------
MESH_RENDERER_DATA MESH_FACTORY::CreateSphereMesh(float radius, int slices, int stacks)
{
	MESH_RENDERER_DATA mesh;

	// 頂点データの生成
	for (int i = 0; i <= stacks; ++i)
	{
		float phi = XM_PI * i / stacks;
		for (int j = 0; j <= slices; ++j)
		{
			float theta = XM_2PI * j / slices;
			VERTEX_3D vertex;
			vertex.Position.x = radius * sinf(phi) * cosf(theta);
			vertex.Position.y = radius * cosf(phi);
			vertex.Position.z = radius * sinf(phi) * sinf(theta);
			VECTOR3 normal = -VECTOR3(vertex.Position.x, vertex.Position.y, vertex.Position.z).GetNormalized();
			vertex.Normal = XMFLOAT3(normal.x, normal.y, normal.z);
			vertex.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex.TexCoord.x = static_cast<float>(j) / slices;
			vertex.TexCoord.y = static_cast<float>(i) / stacks;
			mesh.vertices.push_back(vertex);
		}
	}

	// インデックスデータの生成
	for (int i = 0; i < stacks; ++i)
	{
		for (int j = 0; j < slices; ++j)
		{
			int first = (i * (slices + 1)) + j;
			int second = first + slices + 1;
			mesh.indices.push_back(first);
			mesh.indices.push_back(first + 1);
			mesh.indices.push_back(second);

			mesh.indices.push_back(second);
			mesh.indices.push_back(first + 1);
			mesh.indices.push_back(second + 1);
		}
	}

	return mesh;
}


//-----------------------------------------------
// 円柱メッシュの作成
//-----------------------------------------------
MESH_RENDERER_DATA MESH_FACTORY::CreateCylinderMesh(float radius, float height, int segments)
{
	MESH_RENDERER_DATA mesh;
	float halfHeight = height * 0.5f;

	// 頂点データの生成
	for (int i = 0; i <= segments; ++i)
	{
		float theta = XM_2PI * i / segments;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);
		// 上面の頂点
		VERTEX_3D topVertex;
		topVertex.Position = XMFLOAT3(x, halfHeight, z);
		VECTOR3 normal = VECTOR3(x, 0.0f, z).GetNormalized();
		topVertex.Normal = XMFLOAT3(normal.x, normal.y, normal.z);
		topVertex.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		topVertex.TexCoord = XMFLOAT2(static_cast<float>(i) / segments, 0.0f);
		mesh.vertices.push_back(topVertex);
		// 下面の頂点
		VERTEX_3D bottomVertex;
		bottomVertex.Position = XMFLOAT3(x, -halfHeight, z);
		bottomVertex.Normal = XMFLOAT3(normal.x, normal.y, normal.z);
		bottomVertex.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		bottomVertex.TexCoord = XMFLOAT2(static_cast<float>(i) / segments, 1.0f);
		mesh.vertices.push_back(bottomVertex);
	}

	// インデックスデータの生成
	for (int i = 0; i < segments; ++i)
	{
		int base = i * 2;
		// 側面
		mesh.indices.push_back(base);
		mesh.indices.push_back(base + 2);
		mesh.indices.push_back(base + 1);
		mesh.indices.push_back(base + 2);
		mesh.indices.push_back(base + 3);
		mesh.indices.push_back(base + 1);
	}

	// 上面の中心頂点
	int topCenterIndex = static_cast<int>(mesh.vertices.size());
	VERTEX_3D topCenterVertex;
	topCenterVertex.Position = XMFLOAT3(0.0f, halfHeight, 0.0f);
	topCenterVertex.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	topCenterVertex.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	topCenterVertex.TexCoord = XMFLOAT2(0.5f, 0.5f);
	mesh.vertices.push_back(topCenterVertex);

	// 下面の中心頂点
	int bottomCenterIndex = static_cast<int>(mesh.vertices.size());
	VERTEX_3D bottomCenterVertex;
	bottomCenterVertex.Position = XMFLOAT3(0.0f, -halfHeight, 0.0f);
	bottomCenterVertex.Normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	bottomCenterVertex.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	bottomCenterVertex.TexCoord = XMFLOAT2(0.5f, 0.5f);
	mesh.vertices.push_back(bottomCenterVertex);

	// 上面のインデックス
	for (int i = 0; i < segments; ++i)
	{
		int topVertexIndex = i * 2;
		 mesh.indices.push_back(topCenterIndex);
		 mesh.indices.push_back((topVertexIndex + 2) % (segments * 2));
		 mesh.indices.push_back(topVertexIndex);
	}

	// 下面のインデックス
	for (int i = 0; i < segments; ++i)
	{
		int bottomVertexIndex = i * 2 + 1;
		mesh.indices.push_back(bottomCenterIndex);
		mesh.indices.push_back(bottomVertexIndex);
		mesh.indices.push_back((bottomVertexIndex + 2) % (segments * 2));
	}

	return mesh;
}

//-----------------------------------------------
// 平面メッシュの作成
//-----------------------------------------------
MESH_RENDERER_DATA MESH_FACTORY::CreatePlaneMesh(float width, float depth, int x_segments, int z_segments)
{
	MESH_RENDERER_DATA mesh;

	x_segments = std::max(1, x_segments);
	z_segments = std::max(1, z_segments);

	const float halfW = width * 0.5f;
	const float halfD = depth * 0.5f;

	const int vxCount = x_segments + 1;
	const int vzCount = z_segments + 1;

	mesh.vertices.reserve(vxCount * vzCount);
	mesh.indices.reserve(x_segments * z_segments * 6);

	for (int z = 0; z < vzCount; ++z)
	{
		float tz = (float)z / (float)z_segments;
		float posZ = -halfD + depth * tz;

		for (int x = 0; x < vxCount; ++x)
		{
			float tx = (float)x / (float)x_segments;
			float posX = -halfW + width * tx;

			VERTEX_3D v{};
			v.Position = XMFLOAT3(posX, 0.0f, posZ);
			v.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			v.Diffuse = XMFLOAT4(1, 1, 1, 1);
			v.TexCoord = XMFLOAT2(tx, tz);

			mesh.vertices.push_back(v);
		}
	}

	auto idx = [&](int x, int z) { return (uint32_t)(z * vxCount + x); };

	for (int z = 0; z < z_segments; ++z)
	{
		for (int x = 0; x < x_segments; ++x)
		{
			uint32_t i0 = idx(x, z);
			uint32_t i1 = idx(x + 1, z);
			uint32_t i2 = idx(x + 1, z + 1);
			uint32_t i3 = idx(x, z + 1);

			mesh.indices.push_back(i0);
			mesh.indices.push_back(i2);
			mesh.indices.push_back(i1);

			mesh.indices.push_back(i0);
			mesh.indices.push_back(i3);
			mesh.indices.push_back(i2);
		}
	}

	return mesh;
}

//-----------------------------------------------
// 法線の再計算
//-----------------------------------------------
void MESH_FACTORY::RecalculateNormals(MESH_RENDERER_DATA& mesh)
{
	// 法線初期化
	for (auto& v : mesh.vertices)
	{
		v.Normal = XMFLOAT3(0, 0, 0);
	}

	// 面ごとに法線を計算して頂点に加算
	auto add = [](XMFLOAT3& a, const XMFLOAT3& b)
		{
			a.x += b.x; a.y += b.y; a.z += b.z;
		};


	for (size_t i = 0; i + 2 < mesh.indices.size(); i += 3)
	{
		// 三角形の頂点インデックス取得
		const uint32_t i0 = mesh.indices[i + 0];
		const uint32_t i1 = mesh.indices[i + 1];
		const uint32_t i2 = mesh.indices[i + 2];

		// 三角形の頂点座標取得
		const auto& p0 = mesh.vertices[i0].Position;
		const auto& p1 = mesh.vertices[i1].Position;
		const auto& p2 = mesh.vertices[i2].Position;

		// 辺ベクトルの計算
		const float e1x = p1.x - p0.x, e1y = p1.y - p0.y, e1z = p1.z - p0.z;
		const float e2x = p2.x - p0.x, e2y = p2.y - p0.y, e2z = p2.z - p0.z;

		// 法線ベクトルの計算（外積）
		XMFLOAT3 n;
		n.x = e1y * e2z - e1z * e2y;
		n.y = e1z * e2x - e1x * e2z;
		n.z = e1x * e2y - e1y * e2x;

		add(mesh.vertices[i0].Normal, n);
		add(mesh.vertices[i1].Normal, n);
		add(mesh.vertices[i2].Normal, n);
	}

	// 法線ベクトルの正規化
	for (auto& v : mesh.vertices)
	{
		const float x = v.Normal.x, y = v.Normal.y, z = v.Normal.z;
		const float len2 = x * x + y * y + z * z;
		if (len2 > 1e-12f)
		{
			const float inv = 1.0f / sqrtf(len2);
			v.Normal.x = x * inv; v.Normal.y = y * inv; v.Normal.z = z * inv;
		}
		else
		{
			v.Normal = XMFLOAT3(0, 1, 0);
		}
	}
}

//-----------------------------------------------
// 高さマップの適用
//-----------------------------------------------
void MESH_FACTORY::ApplyHeight(MESH_RENDERER_DATA& mesh, const std::function<float(float x, float z)>& height_func, bool recalc_normals)
{
	for (auto& v : mesh.vertices)
	{
		v.Position.y = height_func(v.Position.x, v.Position.z);
	}
	if (recalc_normals)
	{
		RecalculateNormals(mesh);
	}
}
