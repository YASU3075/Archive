//-----------------------------------------------
// polygon.cpp
// 制作日：2024/05/13
//-----------------------------------------------
#include "polygon.h"
#include <time.h>

//-----------------------------------------------
// マクロ定義
//-----------------------------------------------
#define NUM_VERTEX		(4)	//必要な頂点の数		//６Pチーズの書き方だと2n+1
#define MAX_VERTEX		(4)	//頂点の最大数
#define SIZE_X			(10)		//横幅
#define SIZE_Y			(20)		//高さ
#define MAX_TEXTURE		(10)		//テクスチャーの最大数

#define TEXTURE_X_NUM		(4)
#define TEXTURE_Y_NUM		(1)
#define DIVISION_X_NUM	(8.0f)			//X方向の分割数
#define DIVISION_Y_NUM	(1.0f)			//Y方向の分割数

#define ANIMATION_FRAME	(3)

//-----------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------
void SetVertex(void);


//-----------------------------------------------
// グローバル変数
//-----------------------------------------------
static ID3D11Buffer		*g_VertexBuffer = NULL;		//線用の頂点情報
static ID3D11ShaderResourceView* g_Texture = NULL;

struct VERTEX_T
{
	XMFLOAT2		pos;	//位置ベクトル
	XMFLOAT2		vel;	//速度ベクトル
	XMFLOAT4		color;	//頂点カラー
	XMFLOAT2		texCoord;	//テクスチャ座標
};

static VERTEX_T g_Vertex[MAX_VERTEX];
static double g_Angle;	//角度
static bool g_isSpin;		//回転させるかどうか
static int g_FrameCntX;	//フレームカウンター
static XMFLOAT2 g_Size;


