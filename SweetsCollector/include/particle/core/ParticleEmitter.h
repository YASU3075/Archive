//-----------------------------------------------
// ParticleEmitter.h
// 制作日：2025/06/26
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include <random>
#include <vector>
#include "math/Vector3.h"
#include "particle/shape/EmitterShape.h"

class PARTICLE;

class BURST_EVENT
{
public:
	float m_time = 0.0f;	//起動時間
	int m_count = 0;		//放出する数
};

class PARTICLE_EMITTER
{
private:
	float m_time_accumlator = 0.0f;								//時間蓄積
	float m_total_elapsed = 0.0f;								//全体経過時間
	size_t m_next_burst_index = 0;								//次のバーストイベントの番号

	void Emit(PARTICLE& p);												//放出
	float RandomRange(float min, float max);							//乱数
	VECTOR3 RandomVector(const VECTOR3& base, const VECTOR3& range);	//乱数
	XMFLOAT4 RandomColor(const XMFLOAT4& min, const XMFLOAT4& max);		//乱数

	std::mt19937 m_rng{ std::random_device{}() };

public:
	VECTOR3 m_pos = { 0.0f, 0.0f, 0.0f };						//座標

	float m_min_speed = 0.0f;									//最小速度
	float m_max_speed = 0.0f;									//最大速度

	VECTOR3 m_base_vel = { 0.0f, 0.0f, 0.0f };					//基礎速度
	VECTOR3 m_max_vel = { 0.0f, 0.0f, 0.0f };					//最大速度

	float m_emission_rate = 1.0f;								//1秒当たりの発生数
	float m_particle_lifetime = 1.0f;							//パーティクルの寿命
	VECTOR3 m_size_min = { 1.0f, 1.0f, 1.0f };					//最小サイズ
	VECTOR3 m_size_max = { 1.0f, 1.0f, 1.0f };					//最大サイズ
	XMFLOAT4 m_start_color_min = { 1.0f, 1.0f, 1.0f, 1.0f };	//最小の色
	XMFLOAT4 m_start_color_max = { 1.0f, 1.0f, 1.0f, 1.0f };	//最大の色

	float m_start_delay = 0.0f;									//遅延
	float m_dulation = 1.0f;									//効果時間
	bool m_looping = false;										//ループ
	bool m_is_playing = false;									//プレイ中

	std::vector<BURST_EVENT> m_burst_events;					//バーストイベント

	std::unique_ptr<EMITTER_SHAPE> m_shape;						//放出する形

	void Update(float delta_time, std::vector<PARTICLE>& particles);	//更新
	void Play();														//再生
	void Stop();														//停止
};
