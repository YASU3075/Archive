//-----------------------------------------------
// SceneManager.cpp
// 制作日：2024/12/10
// 制作者：安田晴人
//-----------------------------------------------
#include "SceneManager.h"
#include "Title.h"
#include "Game.h"
#include "Result.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void SCENE_MANAGER::Init(void)
{
	m_p_current_scene = new TITLE(this);
	m_p_current_scene->Init();
}

//-----------------------------------------------
// 終了処理
//-----------------------------------------------
void SCENE_MANAGER::Uninit(void)
{
	m_p_current_scene->Uninit();
	delete m_p_current_scene;
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void SCENE_MANAGER::Update(void)
{
	m_p_current_scene->Update();
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void SCENE_MANAGER::Draw(void)
{
	m_p_current_scene->Draw();
}

//-----------------------------------------------
// シーン切り替え
//-----------------------------------------------
void SCENE_MANAGER::ChangeScene(SCENE scene)
{
	m_p_current_scene->Uninit();
	delete m_p_current_scene;
	m_p_current_scene = nullptr;

	switch (scene)
	{
	case SCENE_TITLE:
		m_p_current_scene = new TITLE(this);
		break;
	case SCENE_GAME:
		m_p_current_scene = new GAME(this);
		break;
	case SCENE_RESULT:
		m_p_current_scene = new RESULT(this);
		break;
	case SCENE_MAX:
		break;
	default:
		break;
	}

	m_p_current_scene->Init();
}