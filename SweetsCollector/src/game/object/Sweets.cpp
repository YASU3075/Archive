//-----------------------------------------------
// Sweets.cpp
// 制作日：2026/01/01
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "game/object/Sweets.h"
#include "game/component/SweetsComponent.h"

#include "physics/SphereCollider.h"
#include "physics/RigidBody.h"

#include "component/renderer/ModelRendererComponent.h"
#include "component/renderer/OutlineRendererComponent.h"
#include "renderer/MeshFactory.h"
#include "renderer/MaterialManager.h"

//------------------------------------------------
// 初期化処理
//------------------------------------------------
void SWEETS::Init()
{
	m_transform.SetSca(VECTOR3(1.0f, 1.0f, 1.0f));

	// モデルレンダラーを追加
	MODEL_RENDERER_COMPONENT* model = AddComponent<MODEL_RENDERER_COMPONENT>();

	OUTLINE_RENDERER_COMPONENT* outline_renderer = AddComponent<OUTLINE_RENDERER_COMPONENT>();
	outline_renderer->SetOutlineColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	outline_renderer->SetOutlineWidth(0.075f);
	
	AddComponent<SWEETS_COMPONENT>();

	SetDataByType(SWEETS_TYPE::SWEETS_TYPE_CUPCAKE);

	// 球コライダーを追加
	AddComponent<SPHERE_COLLIDER>();

	// リジッドボディを追加
	RIGID_BODY* rigid_body = AddComponent<RIGID_BODY>();
	rigid_body->SetType(RIGID_BODY_TYPE::DYNAMIC);
	rigid_body->SetMass(1.0f);
	rigid_body->WakeUp();
}

//-----------------------------------------------
// お菓子の種類に応じたデータの設定
//-----------------------------------------------
void SWEETS::SetDataByType(SWEETS_TYPE type)
{
	SWEETS_COMPONENT* sweets_component = GetComponent<SWEETS_COMPONENT>();
	MODEL_RENDERER_COMPONENT* model_renderer = GetComponent<MODEL_RENDERER_COMPONENT>();
	
	if (!sweets_component || !model_renderer)
	{
		return;
	}

	SWEETS_DATA data;
	data.type = type;

	switch (data.type)
	{
	case SWEETS_TYPE::SWEETS_TYPE_CAKE:
		data.weight = 10;
		model_renderer->SetModel("asset\\model\\cake.obj");
		model_renderer->SetLocalPos(VECTOR3(0.0f, -0.7f, 0.0f));
		model_renderer->SetLocalSca(VECTOR3(3.0f, 3.0f, 3.0f));
		break;
	case SWEETS_TYPE::SWEETS_TYPE_CHOCOLATE:
		data.weight = 1;
		model_renderer->SetModel("asset\\model\\chocolate-wrapper.obj");
		model_renderer->SetLocalPos(VECTOR3(0.0f, -0.7f, 0.0f));
		model_renderer->SetLocalSca(VECTOR3(7.0f, 7.0f, 7.0f));
		break;
	case SWEETS_TYPE::SWEETS_TYPE_COOKIE:
		data.weight = 3;
		model_renderer->SetModel("asset\\model\\cookie.obj");
		model_renderer->SetLocalPos(VECTOR3(0.0f, -0.7f, 0.0f));
		model_renderer->SetLocalSca(VECTOR3(10.0f, 10.0f, 10.0f));
		break;
	case SWEETS_TYPE::SWEETS_TYPE_CUPCAKE:
		data.weight = 7;
		model_renderer->SetModel("asset\\model\\cupcake.obj");
		model_renderer->SetLocalPos(VECTOR3(0.0f, -1.0f, 0.0f));
		model_renderer->SetLocalSca(VECTOR3(5.0f, 5.0f, 5.0f));
		break;
	case SWEETS_TYPE::SWEETS_TYPE_DONUT:
		data.weight = 9;
		model_renderer->SetModel("asset\\model\\donut-sprinkles.obj");
		model_renderer->SetLocalPos(VECTOR3(0.0f, -0.7f, 0.0f));
		model_renderer->SetLocalSca(VECTOR3(7.0f, 7.0f, 7.0f));
		break;
	case SWEETS_TYPE::SWEETS_TYPE_ICE_CREAM:
		data.weight = 8;
		model_renderer->SetModel("asset\\model\\ice-cream.obj");
		model_renderer->SetLocalPos(VECTOR3(0.0f, -1.0f, 0.0f));
		model_renderer->SetLocalSca(VECTOR3(5.0f, 5.0f, 5.0f));
		break;
	case SWEETS_TYPE::SWEETS_TYPE_LOLLYPOP:
		data.weight = 2;
		model_renderer->SetModel("asset\\model\\lollypop.obj");
		model_renderer->SetLocalPos(VECTOR3(0.0f, -1.0f, 0.0f));
		model_renderer->SetLocalSca(VECTOR3(5.0f, 5.0f, 5.0f));
		break;
	case SWEETS_TYPE::SWEETS_TYPE_PANCAKE:
		data.weight = 5;
		model_renderer->SetModel("asset\\model\\pancakes.obj");
		model_renderer->SetLocalPos(VECTOR3(0.0f, -0.7f, 0.0f));
		model_renderer->SetLocalSca(VECTOR3(5.0f, 5.0f, 5.0f));
		break;
	case SWEETS_TYPE::SWEETS_TYPE_POPSICLE:
		data.weight = 4;
		model_renderer->SetModel("asset\\model\\popsicle.obj");
		model_renderer->SetLocalPos(VECTOR3(0.0f, -1.0f, 0.0f));
		model_renderer->SetLocalSca(VECTOR3(5.0f, 5.0f, 5.0f));
		break;
	case SWEETS_TYPE::SWEETS_TYPE_PUDDING:
		data.weight = 6;
		model_renderer->SetModel("asset\\model\\pudding.obj");
		model_renderer->SetLocalPos(VECTOR3(0.0f, -1.0f, 0.0f));
		model_renderer->SetLocalSca(VECTOR3(7.0f, 7.0f, 7.0f));
		break;
	default:
		break;
	}

	sweets_component->SetSweetsData(data);
}
