//-----------------------------------------------
// TitleSweetsGenerateComponent.cpp
// 制作日：2026/01/29
// 制作者：安田晴人
//-----------------------------------------------
#include <random>
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "title/component/TitleSweetsGenerateComponent.h"
#include "title/object/TitleSweets.h"
#include "component/renderer/ModelRendererComponent.h"
#include "title/object/Spline.h"
#include "title/component/SplineComponent.h"
#include "title/component/SplineMoveComponent.h"
#include "EngineTime.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void TITLE_SWEETS_GENERATE_COMPONENT::Init()
{
	m_current_sweets_num = 0;
	m_timer = 0.0f;
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void TITLE_SWEETS_GENERATE_COMPONENT::Update()
{
	// タイマー更新
	m_timer += TIME::DeltaTime();
	
	// スイーツ生成処理
	if (m_timer >= m_generate_interval && m_current_sweets_num < MAX_SWEETS_NUM)
	{
		m_timer = 0.0f;
		m_current_sweets_num++;

		// スイーツオブジェクトの生成
		TITLE_SWEETS* p_sweets = Manager::GetScene()->AddGameObject<TITLE_SWEETS>(LAYER::LAYER_GAME_OBJECT);
		
		// ランダムな位置に配置
		std::random_device rd;
		std::mt19937 rng(rd());
		float x_pos = std::uniform_real_distribution<float>(-X_POS_RANGE, X_POS_RANGE)(rng);
		
		MODEL_RENDERER_COMPONENT* p_model_renderer = p_sweets->GetComponent<MODEL_RENDERER_COMPONENT>();
		VECTOR3 origin_local_pos = p_model_renderer->GetLocalPos();
		p_model_renderer->SetLocalPos(VECTOR3(origin_local_pos.x + x_pos, origin_local_pos.y, origin_local_pos.z));

		// スプラインコンポーネントの取得とスイーツへの設定
		SPLINE* p_spline = Manager::GetScene()->GetGameObject<SPLINE>();
		SPLINE_COMPONENT* p_spline_component = p_spline->GetComponent<SPLINE_COMPONENT>();

		SPLINE_MOVE_COMPONENT* p_spline_move = p_sweets->GetComponent<SPLINE_MOVE_COMPONENT>();
		p_spline_move->SetSplineComponent(p_spline_component);
	}
}