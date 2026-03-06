//-----------------------------------------------
// CircleWipePS.hlsl
// 制作日：2026/01/27
// 制作者：安田晴人
//-----------------------------------------------
#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

float Wrap01(float x)
{ // 0.0f <= x < 1.0f に変換する
    return frac(x + 1.0f);
}

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float4 c = g_Texture.Sample(g_SamplerState, In.TexCoord);
    
    if (c.a <= 0.001f)
    {
        outDiffuse = c;
        return;
    }
    
    float2 p = In.TexCoord - cw_param1.xy;
    p.x *= cw_param0.z;
    
    float ang = atan2(p.y, p.x);
    float ang01 = Wrap01((ang + 3.14159265f) / (2.0f * 3.14159265f));
    
    float a = Wrap01(ang01 - 0.25f);
    
    float threshold = 1.0f - cw_param0.x;
    
    float mask = smoothstep(threshold, threshold + cw_param0.y, a);
    c.a *= 1.0f - mask;
    
    outDiffuse = c;
}