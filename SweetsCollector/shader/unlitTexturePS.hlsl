//-----------------------------------------------
// unlitTexturePS.hlsl
// 制作日：2026/01/19
// 制作者：安田晴人
//-----------------------------------------------
#include "common.hlsl"

Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);

float2 ApplyUV(float2 uv)
{
    return uv * Material.UVScale + Material.UVOffset;
}

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    // UVにスケールとオフセットを適用
    float2 uv = ApplyUV(In.TexCoord);
    
    if (Material.TextureEnable)
    { // テクスチャが有効な場合はテクスチャの色を乗算
        outDiffuse = g_Texture.Sample(g_SamplerState, uv);
        outDiffuse *= In.Diffuse;
    }
    else
    { // テクスチャが無効な場合は頂点カラーをそのまま出力
        outDiffuse = In.Diffuse;
    }
}
