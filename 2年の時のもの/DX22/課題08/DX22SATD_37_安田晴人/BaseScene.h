//-----------------------------------------------
// BaseScene.h
// 制作日：2024/12/10
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

class BASE_SCENE
{
protected:
	class SCENE_MANAGER* m_p_scene_manager;

public:
	BASE_SCENE(class SCENE_MANAGER* p_scene_manager) : m_p_scene_manager(p_scene_manager) {}			//コンストラクタ
	virtual ~BASE_SCENE() {}					//デストラクタ

	virtual void Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
};
