//-----------------------------------------------
// SimpleFadeComponent.h
// 制作日：2025/09/14
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "Component.h"

enum class FADE_MODE
{
	FADE,
	IRIS,
};

class SIMPLE_FADE_COMPONENT : public COMPONENT
{
public:
	enum class CURRENT_SCENE
	{
		TITLE,
		TUTORIAL,
		GAME,
		RESULT
	};


	// 初期化
	void Init() override;
	// 更新
	void Update() override;
	// フェードイン開始
	void StartFadeIn();
	// フェードアウト開始
	void StartFadeOut();

	void SetCurrentScene(CURRENT_SCENE scene) { m_current_scene = scene; }
	void SetFadeMode(FADE_MODE mode) { m_fade_mode = mode; }
	void SetFadeTime(float time) { m_fade_time = time; }

	bool IsFading() const { return m_is_fading; }
private:
	enum class FADE_STATE
	{
		NONE,
		FADING_IN,
		FADING_OUT
	};

	FADE_STATE m_fade_state = FADE_STATE::NONE;
	CURRENT_SCENE m_current_scene = CURRENT_SCENE::TITLE;
	FADE_MODE m_fade_mode = FADE_MODE::FADE;

	float m_fade_amount = 0.0f; // フェード量 (0.0f - 1.0f)
	float m_fade_time = 1.0f;   // フェード時間 (秒)
	float m_time = 0.0f;        // 経過時間
	bool m_is_fading = false;
};