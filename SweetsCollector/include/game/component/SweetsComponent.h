//-----------------------------------------------
// SweetsComponent.h
// 制作日：2025/12/28
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "game/SweetsData.h"
#include "component/Component.h"
#include "physics/ICollisionListener.h"

class SWEETS_COMPONENT : public COMPONENT, public I_COLLISION_LISTENER
{
private:
	SWEETS_DATA m_data; // お菓子のデータ
public:
	void SetSweetsData(const SWEETS_DATA& data) { m_data = data; }
	SWEETS_DATA GetSweetsData() const { return m_data; }

	virtual void OnCollisionEnter(COLLIDER_3D* other) override;
};
