//-----------------------------------------------
// FadeInOutVS.hlsl
// 制作日：2026/01/19
// 制作者：安田晴人
//-----------------------------------------------
#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{
    // ワールド、ビュー、プロジェクション行列を掛け合わせる
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    // 頂点の位置を変換して出力
    Out.Position = mul(float4(In.Position, 1.0f), wvp);
    
    Out.TexCoord = In.TexCoord;
    Out.Diffuse = In.Diffuse * Material.Diffuse;
}