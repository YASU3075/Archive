//-----------------------------------------------
// EmitterShape.h
// 制作日：2025/06/30
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "math/Vector3.h"
#include <random>

class EMITTER_SHAPE
{
public:
	virtual ~EMITTER_SHAPE() = default;

	//発生位置オフセット
	virtual VECTOR3 GetPositionOffset(std::mt19937& rng) const = 0;

	//発生方向の単位ベクトル
	virtual VECTOR3 GetDirection(std::mt19937& rng) const = 0;

	//クローン
	virtual std::unique_ptr<EMITTER_SHAPE> Clone() const = 0;
};