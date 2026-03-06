//-----------------------------------------------
// LightComponent.cpp
// 制作日：2025/11/19
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/LightManager.h"
#include "component/renderer/LightComponent.h"
#include "entity/GameObject.h"


//-----------------------------------------------
// 終了処理
//-----------------------------------------------
void LIGHT_COMPONENT::Uninit()
{
	LIGHT_MANAGER::GetInstance().Unregister(this);
}

//-----------------------------------------------
// ワールド座標取得
//-----------------------------------------------
VECTOR3 LIGHT_COMPONENT::GetWorldPosition() const
{
	if (auto owner = GetOwner())
	{
		return owner->GetTransform()->GetPos();
	}
	return VECTOR3(0.0f, 0.0f, 0.0f);
}

//-----------------------------------------------
// ワールド方向取得
//-----------------------------------------------
VECTOR3 LIGHT_COMPONENT::GetWorldDirection() const
{
	if (auto owner = GetOwner())
	{
		return owner->GetForward();
	}
	return VECTOR3(0.0f, 0.0f, 1.0f);
}