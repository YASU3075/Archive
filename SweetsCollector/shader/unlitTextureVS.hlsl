//-----------------------------------------------
// unlitTextureVS.hlsl
// 制作日：2026/01/19
// 制作者：安田晴人
//-----------------------------------------------
#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
	// ワールドビュー射影行列の計算
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	// 頂点の変換
    Out.Position = mul(float4(In.Position, 1.0f), wvp);
	
	Out.TexCoord = In.TexCoord;
	Out.Diffuse = In.Diffuse * Material.Diffuse;
}

