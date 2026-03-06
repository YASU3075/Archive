//-----------------------------------------------
// AutoDestroy.h
// 制作日：2025/07/29
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "Component.h"
#include "physics/ICollisionListener.h"

class AUTO_DESTROY : public COMPONENT, public I_COLLISION_LISTENER
{
private:
	bool m_is_start = false;	// カウント開始フラグ
	float m_life_time = 0.0f;		// 生存時間
public:
	void Init() override;
	void Update() override;

	void SetLifeTime(float life_time) { m_life_time = life_time; }
	float GetLifeTime() const { return m_life_time; }

	void StartCount() { m_is_start = true; }

	void OnCollisionEnter(COLLIDER_3D* other) override;
};