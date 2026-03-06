//-----------------------------------------------
// AutoDestroy.cpp
// 制作日：2025/07/29
// 制作者：安田晴人
//-----------------------------------------------
#include "component/AutoDestroy.h"
#include "Main.h"
#include "scene/Manager.h"
#include "entity/GameObject.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void AUTO_DESTROY::Init()
{
	// 生存時間を設定
	m_life_time = 1.0f;

	m_is_start = true;
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void AUTO_DESTROY::Update()
{
	const float DELTA_TIME = 1.0f / 60.0f;

	// 生存時間を減少
	if (!m_is_start)
	{
		return;
	}

	m_life_time -= DELTA_TIME;

	// 生存時間が0以下になったらオブジェクトを削除
	if (m_life_time <= 0.0f)
	{
		if (m_p_owner)
		{
			m_p_owner->SetDestroy(true);
		}
	}
}

void AUTO_DESTROY::OnCollisionEnter(COLLIDER_3D* other)
{
	// 衝突時にオブジェクトを削除
	if (m_p_owner)
	{
		m_p_owner->SetDestroy(true);
	}
}
