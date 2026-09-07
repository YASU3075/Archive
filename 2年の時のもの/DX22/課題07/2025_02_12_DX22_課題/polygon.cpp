/*==============================================================================

   頂点管理 [polygon.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "polygon.h"
#include "sprite.h"
#include "model.h"

#include "CollisionEx.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


#define	BALL_SPEED		(0.02f / 100.0f)
void	InitBall();
void	UninitBall();
void	UpdateBall();
void	DrawBall();
Object3D	BallObject;
MODEL*	BallModel;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
bool	gHit = false;

Object3D	Object1;
Object3D	Object2;
Camera		CameraObject;

//頂点データを格納するバッファオブジェクト
static ID3D11Buffer* g_VertexBuffer = NULL;
static ID3D11Buffer* g_VertexBuffer2 = NULL;


//頂点のデータ
VERTEX_3D	Box[4] =
{
	{//頂点0 LEFT-TOP
		XMFLOAT3(-0.5f, 0.0f, 0.5f),		//座標
		XMFLOAT3(0.0f, 1.0f, 0.0f),			//法線　未使用
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//カラー
		XMFLOAT2(0.0f,0.0f)					//テクスチャ座標
	},
	{//頂点1 RIHGT-TOP
		XMFLOAT3(0.5f, 0.0f, 0.5f),
		XMFLOAT3(0.0f, 1.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT2(1.0f,0.0f)
	},
	{//頂点2 LEFT-BOTTOM
		XMFLOAT3(-0.5f, 0.0f, -0.5f),
		XMFLOAT3(0.0f, 1.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT2(0.0f,1.0f)
	},
	{//頂点3 RIGHT-BOTTOM
		XMFLOAT3(0.5f, 0.0f, -0.5f),
		XMFLOAT3(0.0f, 1.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT2(1.0f,1.0f)
	},
};

//線分のデータ
VERTEX_3D	Line[2] =
{
	{//頂点0 P0
		XMFLOAT3(0.0f, 0.5f, 0.0f),			//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//法線　未使用
		XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),	//カラー
		XMFLOAT2(0.0f,0.0f)					//テクスチャ座標
	},
	{//頂点1 P1
		XMFLOAT3(0.0f, -0.5f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),
		XMFLOAT2(1.0f,0.0f)
	}
};

void	Init2()
{
	ZeroMemory(&BallObject, sizeof(Object3D));
	BallObject.position.x = 0.6f;
	BallObject.position.y = 0.6f;
	BallObject.position.z = 0.0f;
	XMFLOAT3	dist = XMFLOAT3(0, 0, 0);
	dist.x -= BallObject.position.x;
	dist.y -= BallObject.position.y;
	dist.z -= BallObject.position.z;
	XMVECTOR temp = XMLoadFloat3(&dist);
	XMVector3Normalize(temp);
	XMStoreFloat3(&dist, temp);
	BallObject.velocity.x = dist.x *= BALL_SPEED;
	BallObject.velocity.y = dist.y *= BALL_SPEED;
	BallObject.velocity.z = dist.z *= BALL_SPEED;

}

void	InitBall()
{
	ZeroMemory(&BallObject, sizeof(Object3D));
	BallObject.position.x = 1.0f;
	BallObject.position.y = 1.0f;
	BallObject.position.z = 0.0f;

	XMFLOAT3	dist = XMFLOAT3(0, 0, 0);
	dist.x -= BallObject.position.x;
	dist.y -= BallObject.position.y;
	dist.z -= BallObject.position.z;
	XMVECTOR temp = XMLoadFloat3(&dist);
	XMVector3Normalize(temp);
	XMStoreFloat3(&dist, temp);
	BallObject.velocity.x = dist.x *= BALL_SPEED;
	BallObject.velocity.y = dist.y *= BALL_SPEED;
	BallObject.velocity.z = dist.z *= BALL_SPEED;

	BallModel = ModelLoad("asset\\model\\ball.fbx");


}
void	UninitBall()
{
	ModelRelease(BallModel);
}
void	UpdateBall()
{
	BallObject.position.x += BallObject.velocity.x;
	BallObject.position.y += BallObject.velocity.y;
	BallObject.position.z += BallObject.velocity.z;

	//反射処理



}

void	DrawBall()
{

	XMMATRIX	TranslationMatrix =
		XMMatrixTranslation(
			BallObject.position.x,
			BallObject.position.y,
			BallObject.position.z
		);

	SetWorldMatrix(TranslationMatrix);

	//マテリアル設定
	MATERIAL	material;
	ZeroMemory(&material, sizeof(MATERIAL));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	ModelDraw(BallModel);


}


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPolygon(void)
{

	//3Dオブジェクト管理構造体初期化
	Object1.position = XMFLOAT3(0, 0, 0);
	Object1.rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Object1.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	Object1.World = XMMatrixIdentity();

	// 3Dポリゴン用頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	//頂点バッファの書き込み先を取得
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	//頂点データをバッファへ書き込む
	CopyMemory(&vertex[0], &Box[0], sizeof(VERTEX_3D) * 4);

	//書き込み完了
	GetDeviceContext()->Unmap(g_VertexBuffer, 0);



	//ラインオブジェクト管理構造体初期化
	Object2.position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Object2.rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Object2.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	Object2.World = XMMatrixIdentity();


	bd.ByteWidth = sizeof(VERTEX_3D) * 2;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer2);
	GetDeviceContext()->Map(g_VertexBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	vertex = (VERTEX_3D*)msr.pData;
	//頂点データをバッファへ書き込む
	CopyMemory(&vertex[0], &Line[0], sizeof(VERTEX_3D) * 2);



	//カメラ管理構造体初期化
	CameraObject.position = XMFLOAT3(0.0f, 0.4f, -2.0f);
	CameraObject.Upvector = XMFLOAT3(0.0f, 1.0f, 0.0f);
	CameraObject.Atposition = XMFLOAT3(0.0f, 0.0f, 1.0f);
	CameraObject.fov = 45.0f;
	CameraObject.nearclip = 0.5f;
	CameraObject.farclip = 1000.0f;


	InitBall();


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPolygon(void)
{
	// 頂点バッファの解放
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
	UninitBall();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePolygon(void)
{
	UpdateBall();

	//ラインの移動
	if (GetAsyncKeyState('A'))
	{
		Object2.position.x -= 0.0003f;
	}
	else if (GetAsyncKeyState('D'))
	{
		Object2.position.x += 0.0003f;
	}

	if (GetAsyncKeyState('S'))
	{
		Object2.position.z -= 0.0003f;
	}
	else if (GetAsyncKeyState('W'))
	{
		Object2.position.z += 0.0003f;
	}

	if (GetAsyncKeyState(VK_DOWN))
	{
		Object2.position.y -= 0.0003f;
	}
	else if (GetAsyncKeyState(VK_UP))
	{
		Object2.position.y += 0.0003f;
	}


	if (GetAsyncKeyState(VK_LEFT))
	{
		Object2.rotate.z += 0.03f;
	}
	else if (GetAsyncKeyState(VK_RIGHT))
	{
		Object2.rotate.z -= 0.03f;
	}

	if (GetAsyncKeyState('Q'))
	{
		Object1.rotate.z += 0.03f;
	}
	else if (GetAsyncKeyState('E'))
	{
		Object1.rotate.z -= 0.03f;
	}



	//反射を無限面として考える
	//ポリゴンの頂点V0-V1-V2をワールド変換してVtに格納
	XMFLOAT3	Vt[3];
	XMVECTOR	xv[3];
	for (int i = 0; i < 3; i++)
	{
		xv[i] = XMLoadFloat3(&Box[i].Position);
		xv[i] = XMVector3Transform(xv[i], Object1.World);
		XMStoreFloat3(&Vt[i], xv[i]);
	}

	//頂点Vtから面の法線Normalを計算
	XMFLOAT3	Normal = XMFLOAT3(0, 0, 0);	//面の法線
	{
		XMStoreFloat3(&Normal, XMVector3Normalize(XMVector3Cross((xv[1] - xv[0]), (xv[2] - xv[1]))));
	}
	//面上の任意の点Vt[0]からBallObjectの中心へのベクトルPolyBallを計算
	XMFLOAT3	PolyBall;
	{
		PolyBall = XMFLOAT3(BallObject.position.x - Vt[0].x, BallObject.position.y - Vt[0].y, BallObject.position.z - Vt[0].z);
		//PolyBall = XMFLOAT3(-BallObject.position.x + Vt[0].x, -BallObject.position.y + Vt[0].y, -BallObject.position.z + Vt[0].z);
	}
	//BallObjectと面との最短距離lenを計算
	XMFLOAT3	len = XMFLOAT3(0.0f, 0.0f, 0.0f);
	{
		XMVECTOR v = XMLoadFloat3(&PolyBall);
		XMVECTOR n = XMLoadFloat3(&Normal);

		XMStoreFloat3(&len, XMVector3Dot(n, v));
	}

	//lenがBallObjectの半径より短ければ面と当たり
	if (len.x <= 0.2f)
	{
		XMFLOAT3 r = XMFLOAT3(0.0f, 0.0f, 0.0f);
		XMVECTOR v, n, d;
		//反射処理 R = V+2(-V・N)Nを計算
	
		v = XMLoadFloat3(&BallObject.velocity);
		n = XMLoadFloat3(&Normal);
		d = XMVector3Dot(-v, n);
		d *= 2.0f;
		d *= n;
		d += v;
		
		XMStoreFloat3(&r, d);
		
		//RをBallObjectの移動ベクトルにセット
		//BallObject.velocity = r;

		//座標調整
		XMFLOAT3 p;
		//ボール面にめり込んだ長さ
		p.x = p.y = p.z = (0.2f - len.x);
		//法線にめり込んだ長さでスケール
		p.x *= Normal.x;
		p.y *= Normal.y;
		p.z *= Normal.z;
		//ボールの座標へ変換
		BallObject.position.x += p.x;
		BallObject.position.y += p.y;
		BallObject.position.z += p.z;

	}

	float d;
	d = (BallObject.position.x * BallObject.position.x);
	d += (BallObject.position.y * BallObject.position.y);
	d += (BallObject.position.z * BallObject.position.z);
	d = sqrtf(d);
	if (d >= 1.0f)
	{
		Init2();//ボール再表示
	}
		return;


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPolygon(void)
{
	//3D用マトリクス設定　3D描画時に最初に一回やっておく
	SetWorldViewProjection3D();
	//3Dなので奥行き処理を有効にする
	SetDepthEnable(true);

	//プロジェクション行列セット
	XMMATRIX ProjectionMatrix =
		XMMatrixPerspectiveFovLH(XMConvertToRadians(CameraObject.fov),
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
			CameraObject.nearclip, CameraObject.farclip
		);
	SetProjectionMatrix(ProjectionMatrix);

	//カメラ行列セット
	XMVECTOR v1 = XMLoadFloat3(&CameraObject.Atposition);
	XMVECTOR v2 = XMLoadFloat3(&CameraObject.position);
	XMVECTOR eyev = XMVector3Normalize(v1 - v2);
	XMVECTOR pos = XMLoadFloat3(&CameraObject.position);
	XMVECTOR up = XMLoadFloat3(&CameraObject.Upvector);
	XMMATRIX ViewMatrix = XMMatrixLookAtLH(pos, eyev, up);
	SetViewMatrix(ViewMatrix);




	{//3Dオブジェクト1つごとの処理
		GetDeviceContext()->PSSetShaderResources(0, 1, &Object1.texID);

		//平行移動行列
		XMMATRIX	TranslationMatrix =
			XMMatrixTranslation(Object1.position.x, Object1.position.y, 0.0f);

		//回転行列 角度をラジアンで与える
		XMMATRIX	RotationMatrix = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(Object1.rotate.x),
			XMConvertToRadians(Object1.rotate.y),
			XMConvertToRadians(Object1.rotate.z)
		);

		//スケーリング行列(1.0が等倍) 注意！ 倍率に0.0fはダメ！
		XMMATRIX	ScalingMatrix = XMMatrixScaling(Object1.scale.x, Object1.scale.y, 1.0f);

		//ワールド行列を作成
		XMMATRIX	WorldMatrix = ScalingMatrix * RotationMatrix * TranslationMatrix;
		Object1.World = WorldMatrix;
		//ワールド行列をセット
		SetWorldMatrix(WorldMatrix);

		// 頂点バッファをセット
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);
		// プリミティブトポロジを設定
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));

		if (!gHit)
		{
			material.Diffuse = XMFLOAT4(0.8f, 0.6f, 0.6f, 1.0f);
		}
		else
		{
			material.Diffuse = XMFLOAT4(0.8f, 0.2f, 0.2f, 1.0f);
		}

		SetMaterial(material);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

	}

	{
		//平行移動行列
		XMMATRIX	TranslationMatrix =
			XMMatrixTranslation(Object2.position.x, Object2.position.y, Object2.position.z);

		//回転行列 角度をラジアンで与える
		XMMATRIX	RotationMatrix = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(Object2.rotate.x),
			XMConvertToRadians(Object2.rotate.y),
			XMConvertToRadians(Object2.rotate.z)
		);

		//スケーリング行列(1.0が等倍) 注意！ 倍率に0.0fはダメ！
		XMMATRIX	ScalingMatrix = XMMatrixScaling(Object2.scale.x, Object2.scale.y, 1.0f);

		//ワールド行列を作成
		XMMATRIX	WorldMatrix = ScalingMatrix * RotationMatrix * TranslationMatrix;
		//ワールド行列をセット
		SetWorldMatrix(WorldMatrix);
		Object2.World = WorldMatrix;

		// 頂点バッファをセット
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer2, &stride, &offset);
		// プリミティブトポロジを設定
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		// マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		SetMaterial(material);

		// ライン描画
		//GetDeviceContext()->Draw(2, 0);



	}

	DrawBall();


}


