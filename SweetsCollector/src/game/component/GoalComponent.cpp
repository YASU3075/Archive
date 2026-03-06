//-----------------------------------------------
// GoalComponent.cpp
// 制作日：2025/12/28
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "renderer/Renderer.h"
#include "game/component/GoalComponent.h"

#include "game/object/ScoreUI.h"
#include "game/component/ScoreManagerComponent.h"

#include "physics/Collider3D.h"

#include "game/object/Sweets.h"
#include "game/component/SweetsComponent.h"

#include "object/particle/GoalHitParticle.h"

//------------------------------------------------
// 初期化処理
//------------------------------------------------
void GOAL_COMPONENT::Init()
{
	m_p_score_manager = Manager::GetScene()->GetGameObject<SCORE_UI>()->GetComponent<SCORE_MANAGER_COMPONENT>();
}

//------------------------------------------------
// 衝突開始時の処理
//------------------------------------------------
void GOAL_COMPONENT::OnCollisionEnter(COLLIDER_3D* other)
{
	SWEETS* sweets = dynamic_cast<SWEETS*>(other->GetOwner());

	if (m_p_score_manager == nullptr)
	{
		m_p_score_manager = Manager::GetScene()->GetGameObject<SCORE_UI>()->GetComponent<SCORE_MANAGER_COMPONENT>();
	}

	if (m_p_score_manager)
	{
		if (sweets)
		{
			SWEETS_COMPONENT* sweets_component = sweets->GetComponent<SWEETS_COMPONENT>();
			if (sweets_component)
			{// スコアを加算
				m_p_score_manager->AddScore(sweets_component->GetSweetsData().weight);
			}

			// パーティクルを生成
			GOAL_HIT_PARTICLE* particle = Manager::GetScene()->AddGameObject<GOAL_HIT_PARTICLE>(LAYER::LAYER_GAME_OBJECT);
			particle->GetTransform()->SetPos(sweets->GetTransform()->GetPos());

			// お菓子を削除
			sweets->SetDestroy(true);
		}
	}
}
