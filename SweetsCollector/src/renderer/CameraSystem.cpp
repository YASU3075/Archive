//-----------------------------------------------
// CameraSystem.cpp
// 制作日：2025/11/13
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "renderer/Renderer.h"
#include "renderer/CameraSystem.h"
#include "component/CameraComponent.h"
#include "renderer/RenderTarget.h"
#include "entity/GameObject.h"

//-----------------------------------------------
// シングルトンインスタンス取得
//-----------------------------------------------
CAMERA_SYSTEM& CAMERA_SYSTEM::GetInstance()
{
	static CAMERA_SYSTEM instance;
	return instance;
}

//-----------------------------------------------
// カメラ登録
//-----------------------------------------------
void CAMERA_SYSTEM::RegisterCamera(CAMERA_COMPONENT* camera)
{
	if (!camera)
	{// nullチェック
		return;
	}

	// 重複登録防止
	for (auto& cam : m_cameras)
	{
		if (cam == camera)
		{
			return;
		}
	}

	m_cameras.push_back(camera);
}

//-----------------------------------------------
// カメラ登録解除
//-----------------------------------------------
void CAMERA_SYSTEM::UnregisterCamera(CAMERA_COMPONENT* camera)
{
	if (!camera)
	{// nullチェック
		return;
	}

	// 登録解除
	auto it = std::find(m_cameras.begin(), m_cameras.end(), camera);
	if (it != m_cameras.end())
	{
		m_cameras.erase(it);
	}
}
