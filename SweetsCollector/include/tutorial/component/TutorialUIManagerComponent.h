//-----------------------------------------------
// TutorialUIManagerComponent.h
// êßçÏì˙ÅF2026/02/11
// êßçÏé“ÅFà¿ìcê∞êl
//-----------------------------------------------
#pragma once

#include <unordered_map>
#include "component/Component.h"
#include "tutorial/TutorialState.h"
#include "math/Vector3.h"

class MATERIAL;

struct STEP_UI_PARAM
{
	MATERIAL* material = nullptr;
    VECTOR3 in_pos;
	VECTOR3 show_pos;
	VECTOR3 out_pos;

    float in_time = 0.5f;
    float out_time = 0.25f;

	bool pulse = false;         // ï\é¶íÜÇÃägëÂèkè¨
	float pulse_scale = 1.05f;  // ägëÂó¶
	float pulse_period = 0.8f;  // ïbêî
};

class TUTORIAL_UI_MANAGER_COMPONENT : public COMPONENT
{
private:
	class UI_ANIMATOR_COMPONENT* m_ui_animator_component = nullptr;
	class SPRITE_RENDERER_COMPONENT* m_sprite_renderer_component = nullptr;

	std::unordered_map<TUTORIAL_STATE, STEP_UI_PARAM> m_params;

	bool m_visible = false;
	bool m_pulse = false;
	float m_time = 0.0f;

	VECTOR3 m_original_scale = VECTOR3(1.0f, 1.0f, 1.0f);

	void ApplyParam(const STEP_UI_PARAM& param);
	const STEP_UI_PARAM& GetParam(TUTORIAL_STATE state) const;

public:
	void Init() override;
	void Update() override;

	void ShowStep(TUTORIAL_STATE state);
	void HideStep(bool immediate = false);

	void PlaySuccessPulse();
};