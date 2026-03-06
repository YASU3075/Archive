//-----------------------------------------------
// DirectionalLight.h
// 制作日：2025/11/27
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "entity/GameObject.h"

class DIRECTIONAL_LIGHT : public GAME_OBJECT
{
private:
	float m_intensity = 0.5f;       // 光の強さ
public:
	DIRECTIONAL_LIGHT() = default;
	// 初期化
	void Init() override;
	// 光の強さ設定
	void SetIntensity(float intensity) { m_intensity = intensity; }
	// 光の強さ取得
	float GetIntensity() const { return m_intensity; }
};