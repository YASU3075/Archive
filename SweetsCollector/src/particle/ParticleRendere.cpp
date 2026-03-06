//-----------------------------------------------
// ParticleRenderer.cpp
// 制作日：2025/06/27
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "renderer/MaterialManager.h"
#include "renderer/Material.h"

#include "object/Camera.h"
#include "component/CameraComponent.h"

#include "particle/core/Particle.h"
#include "particle/core/ParticleRenderer.h"

static constexpr size_t kDefaultMaxParticles = 2000;    // デフォルト最大粒子数

//-----------------------------------------------
// 頂点バッファの容量を確保
//-----------------------------------------------
void PARTICLE_RENDERER::EnsureCapacity(size_t required_vertex_count)
{
    if (required_vertex_count <= m_vertex_capacity)
	{// 十分な容量がある
        return;
    }

	// 新しい容量を計算
    size_t new_cap = (required_vertex_count * 3) / 2;
    if (new_cap < required_vertex_count) new_cap = required_vertex_count;
    m_vertex_capacity = new_cap;

	// 頂点バッファ作成
    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = static_cast<UINT>(sizeof(VERTEX_3D) * m_vertex_capacity);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    ID3D11Buffer* vb = nullptr;
    HRESULT hr = Renderer::GetDevice()->CreateBuffer(&bd, nullptr, &vb);
    assert(SUCCEEDED(hr));

	// 古いバッファを解放して置き換え
    m_vertex_buffer.reset(vb);
}

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void PARTICLE_RENDERER::Init(const char* material_file)
{
	// 頂点バッファ確保
    EnsureCapacity(kDefaultMaxParticles * 6);

	// マテリアル読み込み
    m_material = MATERIAL_MANAGER::LoadFromCustomFile(material_file);
    assert(m_material);
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void PARTICLE_RENDERER::Draw(const std::vector<PARTICLE>& particles)
{
    if (!m_material)
	{// マテリアル未設定
        return;
    }

    // 生存数
    size_t alive = 0;
    for (auto& p : particles)
    {
        if (p.m_alive)
        {
            alive++;
        }
    }

    if (alive == 0)
	{// 描画粒子なし
        return;
    }

	// 頂点バッファ容量確保
    const size_t required_verts = alive * 6;
    EnsureCapacity(required_verts);

    // カメラ
    CAMERA* camera_obj = Manager::GetScene()->GetGameObject<CAMERA>();
    if (!camera_obj)
	{// カメラなし
        return;
    }

    CAMERA_COMPONENT* cam_comp = camera_obj->GetComponent<CAMERA_COMPONENT>();
    if (!cam_comp)
    {
        return;
    }

	// レンダー設定
    Renderer::GetDeviceContext()->IASetInputLayout(m_material->m_vertex_layout);
    Renderer::GetDeviceContext()->VSSetShader(m_material->m_vertex_shader, nullptr, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_material->m_pixel_shader, nullptr, 0);

    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    ID3D11Buffer* vb = m_vertex_buffer.get();
    Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

	// テクスチャセット
    if (m_material->m_material_data.m_texture_enable)
    {
        ID3D11ShaderResourceView* srv = m_material->m_texture;
        Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &srv);
    }

    Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


    MATERIAL_DATA mat = m_material->m_material_data;
    mat.m_diffuse = XMFLOAT4(1, 1, 1, 1);
    Renderer::SetMaterial(mat);

    Renderer::SetWorldMatrix(XMMatrixIdentity());

	// カメラの向き取得
    XMMATRIX view = cam_comp->GetViewMatrix();
    XMMATRIX inv_view = XMMatrixInverse(nullptr, view);

    XMFLOAT3 right_f{}, up_f{};
    XMStoreFloat3(&right_f, inv_view.r[0]);
    XMStoreFloat3(&up_f, inv_view.r[1]);

    VECTOR3 cam_right{ right_f.x, right_f.y, right_f.z };
    VECTOR3 cam_up{ up_f.x, up_f.y, up_f.z };
    cam_right.Normalize();
    cam_up.Normalize();

	// 頂点バッファ更新
    D3D11_MAPPED_SUBRESOURCE msr{};
    HRESULT hr = Renderer::GetDeviceContext()->Map(m_vertex_buffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
    assert(SUCCEEDED(hr));

	// 頂点データ書き込み
    VERTEX_3D* vtx = reinterpret_cast<VERTEX_3D*>(msr.pData);
    size_t w = 0;

	// 頂点設定用ラムダ
    auto SetV = [&](size_t idx, const VECTOR3& pos, const XMFLOAT4& col, float u, float v)
        {
            vtx[idx].Position = XMFLOAT3(pos.x, pos.y, pos.z);
            vtx[idx].Normal = XMFLOAT3(0, 0, 0);
            vtx[idx].Diffuse = col;
            vtx[idx].TexCoord = XMFLOAT2(u, v);
        };

    for (const PARTICLE& p : particles)
    {
        if (!p.m_alive)
		{// 生存していない
            continue;
        }

		// タイル計算
        const int tile_x = (p.m_tile_x > 0) ? p.m_tile_x : 1;
        const int tile_y = (p.m_tile_y > 0) ? p.m_tile_y : 1;
        const int total = tile_x * tile_y;

        int sprite_index = p.m_sprite_index;
        if (total > 0)
        {
            sprite_index %= total;
            if (sprite_index < 0)
            {
                sprite_index += total;
            }
        }

        const float tw = 1.0f / (float)tile_x;
        const float th = 1.0f / (float)tile_y;

        const int col = sprite_index % tile_x;
        const int row = sprite_index / tile_x;

        const float tx = col * tw;
        const float ty = row * th;

		// 頂点計算
        const float rad = XMConvertToRadians(p.m_rot);
        const float c = cosf(rad);
        const float s = sinf(rad);

        VECTOR3 r2 = cam_right * c + cam_up * (-s);
        VECTOR3 u2 = cam_right * s + cam_up * (c);

        const float hx = p.m_sca.x * 0.5f;
        const float hy = p.m_sca.y * 0.5f;

        const VECTOR3 center = p.m_pos;

        const VECTOR3 bl = center + (r2 * -hx) + (u2 * -hy);
        const VECTOR3 tl = center + (r2 * -hx) + (u2 * hy);
        const VECTOR3 br = center + (r2 * hx) + (u2 * -hy);
        const VECTOR3 tr = center + (r2 * hx) + (u2 * hy);

        const XMFLOAT4 col4 = p.m_color;

        SetV(w + 0, bl, col4, tx, ty + th);
        SetV(w + 1, tl, col4, tx, ty);
        SetV(w + 2, br, col4, tx + tw, ty + th);

        SetV(w + 3, br, col4, tx + tw, ty + th);
        SetV(w + 4, tl, col4, tx, ty);
        SetV(w + 5, tr, col4, tx + tw, ty);

        w += 6;
    }

    Renderer::GetDeviceContext()->Unmap(m_vertex_buffer.get(), 0);

    Renderer::GetDeviceContext()->Draw((UINT)w, 0);
}