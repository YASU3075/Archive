//-----------------------------------------------
// debugLineVS.hlsl
// 制作日：2026/01/15
// 制作者：安田晴人
//-----------------------------------------------
#include "common.hlsl"

void main(VS_IN input, out PS_IN output)
{
    float4 world_pos = mul(float4(input.Position, 1.0f), World);
    float4 view_pos = mul(world_pos, View);
    output.Position = mul(view_pos, Projection);

    output.WorldPos = world_pos.xyz;
    output.Diffuse = input.Diffuse;
}