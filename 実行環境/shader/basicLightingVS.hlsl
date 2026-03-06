//-----------------------------------------------
// basicLightingVS.hlsl
// 制作日：2025/11/20
// 制作者：安田晴人
//-----------------------------------------------
#include "lighting.hlsl"

void main(VS_IN input, out PS_IN output)
{
    // WVP行列を使用して頂点の位置を変換
    float4 world_pos = mul(float4(input.Position, 1.0f), World);
    float4 view_pos = mul(world_pos, View);
    output.Position = mul(view_pos, Projection);

    float3 worldNormal = mul(input.Normal, (float3x3) World);
    worldNormal = normalize(worldNormal);

    output.WorldPos = world_pos.xyz;
    output.Normal = worldNormal;
    output.Diffuse = input.Diffuse;
    output.TexCoord = input.TexCoord;
}