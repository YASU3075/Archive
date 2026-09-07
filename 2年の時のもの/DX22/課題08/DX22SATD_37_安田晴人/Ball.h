//-----------------------------------------------
// Ball.h
// 制作日：2024/11/20
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "main.h"
#include "renderer.h"
#include "GameObject3D.h"

class BALL : public GAME_OBJECT_3D
{
public:
	BALL() = default;
	BALL(GAME* p_game, std::string tag, XMFLOAT3 pos, XMFLOAT3 vec, XMFLOAT3 rot, XMFLOAT3 sca, XMFLOAT4 color, float rad) : GAME_OBJECT_3D(p_game, tag, pos, vec, rot, sca, color, rad) {}
	~BALL() {}
	
	ID3D11ShaderResourceView* texture;	//テクスチャー

	void Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
};
