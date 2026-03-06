//-----------------------------------------------
// Enemy.cpp
// êßçÏì˙ÅF2025/06/04
// êßçÏé“ÅFà¿ìcê∞êl
//-----------------------------------------------
#include "Main.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "renderer/Renderer.h"
#include "entity/GameObject.h"
#include "renderer/ModelRenderer.h"
#include "object/Enemy.h"
#include "object/Player.h"

#include "physics/RigidBody.h"
#include "physics/BoxCollider3D.h"
#include "component/renderer/ModelRendererComponent.h"

#include "game/component/EnemyMoveComponent.h"

//-----------------------------------------------
// èâä˙âªèàóù
//-----------------------------------------------
void ENEMY::Init()
{
	m_transform.SetPos(VECTOR3(0.0f, 0.0f, 0.0f));
	m_transform.SetSca(VECTOR3(1.0f, 1.0f, 1.0f));

	AddComponent<BOX_COLLIDER_3D>();
	RIGID_BODY* rb = AddComponent<RIGID_BODY>();
	rb->SetMass(0.5f);
	rb->SetDynamicFriction(0.4f); // ìÆñÄéCåWêîÇê›íË
	rb->SetStaticFriction(0.8f);  // ê√é~ñÄéCåWêîÇê›íË
	rb->SetRestitution(0.5f); // îΩî≠åWêîÇê›íË

	rb->AddConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_ROT_X);
	rb->AddConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_ROT_Y);
	rb->AddConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_ROT_Z);
	
	MODEL_RENDERER_COMPONENT* model_renderer = AddComponent<MODEL_RENDERER_COMPONENT>();
	//model_renderer->SetModel("asset\\model\\germ_slime.mtl");
	model_renderer->SetModel("asset\\model\\germ_slime.obj");
	model_renderer->SetLocalSca(VECTOR3(1.0f, 1.0f, 1.0f));

	ENEMY_MOVE_COMPONENT* move_comp = AddComponent<ENEMY_MOVE_COMPONENT>();
}