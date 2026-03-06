

cbuffer WorldBuffer : register(b0)
{
	matrix World;
}
cbuffer ViewBuffer : register(b1)
{
	matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}




struct MATERIAL
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emission;
	float Shininess;
	bool TextureEnable;
    float2 UVScale;
    float2 UVOffset;
	float2 Dummy;
};

cbuffer MaterialBuffer : register(b3)
{
	MATERIAL Material;
}


static const uint MAX_LIGHTS = 4;

struct LIGHT
{
	bool Enable;
    uint Type;		// 0: Directional, 1: Point, 2: Spot
    uint Padding1;
    uint Padding2;
	
    float4 Position;
	float4 Direction;
	float4 Diffuse;
	float4 Ambient;
	
	float Range;
	float SpotInner;
    float SpotOuter;
    float Padding3;
};

cbuffer LightBuffer : register(b4)
{
	LIGHT Light[MAX_LIGHTS];
    uint LightCount;
    float3 Padding;
}

cbuffer PostEffect : register(b6)
{
    float4 pe_param0; // x=progress, y=softness, z=aspect, w=mode
    float4 pe_param1; // x=centerX, y=centerY,
    float4 pe_color; // color
}

cbuffer CircleWipe : register(b7)
{
    float4 cw_param0; // x=progress, y=softness, z=aspect, w=unused
    float4 cw_param1; // x=centerX, y=centerY, z=unused, w=unused
}

cbuffer OutlineBuffer : register(b8)
{
    float4 outline_color;	// color
    float outline_width; // width
    float3 pad;
}



struct VS_IN
{
	float3 Position		: POSITION0;
	float3 Normal		: NORMAL0;
	float4 Diffuse		: COLOR0;
	float2 TexCoord		: TEXCOORD0;
};


struct PS_IN
{
	float4 Position		: SV_POSITION;
    float3 WorldPos		: TEXCOORD0;
    float3 Normal		: TEXCOORD1;
	float4 Diffuse		: COLOR0;
	float2 TexCoord		: TEXCOORD2;
};
