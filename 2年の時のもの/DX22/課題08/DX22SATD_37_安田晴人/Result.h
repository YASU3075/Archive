//-----------------------------------------------
// Result.h
// 制作日：2024/12/10
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include "BaseScene.h"
#include "Camera.h"

class RESULT : public BASE_SCENE
{
private:
	ID3D11ShaderResourceView* m_texture;
	ID3D11ShaderResourceView* m_texture_enter;
	CAMERA* m_p_camera;
	LIGHT* m_p_light;
public:
	RESULT(class SCENE_MANAGER* p_scene_manager) : BASE_SCENE(p_scene_manager) {}
	~RESULT() {}

	void Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
};
