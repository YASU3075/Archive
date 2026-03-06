//-----------------------------------------------
// Sweets.h
// 制作日：2026/01/01
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "entity/GameObject.h"
#include "game/SweetsData.h"

class SWEETS : public GAME_OBJECT
{
private:
public:
	void Init() override;

	void SetDataByType(SWEETS_TYPE type);
};