//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void InitPolygon(void)
{
	ID3D11Device* pDevice = GetDevice();

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * MAX_VERTEX;		//確保するバッファの大きさ
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	//回転設定
	g_isSpin = false;

	g_Size.x = 1.0f;
	g_Size.y = 1.0f;


	//初期化
	{
		//角度設定
		double angle = -45 * M_PI / 180;
		
		//1つ目の四角形
		{
			for (int i = 0; i < (int)((NUM_VERTEX + 1) / 2); i++)
			{//偶数番目
				double radian = i * M_PI * 2 / NUM_VERTEX + angle;

				g_Vertex[i * 2].pos.x = SIZE_X * cos(radian) + 0.0f;
				g_Vertex[i * 2].pos.y = SIZE_Y * sin(radian) + 0.0f;
				g_Vertex[i * 2].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			}

			for (int i = 0; i < (int)((NUM_VERTEX + 1) / 2); i++)
			{//奇数番目
				double radian = M_PI * 2 - ((i + 1) * M_PI * 2 / NUM_VERTEX) + angle;

				g_Vertex[i * 2 + 1].pos.x = SIZE_X * cos(radian) + 0.0f;
				g_Vertex[i * 2 + 1].pos.y = SIZE_Y * sin(radian) + 0.0f;
				g_Vertex[i * 2 + 1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			}

			g_Vertex[0].texCoord = XMFLOAT2(1.0f, 0.0f);
			g_Vertex[1].texCoord = XMFLOAT2(0.0f, 0.0f);
			g_Vertex[2].texCoord = XMFLOAT2(1.0f, 1.0f);
			g_Vertex[3].texCoord = XMFLOAT2(0.0f, 1.0f);
		}

	}
	
	g_FrameCntX = 0;

	//頂点バッファ更新
	SetVertex();

	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	
	LoadFromWICFile(L"asset\\texture\\runningman000.png" ,WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);
}

//-----------------------------------------------
// 終了処理
//-----------------------------------------------
void UninitPolygon(void)
{
	//頂点バッファの解放
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	//テクスチャーの解放
	g_Texture->Release();
	g_Texture = NULL;
	
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void UpdatePolygon(void)
{

	if (g_isSpin)
	{

		for (int i = 0; i < (int)((NUM_VERTEX + 1) / 2); i++)
		{//偶数番目
			double radian = i * M_PI * 2 / NUM_VERTEX;

			g_Vertex[i * 2].pos.x = SIZE_X * cos(radian + g_Angle) + 0.0f;
			g_Vertex[i * 2].pos.y = SIZE_Y * sin(radian + g_Angle) + 0.0f;
			g_Vertex[i * 2].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}

		for (int i = 0; i < (int)((NUM_VERTEX + 1) / 2); i++)
		{//奇数番目
			double radian = M_PI * 2 - ((i + 1) * M_PI * 2 / NUM_VERTEX);

			g_Vertex[i * 2 + 1].pos.x = SIZE_X * cos(radian + g_Angle) + 0.0f;
			g_Vertex[i * 2 + 1].pos.y = SIZE_Y * sin(radian + g_Angle) + 0.0f;
			g_Vertex[i * 2 + 1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}

		g_Angle += 0.01;
		if (g_Angle > 360.0)
		{
			g_Angle = 0.0;
		}

	}

	//フレーム数増加
	g_FrameCntX++;
	g_FrameCntX = g_FrameCntX % (int)((DIVISION_X_NUM * ANIMATION_FRAME) + 1);
	/*if (g_FrameCntX >= DIVISION_X_NUM * ANIMATION_FRAME)
	{
		g_FrameCntX = 0;
	}*/

	g_Size.x += 0.05f;
	g_Size.y += 0.05f;

	if (g_Size.x >= 30)
	{
		g_Size.x = 0.0f;
		g_Size.y = 0.0f;
	}

	if (g_FrameCntX % 5 == 0)
	{//5フレームごとに
		srand((unsigned int)time(0));
		float color_r = (float)(rand() % 10) / 10.0f;
		float color_g = (float)(rand() % 10) / 10.0f;
		float color_b = (float)(rand() % 10) / 10.0f;

		for (int i = 0; i < NUM_VERTEX; i++)
		{
			g_Vertex[i].color = XMFLOAT4(color_r, color_g, color_b, 1.0f);
		}

	}

	SetVertex();
	
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void DrawPolygon(void)
{
	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//マトリクス設定
	//SetWorldViewProjection2D();	//座標の2D変換

	//プロジェクションマトリクス設定
	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	SetProjectionMatrix(projection);

	//ビューマトリクス設定
	XMMATRIX view;
	view = XMMatrixIdentity();
	SetViewMatrix(view);


	//移動・回転マトリクス設定
	XMMATRIX world, scale, rot, trans;
	rot = XMMatrixRotationZ(0.0f);	//ラジアン角
	scale = XMMatrixScaling(g_Size.x, g_Size.y, 1.0f);
	trans = XMMatrixTranslation(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	world = scale * rot * trans;
	SetWorldMatrix(world);

	//プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//マテリアル設定（半年後に現れる）
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);


	//テクスチャーの座標設定
	g_Vertex[0].texCoord = XMFLOAT2((1.0f / DIVISION_X_NUM) * (g_FrameCntX / ANIMATION_FRAME - 1), (1.0f / DIVISION_Y_NUM) * (TEXTURE_Y_NUM - 1));
	g_Vertex[1].texCoord = XMFLOAT2((1.0f / DIVISION_X_NUM) * (g_FrameCntX / ANIMATION_FRAME), (1.0f / DIVISION_Y_NUM) * (TEXTURE_Y_NUM - 1));
	g_Vertex[2].texCoord = XMFLOAT2((1.0f / DIVISION_X_NUM) * (g_FrameCntX / ANIMATION_FRAME - 1), (1.0f / DIVISION_Y_NUM) * (TEXTURE_Y_NUM));
	g_Vertex[3].texCoord = XMFLOAT2((1.0f / DIVISION_X_NUM) * (g_FrameCntX / ANIMATION_FRAME), (1.0f / DIVISION_Y_NUM) * (TEXTURE_Y_NUM));


	SetVertex();

	//テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);
	//ポリゴン描画
	GetDeviceContext()->Draw(NUM_VERTEX, 0);

	
}



//-----------------------------------------------
// 頂点データ設定
//-----------------------------------------------
void SetVertex(void)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	//頂点バッファのデータを更新する
	for (int i = 0; i < MAX_VERTEX; i++)
	{
		vertex[i].Position = XMFLOAT3(g_Vertex[i].pos.x, g_Vertex[i].pos.y, 0.0f);
		vertex[i].Diffuse = g_Vertex[i].color;
		vertex[i].TexCoord = g_Vertex[i].texCoord;
	}

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);
}


//g_AnimPattern = g_AnimPatttern % 8;
