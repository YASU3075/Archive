//-----------------------------------------------
// SweetsShootComponent.cpp
// 制作日：2026/01/04
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "renderer/Renderer.h"
#include "game/component/SweetsShootComponent.h"
#include "game/component/SweetsManagementComponent.h"
#include "entity/GameObject.h"
#include "game/object/Sweets.h"
#include "game/component/SweetsComponent.h"
#include "game/component/PlayerAnimComponent.h"
#include "physics/RigidBody.h"
#include "input/Input.h"
#include "input/GamePad.h"
#include "component/renderer/ModelRendererComponent.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void SWEETS_SHOOT_COMPONENT::Init()
{
	m_p_sweets_management = GetOwner()->GetComponent<SWEETS_MANAGEMENT_COMPONENT>();
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void SWEETS_SHOOT_COMPONENT::Update()
{
	m_shoot_timer -= 1.0f / 60.0f;
	if ((Input::GetKeyTrigger(VK_SPACE) || INPUT_SYSTEM::Instance().GetGamePad(0).Pressed(PAD_BUTTON::A)) && m_shoot_timer <= 0.0f)
	{
		if (m_p_sweets_management != nullptr)
		{
			const std::queue<SWEETS_DATA>& sweets_queue = m_p_sweets_management->GetSweetsQueue();
			if (!sweets_queue.empty())
			{
				SWEETS_DATA data = m_p_sweets_management->PopSweets();
				ShootSweets(data);
				m_shoot_timer = m_shoot_cooldown;

				// プレイヤーアニメーション再生
				PLAYER_ANIM_COMPONENT* player_anim = m_p_owner->GetComponent<PLAYER_ANIM_COMPONENT>();
				if (player_anim != nullptr)
				{
					player_anim->TriggerShootAnimation();
				}
			}
		}
	}
}

//-----------------------------------------------
// お菓子を撃つ
//-----------------------------------------------
void SWEETS_SHOOT_COMPONENT::ShootSweets(SWEETS_DATA& data)
{
	GAME_OBJECT* p_owner = GetOwner();
	if (p_owner == nullptr)
	{
		return;
	}

	// プレイヤーの位置を取得
	VECTOR3 player_pos = m_p_owner->GetTransform()->GetPos();

	// プレイヤーの前方ベクトルを計算
	QUATERNION rot = m_p_owner->GetComponent<MODEL_RENDERER_COMPONENT>()->GetLocalRot();
	VECTOR3 player_forward = rot.Rotate({ -1.0f, 0.0f, 0.0f });

	// 発射オフセットを計算
	VECTOR3 shoot_offset = player_forward * m_shoot_offset_length;

	VECTOR3 shoot_pos = p_owner->GetTransform()->GetPos() + shoot_offset;
	SWEETS* p_sweets = Manager::GetScene()->AddGameObject<SWEETS>(LAYER::LAYER_GAME_OBJECT);
	p_sweets->GetTransform()->SetPos(shoot_pos);
	p_sweets->GetTransform()->SetRot(QUATERNION::FromEuler(VECTOR3(0.0f, 0.0f, 0.0f)));
	p_sweets->GetComponent<SWEETS_COMPONENT>()->SetSweetsData(data);

	RIGID_BODY* rb = p_sweets->GetComponent<RIGID_BODY>();

	MODEL_RENDERER_COMPONENT* model = p_sweets->GetComponent<MODEL_RENDERER_COMPONENT>();

	switch (data.type)
	{
	case SWEETS_TYPE::SWEETS_TYPE_CAKE:
		model->SetModel("asset\\model\\cake.obj");
		model->SetLocalPos(VECTOR3(0.0f, -0.7f, 0.0f));
		model->SetLocalSca(VECTOR3(3.0f, 3.0f, 3.0f));
		break;
	case SWEETS_TYPE::SWEETS_TYPE_CHOCOLATE:
		model->SetModel("asset\\model\\chocolate-wrapper.obj");
		model->SetLocalPos(VECTOR3(0.0f, -0.7f, 0.0f));
		model->SetLocalSca(VECTOR3(7.0f, 7.0f, 7.0f));
		break;
	case SWEETS_TYPE::SWEETS_TYPE_COOKIE:
		model->SetModel("asset\\model\\cookie.obj");
		model->SetLocalPos(VECTOR3(0.0f, -0.7f, 0.0f));
		model->SetLocalSca(VECTOR3(10.0f, 10.0f, 10.0f));
		break;
	case SWEETS_TYPE::SWEETS_TYPE_CUPCAKE:
		model->SetModel("asset\\model\\cupcake.obj");
		model->SetLocalPos(VECTOR3(0.0f, -1.0f, 0.0f));
		model->SetLocalSca(VECTOR3(5.0f, 5.0f, 5.0f));
		break;
	case SWEETS_TYPE::SWEETS_TYPE_DONUT:
		model->SetModel("asset\\model\\donut-sprinkles.obj");
		model->SetLocalPos(VECTOR3(0.0f, -1.0f, 0.0f));
		model->SetLocalSca(VECTOR3(7.0f, 7.0f, 7.0f));
		break;
	case SWEETS_TYPE::SWEETS_TYPE_ICE_CREAM:
		model->SetModel("asset\\model\\ice-cream.obj");
		model->SetLocalPos(VECTOR3(0.0f, -1.0f, 0.0f));
		model->SetLocalSca(VECTOR3(5.0f, 5.0f, 5.0f));
		break;
	case SWEETS_TYPE::SWEETS_TYPE_LOLLYPOP:
		model->SetModel("asset\\model\\lollypop.obj");
		model->SetLocalPos(VECTOR3(0.0f, -1.0f, 0.0f));
		model->SetLocalSca(VECTOR3(5.0f, 5.0f, 5.0f));
		break;
	case SWEETS_TYPE::SWEETS_TYPE_PANCAKE:
		model->SetModel("asset\\model\\pancakes.obj");
		model->SetLocalPos(VECTOR3(0.0f, -1.0f, 0.0f));
		model->SetLocalSca(VECTOR3(5.0f, 5.0f, 5.0f));
		break;
	case SWEETS_TYPE::SWEETS_TYPE_POPSICLE:
		model->SetModel("asset\\model\\popsicle.obj");
		model->SetLocalPos(VECTOR3(0.0f, -1.0f, 0.0f));
		model->SetLocalSca(VECTOR3(5.0f, 5.0f, 5.0f));
		break;
	case SWEETS_TYPE::SWEETS_TYPE_PUDDING:
		model->SetModel("asset\\model\\pudding.obj");
		model->SetLocalPos(VECTOR3(0.0f, -1.0f, 0.0f));
		model->SetLocalSca(VECTOR3(7.0f, 7.0f, 7.0f));
		break;
	default:
		break;
	}

	if (rb != nullptr)
	{
		rb->AddForce(player_forward.GetNormalized() * m_shoot_power, FORCE_MODE::IMPULSE);
	}
}