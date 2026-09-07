//-----------------------------------------------
// Scene.h
// 制作日：2025/08/05
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <vector>
#include "GameObject.h"

class Player;

class Scene
{
public:
	Scene() = default; // コンストラクタ
	virtual ~Scene() = default; // デストラクタ

	virtual void Update() = 0; // シーンの更新
	virtual void Draw() = 0; // シーンの描画
};