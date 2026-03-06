//-----------------------------------------------
// Component.h
// 制作日：2025/06/16
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

class COLLIDER_3D;
class COMPONENT
{
protected:
	class  GAME_OBJECT* m_p_owner = nullptr;		//親オブジェクト

public:
	COMPONENT() {}
	COMPONENT(GAME_OBJECT* p_object) { m_p_owner = p_object; }
	virtual ~COMPONENT() {}

	void SetOwner(GAME_OBJECT* p_owner) { m_p_owner = p_owner; }
	GAME_OBJECT* GetOwner(void) const { return m_p_owner; }

	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};

	virtual void OnAttach() {};
};