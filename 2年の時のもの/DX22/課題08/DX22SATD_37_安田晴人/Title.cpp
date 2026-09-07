//-----------------------------------------------
// Title.h
// 制作日：2024/12/10
// 制作者：安田晴人
//-----------------------------------------------
#include "Title.h"
#include "sprite.h"
#include "Sprite3D.h"
#include "keyboard.h"
#include "SceneManager.h"
#include "Easing.h"

void	debugSprite(XMFLOAT3 position, float rotate, XMFLOAT2 size, XMFLOAT4 color)
{
	//	//平行移動行列の作成（表示座標を決める）
	XMMATRIX	TranslationMatrix = XMMatrixTranslation(
		position.x, position.y, 0.0f);

	//回転行列（Z回転）行列の作成
	XMMATRIX	RotationMatrix = XMMatrixRotationZ(XMConvertToRadians(rotate));

	//スケーリング行列作成（倍率1.0が等倍、0倍はダメ！）
	XMMATRIX	ScalingMatrix = XMMatrixIdentity();

	//ワールド行列の作成（ポリゴンの表示の仕方を指定する最終的な行列
	XMMATRIX	WorldMatrix = ScalingMatrix * RotationMatrix * TranslationMatrix;

	//ワールド行列をDirectXへセット
	SetWorldMatrix(WorldMatrix);

	DrawSprite(size, color);
}

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void TITLE::Init(void)
{
	m_p_camera = new CAMERA;
	m_p_camera->Init();

	//テクスチャの読み込み
	TexMetadata		metadata1;
	ScratchImage	image1;

	LoadFromWICFile(L"asset\\texture\\enter.png", WIC_FLAGS_NONE, &metadata1, image1);
	CreateShaderResourceView(GetDevice(), image1.GetImages(), image1.GetImageCount(), metadata1, &m_texture_enter);
	assert(m_texture_enter);

	TexMetadata		metadata;
	ScratchImage	image;

	LoadFromWICFile(L"asset\\texture\\title.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_texture);
	assert(m_texture);

	m_p_light = new LIGHT();

	//ライト構造体の初期化
	m_p_light->Direction = XMFLOAT4(1.0f, -1.0f, 1.0f, 0.0f);
	m_p_light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_p_light->Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	XMVECTOR vec = XMLoadFloat4(&m_p_light->Direction);
	vec = XMVector4Normalize(vec);
	XMStoreFloat4(&m_p_light->Direction, vec);

	TimeReset(0);
	TimeReset(1);
	TimeReset(2);
}

//-----------------------------------------------
// 終了処理
//-----------------------------------------------
void TITLE::Uninit(void)
{
	m_p_camera->Uninit();
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void TITLE::Update(void)
{
	m_frame_count++;
	if (m_frame_count >= 360.0f)
	{
		m_frame_count = 0;
		TimeReset(0);
		TimeReset(1);
		TimeReset(2);
		m_easing = true;
	}

	if (Keyboard_IsKeyDownTrigger(KK_ENTER))
	{
		m_p_scene_manager->ChangeScene(SCENE_GAME);
	}
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void TITLE::Draw(void)
{
	SetWorldViewProjection2D();
	SetDepthEnable(false);

	
	easeOutBounce(1.0f / 360.0f, 0);
	float sca = GetEasing(0).posi.y;
	
	easeInOutBounce(1.0f / 360.0f, 2);
	float pos = GetEasing(2).posi.y;

	easeInOutElastic(1.0f / 360.0f, 1);
	float alpha = GetEasing(1).posi.y;
	if (m_easing)
	{
		sca = 1.0f;
		pos = 1.0f;
		alpha = 1.0f;
	}

	GetDeviceContext()->PSSetShaderResources(0, 1, &m_texture);
	debugSprite(XMFLOAT3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f * pos, 0.0f), 0.0f, XMFLOAT2(1600.0f * sca, 200.0f * sca), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	GetDeviceContext()->PSSetShaderResources(0, 1, &m_texture_enter);
	debugSprite(XMFLOAT3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f + 300.0f, 0.0f), 0.0f, XMFLOAT2(1000.0f, 200.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));
}