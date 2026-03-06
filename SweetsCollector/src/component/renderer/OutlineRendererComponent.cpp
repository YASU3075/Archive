//-----------------------------------------------
// OutlineRendererComponent.cpp
// 制作日：2026/02/11
// 制作者：安田晴人
//-----------------------------------------------
#include "component/renderer/OutlineRendererComponent.h"
#include "entity/GameObject.h"
#include "renderer/MaterialManager.h"
#include "renderer/Material.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void OUTLINE_RENDERER_COMPONENT::Init()
{
	SetRenderPass(RENDER_PASS::RENDER_PASS_TRANSPARENT);
	SetOrderInPass(-1000);

	RENDERER_COMPONENT* component = m_p_owner->GetComponent<RENDERER_COMPONENT>();
	if (component)
	{
		m_target = component;
	}

	// 定数バッファ作成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(OUTLINE_PARAMS);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_outline_cbuffer);

	// デフォルト設定
	m_outline_cbuffer_data.outline_color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_outline_cbuffer_data.outline_width = 1.0f;
	if (!m_p_material)
	{
		m_p_material = MATERIAL_MANAGER::LoadFromCustomFile("asset\\material\\OutlineMaterial.mtl");
	}
}

//-----------------------------------------------
// 終了処理
//-----------------------------------------------
void OUTLINE_RENDERER_COMPONENT::Uninit()
{
	if (m_outline_cbuffer)
	{
		m_outline_cbuffer->Release();
		m_outline_cbuffer = nullptr;
	}
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void OUTLINE_RENDERER_COMPONENT::Draw()
{
	if (!m_target || !m_p_material)
	{
		return;
	}
	
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_p_material->m_vertex_layout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_p_material->m_vertex_shader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_p_material->m_pixel_shader, NULL, 0);

	// 深度モード設定
	Renderer::SetDepthMode(DEPTH_MODE::DEPTH_MODE_READ_ONLY);
	
	// カリングモード設定
	Renderer::SetCullMode(CULL_MODE::CULL_MODE_FRONT);

	// マトリクス設定
	Renderer::SetWorldMatrix(m_target->BuildWorldMatrix());

	// アウトラインパラメータ設定
	Renderer::SetOutlineParams(m_outline_cbuffer_data);

	// ジオメトリ描画
	m_target->DrawGeometyrOnly();

	// カリングモード設定戻し
	Renderer::SetCullMode(CULL_MODE::CULL_MODE_BACK);
}

//-----------------------------------------------
// アウトライン色設定
//-----------------------------------------------
void OUTLINE_RENDERER_COMPONENT::SetOutlineColor(const XMFLOAT4& color)
{
	m_outline_cbuffer_data.outline_color = color;
}

//-----------------------------------------------
// アウトライン幅設定
//-----------------------------------------------
void OUTLINE_RENDERER_COMPONENT::SetOutlineWidth(float width)
{
	m_outline_cbuffer_data.outline_width = width;
}