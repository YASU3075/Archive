//-----------------------------------------------
// PointLight.h
// 制作日：2025/11/19
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "entity/GameObject.h"

class POINT_LIGHT : public GAME_OBJECT
{
private:
	float m_range = 10.0f;          // 光の届く範囲
	float m_intensity = 1.0f;       // 光の強さ
	XMFLOAT3 m_color = XMFLOAT3(1.0f, 1.0f, 1.0f); // 光の色

public:
	POINT_LIGHT() = default;

	// 初期化
	void Init() override;
	// 光の届く範囲設定
	void SetRange(float range) { m_range = range; }
	// 光の届く範囲取得
	float GetRange() const { return m_range; }
	// 光の強さ設定
	void SetIntensity(float intensity) { m_intensity = intensity; }
	// 光の強さ取得
	float GetIntensity() const { return m_intensity; }
	// 光の色設定
	void SetColor(const XMFLOAT3& color);
};