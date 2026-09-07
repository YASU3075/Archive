//-----------------------------------------------
// Game.h
// 制作日：2024/12/10
// 制作者：安田晴人
//-----------------------------------------------
#include "Game.h"
#include "GameObject3D.h"
#include "Ball.h"
#include <string>
#include "Player.h"
#include "Enemy.h"
#include "ColliderManager.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "SceneManager.h"
#include "keyboard.h"
#include "SkyBall.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void GAME::Init(void)
{
	m_p_collider_manager = new COLLIDER_MANAGER();

	m_p_collider_manager->Init();

	for (int i = 0; i < MAX_GAMEOBJECT_NUM; i++)
	{
		m_p_game_object[i] = nullptr;
	}

	m_p_camera[0] = new CAMERA(XMFLOAT3(0.0f, 3.0f, -5.0f), XMFLOAT3(1.0f, 3.0f, 1.0f));
	m_p_camera[1] = new CAMERA(XMFLOAT3(50.0f, 30.0f, 50.0f), XMFLOAT3(0.0f, -10.0f, 0.0f));
	m_p_camera[2] = new CAMERA(XMFLOAT3(-1.0f, 5.0f, 50.0f), XMFLOAT3(10.0f, 0.0f, 0.0f));
	

	m_p_camera[0]->Init();
	m_p_camera[1]->Init();
	m_p_camera[2]->Init();
	
	

	m_p_light = new LIGHT();

	m_p_map = new MAP();

	m_p_map->Init();

	GAME_OBJECT_3D* player = new PLAYER(this, m_p_camera[0], m_p_collider_manager, "PLAYER", XMFLOAT3(1.0f, 3.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), 0.0f);
	GAME_OBJECT_3D* enemy = new ENEMY(this, "ENEMY", XMFLOAT3(10.0f, 2.0f, 40.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), 1.0f);
	GAME_OBJECT_3D* enemy_2 = new ENEMY(this, "ENEMY", XMFLOAT3(30.0f, 2.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), 1.0f);
	GAME_OBJECT_3D* enemy_3 = new ENEMY(this, "ENEMY", XMFLOAT3(50.0f, 2.0f, 30.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), 1.0f);
	GAME_OBJECT_3D* enemy_4 = new ENEMY(this, "ENEMY", XMFLOAT3(40.0f, 2.0f, 20.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), 1.0f);
	GAME_OBJECT_3D* enemy_5 = new ENEMY(this, "ENEMY", XMFLOAT3(20.0f, 2.0f, 50.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), 1.0f);

	this->Register(player);
	this->Register(enemy);
	this->Register(enemy_2);
	this->Register(enemy_3);
	this->Register(enemy_4);
	this->Register(enemy_5);

	player->SetColliderIndex(m_p_collider_manager->Register(new SPHERE_COLLIDER(player, player->GetTag())));
	enemy->SetColliderIndex(m_p_collider_manager->Register(new BOX_COLLIDER(enemy, enemy->GetTag())));
	enemy_2->SetColliderIndex(m_p_collider_manager->Register(new BOX_COLLIDER(enemy_2, enemy_2->GetTag())));
	enemy_3->SetColliderIndex(m_p_collider_manager->Register(new BOX_COLLIDER(enemy_3, enemy_3->GetTag())));
	enemy_4->SetColliderIndex(m_p_collider_manager->Register(new BOX_COLLIDER(enemy_4, enemy_4->GetTag())));
	enemy_5->SetColliderIndex(m_p_collider_manager->Register(new BOX_COLLIDER(enemy_5, enemy_5->GetTag())));

	for (int i = 0; i < MAX_GAMEOBJECT_NUM; i++)
	{
		if (m_p_game_object[i])
		{
			m_p_game_object[i]->Init();
		}
	}

	SKY_BALL_OBJECT* p_sky_ball = GetSkyBall();
	p_sky_ball->p_camera = *m_p_camera;
	InitSkyBall();

	//ライト構造体の初期化
	m_p_light->Direction = XMFLOAT4(1.0f, -1.0f, 1.0f, 0.0f);
	m_p_light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_p_light->Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	XMVECTOR vec = XMLoadFloat4(&m_p_light->Direction);
	vec = XMVector4Normalize(vec);
	XMStoreFloat4(&m_p_light->Direction, vec);
}

//-----------------------------------------------
// 終了処理
//-----------------------------------------------
void GAME::Uninit(void)
{
	m_p_camera[0]->Uninit();
	m_p_camera[1]->Uninit();
	m_p_camera[2]->Uninit();
	

	m_p_map->Uninit();

	m_p_collider_manager->Uninit();

	for (int i = 0; i < MAX_GAMEOBJECT_NUM; i++)
	{
		if (m_p_game_object[i])
		{
			delete m_p_game_object[i];
		}
	}

	UninitSkyBall();
	
}

