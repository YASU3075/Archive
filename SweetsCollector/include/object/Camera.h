//-----------------------------------------------
// Camera.h
// 制作日：2025/05/14
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include "entity/GameObject.h"

class CAMERA : public GAME_OBJECT
{
private:
	XMMATRIX m_projection;
	XMMATRIX m_view;

	VECTOR3 m_target{0.0f, 0.0f, 0.0f};

public:
	void Init() override;
	void Draw() override;

	XMMATRIX GetViewMatrix(void) const { return m_view; }

	void SetTarget(const VECTOR3& target) { m_target = target; }
};