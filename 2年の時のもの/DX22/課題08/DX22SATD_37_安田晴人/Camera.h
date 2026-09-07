//-----------------------------------------------
//Camera.h
// 改変日：2024/12/10
// 改変者：安田晴人
//-----------------------------------------------
#pragma once

#include "main.h"
#include "renderer.h"

//-----------------------------------------------
// マクロ定義
//-----------------------------------------------
#define	CAMERA_SPEED	(0.25f)
#define	LOOKAT_SPEED	(0.5f)
#define LOOKAT_ROTATE	(0.5f)
#define FOV_SPEED		(0.1f)
#define FOV_MAX			(30.0f)
#define FOV_MIN			(10.0f)

class CAMERA
{
private:
	XMFLOAT3 m_pos;		//カメラの座標
	XMFLOAT3 m_at_pos;		//カメラの注視点
	XMFLOAT3 m_up_vec;		//上方ベクトル
	float m_fov;			//視野角
	float m_nearclip;		//どこまで近くが見えるか
	float m_farclip;		//どこまで遠くが見えるか

	XMFLOAT3 m_at_pos_offset;		//注視点との差分ベクトル
	XMFLOAT3 m_at_pos_angle;			//回転角度(Y軸)

	XMFLOAT3 m_fwd_vec;				//前方ベクトル
	XMFLOAT3 m_rit_vec;				//右方向ベクトル

	float m_fovbuffer;
	XMFLOAT3 m_velocity;			//移動ベクトル

public:
	CAMERA() = default;
	CAMERA(XMFLOAT3 pos, XMFLOAT3 at_pos) : m_pos(pos), m_at_pos(at_pos) {}
	~CAMERA() = default;

	void Init();
	void Uninit();
	void Update();
	void Draw();

	//セッター
	void SetPos(XMFLOAT3 pos) { m_pos = pos; };
	void SetAtPos(XMFLOAT3 at_pos) { m_at_pos = at_pos; };
	void SetUpVec(XMFLOAT3 up_vec) { m_up_vec = up_vec; };
	void SetFov(float fov) { m_fov = fov; };
	void SetNearclip(float nearclip) { m_nearclip = nearclip; };
	void SetFarclip(float farclip) { m_farclip = farclip; };
	void SetAtPosOffset(XMFLOAT3 at_pos_offset) { m_at_pos_offset = at_pos_offset; };
	void SetAtPosAngle(XMFLOAT3 at_pos_angle) { m_at_pos_angle = at_pos_angle; };
	void SetFwdVec(XMFLOAT3 fwd_vec) { m_fwd_vec = fwd_vec; };
	void SetRitVec(XMFLOAT3 rit_vec) { m_rit_vec = rit_vec; };
	void SetVelocity(XMFLOAT3 velocity) { m_velocity = velocity; };

	XMFLOAT3 GetPos(void) const { return m_pos; };
	XMFLOAT3 GetAtPos(void) const { return m_at_pos; };
	XMFLOAT3 GetUpVec(void) const { return m_up_vec; };
	float GetFov(void) const { return m_fov; };
	float GetNearclip(void) const { return m_nearclip; };
	float GetFarclip(void) const { return m_farclip; };
	XMFLOAT3 GetAtPosOffset(void) const { return m_at_pos_offset; };
	XMFLOAT3 GetAtPosAngle(void) const { return m_at_pos_angle; };
	XMFLOAT3 GetFwdVec(void) const { return m_fwd_vec; };
	XMFLOAT3 GetRitVec(void) const { return m_rit_vec; };
	XMFLOAT3 GetVelocity(void) const { return m_velocity; };
};