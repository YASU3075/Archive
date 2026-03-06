//-----------------------------------------------
// PhysicsContactEvent3D.cpp
// 制作日：2026/01/15
// 制作者：安田晴人
//-----------------------------------------------
#include "physics/PhysicsContactEvent3D.h"

#include "Main.h"
#include "scene/Manager.h"
#include "renderer/Renderer.h"
#include "entity/GameObject.h"

#include "physics/ContactManager.h"
#include "physics/RigidBody.h"
#include "physics/Collider3D.h"

//-----------------------------------------------
// コンタクトイベントの発行
//-----------------------------------------------
void PHYSICS_CONTACT_EVENT_3D::Dispatch(const CONTACT_3D& contact, bool now, bool prev)
{
	// コンタクトから剛体を取得
	RIGID_BODY* rb_a = contact.rb_a;
	RIGID_BODY* rb_b = contact.rb_b;

	// 剛体からコライダーを取得
	COLLIDER_3D* col_a = (rb_a && rb_a->GetCollider()) ? rb_a->GetCollider() : nullptr;
	COLLIDER_3D* col_b = (rb_b && rb_b->GetCollider()) ? rb_b->GetCollider() : nullptr;

	// 剛体からゲームオブジェクトを取得
	GAME_OBJECT* obj_a = rb_a ? rb_a->GetOwner() : nullptr;
	GAME_OBJECT* obj_b = rb_b ? rb_b->GetOwner() : nullptr;

	if (!obj_a || !obj_b || !col_a || !col_b)
	{// オブジェクトまたはコライダーが存在しない場合は何もしない
		return;
	}

	if (now)
	{
		if (contact.is_trigger)
		{// トリガーの場合
			if (!prev)
			{// Enter
				obj_a->DispatchTriggerEnter(col_b);
				obj_b->DispatchTriggerEnter(col_a);
			}
			else
			{// Stay
				obj_a->DispatchTriggerStay(col_b);
				obj_b->DispatchTriggerStay(col_a);
			}
		}
		else
		{// 通常のコリジョンの場合
			if (!prev)
			{// Enter
				obj_a->DispatchCollisionEnter(col_b);
				obj_b->DispatchCollisionEnter(col_a);
			}
			else
			{// Stay
				obj_a->DispatchCollisionStay(col_b);
				obj_b->DispatchCollisionStay(col_a);
			}
		}
	}
	else
	{
		if (!prev)
		{// 既にコンタクトが無効の場合は何もしない
			return;
		}

		if (contact.is_trigger)
		{// トリガーの場合
			obj_a->DispatchTriggerExit(col_b);
			obj_b->DispatchTriggerExit(col_a);
		}
		else
		{// 通常のコリジョンの場合
			obj_a->DispatchCollisionExit(col_b);
			obj_b->DispatchCollisionExit(col_a);
		}
	}
}
