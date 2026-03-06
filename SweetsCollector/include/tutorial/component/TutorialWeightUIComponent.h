//-----------------------------------------------
// TutorialWeightUIComponent.h
// 制作日：206/02/12
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "component/Component.h"

class TUTORIAL_WEIGHT_UI_COMPONENT : public COMPONENT
{
private:
	float m_timer = 0.0f;
	const float DISPLAY_TIME = 3.0f;

public:
	void Init() override;
	void Update() override;
};