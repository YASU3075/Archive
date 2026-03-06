//-----------------------------------------------
// ModelRendererComponent.cpp
// 制作日：2025/07/18
// 制作者：安田晴人
//-----------------------------------------------
#include "renderer/Material.h"
#include "renderer/ModelRenderer.h"
#include "component/renderer/ModelRendererComponent.h"
#include "scene/Manager.h"
#include "entity/GameObject.h"
#include "renderer/Renderer.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void MODEL_RENDERER_COMPONENT::Init()
{
	// デフォルト設定
	SetModel("asset\\model\\Cube.obj");
	if (m_p_material == nullptr)
	{
		m_p_material = new MATERIAL();
	}
	m_p_material->CreateShaders("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
}

//-----------------------------------------------
// 終了処理
//-----------------------------------------------
void MODEL_RENDERER_COMPONENT::Uninit()
{
	delete m_model_renderer;
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void MODEL_RENDERER_COMPONENT::Update()
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
void MODEL_RENDERER_COMPONENT::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_p_material->m_vertex_layout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_p_material->m_vertex_shader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_p_material->m_pixel_shader, NULL, 0);

	//マトリクス設定
	Renderer::SetWorldMatrix(BuildWorldMatrix());

	if (m_use_model_material)
	{
		m_model_renderer->Draw();
	}
	else
	{
		//マテリアル設定
		Renderer::SetMaterial(m_p_material->m_material_data);

		//テクスチャ設定
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_p_material->m_texture);

		DrawGeometyrOnly();
	}
}

//-----------------------------------------------
// ワールド行列の構築
//-----------------------------------------------
XMMATRIX MODEL_RENDERER_COMPONENT::BuildWorldMatrix() const
{
	VECTOR3 pos = m_p_owner->GetTransform()->GetPos();
	QUATERNION q = m_p_owner->GetTransform()->GetRot();
	VECTOR3 sca = m_p_owner->GetTransform()->GetSca();

	VECTOR3 local_pos = GetLocalPos();
	QUATERNION local_q = GetLocalRot();
	VECTOR3 local_sca = GetLocalSca();

	XMMATRIX world, local, scale, rotation, trans;

	scale = XMMatrixScaling(local_sca.x, local_sca.y, local_sca.z);
	XMVECTOR local_q_m = XMVectorSet(local_q.x, local_q.y, local_q.z, local_q.w);
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
void MODEL_RENDERER_COMPONENT::BindGeometry() const
{
	m_model_renderer->BindGeometry();
}

//-----------------------------------------------
// ジオメトリのみ描画
//-----------------------------------------------
void MODEL_RENDERER_COMPONENT::DrawGeometyrOnly() const
{
	m_model_renderer->DrawGeometyrOnly();
}

//-----------------------------------------------
// モデル設定
//-----------------------------------------------
void MODEL_RENDERER_COMPONENT::SetModel(const char* name)
{
	m_model_renderer = new ModelRenderer();
	m_model_renderer->Load(name);
}

//-----------------------------------------------
// モデルのマテリアルを使用するか設定
//-----------------------------------------------
void MODEL_RENDERER_COMPONENT::SetUseModelMaterial(bool use)
{
	m_use_model_material = use;
	if (m_model_renderer)
	{
		m_model_renderer->SetUseModelMaterial(use);
	}
}
