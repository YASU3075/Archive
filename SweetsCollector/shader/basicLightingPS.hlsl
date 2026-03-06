//-----------------------------------------------
// basicLightingPS.hlsl
// 制作日：2025/11/20
// 制作者：安田晴人
//-----------------------------------------------
#include "lighting.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 out_diffuse : SV_Target)
{
    float3 N = normalize(In.Normal);
    float3 w_pos = In.WorldPos;
    
    float3 base_color = Material.Diffuse.rgb * In.Diffuse.rgb;
    
    if (Material.TextureEnable)
    {
        float4 tex_color = g_Texture.Sample(g_SamplerState, In.TexCoord);
        base_color *= tex_color.rgb;
    }
    
    float3 lighting = ComputeLighting(w_pos, N);
    
    out_diffuse = float4(base_color * lighting + Material.Emission.rgb, 1.0f);
}