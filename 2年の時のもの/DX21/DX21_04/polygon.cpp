//-----------------------------------------------
// polygon.cpp
// 制作日：2024/05/13
//-----------------------------------------------
#include "polygon.h"

//-----------------------------------------------
// マクロ定義
//-----------------------------------------------
#define NUM_VERTEX		(4)	//必要な頂点の数		//６Pチーズの書き方だと2n+1
#define MAX_VERTEX		(8)	//頂点の最大数
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
static ID3D11ShaderResourceView* g_Texture = NULL;
static ID3D11ShaderResourceView* g_Texture1 = NULL;

struct VERTEX_T
{
	XMFLOAT2		pos;	//位置ベクトル
	XMFLOAT2		vel;	//速度ベクトル
	XMFLOAT4		color;	//頂点カラー
	XMFLOAT2		texCoord;	//テクスチャ座標
};

static VERTEX_T g_Vertex[MAX_VERTEX];
static double g_Angle;	//角度
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
	bd.ByteWidth = sizeof(VERTEX_3D) * MAX_VERTEX;		//確保するバッファの大きさ
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	//回転設定
	isSpin = false;

	//1つ飛ばし設定
	isSkip = false;

	//初期化

	

	double angle = -45 * M_PI / 180;


	for (int i = 0; i < (int)((NUM_VERTEX + 1) / 2); i++)
	{//偶数番目
		double radian = i * M_PI * 2 / NUM_VERTEX + angle;

		g_Vertex[i * 2].pos.x = SIZE_X * cos(radian) + SCREEN_WIDTH / 2;
		g_Vertex[i * 2].pos.y = SIZE_Y * sin(radian) + SCREEN_HEIGHT / 2;
		g_Vertex[i * 2].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	for (int i = 0; i < (int)((NUM_VERTEX + 1) / 2); i++)
	{//奇数番目
		double radian = M_PI * 2 - ((i + 1) * M_PI * 2 / NUM_VERTEX ) + angle;

		g_Vertex[i * 2 + 1].pos.x = SIZE_X * cos(radian) + SCREEN_WIDTH / 2;
		g_Vertex[i * 2 + 1].pos.y = SIZE_Y * sin(radian) + SCREEN_HEIGHT / 2;
		g_Vertex[i * 2 + 1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	g_Vertex[0].texCoord = XMFLOAT2(1.0f, 0.0f);
	g_Vertex[1].texCoord = XMFLOAT2(0.0f, 0.0f);
	g_Vertex[2].texCoord = XMFLOAT2(1.0f, 1.0f);
	g_Vertex[3].texCoord = XMFLOAT2(0.0f, 1.0f);

	

	g_Vertex[4].pos.x = 100.0f;
	g_Vertex[4].pos.y = 100.0f;
	g_Vertex[4].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	g_Vertex[5].pos.x = 400.0f;
	g_Vertex[5].pos.y = 100.0f;
	g_Vertex[5].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	g_Vertex[6].pos.x = 100.0f;
	g_Vertex[6].pos.y = 400.0f;
	g_Vertex[6].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	g_Vertex[7].pos.x = 400.0f;
	g_Vertex[7].pos.y = 400.0f;
	g_Vertex[7].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	g_Vertex[4].texCoord = XMFLOAT2(0.0f, 0.0f);
	g_Vertex[5].texCoord = XMFLOAT2(1.0f, 0.0f);
	g_Vertex[6].texCoord = XMFLOAT2(0.0f, 1.0f);
	g_Vertex[7].texCoord = XMFLOAT2(1.0f, 1.0f);


	//if (!isSkip)
	//{//一つ飛ばしじゃないとき
	//	for (int i = 0; i < NUM_VERTEX; i++)
	//	{
	//		double radian = (i / 2) * M_PI * 2 / ((NUM_VERTEX - 1) / 2);

	//		if (i % 2 == 0)
	//		{
	//			g_Vertex[i].pos.x = SIZE_X * cos(radian) + SCREEN_WIDTH / 2;
	//			g_Vertex[i].pos.y = SIZE_Y * sin(radian) + SCREEN_HEIGHT / 2;
	//		}

	//		else
	//		{
	//			g_Vertex[i].pos.x = SCREEN_WIDTH / 2;
	//			g_Vertex[i].pos.y = SCREEN_HEIGHT / 2;
	//		}
	//		g_Vertex[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	//	}
	//}

	//else if (NUM_POLYGON >= 5)
	//{
	//	//4以上の多角形で正多角形の頂点1つ飛ばしの図形（5だと星になる）（偶数だと÷２した多角形になる）
	//	for (int i = 0; i < NUM_VERTEX; i++)
	//	{
	//		double radian = i * M_PI * 2 / ((NUM_VERTEX - 1) / 2);

	//		if (i % 2 == 0)
	//		{
	//			g_Vertex[i].pos.x = SIZE_X * cos(radian) + SCREEN_WIDTH / 2;
	//			g_Vertex[i].pos.y = SIZE_Y * sin(radian) + SCREEN_HEIGHT / 2;
	//		}

	//		else
	//		{
	//			g_Vertex[i].pos.x = SCREEN_WIDTH / 2;
	//			g_Vertex[i].pos.y = SCREEN_HEIGHT / 2;
	//		}
	//		g_Vertex[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	//	}
	//}
	//頂点バッファ更新
	SetVertex();

	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\dq.jpg" ,WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);


	TexMetadata metadata1;
	ScratchImage image1;
	LoadFromWICFile(L"asset\\texture\\kirama.jpg", WIC_FLAGS_NONE, &metadata1, image1);
	CreateShaderResourceView(GetDevice(), image1.GetImages(), image1.GetImageCount(), metadata1, &g_Texture1);
	assert(g_Texture1);

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
	if (g_Texture)
	{
		g_Texture->Release();
		g_Texture = NULL;
	}

	if (g_Texture1)
	{
		g_Texture1->Release();
		g_Texture1 = NULL;
	}

}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void UpdatePolygon(void)
{

	/*if (isSpin)
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
		
	}*/

	if (isSpin)
	{

		for (int i = 0; i < (int)((NUM_VERTEX + 1) / 2); i++)
		{//偶数番目
			double radian = i * M_PI * 2 / NUM_VERTEX;

			g_Vertex[i * 2].pos.x = SIZE_X * cos(radian + g_Angle) + SCREEN_WIDTH / 2;
			g_Vertex[i * 2].pos.y = SIZE_Y * sin(radian + g_Angle) + SCREEN_HEIGHT / 2;
			g_Vertex[i * 2].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}

		for (int i = 0; i < (int)((NUM_VERTEX + 1) / 2); i++)
		{//奇数番目
			double radian = M_PI * 2 - ((i + 1) * M_PI * 2 / NUM_VERTEX);

			g_Vertex[i * 2 + 1].pos.x = SIZE_X * cos(radian + g_Angle) + SCREEN_WIDTH / 2;
			g_Vertex[i * 2 + 1].pos.y = SIZE_Y * sin(radian + g_Angle) + SCREEN_HEIGHT / 2;
			g_Vertex[i * 2 + 1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}

		g_Angle += 0.01;
		if (g_Angle > 360.0)
		{
			g_Angle = 0.0;
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
	SetWorldViewProjection2D();	//座標の2D変換

	//プリミティブトポロジ設定
	
	// 線
	//GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);


	//塗りつぶし
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//マテリアル設定（半年後に現れる）
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	SetVertex();

	//テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);
	//ポリゴン描画
	GetDeviceContext()->Draw(NUM_VERTEX, 0);


	//テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture1);
	//ポリゴン描画
	GetDeviceContext()->Draw(NUM_VERTEX, 4);
	
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
		vertex[i].TexCoord = g_Vertex[i].texCoord;

		vertex[i + NUM_VERTEX].Position = XMFLOAT3(g_Vertex[i + NUM_VERTEX].pos.x, g_Vertex[i + NUM_VERTEX].pos.y, 0.0f);
		vertex[i + NUM_VERTEX].Diffuse = g_Vertex[i + NUM_VERTEX].color;
		vertex[i + NUM_VERTEX].TexCoord = g_Vertex[i + NUM_VERTEX].texCoord;
	}

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);
}
