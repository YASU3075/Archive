//-----------------------------------------------
// TutorialSweetsComponent.h
// 制作日：2026/02/11
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "component/Component.h"
#include "physics/ICollisionListener.h"

class TUTORIAL_SWEETS_COMPONENT : public COMPONENT, public I_COLLISION_LISTENER
{
public:
	void OnCollisionEnter(COLLIDER_3D* other) override;
};