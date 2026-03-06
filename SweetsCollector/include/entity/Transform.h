//-----------------------------------------------
// Transform.h
// 制作日：2025/07/06
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "math/Matrix4x4.h"
#include "math/Quaternion.h"
#include "math/Vector3.h"

class TRANSFORM
{
private:
	VECTOR3 m_pos = VECTOR3(0.0f, 0.0f, 0.0f);			//位置
	QUATERNION m_rot = QUATERNION::Identity();		//回転
	VECTOR3 m_sca = VECTOR3(1.0f, 1.0f, 1.0f);		//拡大縮小

	VECTOR3 m_add_pos = VECTOR3(0.0f, 0.0f, 0.0f);		//追加位置
	QUATERNION m_add_rot = QUATERNION::Identity();	//追加回転

	TRANSFORM* m_parent = nullptr;	//親Transform

	mutable bool m_dirty = true;	//更新フラグ
	mutable MATRIX4X4 m_cashed_world_matrix;	//キャッシュされたワールド行列

public:

	//セッター・ゲッター
	VECTOR3 GetPos() const { return m_pos; }
	void SetPos(const VECTOR3& pos) { m_pos = pos; m_dirty = true; }

	void AddPos(const VECTOR3& add_pos) { m_add_pos += add_pos;  m_dirty = true; }
	VECTOR3 GetFinalPos() const { return m_pos + m_add_pos; }

	QUATERNION GetRot() const { return m_rot; }
	void SetRot(const QUATERNION& rot) { m_rot = rot;  m_dirty = true; }

	void AddRot(const QUATERNION& add_rot) { m_add_rot = m_add_rot * add_rot;  m_dirty = true; }
	QUATERNION GetFinalRot() const { return m_rot * m_add_rot; }

	MATRIX3x3 GetRotationMatrix() const { return m_rot.ToMatrix3x3(); }

	VECTOR3 GetSca() const { return m_sca; }
	void SetSca(const VECTOR3& scale) { m_sca = scale;  m_dirty = true; }

	TRANSFORM* GetParent() const { return m_parent; }
	void SetParent(TRANSFORM* parent) { m_parent = parent;  m_dirty = true; }

	//行列計算
	MATRIX4X4 GetWorldMatrix() const
	{
		UpdateWorldMatrix();
		return m_cashed_world_matrix;
	}

	VECTOR3 TransformDirection(const VECTOR3& direction) const
	{
		return GetWorldMatrix().MultiplyVector(direction);
	}

	VECTOR3 TransformPoint(const VECTOR3& point) const
	{
		return GetWorldMatrix().MultiplyPoint(point);
	}

	VECTOR3 InverseTransformPoint(const VECTOR3& point) const
	{
		// 1) 並進を打ち消す
		VECTOR3 p = point;
		VECTOR3 rel = p - m_pos;
		// 2) 回転を打ち消す（クォータニオンの共役で逆回転）
		VECTOR3 unrot = m_rot.Conjugate().Rotate(rel);
		// 3) スケールを打ち消す
		return VECTOR3(
			unrot.x / m_sca.x,
			unrot.y / m_sca.y,
			unrot.z / m_sca.z
		);
	}

	void UpdateWorldMatrix() const
	{
		if (m_dirty)
		{
			const VECTOR3 pos = GetFinalPos();
			const QUATERNION rot = GetFinalRot();

			MATRIX4X4 scale = MATRIX4X4::Scale(m_sca);
			MATRIX4X4 rotation = MATRIX4X4::Rotation(rot);
			MATRIX4X4 translation = MATRIX4X4::Translation(pos);
			MATRIX4X4 localToWorld = translation * rotation * scale;

			if (m_parent)
			{
				m_cashed_world_matrix = m_parent->GetWorldMatrix() * localToWorld;
			}
			else
			{
				m_cashed_world_matrix = localToWorld;
			}
			m_dirty = false;
		}
	}

	void ClearAdditive()
	{
		m_add_pos = VECTOR3(0.0f, 0.0f, 0.0f);
		m_add_rot = QUATERNION::Identity();
		m_dirty = true;
	}
};