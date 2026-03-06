//-----------------------------------------------
// Tree.h
// 制作日：2025/12/17
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "entity/GameObject.h"

class TREE : public GAME_OBJECT
{
	public:
	void Init() override;
	void Draw() override;
};