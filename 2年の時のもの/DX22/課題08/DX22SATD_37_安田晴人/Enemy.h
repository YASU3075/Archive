//-----------------------------------------------
// Enemy.h
// 制作日：2025/01/08
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include "GameObject3D.h"



class ENEMY : public GAME_OBJECT_3D
{
private:
	
	ID3D11ShaderResourceView* m_texture;
	

public:
	ENEMY() = default;
	ENEMY(GAME* p_game, const std::string& tag, XMFLOAT3 pos, XMFLOAT3 vec, XMFLOAT3 rot, XMFLOAT3 sca, XMFLOAT4 color, float rad)
		: GAME_OBJECT_3D(p_game, tag, pos, vec, rot, sca, color, rad) {}
	~ENEMY() {}

	void Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
};
