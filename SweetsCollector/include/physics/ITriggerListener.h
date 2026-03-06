//-----------------------------------------------
// ITriggerListener.h
// 制作日：2025/08/11
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

class COLLIDER_3D;
class I_TRIGGER_LISTENER
{
public:
	virtual void OnTriggerEnter(COLLIDER_3D* other);
	virtual void OnTriggerExit(COLLIDER_3D* other);
	virtual void OnTriggerStay(COLLIDER_3D* other);
};