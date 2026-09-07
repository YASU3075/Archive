

//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// マトリクスバッファ
cbuffer ConstantBuffer : register( b0 )
{
	matrix WorldViewProjection;
}

// マテリアルバッファ
cbuffer MaterialBuffer : register( b1 )
{
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4		Emission;
	float		Shininess;
	float3		Dummy;//16bit境界用
}

struct LIGHT
{
    float4 Direction;
    float4 Diffuse;
    float4 Ambient;
};

//ライトバッファ
cbuffer LightBuffer : register(b2)
{
    LIGHT Light;
};

//ワールド行列
cbuffer WorldMatrrixBuffer : register(b3)
{
    matrix World;
};


//=============================================================================
// 頂点シェーダ
//=============================================================================
void VertexShaderPolygon( in  float4 inPosition		: POSITION0,
						  in  float4 inNormal		: NORMAL0,
						  in  float4 inDiffuse		: COLOR0,
						  in  float2 inTexCoord		: TEXCOORD0,

						  out float4 outPosition	: SV_POSITION,
						  out float4 outNormal		: NORMAL0,
						  out float2 outTexCoord	: TEXCOORD0,
						  out float4 outDiffuse		: COLOR0 )
{
	outPosition = mul( inPosition, WorldViewProjection );

	//法線ベクトルをワールド行列で回転させる
    float4 normal = 0;
    normal.xyz = inNormal.xyz;		//法線をコピーする
    outNormal = mul(normal, World);	//法線のワールド変換＆出力
	
	//テクスチャ座標を出力
	outTexCoord = inTexCoord;
	//頂点色とマテリアル色を合成して出力
	outDiffuse = inDiffuse * Diffuse;
}



//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		g_Texture : register( t0 );
SamplerState	g_SamplerState : register( s0 );


//=============================================================================
// ピクセルシェーダ
//=============================================================================
void PixelShaderPolygon( in  float4 inPosition		: POSITION0,
						 in  float4 inNormal		: NORMAL0,
						 in  float2 inTexCoord		: TEXCOORD0,
						 in  float4 inDiffuse		: COLOR0,

						 out float4 outDiffuse		: SV_Target )
{
	//法線を正規化
    float3 normal = normalize(inNormal.xyz);
	//明るさを計算
    float3 light = dot(normal, -Light.Direction.xyz);
    light = clamp(light, 0.1f, 1.0f);
	//明るさに環境光をプラス
    light += Light.Ambient.rgb;
	
	
    outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord);//テクスチャピクセル取得
    outDiffuse *= (inDiffuse * Light.Diffuse); //頂点色とライトの色との合成

	//最終的な色を明るさで調整
    outDiffuse.rgb *= light;
	
}
