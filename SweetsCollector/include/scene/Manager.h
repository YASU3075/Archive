#pragma once

#include <list>
#include <vector>
#include "entity/GameObject.h"

class Manager
{
private:
	static class SCENE* m_p_scene;
	static class SCENE* m_p_next_scene;
	static int m_score;
	static bool m_hit_stop;
	static float m_hit_stop_timer;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static class SCENE* GetScene() { return m_p_scene; }

	template <typename T>
	static void SetScene()
	{
		m_p_next_scene = new T();
	}

	static int GetScore() { return m_score; }
	static void SetScore(int score) { m_score = score; }


	static void StartHitStop(float time)
	{
		m_hit_stop = true;
		m_hit_stop_timer = time;
	}
};