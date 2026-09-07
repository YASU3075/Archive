/*==============================================================================

   頂点管理 [sprite.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include	"main.h"
#include	"sprite.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_SPRITEVERTEX 4

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void SetVertex2D(void);

struct VERTEX_T
{
	XMFLOAT2 pos;
	XMFLOAT2 vec;
	XMFLOAT4 color;
	XMFLOAT2 texCoord;
};

static VERTEX_T g_Vertex[NUM_SPRITEVERTEX];

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSprite(void)
{
	ID3D11Device *pDevice = GetDevice();

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * NUM_SPRITEVERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	g_Vertex[0].pos.x = -0.5f;
	g_Vertex[0].pos.y = -0.5f;
	g_Vertex[0].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_Vertex[0].texCoord = XMFLOAT2(0.0f, 0.0f);

	g_Vertex[1].pos.x = 0.5f;
	g_Vertex[1].pos.y = -0.5f;
	g_Vertex[1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_Vertex[1].texCoord = XMFLOAT2(1.0f, 0.0f);

	g_Vertex[2].pos.x = -0.5f;
	g_Vertex[2].pos.y = 0.5f;
	g_Vertex[2].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_Vertex[2].texCoord = XMFLOAT2(0.0f, 1.0f);

	g_Vertex[0].pos.x = 0.5f;
	g_Vertex[0].pos.y = 0.5f;
	g_Vertex[0].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_Vertex[0].texCoord = XMFLOAT2(1.0f, 1.0f);

	SetVertex2D();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSprite(void)
{
	// 頂点バッファの解放
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

//=============================================================================
// 描画処理
//====================
void DrawSpriteUI(XMFLOAT3 pos, XMFLOAT3 sca, float rot, XMFLOAT4 color, int xnum, int ynum, int anim)
{
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	XMMATRIX TranslationMatrix = XMMatrixTranslation(SCREEN_WIDTH * 0.5f + pos.x, SCREEN_HEIGHT * 0.5f + pos.y, pos.z);
	XMMATRIX RotationMatrix = XMMatrixRotationZ(XMConvertToRadians(rot));
	XMMATRIX ScalingMatirx = XMMatrixScaling(sca.x, sca.y, 1.0f);
	XMMATRIX WorldMatrix = ScalingMatirx * RotationMatrix * TranslationMatrix;

	SetWorldMatrix(WorldMatrix);

	float x, y;
	x = anim % xnum;
	y = anim / ynum;

	g_Vertex[0].texCoord = XMFLOAT2((1.0f / xnum) * x, 1.0f / ynum * y);
	g_Vertex[1].texCoord = XMFLOAT2((1.0f / xnum) * (x + 1), 1.0f / ynum * y);
	g_Vertex[2].texCoord = XMFLOAT2((1.0f / xnum) * x, 1.0f / ynum * (y + 1));
	g_Vertex[3].texCoord = XMFLOAT2((1.0f / xnum) * (x + 1), 1.0f / ynum * (y + 1));

	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = color;
	SetMaterial(material);

	SetVertex2D();

	GetDeviceContext()->Draw(4, 0);
}

void DrawSprite(XMFLOAT2 size, XMFLOAT4 color)
{

	//頂点の作成
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	float size_W = size.x * 0.5f;
	float size_H = size.y * 0.5f;

	//左上 中心からサイズの半分の場所で座標を作る
	vertex[0].Position = XMFLOAT3(-size_W, -size_H, 0.0f);
	vertex[0].Diffuse = color;
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	//右上 中心からサイズの半分の場所で座標を作る
	vertex[1].Position = XMFLOAT3(size_W, -size_H, 0.0f);
	vertex[1].Diffuse = color;
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
	//左下 中心からサイズの半分の場所で座標を作る
	vertex[2].Position = XMFLOAT3(-size_W, size_H, 0.0f);
	vertex[2].Diffuse = color;
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
	//右下 中心からサイズの半分の場所で座標を作る
	vertex[3].Position = XMFLOAT3(size_W, size_H, 0.0f);
	vertex[3].Diffuse = color;
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);


	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
}

void SetVertex2D(void)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
	for (int i = 0; i < NUM_SPRITEVERTEX; i++)
	{
		vertex[i].Position = XMFLOAT3(g_Vertex[i].pos.x, g_Vertex[i].pos.y, 0.0f);
		vertex[i].Diffuse = g_Vertex[i].color;
		vertex[i].TexCoord = g_Vertex[i].texCoord;
	}

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);
}