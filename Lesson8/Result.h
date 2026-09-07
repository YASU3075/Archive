//-----------------------------------------------
// Result.h
// 制作日：2025/08/07
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "Scene.h"

class Result : public Scene
{
private:
	float m_time; // 結果画面の時間管理用変数
	int m_score; // スコア管理用変数
public:
	Result() : m_time(0.0f), m_score(0) {} // デフォルトコンストラクタ
	Result(int score) : m_time(0.0f), m_score(score) {} // パラメータ付きコンストラクタ

	virtual ~Result() override = default; // デストラクタ

	// シーンの更新処理
	virtual void Update() override;

	// シーンの描画処理
	virtual void Draw() override;

	// 結果画面の時間を取得
	float GetTime() const { return m_time; }
};