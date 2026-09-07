//-----------------------------------------------
// Game.h
// 制作日：2024/12/10
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <string>
#include "renderer.h"
#include "BaseScene.h"

#include "Camera.h"
#include "Map.h"


class GAME_OBJECT_3D;
class GAME_OBJECT_3D_CONTAINER;
class COLLIDER_MANAGER;

class GAME : public BASE_SCENE
{
private:
	LIGHT* m_p_light;
	CAMERA* m_p_camera[3];
	MAP* m_p_map;
	static constexpr int MAX_GAMEOBJECT_NUM = 100;
	GAME_OBJECT_3D* m_p_game_object[MAX_GAMEOBJECT_NUM];
	COLLIDER_MANAGER* m_p_collider_manager;

	int m_camera_num = 0;

public:
	GAME(class SCENE_MANAGER* p_scene_manager) : BASE_SCENE(p_scene_manager) {}
	~GAME() {}

	void Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void Register(GAME_OBJECT_3D* p_object);

	GAME_OBJECT_3D_CONTAINER GetGameObjectContainer(std::string tag);
	GAME_OBJECT_3D_CONTAINER GetGameObjectContainer(int collider_id);

};
