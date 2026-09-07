//-----------------------------------------------
// Player.cpp
// 制作日：2024/12/27
// 制作者：安田晴人
//-----------------------------------------------
#include "Player.h"
#include "Primitive3D.h"
#include "keyboard.h"
#include "mouse.h"
#include "Sprite3D.h"
#include "Camera.h"
#include "Game.h"
#include "Bullet.h"
#include "ColliderManager.h"
#include "SphereCollider.h"

//-----------------------------------------------
// マクロ定義
//-----------------------------------------------


//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void PLAYER::Init(void)
{
	//テクスチャの読み込み
	TexMetadata		metadata;
	ScratchImage	image;

	LoadFromWICFile(L"asset\\texture\\mapchip.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_texture);
	assert(m_texture);

	//モデル読み込み
	m_p_model = ModelLoad("asset\\model\\ball.fbx");

	//カメラ調整
	m_p_camera->SetPos(XMFLOAT3(m_pos.x, m_pos.y + 5.0f, m_pos.z - 10.0f));
	m_p_camera->SetAtPos(XMFLOAT3(m_pos.x, m_pos.y, m_pos.z));
}

//-----------------------------------------------
// 終了処理
//-----------------------------------------------
void PLAYER::Uninit(void)
{
	if (m_texture != NULL)
	{
		m_texture->Release();
		m_texture = NULL;
	}

	ModelRelease(m_p_model);
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void PLAYER::Update(void)
{
	
	

	XMFLOAT3 at_pos_offset = m_p_camera->GetAtPosOffset();
	XMFLOAT3 up_vec = m_p_camera->GetUpVec();
	XMFLOAT3 rit_vec = m_p_camera->GetRitVec();
	XMFLOAT3 fwd_vec = m_p_camera->GetFwdVec();
	XMFLOAT3 at_pos = m_p_camera->GetAtPos();
	XMFLOAT3 at_pos_angle = m_p_camera->GetAtPosAngle();
	XMFLOAT3 cam_pos = m_p_camera->GetPos();

	//ベクトル演算用の領域へデータを転送
	XMVECTOR Fwd, Rit, Up;
	XMFLOAT3 temp = XMFLOAT3(at_pos_offset.x, 0.0f, at_pos_offset.z);
	Fwd = XMLoadFloat3(&temp);
	Up = XMLoadFloat3(&up_vec);
	Rit = XMVector3Cross(Up, Fwd);		//Rit = Up X Fwdを計算

	Rit = XMVector3Normalize(Rit);		//正規化して長さを1.0にする
	XMStoreFloat3(&rit_vec, Rit);	//構造体の変数へ転送
	Fwd = XMVector3Normalize(Fwd);		//正規化して長さを1.0にする
	XMStoreFloat3(&fwd_vec, Fwd);	//構造体の変数へ転送


	if (Keyboard_IsKeyDown(KK_W))
	{
		m_pos.x += fwd_vec.x * CAMERA_SPEED;
		m_pos.y += fwd_vec.y * CAMERA_SPEED;
		m_pos.z += fwd_vec.z * CAMERA_SPEED;
		cam_pos.x += fwd_vec.x * CAMERA_SPEED;
		cam_pos.y += fwd_vec.y * CAMERA_SPEED;
		cam_pos.z += fwd_vec.z * CAMERA_SPEED;
		at_pos.x += fwd_vec.x * CAMERA_SPEED;
		at_pos.y += fwd_vec.y * CAMERA_SPEED;
		at_pos.z += fwd_vec.z * CAMERA_SPEED;

		//m_pos.z += CAMERA_SPEED;	//カメラの座標を変更
		//m_at_pos.z += CAMERA_SPEED;	//カメラの注視点を変更
	}
	if (Keyboard_IsKeyDown(KK_S))
	{
		m_pos.x += fwd_vec.x * -CAMERA_SPEED;
		m_pos.y += fwd_vec.y * -CAMERA_SPEED;
		m_pos.z += fwd_vec.z * -CAMERA_SPEED;
		cam_pos.x += fwd_vec.x * -CAMERA_SPEED;
		cam_pos.y += fwd_vec.y * -CAMERA_SPEED;
		cam_pos.z += fwd_vec.z * -CAMERA_SPEED;
		at_pos.x += fwd_vec.x * -CAMERA_SPEED;
		at_pos.y += fwd_vec.y * -CAMERA_SPEED;
		at_pos.z += fwd_vec.z * -CAMERA_SPEED;

		/*m_pos.z += -CAMERA_SPEED;
		m_at_pos.z += -CAMERA_SPEED;*/
	}


	//---------------------------------------------------------------------------------------------------------

	if (Keyboard_IsKeyDown(KK_A))
	{
		m_pos.x += rit_vec.x * -CAMERA_SPEED;
		m_pos.y += rit_vec.y * -CAMERA_SPEED;
		m_pos.z += rit_vec.z * -CAMERA_SPEED;
		cam_pos.x += rit_vec.x * -CAMERA_SPEED;
		cam_pos.y += rit_vec.y * -CAMERA_SPEED;
		cam_pos.z += rit_vec.z * -CAMERA_SPEED;
		at_pos.x += rit_vec.x * -CAMERA_SPEED;
		at_pos.y += rit_vec.y * -CAMERA_SPEED;
		at_pos.z += rit_vec.z * -CAMERA_SPEED;

		/*m_pos.x += -CAMERA_SPEED;
		m_at_pos.x += -CAMERA_SPEED;*/
	}

	if (Keyboard_IsKeyDown(KK_D))
	{
		m_pos.x += rit_vec.x * CAMERA_SPEED;
		m_pos.y += rit_vec.y * CAMERA_SPEED;
		m_pos.z += rit_vec.z * CAMERA_SPEED;
		cam_pos.x += rit_vec.x * CAMERA_SPEED;
		cam_pos.y += rit_vec.y * CAMERA_SPEED;
		cam_pos.z += rit_vec.z * CAMERA_SPEED;
		at_pos.x += rit_vec.x * CAMERA_SPEED;
		at_pos.y += rit_vec.y * CAMERA_SPEED;
		at_pos.z += rit_vec.z * CAMERA_SPEED;

		/*m_pos.x -= -CAMERA_SPEED;
		m_at_pos.x -= -CAMERA_SPEED;*/
	}

	if (Keyboard_IsKeyDown(KK_UP))
	{
		m_pos.y += CAMERA_SPEED;
		cam_pos.y += CAMERA_SPEED;
		at_pos.y += CAMERA_SPEED;
	}

	if (Keyboard_IsKeyDown(KK_DOWN))
	{
		m_pos.y += -CAMERA_SPEED;
		cam_pos.y += -CAMERA_SPEED;
		at_pos.y += -CAMERA_SPEED;
	}

	m_p_camera->SetAtPos(m_pos);
	m_p_camera->SetPos(cam_pos);

	Mouse_State mouse;
	Mouse_GetState(&mouse);

	
	if (!mouse.leftButton)
	{
		m_old_click = false;
	}

	if (!m_old_click && mouse.leftButton)
	{
		GAME_OBJECT_3D* p_bullet = new BULLET(m_p_game, m_pos, fwd_vec, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 0.5f);
		p_bullet->Init();
		m_p_game->Register(p_bullet);
		p_bullet->SetColliderIndex(m_p_collider_manager->Register(new SPHERE_COLLIDER(p_bullet, p_bullet->GetTag())));
		m_old_click = true;
	}

	


	/*GAME_OBJECT_3D_CONTAINER objects =  m_p_game->GetGameObjectContainer(m_collider_index);
	for (int i = 0; i < objects.GetCount(); i++)
	{
		
		if (objects.GetGameObject(i)->GetTag() == "ENEMY")
		{
			m_color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		}
	}*/

}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void PLAYER::Draw(void)
{
	DrawSpriteModel(m_pos, m_rot, m_sca, m_color, m_p_model);
}
