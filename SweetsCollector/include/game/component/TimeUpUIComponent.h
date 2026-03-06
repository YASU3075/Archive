//-----------------------------------------------
// TimeUpUIComponent.h
// 制作日：2026/01/19
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "math/Vector3.h"
#include "component/Component.h"

class SPRITE_RENDERER_COMPONENT;

class TIME_UP_UI_COMPONENT : public COMPONENT
{
private:
	bool m_is_active = false;
	float m_duration = 1.0f;
	float m_time = 0.0f;

	VECTOR3 m_start_position = VECTOR3(0.0f, 0.0f, 0.0f);
	VECTOR3 m_target_position = VECTOR3(0.0f, 0.0f, 0.0f);
public:
	// 初期化
	void Init() override;
	// 更新
	void Update() override;

	// アクティブ設定
	void SetActive(bool is_active) { m_is_active = is_active; }

	// 表示時間設定
	void SetDuration(float duration) { m_duration = duration; }

	// 開始位置設定
	void SetStartPosition(const VECTOR3& position) { m_start_position = position; }
	// 目標位置設定
	void SetTargetPosition(const VECTOR3& position) { m_target_position = position; }
};
