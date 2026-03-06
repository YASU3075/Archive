//-----------------------------------------------
// CameraComponent.cpp
// 制作日：2025/11/13
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "renderer/CameraSystem.h"
#include "component/CameraComponent.h"
#include "entity/GameObject.h"
#include "renderer/RenderTarget.h"

//-----------------------------------------------
// ビュー行列更新
//-----------------------------------------------
void CAMERA_COMPONENT::UpdateViewMatrix()
{
	TRANSFORM* transform = GetOwner()->GetTransform();

	VECTOR3 pos = transform->GetFinalPos();
	VECTOR3 forward = GetOwner()->GetForward();
	VECTOR3 up = GetOwner()->GetUp();

	VECTOR3 target = pos + forward;

	XMVECTOR eye = XMVectorSet(pos.x, pos.y, pos.z, 1.0f);
	XMVECTOR at = XMVectorSet(target.x, target.y, target.z, 1.0f);
	XMVECTOR upv = XMVectorSet(up.x, up.y, up.z, 0.0f);

	m_view_matrix = XMMatrixLookAtLH(eye, at, upv);

	m_viewport_x = 0.0f;
	m_viewport_y = 0.0f;
	m_viewport_width = static_cast<float>(SCREEN_WIDTH);
	m_viewport_height = static_cast<float>(SCREEN_HEIGHT);
}

//-----------------------------------------------
// 投影行列更新
//-----------------------------------------------
void CAMERA_COMPONENT::UpdateProjectionMatrix()
{
	// 画面アスペクト比
	float aspect = static_cast<float>(m_viewport_width) / static_cast<float>(m_viewport_height);

	// 投影行列計算
	if (m_projection_type == PROJECTION_TYPE::PERSPECTIVE)
	{// 透視投影
		m_projection_matrix = XMMatrixPerspectiveFovLH(m_fov_y_radians, aspect, m_near_clip, m_far_clip);
	}
	else if (m_projection_type == PROJECTION_TYPE::ORTHOGRAPHIC)
	{// 平行投影
		float ortho_width = m_orotho_size * aspect;
		float ortho_height = m_orotho_size;
		m_projection_matrix = XMMatrixOrthographicLH(ortho_width, ortho_height, m_near_clip, m_far_clip);
	}
}


//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void CAMERA_COMPONENT::Init()
{
	m_projection_type = PROJECTION_TYPE::PERSPECTIVE;
	m_fov_y_radians = XMConvertToRadians(90.0f);
	m_near_clip = 0.1f;
	m_far_clip = 1000.0f;

	CAMERA_SYSTEM::GetInstance().RegisterCamera(this);
}

//-----------------------------------------------
// 終了処理
//-----------------------------------------------
void CAMERA_COMPONENT::Uninit()
{
	CAMERA_SYSTEM::GetInstance().UnregisterCamera(this);
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void CAMERA_COMPONENT::Update()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

//-----------------------------------------------
// パス記述子変換
//-----------------------------------------------
CAMERA_PASS_DESC CAMERA_COMPONENT::ToPassDesc() const
{
	CAMERA_PASS_DESC desc = {};
	// レンダーターゲット設定
	desc.render_target = m_render_target;
	// ビューポート設定
	desc.viewport_rect.x = m_viewport_x;
	desc.viewport_rect.y = m_viewport_y;
	desc.viewport_rect.w = m_viewport_width;
	desc.viewport_rect.h = m_viewport_height;
	// クリア設定
	switch (m_clear_flags)
	{
	case CLEAR_FLAGS::SOLID_COLOR:
		desc.clear_params.clear_color = true;
		desc.clear_params.clear_depth = true;
		desc.clear_params.Color[0] = m_clear_color.x;
		desc.clear_params.Color[1] = m_clear_color.y;
		desc.clear_params.Color[2] = m_clear_color.z;
		desc.clear_params.Color[3] = m_clear_color.w;
		break;
	case CLEAR_FLAGS::DEPTH_ONLY:
		desc.clear_params.clear_color = false;
		desc.clear_params.clear_depth = true;
		break;
	case CLEAR_FLAGS::NOTHING:
		desc.clear_params.clear_color = false;
		desc.clear_params.clear_depth = false;
		break;
	}
	
	return desc;
}

