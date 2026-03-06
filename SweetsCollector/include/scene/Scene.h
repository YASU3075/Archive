//-----------------------------------------------
// Scene.h
// 制作日：2025/06/25
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include <string>
#include <list>
#include <vector>
#include "entity/GameObject.h"

enum class LAYER
{
	LAYER_CAMERA,
	LAYER_BACKGROUND,
	LAYER_PLAYER,
	LAYER_GAME_OBJECT,
	LAYER_UI,

	LAYER_MAX,
};

class RENDERER_COMPONENT;
class GAME_OBJECT;

struct RENDER_ENTRY
{
	RENDERER_COMPONENT* p_renderer_component;
	GAME_OBJECT* p_owner;
	float z;
};

class SCENE
{
private:
	std::list<GAME_OBJECT*> m_game_object[static_cast<unsigned long long>(LAYER::LAYER_MAX)];

public:

	virtual void Init() = 0;
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void DrawByRenderPass(class CAMERA_COMPONENT* p_camera);

	template <typename T>	//テンプレート
	T* AddGameObject(LAYER layer)
	{
		T* game_object = new T();
		game_object->Init();
		game_object->SetLayer(layer);
		m_game_object[static_cast<int>(layer)].push_back(game_object);

		return game_object;
	}

	template <typename T>
	T* GetGameObject()
	{
		for (int i = 0; i < static_cast<int>(LAYER::LAYER_MAX); i++)
		{
			for (auto game_object : m_game_object[i])
			{
				T* find = dynamic_cast<T*>(game_object);
				if (find != nullptr)
				{
					return find;
				}
			}
		}

		return nullptr;
	};

	template <typename T>
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> finds;

		for (int i = 0; i < static_cast<int>(LAYER::LAYER_MAX); i++)
		{
			for (auto game_object : m_game_object[i])
			{
				T* find = dynamic_cast<T*>(game_object);
				if (find != nullptr)
				{
					finds.push_back(find);
				}
			}
		}

		return finds;
	};
};