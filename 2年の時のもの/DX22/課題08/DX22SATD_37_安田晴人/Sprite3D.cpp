//-----------------------------------------------
// Sprite3D.cpp
// 制作日：2024/11/20
// 制作者：安田晴人
//-----------------------------------------------
#include "Sprite3D.h"

//-----------------------------------------------
// グローバル変数
//-----------------------------------------------
static	ID3D11Buffer* g_vertex_buffer = NULL;//テスト用頂点バッファ
static	ID3D11Buffer* g_index_buffer = NULL;//テスト用インデックスバッファ

//インデックスデータ
UINT	g_box_index[36] =
{
	0, 1, 2,	//天井
	3, 4, 5,

	6, 7, 8,	//前面
	9,10,11,

	12,13,14,	//右側面
	15,16,17,

	18,19,20,	//背面
	21,22,23,

	24,25,26,	//左側面
	27,28,29,

	30,31,32,	//底面
	33,34,35,
};

//頂点データ
#define	BOX_NUM_VERTEX	(36)
VERTEX_3D	g_box[BOX_NUM_VERTEX] =
{
	//天井////////////////////////////////////////////
	{//頂点V0 LEFT-TOP
		XMFLOAT3(-0.5f, 0.5f, 0.5f),	//頂点座標
		XMFLOAT3(0.0f,1.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(0.0f,0.0f)				//テクスチャ座標
	},
	{//頂点V1 LEFT-TOP
		XMFLOAT3(0.5f, 0.5f, 0.5f),	//頂点座標
		XMFLOAT3(0.0f,1.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(1.0f,0.0f)				//テクスチャ座標
	},
	{//頂点V2 LEFT-TOP
		XMFLOAT3(-0.5f, 0.5f, -0.5f),	//頂点座標
		XMFLOAT3(0.0f,1.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(0.0f,1.0f)				//テクスチャ座標
	},

	{//頂点V1 LEFT-TOP
		XMFLOAT3(0.5f, 0.5f, 0.5f),	//頂点座標
		XMFLOAT3(0.0f,1.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(1.0f,0.0f)				//テクスチャ座標
	},
	{//頂点V3 RIGHT-TOP
		XMFLOAT3(0.5f, 0.5f, -0.5f),		//頂点座標
		XMFLOAT3(0.0f,1.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(1.0f,1.0f)				//テクスチャ座標
	},
	{//頂点V2 LEFT-TOP
		XMFLOAT3(-0.5f, 0.5f, -0.5f),	//頂点座標
		XMFLOAT3(0.0f,1.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(0.0f,1.0f)				//テクスチャ座標
	},
	//前面////////////////////////////////////////////
	{//頂点V2 LEFT-TOP
		XMFLOAT3(-0.5f, 0.5f, -0.5f),	//頂点座標
		XMFLOAT3(0.0f,0.0f,-1.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(0.0f,0.0f)				//テクスチャ座標
	},
	{//頂点V3 LEFT-TOP
		XMFLOAT3(0.5f, 0.5f, -0.5f),	//頂点座標
		XMFLOAT3(0.0f,0.0f,-1.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(1.0f,0.0f)				//テクスチャ座標
	},
	{//頂点V7 LEFT-TOP
		XMFLOAT3(-0.5f, -0.5f, -0.5f),	//頂点座標
		XMFLOAT3(0.0f,0.0f,-1.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(0.0f,1.0f)				//テクスチャ座標
	},
	{//頂点V3 LEFT-TOP
		XMFLOAT3(0.5f, 0.5f, -0.5f),	//頂点座標
		XMFLOAT3(0.0f,0.0f,-1.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(1.0f,0.0f)				//テクスチャ座標
	},
	{//頂点V6 LEFT-TOP
		XMFLOAT3(0.5f,-0.5f, -0.5f),	//頂点座標
		XMFLOAT3(0.0f,0.0f,-1.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(1.0f,1.0f)				//テクスチャ座標
	},
	{//頂点V7 LEFT-TOP
		XMFLOAT3(-0.5f, -0.5f, -0.5f),	//頂点座標
		XMFLOAT3(0.0f,0.0f,-1.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(0.0f,1.0f)				//テクスチャ座標
	},
	//右側面////////////////////////////////////////////
	{//頂点V3 LEFT-TOP
		XMFLOAT3(0.5f, 0.5f, -0.5f),	//頂点座標
		XMFLOAT3(1.0f,0.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(0.0f,0.0f)				//テクスチャ座標
	},
	{//頂点V1 LEFT-TOP
		XMFLOAT3(0.5f, 0.5f, 0.5f),	//頂点座標
		XMFLOAT3(1.0f,0.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(1.0f,0.0f)				//テクスチャ座標
	},
	{//頂点V6 LEFT-TOP
		XMFLOAT3(0.5f,-0.5f, -0.5f),	//頂点座標
		XMFLOAT3(1.0f,0.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(0.0f,1.0f)				//テクスチャ座標
	},
	{//頂点V1 LEFT-TOP
		XMFLOAT3(0.5f, 0.5f, 0.5f),	//頂点座標
		XMFLOAT3(1.0f,0.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(1.0f,0.0f)				//テクスチャ座標
	},
	{//頂点V4 LEFT-TOP
		XMFLOAT3(0.5f, -0.5f, 0.5f),	//頂点座標
		XMFLOAT3(1.0f,0.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(1.0f,1.0f)				//テクスチャ座標
	},
	{//頂点V6 LEFT-TOP
		XMFLOAT3(0.5f,-0.5f, -0.5f),	//頂点座標
		XMFLOAT3(1.0f,0.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(0.0f,1.0f)				//テクスチャ座標
	},
	//背面////////////////////////////////////////////
	{//頂点V1 LEFT-TOP
		XMFLOAT3(0.5f, 0.5f, 0.5f),	//頂点座標
		XMFLOAT3(0.0f,0.0f,1.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(0.0f,0.0f)				//テクスチャ座標
	},
	{//頂点V0 LEFT-TOP
		XMFLOAT3(-0.5f, 0.5f, 0.5f),	//頂点座標
		XMFLOAT3(0.0f,0.0f,1.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(1.0f,0.0f)				//テクスチャ座標
	},
	{//頂点V4 LEFT-TOP
		XMFLOAT3(0.5f, -0.5f, 0.5f),	//頂点座標
		XMFLOAT3(0.0f,0.0f,1.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(0.0f,1.0f)				//テクスチャ座標
	},
	{//頂点V0 LEFT-TOP
		XMFLOAT3(-0.5f, 0.5f, 0.5f),	//頂点座標
		XMFLOAT3(0.0f,0.0f,1.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(1.0f,0.0f)				//テクスチャ座標
	},
	{//頂点V5 LEFT-TOP
		XMFLOAT3(-0.5f, -0.5f, 0.5f),	//頂点座標
		XMFLOAT3(0.0f,0.0f,1.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(1.0f,1.0f)				//テクスチャ座標
	},
	{//頂点V4 LEFT-TOP
		XMFLOAT3(0.5f, -0.5f, 0.5f),	//頂点座標
		XMFLOAT3(0.0f,0.0f,1.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(1.0f,0.0f)				//テクスチャ座標
	},
	//左側面////////////////////////////////////////////
	{//頂点V0 LEFT-TOP
		XMFLOAT3(-0.5f, 0.5f, 0.5f),	//頂点座標
		XMFLOAT3(-1.0f,0.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(0.0f,0.0f)				//テクスチャ座標
	},
	{//頂点V2 LEFT-TOP
		XMFLOAT3(-0.5f, 0.5f, -0.5f),	//頂点座標
		XMFLOAT3(-1.0f,0.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(1.0f,0.0f)				//テクスチャ座標
	},
	{//頂点V5 LEFT-TOP
		XMFLOAT3(-0.5f, -0.5f, 0.5f),	//頂点座標
		XMFLOAT3(-1.0f,0.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(0.0f,1.0f)				//テクスチャ座標
	},
	{//頂点V2 LEFT-TOP
		XMFLOAT3(-0.5f, 0.5f, -0.5f),	//頂点座標
		XMFLOAT3(-1.0f,0.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(1.0f,0.0f)				//テクスチャ座標
	},
	{//頂点V7 LEFT-TOP
		XMFLOAT3(-0.5f, -0.5f, -0.5f),	//頂点座標
		XMFLOAT3(-1.0f,0.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(1.0f,1.0f)				//テクスチャ座標
	},
	{//頂点V5 LEFT-TOP
		XMFLOAT3(-0.5f, -0.5f, 0.5f),	//頂点座標
		XMFLOAT3(-1.0f,0.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(0.0f,1.0f)				//テクスチャ座標
	},
	//底面////////////////////////////////////////////
	{//頂点V7 LEFT-TOP
		XMFLOAT3(-0.5f, -0.5f, -0.5f),	//頂点座標
		XMFLOAT3(0.0f,-1.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(0.0f,0.0f)				//テクスチャ座標
	},
	{//頂点V6 LEFT-TOP
		XMFLOAT3(0.5f,-0.5f, -0.5f),	//頂点座標
		XMFLOAT3(0.0f,-1.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(1.0f,0.0f)				//テクスチャ座標
	},
	{//頂点V5 LEFT-TOP
		XMFLOAT3(-0.5f, -0.5f, 0.5f),	//頂点座標
		XMFLOAT3(0.0f,-1.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(0.0f,1.0f)				//テクスチャ座標
	},
	{//頂点V6 LEFT-TOP
		XMFLOAT3(0.5f,-0.5f, -0.5f),	//頂点座標
		XMFLOAT3(0.0f,-1.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(1.0f,0.0f)				//テクスチャ座標
	},
	{//頂点V4 LEFT-TOP
		XMFLOAT3(0.5f, -0.5f, 0.5f),	//頂点座標
		XMFLOAT3(0.0f,-1.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(1.0f,1.0f)				//テクスチャ座標
	},
	{//頂点V5 LEFT-TOP
		XMFLOAT3(-0.5f, -0.5f, 0.5f),	//頂点座標
		XMFLOAT3(0.0f,-1.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(0.0f,1.0f)				//テクスチャ座標
	},

};

void InitSprite3D(void)
{
	D3D11_BUFFER_DESC	bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * BOX_NUM_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_vertex_buffer);

	//頂点バッファの書き込み先のポインターを取得
	D3D11_MAPPED_SUBRESOURCE	msr;
	GetDeviceContext()->Map(g_vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	//頂点データをコピー
	CopyMemory(&vertex[0], &g_box[0], sizeof(VERTEX_3D) * BOX_NUM_VERTEX);
	//書き込み完了
	GetDeviceContext()->Unmap(g_vertex_buffer, 0);

	//インデックスバッファ作成
	{
		D3D11_BUFFER_DESC	bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(UINT) * BOX_NUM_VERTEX;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		GetDevice()->CreateBuffer(&bd, NULL, &g_index_buffer);

		//インデックバッファの書き込み先のポインターを取得
		D3D11_MAPPED_SUBRESOURCE	msr;
		GetDeviceContext()->Map(g_index_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		UINT* index = (UINT*)msr.pData;

		//頂点データをコピー
		CopyMemory(&index[0], &g_box_index[0], sizeof(UINT) * BOX_NUM_VERTEX);
		//書き込み完了
		GetDeviceContext()->Unmap(g_index_buffer, 0);
	}

}

void UninitSprite3D(void)
{
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
}

void DrawSpriteModel(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale, XMFLOAT4 color, MODEL* model)
{

	//平行移動行列作成
	XMMATRIX TranslationMatrix = XMMatrixTranslation(pos.x, pos.y, pos.z);

	//回転行列作成
	XMMATRIX	RotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);

	//スケーリング行列作成
	XMMATRIX	ScalingMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);

	//ワールド行列作成 ※乗算の順番に注意
	XMMATRIX	WorldMatrix =
		ScalingMatrix *
		RotationMatrix *
		TranslationMatrix;

	//ワールド行列をセット
	SetWorldMatrix(WorldMatrix);

	//マテリアル設定
	MATERIAL	material;
	ZeroMemory(&material, sizeof(MATERIAL));
	material.Diffuse = color;
	SetMaterial(material);

	ModelDraw(model);
}

void DrawSpriteBox(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale, XMFLOAT4 color, int xnum, int ynum, BOX_TEXTURE* p_box_texture)
{
	XMMATRIX TranslationMatrix = XMMatrixTranslation(pos.x, pos.y, pos.z);


	//テクスチャ設定
	for (int i = 0; i < 6; i++)
	{
		float x = p_box_texture->m_anim_pattern[i] % xnum;
		float y = p_box_texture->m_anim_pattern[i] / ynum;

		XMFLOAT2 texture[4] = {};

		texture[0] = XMFLOAT2((1.0f / xnum) * x, 1.0f / ynum * y);
		texture[1] = XMFLOAT2((1.0f / xnum) * (x + 1), 1.0f / ynum * y);
		texture[2] = XMFLOAT2((1.0f / xnum) * x, 1.0f / ynum * (y + 1));
		texture[3] = XMFLOAT2((1.0f / xnum) * (x + 1), 1.0f / ynum * (y + 1));

		
		g_box[i * 6].TexCoord = texture[0];
		g_box[i * 6 + 1].TexCoord = texture[1];
		g_box[i * 6 + 2].TexCoord = texture[2];
		g_box[i * 6 + 3].TexCoord = texture[1];
		g_box[i * 6 + 4].TexCoord = texture[3];
		g_box[i * 6 + 5].TexCoord = texture[2];
		

	}
	//回転行列作成
	XMMATRIX	RotationMatrix =
		XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);

	//スケーリング行列作成
	XMMATRIX	ScalingMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);

	//ワールド行列作成 ※乗算の順番に注意
	XMMATRIX	WorldMatrix = ScalingMatrix * RotationMatrix * TranslationMatrix;

	//ワールド行列をセット
	SetWorldMatrix(WorldMatrix);

	//頂点バッファをセット
	UINT	stride = sizeof(VERTEX_3D);
	UINT	offset = 0;
	GetDeviceContext()->IASetVertexBuffers( 0, 1, &g_vertex_buffer, &stride, &offset);

	//インデックスバッファをセット
	GetDeviceContext()->IASetIndexBuffer(g_index_buffer, DXGI_FORMAT_R32_UINT, 0);

	//プリミティブトポロジーの設定
	GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
	//マテリアル設定
	MATERIAL	material;
	ZeroMemory(&material, sizeof(MATERIAL));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	//テクスチャセット
	//GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

	SetVertex();

	//描画
	GetDeviceContext()->DrawIndexed(BOX_NUM_VERTEX, 0, 0);//インデックス有り描画

}

void SetVertex(void)
{
	//頂点バッファの書き込み先のポインターを取得
	D3D11_MAPPED_SUBRESOURCE	msr;
	GetDeviceContext()->Map(g_vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	//頂点データをコピー
	CopyMemory(&vertex[0], &g_box[0], sizeof(VERTEX_3D) * BOX_NUM_VERTEX);
	//書き込み完了
	GetDeviceContext()->Unmap(g_vertex_buffer, 0);
}
