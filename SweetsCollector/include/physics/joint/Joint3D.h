//-----------------------------------------------
// Joint3D.h
// 制作日：2026/02/26
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "math/Vector3.h"
#include "math/Matrix3x3.h"

class RIGID_BODY;

class JOINT_3D
{
public:
	virtual ~JOINT_3D() = default;

	virtual void PreStep(float dt) = 0;	// ステップ前の処理
	virtual void SolveVelocity(float dt) = 0;	// 速度の解決
	virtual void SolvePosition(float dt) = 0;	// 位置の解決

	virtual RIGID_BODY* GetBodyA() const = 0;	// ジョイントの剛体Aを取得
	virtual RIGID_BODY* GetBodyB() const = 0;	// ジョイントの剛体Bを取得
};
