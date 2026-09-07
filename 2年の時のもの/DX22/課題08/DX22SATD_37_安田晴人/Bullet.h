//-----------------------------------------------
// Bullet.h
// 制作日：2025/01/06
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "GameObject3D.h"
#include "renderer.h"
#include "model.h"

class BULLET : public GAME_OBJECT_3D
{
private:
	MODEL* m_p_model = nullptr;

public:
	BULLET() = default;
	BULLET(GAME* p_game, XMFLOAT3 pos, XMFLOAT3 vec, XMFLOAT3 rot, XMFLOAT3 sca, XMFLOAT4 color, float rad) : GAME_OBJECT_3D(p_game, "BULLET", pos, vec, rot, sca, color, rad) {}
	~BULLET() {}

	void Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
};
