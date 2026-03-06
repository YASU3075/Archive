//-----------------------------------------------
// TutorialManager.cpp
// 制作日：2026/02/11
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "scene/Manager.h"
#include "scene/Scene.h"
#include "tutorial/object/TutorialManager.h"
#include "tutorial/component/TutorialManagerComponent.h"

#include "audio.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void TUTORIAL_MANAGER::Init()
{
	// コンポーネントの追加
	AddComponent<TUTORIAL_MANAGER_COMPONENT>();

	AUDIO* audio = AddComponent<AUDIO>();
	audio->Load("asset\\sound\\Tutorial.wav");
	audio->Play(true);
	audio->SetVolume(0.3f);
}