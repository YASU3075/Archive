//-----------------------------------------------
// OutlinePS.hlsl
// 制作日：2026/02/11
// 制作者；安田晴人
//-----------------------------------------------
#include "common.hlsl"

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    // アウトラインの色を指定
    outDiffuse = outline_color;
}