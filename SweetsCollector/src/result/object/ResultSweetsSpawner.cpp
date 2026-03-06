//-----------------------------------------------
// ResultSweetsSpawner.cpp
// 制作日：2026/02/07
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "result/object/ResultSweetsSpawner.h"
#include "result/component/ResultSweetsSpawnComponent.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void RESULT_SWEETS_SPAWNER::Init()
{
	RESULT_SWEETS_SPAWN_COMPONENT* spawn_component = AddComponent<RESULT_SWEETS_SPAWN_COMPONENT>();
	spawn_component->SetSpawnInterval(0.1f);
	spawn_component->SetMaxSweets(30);
}