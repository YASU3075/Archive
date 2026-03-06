//-----------------------------------------------
// MeshRendererComponent.cpp
// 制作日：2025/11/30
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "component/renderer/MeshRendererComponent.h"
#include "renderer/Renderer.h"
#include "entity/GameObject.h"
#include "renderer/Material.h"
#include "renderer/MeshFactory.h"
#include "renderer/MaterialManager.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void MESH_RENDERER_COMPONENT::Init()
{
	m_index_buffer = nullptr;
	m_vertex_buffer = nullptr;

	if (!m_p_material)
	{
		m_p_material = MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\DefaultMaterial.mtl");
	}

	// デフォルトでキューブ
	MESH_RENDERER_DATA cube_data;
	cube_data = MESH_FACTORY::CreateCubeMesh(1.0f, 1.0f, 1.0f);
	SetMeshData(cube_data);
}

//-----------------------------------------------
// 終了処理
//-----------------------------------------------
void MESH_RENDERER_COMPONENT::Uninit()
{
	if (m_vertex_buffer)
	{
		m_vertex_buffer->Release();
		m_vertex_buffer = nullptr;
	}
	if (m_index_buffer)
	{
		m_index_buffer->Release();
		m_index_buffer = nullptr;
	}
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void MESH_RENDERER_COMPONENT::Update()
{
	if (!m_p_material)
	{
		return;
	}

	if (m_enbale_uv_scroll)
	{
		XMFLOAT2 offset = m_p_material->m_material_data.m_uv_offset;
		offset.x += m_uv_speed.x;
		offset.y += m_uv_speed.y;

		// 0～1の範囲に収める
		offset.x -= floorf(offset.x);
		offset.y -= floorf(offset.y);

		m_p_material->m_material_data.m_uv_offset = offset;
	}
}


//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void MESH_RENDERER_COMPONENT::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_p_material->m_vertex_layout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_p_material->m_vertex_shader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_p_material->m_pixel_shader, NULL, 0);

	// マトリクス設定
	Renderer::SetWorldMatrix(BuildWorldMatrix());

	// マテリアル設定
	Renderer::SetMaterial(m_p_material->m_material_data);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_p_material->m_texture);

	// 描画
	DrawGeometyrOnly();
}

//-----------------------------------------------
// ワールド行列の構築
//-----------------------------------------------
XMMATRIX MESH_RENDERER_COMPONENT::BuildWorldMatrix() const
{
	VECTOR3 pos = m_p_owner->GetTransform()->GetPos();
	QUATERNION q = m_p_owner->GetTransform()->GetRot();
	VECTOR3 sca = m_p_owner->GetTransform()->GetSca();

	VECTOR3 local_pos = GetLocalPos();
	QUATERNION local_rot = GetLocalRot();
	VECTOR3 local_sca = GetLocalSca();

	XMMATRIX world, local, scale, rotation, trans;
	scale = XMMatrixScaling(local_sca.x, local_sca.y, local_sca.z);
	XMVECTOR local_q_m = XMVectorSet(local_rot.x, local_rot.y, local_rot.z, local_rot.w);
	rotation = XMMatrixRotationQuaternion(local_q_m);

	trans = XMMatrixTranslation(local_pos.x, local_pos.y, local_pos.z);
	local = scale * rotation * trans;

	scale = XMMatrixScaling(sca.x, sca.y, sca.z);

	XMVECTOR q_m = XMVectorSet(q.x, q.y, q.z, q.w);
	rotation = XMMatrixRotationQuaternion(q_m);

	trans = XMMatrixTranslation(pos.x, pos.y, pos.z);
	world = scale * rotation * trans;
	return local * world;
}

//-----------------------------------------------
// ジオメトリのバインド
//-----------------------------------------------
void MESH_RENDERER_COMPONENT::BindGeometry() const
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);
	// インデックスバッファ設定
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);
	// プリミティブトポロジー設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

//-----------------------------------------------
// ジオメトリのみ描画
//-----------------------------------------------
void MESH_RENDERER_COMPONENT::DrawGeometyrOnly() const
{
	BindGeometry();
	Renderer::GetDeviceContext()->DrawIndexed(static_cast<UINT>(m_mesh_data.indices.size()), 0, 0);
}



//-----------------------------------------------
// メッシュデータの設定
//-----------------------------------------------
void MESH_RENDERER_COMPONENT::SetMeshData(const MESH_RENDERER_DATA& mesh_data)
{
	m_mesh_data = mesh_data;
	// 頂点バッファの作成
	if (m_vertex_buffer)
	{
		m_vertex_buffer->Release();
		m_vertex_buffer = nullptr;
	}
	D3D11_BUFFER_DESC vb_desc = {};
	vb_desc.Usage = D3D11_USAGE_DEFAULT;
	vb_desc.ByteWidth = sizeof(VERTEX_3D) * static_cast<UINT>(m_mesh_data.vertices.size());
	vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vb_desc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA vb_data = {};
	vb_data.pSysMem = m_mesh_data.vertices.data();
	HRESULT hr = Renderer::GetDevice()->CreateBuffer(&vb_desc, &vb_data, &m_vertex_buffer);
	
	assert(SUCCEEDED(hr));

	// インデックスバッファの作成
	if (m_index_buffer)
	{
		m_index_buffer->Release();
		m_index_buffer = nullptr;
	}
	D3D11_BUFFER_DESC ib_desc = {};
	ib_desc.Usage = D3D11_USAGE_DEFAULT;
	ib_desc.ByteWidth = sizeof(UINT) * static_cast<UINT>(m_mesh_data.indices.size());
	ib_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ib_desc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA ib_data = {};
	ib_data.pSysMem = m_mesh_data.indices.data();
	hr = Renderer::GetDevice()->CreateBuffer(&ib_desc, &ib_data, &m_index_buffer);
	
	assert(SUCCEEDED(hr));
}
