//-----------------------------------------------
// GameObject.h
// 制作日：2025/08/05
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include <stdio.h>

#include "Vector2.h"

class GameObject
{
protected:
	Vector2 m_last_position; // 前回の位置
	Vector2 m_position; // 位置
	char m_symbol; // 表示するシンボル
	int m_layer;
	bool m_destroy = false; // オブジェクトが破棄されるかどうか

	class DrawManager* m_p_draw_manager;

public:
	GameObject() : m_position(0, 0), m_symbol('@'), m_p_draw_manager(nullptr), m_layer(0) {} // コンストラクタ

	GameObject(const Vector2& position, char symbol)
		: m_position(position), m_symbol(symbol), m_p_draw_manager(nullptr), m_layer(0) {} // コンストラクタ

	virtual ~GameObject() {} // 仮想デストラクタ

	// 位置を取得
	Vector2 GetPosition() const { return m_position; }
	// 前回の位置を取得
	Vector2 GetLastPosition() const { return m_last_position; }

	// シンボルを取得
	char GetSymbol() const { return m_symbol; }

	// オブジェクトが破棄されるかどうかを取得
	bool IsDestroyed() const { return m_destroy; }

	// 位置を設定
	void SetPosition(const Vector2& position) { m_position = position; }

	// シンボルを設定
	void SetSymbol(char symbol) { m_symbol = symbol; }

	// レイヤーを設定
	void SetLayer(int layer) { m_layer = layer; }

	// 描画マネージャーを設定
	void SetDrawManager(class DrawManager* draw_manager) { m_p_draw_manager = draw_manager; } // 描画マネージャーを設定

	// オブジェクトを破棄する
	void Destroy() { m_destroy = true; }

	// オブジェクトの更新
	virtual void Update() = 0;

	// オブジェクトの描画
	virtual void Draw() = 0;
};