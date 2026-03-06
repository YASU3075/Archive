//-----------------------------------------------
// RenderTarget.h
// 制作日：2025/11/13
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include "Main.h"

struct RENDER_TARGET_DESC
{
	unsigned int width;          // 幅
	unsigned int height;         // 高さ
	DXGI_FORMAT color_format = DXGI_FORMAT_R8G8B8A8_UNORM; // カラーフォーマット
	bool has_depth = true;    // 深度バッファを持つかどうか
	DXGI_FORMAT depth_format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 深度フォーマット
	bool shader_resource = true; // シェーダーリソースビューを持つかどうか
};

class RENDER_TARGET
{
public:
	~RENDER_TARGET() { Release(); };

	ID3D11Texture2D* color_texture = nullptr;                   // カラーテクスチャ
	ID3D11RenderTargetView* rtv = nullptr;                    // レンダーターゲットビュー
	ID3D11ShaderResourceView* srv = nullptr;                  // シェーダーリソースビュー
	ID3D11Texture2D* depth_texture = nullptr;                   // 深度テクスチャ
	ID3D11DepthStencilView* dsv = nullptr;                   // 深度ステンシルビュー

	unsigned int width = 0;          // 幅
	unsigned int height = 0;         // 高さ

	void Release()
	{
		if (srv) { srv->Release(); srv = nullptr; }
		if (rtv) { rtv->Release(); rtv = nullptr; }
		if (color_texture) { color_texture->Release(); color_texture = nullptr; }
		if (dsv) { dsv->Release(); dsv = nullptr; }
		if (depth_texture) { depth_texture->Release(); depth_texture = nullptr; }
	};
};
