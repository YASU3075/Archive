//-----------------------------------------------
// UIAnimatorComponent.h
// 制作日：2026/02/09
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include <deque>
#include <functional>

#include "component/Component.h"
#include "math/Vector3.h"
#include "math/Easing.h"


class UI_ANIMATOR_COMPONENT : public COMPONENT
{
private:

	struct ACTION
	{
		enum class TYPE
		{
			MOVE,
			SCALE,
			FADE,
			WAIT,
			MOVE_FADE
		};

		TYPE type;

		// 共通
		float t = 0.0f;
		float duration = 0.0f;
		EASE_TYPE ease_type;

		// move, scale
		VECTOR3 from_vec;
		VECTOR3 to_vec;

		// fade
		float from_alpha = 0.0f;
		float to_alpha = 0.0f;

		// move_fade
		EASE_TYPE ease_type2;
		VECTOR3 from_pos;
		VECTOR3 to_pos;
		float from_a = 0.0f;
		float to_a = 0.0f;
	};

	class SPRITE_RENDERER_COMPONENT* m_p_sprite_renderer = nullptr;

	std::deque<ACTION> m_actions;
	bool m_is_playing = false;
	ACTION m_current_action{};

	void StartAction(ACTION& action);
	bool StepAction(ACTION& action, float dt);

public:
	void Init() override;
	void Update() override;

	void SetTargetSpriteRenderer(class SPRITE_RENDERER_COMPONENT* p_sprite_renderer) { m_p_sprite_renderer = p_sprite_renderer; }

	void KillAll();
	bool IsPlaying() const { return !m_actions.empty() || m_is_playing;}

	// 基本アクション
	void MoveTo(const VECTOR3& to, float time, EASE_TYPE ease = EASE_TYPE::EASE_OUT_CUBIC);
	void ScaleTo(const VECTOR3& to, float time, EASE_TYPE ease = EASE_TYPE::EASE_OUT_CUBIC);
	void FadeTo(float to_alpha, float time, EASE_TYPE ease = EASE_TYPE::EASE_OUT_CUBIC);
	void Wait(float time);

	void MoveAndFadeTo(const VECTOR3& to_pos, float to_alpha, float time, EASE_TYPE ease_pos = EASE_TYPE::EASE_OUT_CUBIC, EASE_TYPE ease_alpha = EASE_TYPE::EASE_OUT_CUBIC);
};