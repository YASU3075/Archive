//-----------------------------------------------
// ParticleModule.h
// 制作日：2025/07/02
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <memory>
#include "particle/core/Particle.h"

class PARTICLE_MODULE
{
public:
	virtual ~PARTICLE_MODULE() = default;

	//各パーティクルに対してマイフレーム適用する
	virtual void Apply(PARTICLE& particle, float deltaTime) = 0;

	virtual std::unique_ptr<PARTICLE_MODULE> Clone() const = 0; // モジュールのクローンを生成

	virtual bool IsEventModule() const
	{
		return false; // デフォルトではイベントモジュールではない
	}
};