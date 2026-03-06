//-----------------------------------------------
// SweetsComponent.cpp
// 制作日：2025/12/28
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "scene/Scene.h"
#include "scene/Manager.h"
#include "game/component/SweetsComponent.h"
#include "physics/Collider3D.h"
#include "game/component/SweetsManagementComponent.h"

#include "object/Player.h"

//-----------------------------------------------
// 衝突開始時の処理
//-----------------------------------------------
void SWEETS_COMPONENT::OnCollisionEnter(COLLIDER_3D* other)
{
	PLAYER* p_player = dynamic_cast<PLAYER*>(other->GetOwner());
	if (p_player != nullptr)
	{
		SWEETS_MANAGEMENT_COMPONENT* p_sweets_management = p_player->GetComponent<SWEETS_MANAGEMENT_COMPONENT>();
		if (p_sweets_management != nullptr)
		{
			if (p_sweets_management->AddSweets(m_data))
			{
				// お菓子を追加できた場合、オブジェクトを破棄
				GetOwner()->SetDestroy(true);
			}
		}
	}

}