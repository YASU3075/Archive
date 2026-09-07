//-----------------------------------------------
// SpherCollider.h
// 制作日：2024/12/10
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include "BaseCollider.h"
#include "Primitive3D.h"

class SPHERE_COLLIDER : public BASE_COLLIDER
{
private:
	SPHERE m_sphere;			//球
	bool m_is_collision = false;			//当たってるかどうか

public:
	//デフォルトコンストラクタ
	SPHERE_COLLIDER() = default;
	//引数付きコンストラクタ
	SPHERE_COLLIDER(GAME_OBJECT_3D* p_game_object, std::string tag) : BASE_COLLIDER(p_game_object, tag, COLLIDER_TYPE_SPHERE)
	{
		m_sphere.p = GetGameObject()->GetPos();
		m_sphere.r = GetGameObject()->GetRad();
	}
	//デストラクタ
	~SPHERE_COLLIDER() {}

	void Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
};