//-----------------------------------------------
// BaseCollider.h
// 制作日：2024/12/10
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include "GameObject3D.h"

//-----------------------------------------------
// 列挙体宣言
//-----------------------------------------------
enum COLLIDER_TYPE
{
	NO_COLLIDER = 0,
	COLLIDER_TYPE_SPHERE,
	COLLIDER_TYPE_BOX,
};

class COLLIDER_MANAGER;

class BASE_COLLIDER
{
private:
	bool m_is_collision;
	GAME_OBJECT_3D* m_p_game_object = nullptr;
	std::string m_tag;

	COLLIDER_MANAGER* m_p_collider_manager = nullptr;

protected:
	COLLIDER_TYPE m_type = NO_COLLIDER;
	COLLIDER_MANAGER* GetColliderManager(void) const { return m_p_collider_manager; };

public:
	BASE_COLLIDER() = default;
	BASE_COLLIDER(GAME_OBJECT_3D* p_game_object, std::string tag, COLLIDER_TYPE type) : m_is_collision(false), m_p_game_object(p_game_object), m_tag(tag), m_type(type) {}
	virtual ~BASE_COLLIDER() {}

	//セッター
	void SetIsCollision(bool is_collison) { m_is_collision = is_collison; };
	void SetTag(std::string tag) { m_tag = tag; };
	void SetType(COLLIDER_TYPE type) { m_type = type; };
	void SetColliderManager(COLLIDER_MANAGER* p_collider_manager) { m_p_collider_manager = p_collider_manager; };

	//ゲッター
	bool GetIsCollision(void) const { return m_is_collision; };
	GAME_OBJECT_3D* GetGameObject(void) const { return m_p_game_object; };
	std::string GetTag(void) const { return m_tag; };
	COLLIDER_TYPE GetType(void) const { return m_type; };

	//当たり判定マネージャーに登録する的なやつ？
	int Register(class COLLIDER_MANAGER* p_collider_manager);

	virtual void Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
};

class BASE_COLLIDER_CONTAINER
{
private:
	BASE_COLLIDER** m_p_p_base_collider;
	int m_count = 0;

public:
	BASE_COLLIDER_CONTAINER() = default;
	BASE_COLLIDER_CONTAINER(int num) : m_count(num) 
	{
		m_p_p_base_collider = new BASE_COLLIDER * [num];
	}

	~BASE_COLLIDER_CONTAINER()
	{
		delete[] m_p_p_base_collider;
	}

	BASE_COLLIDER* GetCollider(int index) const
	{
		return m_p_p_base_collider[index];
	}

	int GetCount(void) const { return m_count; };

	void SetCollider(int index, BASE_COLLIDER* p_collider)
	{
		m_p_p_base_collider[index] = p_collider;
	}

};

