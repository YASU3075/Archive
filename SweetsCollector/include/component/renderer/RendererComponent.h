//-----------------------------------------------
// RendererComponent.h
// 制作日：2025/07/17
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "math/Vector3.h"
#include "math/Quaternion.h"
#include "component/Component.h"

enum class RENDER_PASS
{
	RENDER_PASS_OPAQUE,
	RENDER_PASS_TRANSPARENT,
	RENDER_PASS_PARTICLE,
	RENDER_PASS_UI,
};

class RENDERER_COMPONENT : public COMPONENT
{
protected:
	class MATERIAL* m_p_material;	

	RENDER_PASS m_render_pass = RENDER_PASS::RENDER_PASS_OPAQUE;
	int m_order_in_pass = 0;	// 描画順序
	bool m_depth_test = true;	// デプステスト有効化フラグ
	bool m_depth_write = true;	// デプスライト有効化フラグ

	VECTOR3 m_local_pos = VECTOR3(0.0f, 0.0f, 0.0f);
	QUATERNION m_local_rot = QUATERNION::Identity();
	VECTOR3 m_local_sca = VECTOR3(1.0f, 1.0f, 1.0f);

	bool m_enbale_uv_scroll = false;
	XMFLOAT2 m_uv_speed = XMFLOAT2(0.0f, 0.0f);
public:
	virtual void Init() override = 0;
	virtual void Uninit() override = 0;
	virtual void Update() override = 0;
	virtual void Draw() override = 0;

	virtual XMMATRIX BuildWorldMatrix() const = 0;
	virtual void BindGeometry() const = 0;
	virtual void DrawGeometyrOnly() const = 0;

	void SetRenderPass(RENDER_PASS pass) { m_render_pass = pass; }
	RENDER_PASS GetRenderPass() const { return m_render_pass; }

	void SetOrderInPass(int order) { m_order_in_pass = order; }
	int GetOrderInPass() const { return m_order_in_pass; }

	void SetDepthTest(bool enable) { m_depth_test = enable; }
	bool IsDepthTest() const { return m_depth_test; }

	void SetDepthWrite(bool enable) { m_depth_write = enable; }
	bool IsDepthWrite() const { return m_depth_write; }

	void SetMaterial(MATERIAL* p_material) { m_p_material = p_material; }
	MATERIAL* GetMaterial() const { return m_p_material; }

	void SetLocalPos(const VECTOR3& pos) { m_local_pos = pos; }
	VECTOR3 GetLocalPos() const { return m_local_pos; }

	void SetLocalRot(const QUATERNION& rot) { m_local_rot = rot; }
	QUATERNION GetLocalRot() const { return m_local_rot; }

	void SetLocalSca(const VECTOR3& scale) { m_local_sca = scale; }
	VECTOR3 GetLocalSca() const { return m_local_sca; }

	void SetUVScrollEnable(bool enable) { m_enbale_uv_scroll = enable; }
	bool IsUVScrollEnable() const { return m_enbale_uv_scroll; }

	void SetUVSpeed(const XMFLOAT2& speed) { m_uv_speed = speed; }
	XMFLOAT2 GetUVSpeed() const { return m_uv_speed; }
};