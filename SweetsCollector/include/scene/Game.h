//-----------------------------------------------
// scene/Game.h
// 制作日：2025/06/25
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include "Scene.h"

class GAME : public SCENE
{
private:
	class Audio* m_bgm = nullptr;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
};