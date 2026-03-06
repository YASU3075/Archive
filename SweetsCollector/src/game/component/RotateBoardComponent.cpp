//-----------------------------------------------
// RotateBoardComponent.cpp
// 制作日：2026/02/16
// 制作者：安田晴人
//-----------------------------------------------
#include <random>
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "game/component/RotateBoardComponent.h"
#include "game/object/Sweets.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void ROTATE_BOARD_COMPONENT::Init()
{
	
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void ROTATE_BOARD_COMPONENT::Update()
{
	m_current_angle = GetOwner()->GetTransform()->GetRot().ToEuler().y;

	float delta_angle = m_current_angle - m_previous_angle;

	while (delta_angle < -180.0f) delta_angle += 360.0f;
	while (delta_angle > 180.0f) delta_angle -= 360.0f;

	m_sum_angle += fabsf(delta_angle);

	m_previous_angle = m_current_angle;

	if (m_sum_angle > 360.0f)
	{
		m_sum_angle -= 360.0f;

		SWEETS* sweets = Manager::GetScene()->AddGameObject<SWEETS>(LAYER::LAYER_GAME_OBJECT);

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> rand_x(-m_spawn_range.x, m_spawn_range.x);
		std::uniform_real_distribution<float> rand_z(-m_spawn_range.z, m_spawn_range.z);
		
		VECTOR3 spawn_pos = GetOwner()->GetTransform()->GetPos() + VECTOR3(rand_x(gen), 5.0f, rand_z(gen));

		sweets->GetTransform()->SetPos(spawn_pos);

		SWEETS_TYPE type = static_cast<SWEETS_TYPE>(std::uniform_int_distribution<int>(1, (int)SWEETS_TYPE::SWEETS_TYPE_MAX - 1)(gen));

		sweets->SetDataByType(type);
	}
}