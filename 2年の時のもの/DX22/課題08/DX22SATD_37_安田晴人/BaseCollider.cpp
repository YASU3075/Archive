//-----------------------------------------------
// BaseCollider.cpp
// 制作日：2024/12/11
// 制作者：安田晴人
//-----------------------------------------------
#include "BaseCollider.h"
#include "ColliderManager.h"

int BASE_COLLIDER::Register(COLLIDER_MANAGER* p_collider_manager)
{
	int ret = p_collider_manager->Register(this);
	return ret;
}