//-----------------------------------------------
// 登録処理
//-----------------------------------------------
void GAME::Register(GAME_OBJECT_3D* p_object)
{
	for (int i = 0; i < MAX_GAMEOBJECT_NUM; i++)
	{
		if (!m_p_game_object[i])
		{
			m_p_game_object[i] = p_object;
			m_p_game_object[i]->SetGame(this);
			break;
		}
	}
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void GAME::Update(void)
{
	if (Keyboard_IsKeyDownTrigger(KK_D1))
	{
		m_camera_num = 0;
	}

	else if (Keyboard_IsKeyDownTrigger(KK_D2))
	{
		m_camera_num = 1;
	}

	else if (Keyboard_IsKeyDownTrigger(KK_D3))
	{
		m_camera_num = 2;
	}

	m_p_camera[0]->Update();
	
	m_p_map->Update();

	for (int i = 0; i < MAX_GAMEOBJECT_NUM; i++)
	{
		if (m_p_game_object[i])
		{
			m_p_game_object[i]->Update();
		}
	}

	m_p_collider_manager->Update();

	for (int i = 0; i < MAX_GAMEOBJECT_NUM; i++)
	{
		if (m_p_game_object[i])
		{
			if (m_p_game_object[i]->IsDestroy())
			{
				m_p_collider_manager->Destroy(m_p_game_object[i]->GetColliderIndex());
				delete m_p_game_object[i];
				m_p_game_object[i] = nullptr;
			}
		}
	}

	UpdateSkyBall();

	int count = 0;
	for (int i = 0; i < MAX_GAMEOBJECT_NUM; i++)
	{
		if (m_p_game_object[i])
		{
			if (m_p_game_object[i]->GetTag() == "ENEMY")
			{
				count++;
			}
		}
	}

	if (count <= 0)
	{
		m_p_scene_manager->ChangeScene(SCENE_RESULT);
	}
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void GAME::Draw(void)
{
	SetWorldViewProjection3D();
	SetDepthEnable(true);
	SetLight(*m_p_light);

	m_p_camera[m_camera_num]->Draw();
	
	m_p_map->Draw();

	for (int i = 0; i < MAX_GAMEOBJECT_NUM; i++)
	{
		if (m_p_game_object[i])
		{
			m_p_game_object[i]->Draw();
		}
	}

	m_p_collider_manager->Draw();

	DrawSkyBall();
}

//-----------------------------------------------
// tagでオブジェクト取得
//-----------------------------------------------
GAME_OBJECT_3D_CONTAINER GAME::GetGameObjectContainer(std::string tag)
{
	//対象の数を数える
	int count = 0;
	for (int i = 0; i < MAX_GAMEOBJECT_NUM; i++)
	{
		if (m_p_game_object[i] && m_p_game_object[i]->GetTag() == tag)
		{
			count++;
		}
	}
	GAME_OBJECT_3D_CONTAINER ret(count);

	count = 0;

	//ゲームオブジェクトコンテナに対象のポインタを詰める
	for (int i = 0; i < MAX_GAMEOBJECT_NUM; i++)
	{
		if (m_p_game_object[i] && m_p_game_object[i]->GetTag() == tag)
		{
			ret.SetGameObject(count++, m_p_game_object[i]);
		}
	}

	return ret;
}

//-----------------------------------------------
// 当たり判定でオブジェクト取得
//-----------------------------------------------
GAME_OBJECT_3D_CONTAINER GAME::GetGameObjectContainer(int collider_id)
{
	//対象の数を数える
	BASE_COLLIDER* p_collider = m_p_collider_manager->GetCollider(collider_id);

	int count = 0;
	for (int i = 0; i < MAX_GAMEOBJECT_NUM; i++)
	{
		if (m_p_game_object[i] && m_p_collider_manager->GetCollision(*p_collider, *m_p_collider_manager->GetCollider(m_p_game_object[i]->GetColliderIndex())))
		{
			count++;
		}
	}
	GAME_OBJECT_3D_CONTAINER ret(count);

	count = 0;

	//ゲームオブジェクトコンテナに対象のポインタを詰める
	for (int i = 0; i < MAX_GAMEOBJECT_NUM; i++)
	{
		if (m_p_game_object[i] && m_p_collider_manager->GetCollision(*p_collider, *m_p_collider_manager->GetCollider(m_p_game_object[i]->GetColliderIndex())))
		{
			ret.SetGameObject(count++, m_p_game_object[i]);
		}
	}

	return ret;
}
