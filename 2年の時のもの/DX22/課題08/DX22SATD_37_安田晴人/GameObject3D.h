//-----------------------------------------------
// GameObject3D.h
// 制作日：2024/12/10
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include "renderer.h"
#include <string>

class GAME;

class GAME_OBJECT_3D
{
private:
	

protected:
	GAME* m_p_game = nullptr;
	bool m_destroy = false;
	std::string m_tag;

	XMFLOAT3 m_pos;							//座標
	XMFLOAT3 m_vec;							//移動地
	XMFLOAT3 m_rot;							//回転
	XMFLOAT3 m_sca;							//大きさ
	XMFLOAT4 m_color;						//色

	float m_rad;							//半径

	int m_collider_index = -1;

	
	GAME* GetGame(void) const { return m_p_game; };

public:
	GAME_OBJECT_3D() : m_pos(XMFLOAT3(0.0f, 0.0f, 0.0f)), m_vec(XMFLOAT3(0.0f, 0.0f, 0.0f)), m_rot(XMFLOAT3(0.0f, 0.0f, 0.0f)),
		m_sca(XMFLOAT3(1.0f, 1.0f, 1.0f)), m_color(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)), m_rad(0.0f) {}

	GAME_OBJECT_3D(GAME* p_game, const std::string& tag, XMFLOAT3 pos, XMFLOAT3 vec, XMFLOAT3 rot, XMFLOAT3 sca, XMFLOAT4 color, float rad) : m_p_game(p_game), m_tag(tag), m_pos(pos), m_vec(vec), m_rot(rot), m_sca(sca), m_color(color), m_rad(rad) {}
	virtual ~GAME_OBJECT_3D() {}

	//セッター
	void SetGame(GAME* p_game) { m_p_game = p_game; };
	void SetTag(const std::string& tag) { m_tag = tag; };
	void SetPos(XMFLOAT3 pos) { m_pos = pos; };
	void SetVec(XMFLOAT3 vec) { m_vec = vec; };
	void SetRot(XMFLOAT3 rot) { m_rot = rot; };
	void SetSca(XMFLOAT3 sca) { m_sca = sca; };
	void SetColor(XMFLOAT4 color) { m_color = color; };
	void SetRad(float rad) { m_rad = rad; };
	void SetColliderIndex(int index) { m_collider_index = index; };
	void SetDestroy(bool destroy) { m_destroy = destroy; };

	//ゲッター
	const std::string& GetTag(void) const { return m_tag; };
	XMFLOAT3 GetPos(void) { return m_pos; };
	XMFLOAT3 GetVec(void) { return m_vec; };
	XMFLOAT3 GetRot(void) { return m_rot; };
	XMFLOAT3 GetSca(void) { return m_sca; };
	XMFLOAT4 GetColor(void) { return m_color; };
	float GetRad(void) { return m_rad; };
	int GetColliderIndex(void) { return m_collider_index; };
	bool IsDestroy(void) const { return m_destroy; };

	virtual void Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
};

class GAME_OBJECT_3D_CONTAINER
{
private:
	GAME_OBJECT_3D** m_p_game_object;
	int m_count = 0;

public:
	GAME_OBJECT_3D_CONTAINER() = default;
	GAME_OBJECT_3D_CONTAINER(int num) : m_count(num)
	{
		m_p_game_object = new GAME_OBJECT_3D * [num];
	}
	~GAME_OBJECT_3D_CONTAINER()
	{
		delete[] m_p_game_object;
	}

	GAME_OBJECT_3D* GetGameObject(int index) const
	{
		return m_p_game_object[index];
	}

	int GetCount(void) const { return m_count; };

	void SetGameObject(int index, GAME_OBJECT_3D* p_object)
	{
		m_p_game_object[index] = p_object;
	}
};