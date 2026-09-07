//-----------------------------------------------
// Player.h
// 制作日：2024/12/27
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "GameObject3D.h"
#include "renderer.h"
#include "model.h"

class CAMERA;
class COLLIDER_MANAGER;

class PLAYER : public GAME_OBJECT_3D
{
private:
	bool m_old_click = false;

	ID3D11ShaderResourceView* m_texture;
	CAMERA* m_p_camera;
	COLLIDER_MANAGER* m_p_collider_manager;
	MODEL* m_p_model;

public:
	PLAYER() = default;
	PLAYER(GAME* p_game, CAMERA* p_camera, COLLIDER_MANAGER* p_collider_manager, const std::string& tag, XMFLOAT3 pos, XMFLOAT3 vec, XMFLOAT3 rot, XMFLOAT3 sca, XMFLOAT4 color, float rad)
		: GAME_OBJECT_3D(p_game, tag, pos, vec, rot, sca, color, rad), m_p_camera(p_camera), m_p_collider_manager(p_collider_manager) {}
	~PLAYER() {}

	void Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	CAMERA* GetCamera(void) const { return m_p_camera; };
};