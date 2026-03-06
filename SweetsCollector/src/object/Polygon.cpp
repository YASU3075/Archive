//-----------------------------------------------
// Polygon.cpp
// 制作日：2025/04/23
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "entity/GameObject.h"
#include "scene/Manager.h"
#include "object/Polygon.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void POLYGON_2D::Init()
{
	m_transform.SetSca(VECTOR3(100.0f, 100.0f, 0.0f));
	m_transform.SetPos(VECTOR3(150.0f, 200.0f, 0.0f));
}


//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void POLYGON_2D::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_vertex_layout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_vertex_shader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_pixel_shader, NULL, 0);

	//マテリアル設定
	MATERIAL_DATA material{};
	material.m_diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	material.m_texture_enable = true;
	Renderer::SetMaterial(material);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	VECTOR3 euler = m_transform.GetRot().ToEuler();
	float rot = euler.z * 3.14159f / 180.0f;

	//SetWorldMatrix(m_transform.GetSca(), VECTOR3(euler.x, euler.y, rot), m_transform.GetPos());

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//描画処理
	Renderer::GetDeviceContext()->Draw(4, 0);
}

