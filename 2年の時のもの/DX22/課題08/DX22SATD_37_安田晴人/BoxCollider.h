//-----------------------------------------------
// BoxCollider.h
// 制作日：2025/01/06
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include "BaseCollider.h"
#include "Primitive3D.h"

class BOX_COLLIDER : public BASE_COLLIDER
{
private:
	XMFLOAT3 m_pos;
	XMFLOAT3 m_sca;
	XMFLOAT3 m_rot;

public:
	//デフォルトコンストラクタ
	BOX_COLLIDER() = default;
	//引数コンストラクタ
	BOX_COLLIDER(GAME_OBJECT_3D* p_game_object, std::string tag) : BASE_COLLIDER(p_game_object, tag, COLLIDER_TYPE_BOX)
	{
		m_pos = GetGameObject()->GetPos();
		m_sca = GetGameObject()->GetSca();
		m_rot = GetGameObject()->GetRot();
	}

	//デストラクタ
	~BOX_COLLIDER() {}

	void Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
};