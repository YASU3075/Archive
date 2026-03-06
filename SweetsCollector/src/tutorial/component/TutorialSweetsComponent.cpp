//-----------------------------------------------
// TutorialSweetsComponent.cpp
// 制作日：2026/02/11
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "tutorial/component/TutorialSweetsComponent.h"
#include "physics/Collider3D.h"

#include "object/Player.h"
#include "tutorial/object/TutorialManager.h"
#include "tutorial/component/TutorialManagerComponent.h"

//-----------------------------------------------
// 衝突開始時の処理
//-----------------------------------------------
void TUTORIAL_SWEETS_COMPONENT::OnCollisionEnter(COLLIDER_3D* other)
{
	PLAYER* player = dynamic_cast<PLAYER*>(other->GetOwner());
	if (player)
	{// プレイヤーと衝突した
		TUTORIAL_MANAGER* manager = Manager::GetScene()->GetGameObject<TUTORIAL_MANAGER>();

		if (manager)
		{
			TUTORIAL_MANAGER_COMPONENT* component = manager->GetComponent<TUTORIAL_MANAGER_COMPONENT>();
			if (component)
			{
				component->OnMoveCompleted();
			}
		}

	}
}