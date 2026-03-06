//-----------------------------------------------
// SphereShape.h
// 制作日：2025/06/30
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "EmitterShape.h"

class SPHERE_SHAPE : public EMITTER_SHAPE
{
public:
	float m_radius = 1.0f;		//半径
	bool m_emit_shell = false;	//true:殻上 false:内部全体

	SPHERE_SHAPE(float radius = 1.0f, bool emit_shell = false)
		: m_radius(radius), m_emit_shell(emit_shell) {}

	VECTOR3 GetPositionOffset(std::mt19937& rng) const override
	{
		std::uniform_real_distribution<float> u(-1, 1);
		VECTOR3 v = {1.0f, 1.0f, 1.0f};
		while (v.LengthSquared() > 1.0f)
		{//球の範囲内に収まるまで
			v = { u(rng), u(rng), u(rng) };
		}
		v.Normalize();

		//内部全体だったらランダムなオフセットに
		float r = m_emit_shell ? 1.0f : std::cbrt(std::uniform_real_distribution<float>(0, 1)(rng));
		return v * (r * m_radius);
	}

	VECTOR3 GetDirection(std::mt19937& rng) const override
	{
		//中心から放射状
		VECTOR3 off = GetPositionOffset(rng);
		off.Normalize();
		return off;
	}

	std::unique_ptr<EMITTER_SHAPE> Clone() const override
	{
		return std::make_unique<SPHERE_SHAPE>(*this);
	}

};