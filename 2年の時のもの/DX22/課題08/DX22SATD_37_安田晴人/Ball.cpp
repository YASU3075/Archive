//-----------------------------------------------
// Ball.cpp
// 制作日：2024/11/20
// 制作者：安田晴人
//-----------------------------------------------
#include "Ball.h"
#include "model.h"
#include "keyboard.h"
#include "Camera.h"
#include "Sprite3D.h"

//-----------------------------------------------
// マクロ定義
//-----------------------------------------------
#define BALL_VELOCITY		(0.01f)
#define BALL_RADIUS			(0.2f)
#define BALL_START_POS_X	(0.0f)
#define BALL_START_POS_Y	(BALL_RADIUS + 0.5f)
#define BALL_START_POS_Z	(0.0f)


//-----------------------------------------------
// グローバル変数
//-----------------------------------------------
static ID3D11Buffer* g_vertex_buffer = NULL;
static ID3D11Buffer* g_index_buffer = NULL;
static ID3D11ShaderResourceView* g_texture;


MODEL* g_ball_model;

static LIGHT g_light;		//ライト構造体

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void BALL::Init(void)
{
	g_ball_model = ModelLoad("asset\\model\\ball.fbx");

	g_light.Direction = XMFLOAT4(1.0f, -1.0f, 1.0f, 0.0f);	
	g_light.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);		//色
	g_light.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);		//環境光
	XMVECTOR vec = XMLoadFloat4(&g_light.Direction);
	vec = XMVector4Normalize(vec);
	XMStoreFloat4(&g_light.Direction, vec);
}

//-----------------------------------------------
// 終了処理
//-----------------------------------------------
void BALL::Uninit(void)
{
	ModelRelease(g_ball_model);
	if (g_vertex_buffer != NULL)
	{
		g_vertex_buffer->Release();
		g_vertex_buffer = NULL;
	}

	if (g_index_buffer != NULL)
	{
		g_index_buffer->Release();
		g_index_buffer = NULL;
	}

	if (g_texture != NULL)
	{
		g_texture->Release();
		g_texture = NULL;
	}

}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void BALL::Update(void)
{
	
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void BALL::Draw(void)
{
	DrawSpriteModel(m_pos, m_rot, m_sca, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), g_ball_model);
}