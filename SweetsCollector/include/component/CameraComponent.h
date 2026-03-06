//-----------------------------------------------
// CameraComponent.h
// 制作日：2025/11/12
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "component/Component.h"

class RENDER_TARGET;

class CAMERA_COMPONENT : public COMPONENT
{
public:
	enum class PROJECTION_TYPE
	{
		PERSPECTIVE,
		ORTHOGRAPHIC
	};

	enum class CLEAR_FLAGS
	{
		SOLID_COLOR,
		DEPTH_ONLY,
		NOTHING
	};

private:
	RENDER_TARGET* m_render_target = nullptr;

	PROJECTION_TYPE m_projection_type = PROJECTION_TYPE::PERSPECTIVE;

	float m_fov_y_radians = 0.0f;
	float m_orotho_size = 10.0f;

	float m_near_clip = 0.1f;
	float m_far_clip = 1000.0f;

	CLEAR_FLAGS m_clear_flags = CLEAR_FLAGS::SOLID_COLOR;
	XMFLOAT4 m_clear_color = { 0.0f, 0.0f, 0.0f, 1.0f };

	uint32_t m_culling_mask = 0xFFFFFFFu;

	// 正規化ビューポート
	float m_viewport_x = 0.0f;
	float m_viewport_y = 0.0f;
	float m_viewport_width = 1.0f;
	float m_viewport_height = 1.0f;

	// キャッシュ行列
	XMMATRIX m_view_matrix;
	XMMATRIX m_projection_matrix;

	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

public:
	~CAMERA_COMPONENT() override = default;
	void Init() override;
	void Uninit() override;
	void Update() override;

	// セッター・ゲッター
	void SetProjectionType(PROJECTION_TYPE type) { m_projection_type = type; }
	PROJECTION_TYPE GetProjectionType() const { return m_projection_type; }

	void SetPerspective(float fov_y_radians, float near_clip, float far_clip)
	{
		m_projection_type = PROJECTION_TYPE::PERSPECTIVE;
		m_fov_y_radians = fov_y_radians;
		m_near_clip = near_clip;
		m_far_clip = far_clip;
	}
	void SetOrthographic(float ortho_size, float near_clip, float far_clip)
	{
		m_projection_type = PROJECTION_TYPE::ORTHOGRAPHIC;
		m_orotho_size = ortho_size;
		m_near_clip = near_clip;
		m_far_clip = far_clip;
	}

	void SetClearFlags(CLEAR_FLAGS flags) { m_clear_flags = flags; }
	CLEAR_FLAGS GetClearFlags() const { return m_clear_flags; }

	void SetClearColor(const XMFLOAT4& color) { m_clear_color = color; }
	XMFLOAT4 GetClearColor() const { return m_clear_color; }

	void SetCullingMask(uint32_t mask) { m_culling_mask = mask; }
	uint32_t GetCullingMask() const { return m_culling_mask; }

	void SetViewport(float x, float y, float width, float height)
	{
		m_viewport_x = x;
		m_viewport_y = y;
		m_viewport_width = width;
		m_viewport_height = height;
	}
	void GetViewport(float& x, float& y, float& width, float& height) const
	{
		x = m_viewport_x;
		y = m_viewport_y;
		width = m_viewport_width;
		height = m_viewport_height;
	}

	XMMATRIX GetViewMatrix() const { return m_view_matrix; }
	XMMATRIX GetProjectionMatrix() const { return m_projection_matrix; }

	void SetRenderTarget(RENDER_TARGET* render_target) { m_render_target = render_target; }
	RENDER_TARGET* GetRenderTarget() const { return m_render_target; }

	CAMERA_PASS_DESC ToPassDesc() const;

	CLEAR_FLAGS clear_flags = CLEAR_FLAGS::SOLID_COLOR;
	XMFLOAT4 clear_color = { 0.0f, 0.0f, 0.0f, 1.0f };
};
