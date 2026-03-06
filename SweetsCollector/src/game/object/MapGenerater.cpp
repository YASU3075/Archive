//-----------------------------------------------
// MapGenerater.cpp
// 制作日：2026/01/06
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "game/object/MapGenerater.h"
#include "game/component/GenerateMapComponent.h"
#include "game/component/SweetsSpawnComponent.h"

//------------------------------------------------
// 初期化処理
//------------------------------------------------
void MAP_GENERATER::Init()
{
	m_transform.SetPos(VECTOR3(0.0f, 0.0f, 0.0f));
	AddComponent<GENERATE_MAP_COMPONENT>();
	AddComponent<SWEETS_SPAWN_COMPONENT>();
}