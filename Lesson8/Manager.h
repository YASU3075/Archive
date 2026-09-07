//-----------------------------------------------
// Manager.h
// 制作日：2025/08/05
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <stdio.h>
#include "Scene.h"

class Manager
{
private:
	static Manager* m_instance; // シングルトンインスタンス

	bool m_is_quit; // 終了フラグ

	Scene* m_current_scene; // 現在のシーン
	Scene* m_next_scene; // 次のシーン
public:
	Manager() : m_current_scene(nullptr), m_next_scene(nullptr), m_is_quit(false) {} // デフォルトコンストラクタ
	~Manager() = default; // デストラクタ

	// シングルトンインスタンスを取得
	static Manager* GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new Manager();
		}
		return m_instance;
	}

	// 現在のシーンを設定
	void SetCurrentScene(Scene* scene) { m_current_scene = scene; }

	// 次のシーンを設定
	void SetNextScene(Scene* scene) { m_next_scene = scene; }

	// 現在のシーンを更新
	void UpdateCurrentScene()
	{
		if (m_current_scene)
			m_current_scene->Update();
	}

	// 現在のシーンを描画
	void DrawCurrentScene()
	{
		if (m_current_scene)
			m_current_scene->Draw();
	}

	// シーンの切り替え処理
	void SwitchScene()
	{
		if (m_next_scene)
		{
			printf("\x1b[2J");	// 画面をクリア

			delete m_current_scene; // 現在のシーンを削除
			m_current_scene = m_next_scene; // 次のシーンを現在のシーンに設定
			m_next_scene = nullptr; // 次のシーンをリセット
		}
	}

	// 終了フラグを設定
	void SetQuit(bool is_quit) { m_is_quit = is_quit; }

	// 終了フラグを取得
	bool IsQuit() const { return m_is_quit; }
};