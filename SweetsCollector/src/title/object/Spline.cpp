//-----------------------------------------------
// Spline.cpp
// 制作日：2026/01/29
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "title/object/Spline.h"
#include "title/component/SplineComponent.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void SPLINE::Init()
{
	SPLINE_COMPONENT* spline_component = AddComponent<SPLINE_COMPONENT>();
}