//-----------------------------------------------
// CameraSystem.h
// 制作日：2025/11/13
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <vector>

class CAMERA_COMPONENT;

class CAMERA_SYSTEM
{
private:
	std::vector<CAMERA_COMPONENT*> m_cameras;
	CAMERA_COMPONENT* m_main_camera = nullptr;

	CAMERA_SYSTEM() = default;
	~CAMERA_SYSTEM() = default;

	CAMERA_SYSTEM(const CAMERA_SYSTEM&) = delete;
	CAMERA_SYSTEM& operator=(const CAMERA_SYSTEM&) = delete;

public:
	static CAMERA_SYSTEM& GetInstance();

	void RegisterCamera(CAMERA_COMPONENT* camera);
	void UnregisterCamera(CAMERA_COMPONENT* camera);

	void SetMainCamera(CAMERA_COMPONENT* camera) { m_main_camera = camera; }
	CAMERA_COMPONENT* GetMainCamera() const { return m_main_camera; }

	const std::vector<CAMERA_COMPONENT*>& GetAllCameras() const { return m_cameras; }
};