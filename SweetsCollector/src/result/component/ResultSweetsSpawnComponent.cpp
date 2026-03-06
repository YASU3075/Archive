//-----------------------------------------------
// ResultSweetsSpawnComponent.cpp
// 制作日：2026/02/07
// 制作者：安田晴人
//-----------------------------------------------
#include <random>
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "entity/GameObject.h"
#include "result/component/ResultSweetsSpawnComponent.h"
#include "component/renderer/ModelRendererComponent.h"
#include "game/SweetsData.h"
#include "game/object/Sweets.h"
#include "EngineTime.h"


//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void RESULT_SWEETS_SPAWN_COMPONENT::Init()
{
	m_spawn_timer = m_spawn_interval;
	m_current_sweets = 0;
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void RESULT_SWEETS_SPAWN_COMPONENT::Update()
{
	const float RANGE_SPAWN_AREA = 2.0f;

	if (m_current_sweets >= m_max_sweets)
	{
		return;
	}

	m_spawn_timer -= TIME::DeltaTime();

	if (m_spawn_timer <= 0.0f)
	{
		m_spawn_timer = m_spawn_interval;
		VECTOR3 parent_pos = m_p_owner->GetTransform()->GetPos();
		VECTOR3 spawn_pos = parent_pos;

		// ランダムな位置にスポーン
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<float> dist_x(-RANGE_SPAWN_AREA, RANGE_SPAWN_AREA);
		std::uniform_real_distribution<float> dist_z(-RANGE_SPAWN_AREA, RANGE_SPAWN_AREA);		

		spawn_pos.x += dist_x(mt);
		spawn_pos.z += dist_z(mt);

		SWEETS* sweets = Manager::GetScene()->AddGameObject<SWEETS>(LAYER::LAYER_GAME_OBJECT);
		sweets->GetTransform()->SetPos(spawn_pos);
		MODEL_RENDERER_COMPONENT* model_renderer = sweets->GetComponent<MODEL_RENDERER_COMPONENT>();

		SWEETS_TYPE random_type = static_cast<SWEETS_TYPE>(std::uniform_int_distribution<int>(1, static_cast<int>(SWEETS_TYPE::SWEETS_TYPE_MAX) - 1)(mt));

		switch (random_type)
		{
		case SWEETS_TYPE::SWEETS_TYPE_CAKE:
			model_renderer->SetModel("asset\\model\\cake.obj");
			model_renderer->SetLocalPos(VECTOR3(0.0f, -0.7f, 0.0f));
			model_renderer->SetLocalSca(VECTOR3(3.0f, 3.0f, 3.0f));
			break;
		case SWEETS_TYPE::SWEETS_TYPE_CHOCOLATE:
			model_renderer->SetModel("asset\\model\\chocolate-wrapper.obj");
			model_renderer->SetLocalPos(VECTOR3(0.0f, -0.7f, 0.0f));
			model_renderer->SetLocalSca(VECTOR3(7.0f, 7.0f, 7.0f));
			break;
		case SWEETS_TYPE::SWEETS_TYPE_COOKIE:
			model_renderer->SetModel("asset\\model\\cookie.obj");
			model_renderer->SetLocalPos(VECTOR3(0.0f, -0.7f, 0.0f));
			model_renderer->SetLocalSca(VECTOR3(10.0f, 10.0f, 10.0f));
			break;
		case SWEETS_TYPE::SWEETS_TYPE_CUPCAKE:
			model_renderer->SetModel("asset\\model\\cupcake.obj");
			model_renderer->SetLocalPos(VECTOR3(0.0f, -1.0f, 0.0f));
			model_renderer->SetLocalSca(VECTOR3(5.0f, 5.0f, 5.0f));
			break;
		case SWEETS_TYPE::SWEETS_TYPE_DONUT:
			model_renderer->SetModel("asset\\model\\donut-sprinkles.obj");
			model_renderer->SetLocalPos(VECTOR3(0.0f, -0.7f, 0.0f));
			model_renderer->SetLocalSca(VECTOR3(7.0f, 7.0f, 7.0f));
			break;
		case SWEETS_TYPE::SWEETS_TYPE_ICE_CREAM:
			model_renderer->SetModel("asset\\model\\ice-cream.obj");
			model_renderer->SetLocalPos(VECTOR3(0.0f, -1.0f, 0.0f));
			model_renderer->SetLocalSca(VECTOR3(5.0f, 5.0f, 5.0f));
			break;
		case SWEETS_TYPE::SWEETS_TYPE_LOLLYPOP:
			model_renderer->SetModel("asset\\model\\lollypop.obj");
			model_renderer->SetLocalPos(VECTOR3(0.0f, -1.0f, 0.0f));
			model_renderer->SetLocalSca(VECTOR3(5.0f, 5.0f, 5.0f));
			break;
		case SWEETS_TYPE::SWEETS_TYPE_PANCAKE:
			model_renderer->SetModel("asset\\model\\pancakes.obj");
			model_renderer->SetLocalPos(VECTOR3(0.0f, -0.7f, 0.0f));
			model_renderer->SetLocalSca(VECTOR3(5.0f, 5.0f, 5.0f));
			break;
		case SWEETS_TYPE::SWEETS_TYPE_POPSICLE:
			model_renderer->SetModel("asset\\model\\popsicle.obj");
			model_renderer->SetLocalPos(VECTOR3(0.0f, -1.0f, 0.0f));
			model_renderer->SetLocalSca(VECTOR3(5.0f, 5.0f, 5.0f));
			break;
		case SWEETS_TYPE::SWEETS_TYPE_PUDDING:
			model_renderer->SetModel("asset\\model\\pudding.obj");
			model_renderer->SetLocalPos(VECTOR3(0.0f, -1.0f, 0.0f));
			model_renderer->SetLocalSca(VECTOR3(7.0f, 7.0f, 7.0f));
			break;
		default:
			break;
		}

		m_current_sweets++;
	}
}