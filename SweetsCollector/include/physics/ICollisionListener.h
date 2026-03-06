//-----------------------------------------------
// ICollisionListener.h
// 制作日：2025/08/11
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

class COLLIDER_3D;
class I_COLLISION_LISTENER
{
public:
	virtual void OnCollisionEnter(COLLIDER_3D* other) {};
	virtual void OnCollisionStay(COLLIDER_3D* other) {};
	virtual void OnCollisionExit(COLLIDER_3D* other) {};
};