//-----------------------------------------------
// lighting.hlsl
// 制作日：2025//11/20
// 制作者：安田晴人
//-----------------------------------------------

# pragma once

#include "common.hlsl"

static const uint LIGHT_TYPE_DIRECTIONAL = 0;
static const uint LIGHT_TYPE_POINT = 1;
static const uint LIGHT_TYPE_SPOT = 2;

//-----------------------------------------------
// 距離減衰関数
//-----------------------------------------------
float ComputeAttenuation(float distance, float range)
{
    // 距離を範囲で割って0～1に正規化
    float t = saturate(distance / max(range, 0.0001f));
    
    // 1から引く
    float att = 1.0f - t;
    
    // 二乗して滑らかにする
    return att * att;
}

//-----------------------------------------------
// DirectionalLightの計算
//-----------------------------------------------
float3 ComputeDirectionalLight(LIGHT light, float3 w_pos, float3 normal)
{
    if (light.Enable == false)
    { // 無効なら0を返す
        return float3(0, 0, 0);
    }
    
    // 法線、光、視線、ハーフベクトルの計算
    float3 N = normalize(normal);
    float3 L = normalize(-light.Direction.xyz);
    float3 V = normalize(-w_pos);
    float3 H = normalize(L + V);
    
    // ランバート・フォンの計算
    float NdotL = max(dot(N, L), 0.0f);
    float NdotH = max(dot(N, H), 0.0f);
    
    // 各成分の計算
    float3 diffuse = light.Diffuse.rgb * NdotL;
    float3 specular = light.Diffuse.rgb * pow(NdotH, Material.Shininess);
    float3 ambient = light.Ambient.rgb;
    
    float3 result = ambient + diffuse + specular;
    
    return result;
}

//-----------------------------------------------
// Point・Spotの準備
//-----------------------------------------------
void SetupPointSpotLight(LIGHT light, float3 w_pos, out float3 L, out float dist, out float attenuation)
{
    // ライトから頂点へのベクトル
    float3 to_light = light.Position.xyz - w_pos;
    
    // 距離の計算
    dist = length(to_light);
    
    // 正規化したライト方向ベクトル
    L = to_light / max(dist, 0.0001f);
    
    // 減衰の計算
    attenuation = ComputeAttenuation(dist, light.Range);
}


//-----------------------------------------------
// PointLightの計算
//-----------------------------------------------
float3 ComputePointLight(LIGHT light, float3 w_pos, float3 normal)
{
    if (light.Enable == false)
    { // 無効なら0を返す
        return float3(0, 0, 0);
    }
    
    // 法線、光、視線、ハーフベクトルの計算
    float3 N = normalize(normal);
    float3 L;
    float dist;
    float attenuation;
    SetupPointSpotLight(light, w_pos, L, dist, attenuation);
    
    if (attenuation <= 0.0f)
    { // 範囲外なら0を返す
        return float3(0, 0, 0);
    }
    
    float3 V = normalize(-w_pos);
    float3 H = normalize(L + V);
    
    // ランバート・フォンの計算
    float NdotL = max(dot(N, L), 0.0f);
    float NdotH = max(dot(N, H), 0.0f);
    
    // 各成分の計算
    float3 diffuse = light.Diffuse.rgb * NdotL;
    float3 specular = light.Diffuse.rgb * pow(NdotH, Material.Shininess);
    float3 ambient = light.Ambient.rgb;
    
    float3 result = ambient + (diffuse + specular) * attenuation;
    
    return result;
}

//-----------------------------------------------
// SpotLightの計算
//-----------------------------------------------
float3 ComputeSpotLight(LIGHT light, float3 w_pos, float3 normal)
{
    if (light.Enable == false)
    { // 無効なら0を返す
        return float3(0, 0, 0);
    }
    
    // 法線、光、視線、ハーフベクトルの計算
    float3 N = normalize(normal);
    float3 L;
    float dist;
    float attenuation;
    SetupPointSpotLight(light, w_pos, L, dist, attenuation);
    
    if (attenuation <= 0.0f)
    { // 範囲外なら0を返す
        return float3(0, 0, 0);
    }
    
    // スポットライトの内外判定
    float3 spot_dir = normalize(-light.Direction.xyz);
    float spot_cos = dot(spot_dir, -L);
    
    float spot_effect = smoothstep(light.SpotOuter, light.SpotInner, spot_cos);
    
    if (spot_effect <= 0.0f)
    { // スポット範囲外なら0を返す
        return float3(0, 0, 0);
    }
    
    float3 V = normalize(-w_pos);
    float3 H = normalize(L + V);
    
    // ランバート・フォンの計算
    float NdotL = max(dot(N, L), 0.0f);
    float NdotH = max(dot(N, H), 0.0f);
    
    // 各成分の計算
    float3 diffuse = light.Diffuse.rgb * NdotL;
    float3 specular = light.Diffuse.rgb * pow(NdotH, Material.Shininess);
    float3 ambient = light.Ambient.rgb;
    
    float3 result = ambient + (diffuse + specular) * attenuation * spot_effect;
    
    return result;
}

//-----------------------------------------------
// 1つのライトによるライティング計算
//-----------------------------------------------
float3 ComputeLight(LIGHT light, float3 w_pos, float3 normal)
{
    if (light.Enable == false)
    { // 無効なら0を返す
        return float3(0, 0, 0);
    }
    
    if (light.Type == LIGHT_TYPE_DIRECTIONAL)
    { // ディレクショナルライト
        return ComputeDirectionalLight(light, w_pos, normal);
    }
    else if (light.Type == LIGHT_TYPE_POINT)
    { // ポイントライト
        return ComputePointLight(light, w_pos, normal);
    }
    else if (light.Type == LIGHT_TYPE_SPOT)
    { // スポットライト
        return ComputeSpotLight(light, w_pos, normal);
    }
    
    return float3(0, 0, 0);
}

//-----------------------------------------------
// 全ライトによるライティング計算
//-----------------------------------------------
float3 ComputeLighting(float3 w_pos, float3 normal)
{
    float3 final_color = float3(0, 0, 0);
    
    // 全ライト分ループ
    for (uint i = 0; i < LightCount; i++)
    {
        final_color += ComputeLight(Light[i], w_pos, normal);
    }
    
    return final_color;
}