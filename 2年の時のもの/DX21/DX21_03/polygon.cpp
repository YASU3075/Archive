//-----------------------------------------------
// polygon.cpp
// 制作日：2024/05/13
//-----------------------------------------------
#include "polygon.h"

//-----------------------------------------------
// マクロ定義
//-----------------------------------------------
#define NUM_POLYGON		(5)		//角の数
#define NUM_VERTEX		(NUM_POLYGON * 2 + 1)	//必要な頂点の数
#define SIZE_X			(300)		//横幅
#define SIZE_Y			(300)		//高さ


//-----------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------
void SetVertex(void);


//-----------------------------------------------
// グローバル変数
//-----------------------------------------------
static ID3D11Buffer		*g_VertexBuffer = NULL;		//線用の頂点情報

struct VERTEX_T
{
	XMFLOAT2		pos;	//位置ベクトル
	XMFLOAT2		vel;	//速度ベクトル
	XMFLOAT4		color;	//頂点カラー
};

static VERTEX_T g_Vertex[NUM_VERTEX];
static double angle;	//角度
static bool isSpin;		//回転させるかどうか
static bool isSkip;		//1つ飛ばしするかどうか

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
	bd.ByteWidth = sizeof(VERTEX_3D) * NUM_VERTEX;		//確保するバッファの大きさ
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	//回転設定
	isSpin = false;

	//1つ飛ばし設定
	isSkip = true;

	//初期化

	if (!isSkip)
	{
		for (int i = 0; i < NUM_VERTEX; i++)
		{
			double radian = (i / 2) * M_PI * 2 / ((NUM_VERTEX - 1) / 2);

			if (i % 2 == 0)
			{
				g_Vertex[i].pos.x = SIZE_X * cos(radian) + SCREEN_WIDTH / 2;
				g_Vertex[i].pos.y = SIZE_Y * sin(radian) + SCREEN_HEIGHT / 2;
			}

			else
			{
				g_Vertex[i].pos.x = SCREEN_WIDTH / 2;
				g_Vertex[i].pos.y = SCREEN_HEIGHT / 2;
			}
			g_Vertex[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		}
	}

	else if (NUM_POLYGON >= 5)
	{
		//4以上の多角形で正多角形の頂点1つ飛ばしの図形（5だと星になる）（偶数だと÷２した多角形になる）
		for (int i = 0; i < NUM_VERTEX; i++)
		{
			double radian = i * M_PI * 2 / ((NUM_VERTEX - 1) / 2);

			if (i % 2 == 0)
			{
				g_Vertex[i].pos.x = SIZE_X * cos(radian) + SCREEN_WIDTH / 2;
				g_Vertex[i].pos.y = SIZE_Y * sin(radian) + SCREEN_HEIGHT / 2;
			}

			else
			{
				g_Vertex[i].pos.x = SCREEN_WIDTH / 2;
				g_Vertex[i].pos.y = SCREEN_HEIGHT / 2;
			}
			g_Vertex[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		}
	}
	//頂点バッファ更新
	SetVertex();
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
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void UpdatePolygon(void)
{

	if (isSpin)
	{

		for (int i = 0; i < NUM_VERTEX; i++)
		{
			if (i % 2 == 0)
			{
				double radian;

				if (isSkip)
				{
					radian = i * M_PI * 2 / ((NUM_VERTEX - 1) / 2);
				}

				else
				{
					radian = (i / 2) * M_PI * 2 / ((NUM_VERTEX - 1) / 2);
				}

				g_Vertex[i].pos.x = SIZE_X * cos(radian + angle) + SCREEN_WIDTH / 2;
				g_Vertex[i].pos.y = SIZE_Y * sin(radian + angle) + SCREEN_HEIGHT / 2;
			}

			else
			{
				g_Vertex[i].pos.x = SCREEN_WIDTH / 2;
				g_Vertex[i].pos.y = SCREEN_HEIGHT / 2;
			}

		}

		angle += 0.01;
		if (angle > 360.0)
		{
			angle = 0.0;
		}

		SetVertex();
	}
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
	SetWorldViewProjection2D();	//座標の2D変換

	//プリミティブトポロジ設定
	
	// 線
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);


	//塗りつぶし
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//マテリアル設定（半年後に現れる）
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	SetVertex();

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
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertex[i].Position = XMFLOAT3(g_Vertex[i].pos.x, g_Vertex[i].pos.y, 0.0f);
		vertex[i].Diffuse = g_Vertex[i].color;
	}

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);
}
