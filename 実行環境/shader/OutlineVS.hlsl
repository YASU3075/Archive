//-----------------------------------------------
// OutlineVS.hlsl
// 制作日：2026/02/11
// 制作者；安田晴人
//-----------------------------------------------
#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{    
    // ワールド空間での頂点位置と法線の計算
    float4 world_pos = mul(float4(In.Position, 1.0f), World);
    float3 world_n = mul(float4(In.Normal, 0.0f), World).xyz;
    world_n = normalize(world_n);
    
    // 法線方向に頂点を移動させてアウトラインを作成
    world_pos.xyz += world_n * outline_width;

    // ワールドビュー射影行列の計算
    matrix vp;
    vp = mul(View, Projection);

    // 頂点の変換
    Out.Position = mul(world_pos, vp);

    Out.TexCoord = In.TexCoord;
    Out.Diffuse = In.Diffuse;
    Out.WorldPos = world_pos.xyz;
    Out.Normal = world_n;
}