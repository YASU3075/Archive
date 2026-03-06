//-----------------------------------------------
// SplineMoveComponent.cpp
// 制作日：2026/01/29
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "entity/GameObject.h"
#include "title/component/SplineMoveComponent.h"
#include "title/component/SplineComponent.h"
#include "EngineTime.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void SPLINE_MOVE_COMPONENT::Init()
{
	m_p_spline = nullptr;
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void SPLINE_MOVE_COMPONENT::Update()
{
	if (m_p_spline == nullptr)
	{// nullチェック
		return;
	}

	// 進行度を更新
	m_progress += m_speed * TIME::DeltaTime();

	if (m_progress >= 1.0f)
	{// 次の制御点へ
		m_progress = 0.0f;
		m_current_index = (m_current_index + 1) % static_cast<int>(m_p_spline->GetControlPointCount());
	}
	
	// 位置更新
	VECTOR3 new_pos = m_p_spline->GetPoint(m_current_index, m_progress);
	GetOwner()->GetTransform()->SetPos(new_pos);
}