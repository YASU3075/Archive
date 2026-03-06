//-----------------------------------------------
// TitleSweets.cpp
// 制作日：2026/01/29
// 制作者：安田晴人
//-----------------------------------------------
#include <random>
#include "Main.h"
#include "renderer/Renderer.h"
#include "game/SweetsData.h"
#include "title/object/TitleSweets.h"
#include "component/renderer/ModelRendererComponent.h"
#include "title/component/SplineMoveComponent.h"
#include "game/component/SweetsFloatRotateComponent.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void TITLE_SWEETS::Init()
{
	MODEL_RENDERER_COMPONENT* model_renderer = AddComponent<MODEL_RENDERER_COMPONENT>();

	std::random_device rd;
	std::mt19937 rng(rd());
	int type = std::uniform_int_distribution<int>(1, (int)SWEETS_TYPE::SWEETS_TYPE_MAX - 1)(rng);

	switch (static_cast<SWEETS_TYPE>(type))
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
		model_renderer->SetLocalRot(QUATERNION::FromEuler(VECTOR3(0.0f, 90.0f, 0.0f)));
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
		model_renderer->SetLocalRot(QUATERNION::FromEuler(VECTOR3(0.0f, 90.0f, 0.0f)));
		break;
	case SWEETS_TYPE::SWEETS_TYPE_PUDDING:
		model_renderer->SetModel("asset\\model\\pudding.obj");
		model_renderer->SetLocalPos(VECTOR3(0.0f, -1.0f, 0.0f));
		model_renderer->SetLocalSca(VECTOR3(7.0f, 7.0f, 7.0f));
		break;
	default:
		break;
	}

	SPLINE_MOVE_COMPONENT* spline_move = AddComponent<SPLINE_MOVE_COMPONENT>();
}