//-----------------------------------------------
// ConeShape.h
// 制作日：2025/07/01
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "EmitterShape.h"

class CONE_SHAPE : public EMITTER_SHAPE
{
public:
	VECTOR3 m_direction = { 0.0f, 1.0f, 0.0f };			//軸
	float m_angle_deg = 30.0f;							//開口角（degree）
	float m_radius = 1.0f;								//半径
	float m_length = 1.0f;								//長さ

	VECTOR3 GetPositionOffset(std::mt19937& rng) const override
	{
		//軸方向に沿ったランダムな高さ
		float h = std::uniform_real_distribution<float>(0, m_length)(rng);
		//断面円のランダムな点
		float r = (h / m_length) * m_radius;
		float theta = std::uniform_real_distribution<float>(0, 2 * 3.141592f)(rng);
		float x = cosf(theta) * r;
		float z = sinf(theta) * r;

		return { x, h, z };
	}

	VECTOR3 GetDirection(std::mt19937& rng) const override
	{
		//コーン内部方向をランダムに
		float cos = cosf(m_angle_deg * 3.141592f / 180.0f);
		std::uniform_real_distribution<float> du(cos, 1.0f);
		std::uniform_real_distribution<float> dt(0, 2 * 3.141592f);

		float z = du(rng);
		float phi = dt(rng);
		float r = sqrtf(1 - z * z);
		return VECTOR3(r * cosf(phi), z, r * sinf(phi));

	}

	std::unique_ptr<EMITTER_SHAPE> Clone() const override
	{
		return std::make_unique<CONE_SHAPE>(*this);
	}

};