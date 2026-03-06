//-----------------------------------------------
// RotationObjComponent.cpp
// 制作日：2025/12/19
// 制作者：安田晴人
//-----------------------------------------------
#include "component/RotationObjComponent.h"
#include "Main.h"
#include "renderer/Renderer.h"
#include "entity/GameObject.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void ROTATION_OBJ_COMPONENT::Init()
{
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void ROTATION_OBJ_COMPONENT::Update()
{
	if (m_p_owner)
	{
		VECTOR3 rot = m_p_owner->GetTransform()->GetRot().ToEuler();
		rot += m_rotation_speed;
		m_p_owner->GetTransform()->SetRot(QUATERNION::FromEuler(rot));
	}
}