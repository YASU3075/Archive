//-----------------------------------------------
//Camera.cpp
// 改変日：2024/12/10
// 改変者：安田晴人
//-----------------------------------------------
#include	"Camera.h"
#include	"keyboard.h"
#include "mouse.h"



//-----------------------------------------------
// グローバル変数
//-----------------------------------------------
Mouse_State g_mouse;

void CAMERA::Init()
{
	//カメラの初期化
	//m_pos = XMFLOAT3(0.0f, 3.0f, -5.0f);
	m_up_vec = XMFLOAT3(0.0f, 1.0f, 0.0f);
	//m_at_pos = XMFLOAT3(1.0f, 3.0f, 1.0f);
	m_fov = 45.0f;
	m_nearclip = 0.5f;
	m_farclip = 1000.0f;

	m_at_pos_offset.x = m_at_pos.x - m_pos.x;
	m_at_pos_offset.y = m_at_pos.y - m_pos.y;
	m_at_pos_offset.z = m_at_pos.z - m_pos.z;
	m_at_pos_angle = XMFLOAT3(0.0f, 0.0f, 0.0f);

	ZeroMemory(&g_mouse, sizeof(Mouse_State));
}

void CAMERA::Uninit()
{

}

void CAMERA::Update()
{
	//マウス情報取得
	Mouse_GetState(&g_mouse);
	Mouse_ResetScrollWheelValue();

	//ベクトル演算用の領域へデータを転送
	XMVECTOR Fwd, Rit, Up;
	XMFLOAT3 temp = XMFLOAT3(m_at_pos_offset.x, 0.0f, m_at_pos_offset.z);
	Fwd = XMLoadFloat3(&temp);
	Up = XMLoadFloat3(&m_up_vec);
	Rit = XMVector3Cross(Up, Fwd);		//Rit = Up X Fwdを計算

	Rit = XMVector3Normalize(Rit);		//正規化して長さを1.0にする
	XMStoreFloat3(&m_rit_vec, Rit);	//構造体の変数へ転送
	Fwd = XMVector3Normalize(Fwd);		//正規化して長さを1.0にする
	XMStoreFloat3(&m_fwd_vec, Fwd);	//構造体の変数へ転送

	m_at_pos_offset.x = m_at_pos.x - m_pos.x;
	m_at_pos_offset.y = m_at_pos.y - m_pos.y;
	m_at_pos_offset.z = m_at_pos.z - m_pos.z;

	//注視点を固定してカメラの位置を回転
	XMMATRIX rotm;		//今回の状態変化量をからの表す行列
	rotm = XMMatrixIdentity();
	XMVECTOR vec;
	XMFLOAT3 v(0.0f, 1.0f, 0.0f);	//外積用アップベクトル
	vec = XMLoadFloat3(&v);

	float rot = 0.0f;
	float angle = 0.0f;

	if (fabsf(g_mouse.x) < fabsf(g_mouse.y))	//移動量の大きいほうを優先
	{//カメラを上下回転
		angle = LOOKAT_ROTATE * g_mouse.y;	//回転角度を作る
		m_at_pos_angle.x += angle;
		if (m_at_pos_angle.x > 30.0f)
		{//角度制限
			m_at_pos_angle.x = 30.0f;
			angle = 0.0f;
		}

		else if (m_at_pos_angle.x < -40.0f)
		{//角度制限
			m_at_pos_angle.x = -40.0f;
			angle = 0.0f;
		}

		vec = XMLoadFloat3(&m_rit_vec);
		rot = XMConvertToRadians(angle);
	}

	else if (fabsf(g_mouse.x) > fabsf(g_mouse.y))
	{//カメラを左右回転
		angle = LOOKAT_ROTATE * g_mouse.x;
		m_at_pos_angle.y += LOOKAT_ROTATE * g_mouse.x;
		vec = XMLoadFloat3(&v);
		rot = XMConvertToRadians(angle);
	}

	rotm = XMMatrixRotationAxis(vec, rot);	//任意軸による回転行列作成

	m_at_pos_offset.x = m_at_pos.x - m_pos.x;
	m_at_pos_offset.y = m_at_pos.y - m_pos.y;
	m_at_pos_offset.z = m_at_pos.z - m_pos.z;

	//計算用の領域に視線ベクトルを格納
	XMVECTOR eyev;
	eyev = XMLoadFloat3(&m_at_pos_offset);
	//視線ベクトルの回転（ベクトルは反転して使う）
	eyev *= -1.0f;
	eyev = XMVector3TransformCoord(eyev, rotm);

	XMFLOAT3 tempv;
	XMStoreFloat3(&tempv, eyev);	//回転後のカメラの位置（視線）ベクトル
	eyev *= -1.0f;	//ベクトルの向きを戻す
	XMStoreFloat3(&m_at_pos_offset, eyev);	//現在の視線ベクトル保存

	//回転後の視線ベクトル（反転）に注視点を加算して回転後のカメラ座標へ更新する
	m_pos.x = tempv.x + m_at_pos.x;
	m_pos.y = tempv.y + m_at_pos.y;
	m_pos.z = tempv.z + m_at_pos.z;

	//ホイールを動かしたらズームアップかズームアウト
	if (g_mouse.scrollWheelValue > 0)
	{
		m_fovbuffer += FOV_SPEED;
	}

	else if (g_mouse.scrollWheelValue < 0)
	{
		m_fovbuffer -= FOV_SPEED;
	}

	m_fovbuffer *= 0.95f;
	m_fov += m_fovbuffer;
	if (m_fov < FOV_MIN)
	{
		m_fov = FOV_MIN;
	}

	else if (m_fov > FOV_MAX)
	{
		m_fov = FOV_MAX;
	}

	/*m_pos.x += m_velocity.x;
	m_pos.y += m_velocity.y;
	m_pos.z += m_velocity.z;

	m_at_pos.x += m_velocity.x;
	m_at_pos.y += m_velocity.y;
	m_at_pos.z += m_velocity.z;*/

}
void CAMERA::Draw()
{
	//プロジェクション行列を作成
	XMMATRIX	ProjectionMatrix =
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(m_fov),
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
			m_nearclip,
			m_farclip
		);
	//DirectXへセット
	SetProjectionMatrix(ProjectionMatrix);

	//カメラ行列を作成
	XMVECTOR	eyev = XMLoadFloat3(&m_at_pos);
	XMVECTOR	pos = XMLoadFloat3(&m_pos);
	XMVECTOR	up = XMLoadFloat3(&m_up_vec);

	XMMATRIX	ViewMatrix =
		XMMatrixLookAtLH(pos, eyev, up);

	//行列をセット
	SetViewMatrix(ViewMatrix);
}