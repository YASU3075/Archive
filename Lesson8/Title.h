//-----------------------------------------------
// Title.h
// 制作日：2025/08/07
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "Scene.h"

class Title : public Scene
{
private:
	float m_time; // タイトル画面の時間管理用変数
public:
	Title() : m_time(0.0f) {} // デフォルトコンストラクタ

	virtual ~Title() override = default; // デストラクタ

	// シーンの更新処理
	virtual void Update() override;

	// シーンの描画処理
	virtual void Draw() override;

	// タイトル画面の時間を取得
	float GetTime() const { return m_time; }
};
