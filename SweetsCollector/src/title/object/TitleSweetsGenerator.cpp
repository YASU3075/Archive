//-----------------------------------------------
// TitleSweetsGenerator.cpp
// 制作日：2026/01/29
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "title/object/TitleSweetsGenerator.h"
#include "title/component/TitleSweetsGenerateComponent.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void TITLE_SWEETS_GENERATOR::Init()
{
	m_transform.SetPos(VECTOR3(0.0f, 0.0f, 0.0f));
	AddComponent<TITLE_SWEETS_GENERATE_COMPONENT>();
}