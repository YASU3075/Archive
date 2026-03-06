//-----------------------------------------------
// FadeInOutPS.hlsl
// 制作日：2026/01/19
// 制作者：安田晴人
//-----------------------------------------------
#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

float IrisOutsideMask(float2 uv, float2 center, float progress, float softness, float aspect)
{
    float2 d = uv - center;
    d.x *= aspect;
    float dist = length(d);

    // distがedge0以下なら0、edge1以上なら1、それ以外は線形補間
    float edge0 = progress;
    float edge1 = progress + max(softness, 1e-5);
    return saturate((dist - edge0) / (edge1 - edge0));
}

float MaxRadius(float2 center, float aspect)
{
    // テクスチャの4隅との距離の最大値を求める
    
    float2 corners[4] =
    {
        float2(0.0f, 0.0f),
        float2(1.0f, 0.0f),
        float2(0.0f, 1.0f),
        float2(1.0f, 1.0f)
    };

    float max_dist = 0.0f;

    for (int i = 0; i < 4; i++)
    {// centerからcornerへの距離を求める
        float2 d = corners[i] - center;
        d.x *= aspect;
        float dist = length(d);
        max_dist = max(max_dist, dist);
    }
    return max_dist;
}

void main(in PS_IN In, out float4 out_diffuse : SV_Target)
{
    float progress = pe_param0.x;
    float softness = pe_param0.y;
    float aspect = pe_param0.z;
    float mode = pe_param0.w;

    float2 center = pe_param1.xy;
    

    // mode: 0=none, 1=fade, 2=iris
    if (mode < 0.5f)
    {// none
        out_diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
        return;
    }
        
    float a = 0.0f;

    if (mode < 1.5f)
    { // fade
        a = saturate(progress);
    }
    else
    { // iris
        float max_radius = MaxRadius(center, aspect);
        float radius = (1 - progress) * max_radius;
        radius -= softness;
        float mask = IrisOutsideMask(In.TexCoord, center, radius, softness, aspect);
        a = mask;
    }
    
    // 出力
    out_diffuse = float4(pe_color.rgb, a);
}