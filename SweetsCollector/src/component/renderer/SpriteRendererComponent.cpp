//-----------------------------------------------
// SpriteRendererComponent.cpp
// 制作日：2025/07/18
// 制作者：安田晴人
//-----------------------------------------------
#include "component/renderer/SpriteRendererComponent.h"
#include "renderer/Renderer.h"
#include "renderer/Material.h"
#include "renderer/MaterialManager.h"
#include "renderer/TextureManager.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "entity/GameObject.h"
#include "object/Camera.h"
#include "component/CameraComponent.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void SPRITE_RENDERER_COMPONENT::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[1].Position = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(0.5f, -0.5f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(1.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(0.5f, 0.5f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 0.0f);

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_vertex_buffer);

	m_p_material = MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\DefaultMaterial.mtl");

	m_is_billboard = false;
	m_uv_sheet = false;
	m_sprite_index = 0;
	m_x_num = 1;
	m_y_num = 1;
}

//-----------------------------------------------
// 終了処理
//-----------------------------------------------
void SPRITE_RENDERER_COMPONENT::Uninit()
{
	if (m_vertex_buffer)
	{
		m_vertex_buffer->Release();
		m_vertex_buffer = nullptr;
	}
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void SPRITE_RENDERER_COMPONENT::Update()
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
void SPRITE_RENDERER_COMPONENT::Draw()
{

	bool need_map = (!m_is_billboard) || (m_uv_sheet);

	VERTEX_3D* vertex = nullptr;
	if (need_map)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		vertex = (VERTEX_3D*)msr.pData;
	}

	if (!m_is_billboard)
	{
		//頂点データ書き換え
		VECTOR3 owner_pos = m_p_owner->GetTransform()->GetPos();
		VECTOR3 owner_sca = m_p_owner->GetTransform()->GetSca();

		VECTOR3 pos = owner_pos + m_local_pos;
		VECTOR3 sca = VECTOR3(owner_sca.x * m_local_sca.x, owner_sca.y * m_local_sca.y, owner_sca.z * m_local_sca.z);

		vertex[0].Position = XMFLOAT3(pos.x - sca.x * 0.5f + SCREEN_WIDTH * 0.5f, pos.y - sca.y * 0.5f + SCREEN_HEIGHT * 0.5f, 0.0f);
		vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

		vertex[1].Position = XMFLOAT3(pos.x + sca.x * 0.5f + SCREEN_WIDTH * 0.5f, pos.y - sca.y * 0.5f + SCREEN_HEIGHT * 0.5f, 0.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

		vertex[2].Position = XMFLOAT3(pos.x - sca.x * 0.5f + SCREEN_WIDTH * 0.5f, pos.y + sca.y * 0.5f + SCREEN_HEIGHT * 0.5f, 0.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

		vertex[3].Position = XMFLOAT3(pos.x + sca.x * 0.5f + SCREEN_WIDTH * 0.5f, pos.y + sca.y * 0.5f + SCREEN_HEIGHT * 0.5f, 0.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);
	}

	if (m_uv_sheet)
	{
		int frame_x = m_sprite_index % m_x_num;
		int frame_y = m_sprite_index / m_x_num;

		float u_size = 1.0f / static_cast<float>(m_x_num);
		float v_size = 1.0f / static_cast<float>(m_y_num);

		vertex[0].TexCoord = XMFLOAT2(u_size * frame_x, v_size * frame_y);
		vertex[1].TexCoord = XMFLOAT2(u_size * (frame_x + 1), v_size * frame_y);
		vertex[2].TexCoord = XMFLOAT2(u_size * frame_x, v_size * (frame_y + 1));
		vertex[3].TexCoord = XMFLOAT2(u_size * (frame_x + 1), v_size * (frame_y + 1));
	}

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_p_material->m_vertex_layout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_p_material->m_vertex_shader, nullptr, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_p_material->m_pixel_shader, nullptr, 0);

	//マテリアル設定
	Renderer::SetMaterial(m_p_material->m_material_data);

	//マトリクス設定
	if (m_is_billboard)
	{//ビルボード
		VECTOR3 owner_pos = m_p_owner->GetTransform()->GetPos();
		VECTOR3 owner_sca = m_p_owner->GetTransform()->GetSca();

		VECTOR3 pos = owner_pos;
		VECTOR3 sca = owner_sca;

		VECTOR3 local_pos = m_local_pos;
		VECTOR3 local_sca = m_local_sca;

		XMMATRIX world, local, scale, rotation, trans;
		scale = XMMatrixScaling(local_sca.x, local_sca.y, local_sca.z);
		trans = XMMatrixTranslation(local_pos.x, local_pos.y, local_pos.z);
		rotation = XMMatrixIdentity();
		local = scale * rotation * trans;

		scale = XMMatrixScaling(sca.x, sca.y, sca.z);
		trans = XMMatrixTranslation(pos.x, pos.y, pos.z);

		CAMERA* p_camera = Manager::GetScene()->GetGameObject<CAMERA>();
		CAMERA_COMPONENT* p_camera_component = p_camera->GetComponent<CAMERA_COMPONENT>();

		XMMATRIX view = p_camera_component->GetViewMatrix();
		
		XMMATRIX inv_view;
		inv_view = XMMatrixInverse(nullptr, view);
		inv_view.r[3].m128_f32[0] = 0.0f;
		inv_view.r[3].m128_f32[1] = 0.0f;
		inv_view.r[3].m128_f32[2] = 0.0f;

		rotation = inv_view;

		world = scale * rotation * trans;
		world = local * world;

		Renderer::SetWorldMatrix(world);
	}

	else
	{
		Renderer::SetWorldViewProjection2D();
	}

	

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_p_material->m_texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	if (need_map)
	{
		Renderer::GetDeviceContext()->Unmap(m_vertex_buffer, 0);
	}

	//描画処理
	Renderer::GetDeviceContext()->Draw(4, 0);
}

//-----------------------------------------------
// テクスチャ設定
//-----------------------------------------------
void SPRITE_RENDERER_COMPONENT::SetTexture(const char* name)
{
	m_p_material->m_texture = TEXTURE_MANAGER::Load(name);
}
