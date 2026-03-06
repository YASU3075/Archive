//-----------------------------------------------
// debugLinePS.hlsl
// 制作日：2026/01/15
// 制作者：安田晴人
//-----------------------------------------------
#include "common.hlsl"

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    outDiffuse = In.Diffuse;
}