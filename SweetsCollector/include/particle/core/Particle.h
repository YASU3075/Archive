//-----------------------------------------------
// Particle.h
// 制作日：2025/06/26
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include "math/Vector3.h"

class PARTICLE
{
private:
	
public:
	VECTOR3 m_pos = { 0.0f, 0.0f, 0.0f };					//座標
	VECTOR3 m_vel = { 0.0f, 0.0f, 0.0f };					//速度
	VECTOR3 m_acc = { 0.0f, 0.0f, 0.0f };					//加速度

	float m_rot = 0.0f;										//角度
	float m_angular_velocity = 0.0f;						//角速度

	VECTOR3 m_sca = { 1.0f, 1.0f, 1.0f };					//サイズ
	VECTOR3 m_initial_sca = { 1.0f, 1.0f, 1.0f };			//初期サイズ

	XMFLOAT4 m_color = { 1.0f, 1.0f, 1.0f, 1.0f };			//色
	XMFLOAT4 m_initial_color = { 1.0f, 1.0f, 1.0f, 1.0f};	//初期色
	
	float m_life_time = 1.0f;								//寿命
	float m_age = 0.0f;										//年齢

	int m_tile_x = 1;										//タイル数X
	int m_tile_y = 1;										//タイル数Y
	int m_sprite_index = 0;									//アニメーション用
	float m_frame_over_life = 0.0f;							//フレームの寿命に対する割合

	float m_custom_data[4] = { 0.0f, 0.0f, 0.0f, 0.0f };	//カスタムデータ

	bool m_sub_emitted = false;					//サブエミッターが発火したかどうか

	bool m_alive = true;

	void Init();
	void Update(float delta_time)
	{
		if (!m_alive)
		{//生きてなかったら
			return;
		}

		//時間加算
		m_age += delta_time;
		if (m_age >= m_life_time)
		{//寿命を超えたら
			m_alive = false;
			return;
		}

		//運動方程式
		m_vel += m_acc * delta_time;
		m_pos += m_vel * delta_time;
		m_rot += m_angular_velocity * delta_time;

		//
		m_frame_over_life = m_age / m_life_time;
	}

	void Draw() const;
	

	
};
