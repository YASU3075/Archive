//-----------------------------------------------
// SkyDome.h
// 制作日：2025/09/14
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "entity/GameObject.h"

class SKY_DOME : public GAME_OBJECT
{
private:
public:
	// 初期化
	void Init() override;
	// 描画
	void Draw() override;
};
