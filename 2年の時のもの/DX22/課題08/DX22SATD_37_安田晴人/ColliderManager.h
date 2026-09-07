//-----------------------------------------------
// ColliderManager.h
// 制作日：2024/12/10
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include "BaseCollider.h"

//-----------------------------------------------
// マクロ定義
//-----------------------------------------------
#define MAX_COLLIDER		(100)

class COLLIDER_MANAGER
{
private:
	BASE_COLLIDER* m_p_collider[MAX_COLLIDER];

public:
	COLLIDER_MANAGER() = default;
	~COLLIDER_MANAGER() {}

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//当たり判定登録
	int Register(BASE_COLLIDER* p_base_collider);

	//当たり判定削除
	void Destroy(int index);

	//当たり判定取得
	BASE_COLLIDER* GetCollider(int id);
	BASE_COLLIDER_CONTAINER GetColliderContainer(const std::string tag);
	BASE_COLLIDER_CONTAINER GetColliderContainer(const BASE_COLLIDER& collider);

	//当たり判定処理
	bool GetCollision(const BASE_COLLIDER& collider_1, const BASE_COLLIDER& collider_2);
};
