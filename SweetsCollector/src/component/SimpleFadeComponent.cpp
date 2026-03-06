//-----------------------------------------------
// SimpleFadeComponent.cpp
// 制作日：2025/09/14
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "scene/Manager.h"
#include "scene/Scene.h"

#include "scene/Title.h"
#include "scene/Tutorial.h"
#include "scene/Game.h"
#include "scene/Result.h"

#include "component/SimpleFadeComponent.h"
#include "component/renderer/SpriteRendererComponent.h"

#include "EngineTime.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void SIMPLE_FADE_COMPONENT::Init()
{
	m_fade_state = FADE_STATE::NONE;
	m_fade_amount = 0.0f;
	m_fade_mode = FADE_MODE::FADE;
	m_fade_time = 1.0f;
	m_time = 0.0f;
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void SIMPLE_FADE_COMPONENT::Update()
{
	if (m_fade_state == FADE_STATE::FADING_IN)
	{
		m_time += TIME::UnscaledDeltaTime();
		m_fade_amount = 1.0f - (m_time / m_fade_time);
		if (m_fade_amount < 0.0f)
		{
			m_fade_amount = 0.0f;
			m_fade_state = FADE_STATE::NONE;
			m_is_fading = false;
		}
	}

	else if (m_fade_state == FADE_STATE::FADING_OUT)
	{
		m_time += TIME::UnscaledDeltaTime();
		m_fade_amount = m_time / m_fade_time;
		if (m_fade_amount > 1.0f)
		{
			m_fade_amount = 1.0f;
			m_fade_state = FADE_STATE::NONE;
			m_is_fading = false;
			// シーン切り替え
			if (m_current_scene == CURRENT_SCENE::TITLE)
			{
				Manager::SetScene<TUTORIAL>();
			}
			else if (m_current_scene == CURRENT_SCENE::TUTORIAL)
			{
				Manager::SetScene<GAME>();
			}
			else if (m_current_scene == CURRENT_SCENE::GAME)
			{
				Manager::SetScene<RESULT>();
			}
			else if (m_current_scene == CURRENT_SCENE::RESULT)
			{
				Manager::SetScene<TITLE>();
			}
		}
	}
	
	POST_EFFECT_PARAMS post_effect_params = {};
	post_effect_params.param0.x = m_fade_amount; // 進捗
	post_effect_params.param0.y = 0.0f; // ぼかし幅
	post_effect_params.param0.z = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT); // 解像度

	switch (m_fade_mode)
	{
	case FADE_MODE::FADE:
		post_effect_params.param0.w = static_cast<float>(POST_EFFECT_MODE::POST_EFFECT_MODE_FADE);
		break;
	case FADE_MODE::IRIS:
		post_effect_params.param0.w = static_cast<float>(POST_EFFECT_MODE::POST_EFFECT_MODE_IRIS);
		break;
	default:
		break;
	}

	post_effect_params.param1.x = 0.5f; // 中心X
	post_effect_params.param1.y = 0.5f; // 中心Y

	post_effect_params.color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f); // 色

	Renderer::SetPostEffectParams(post_effect_params);
}

//-----------------------------------------------
// フェードイン開始
//-----------------------------------------------
void SIMPLE_FADE_COMPONENT::StartFadeIn()
{
	if (m_fade_state == FADE_STATE::FADING_OUT || m_fade_state == FADE_STATE::FADING_IN)
	{
		return;
	}
	m_fade_state = FADE_STATE::FADING_IN;
	m_fade_amount = 1.0f;
	m_time = 0.0f;
	m_is_fading = true;
}

//-----------------------------------------------
// フェードアウト開始
//-----------------------------------------------
void SIMPLE_FADE_COMPONENT::StartFadeOut()
{
	if (m_fade_state == FADE_STATE::FADING_OUT || m_fade_state == FADE_STATE::FADING_IN)
	{
		return;
	}
	m_fade_state = FADE_STATE::FADING_OUT;
	m_fade_amount = 0.0f;
	m_time = 0.0f;
	m_is_fading = true;
}