
#include "Main.h"
#include "renderer/Renderer.h"
#include "component/renderer/RendererComponent.h"
#include <io.h>


D3D_FEATURE_LEVEL       Renderer::m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device*           Renderer::m_Device{};
ID3D11DeviceContext*    Renderer::m_DeviceContext{};
IDXGISwapChain*         Renderer::m_SwapChain{};
ID3D11RenderTargetView* Renderer::m_RenderTargetView{};
ID3D11DepthStencilView* Renderer::m_DepthStencilView{};

RENDER_TARGET* Renderer::m_BackBufferRenderTarget{};

ID3D11Buffer*			Renderer::m_WorldBuffer{};
ID3D11Buffer*			Renderer::m_ViewBuffer{};
ID3D11Buffer*			Renderer::m_ProjectionBuffer{};
ID3D11Buffer*			Renderer::m_MaterialBuffer{};
ID3D11Buffer*			Renderer::m_LightBuffer{};
LIGHT_BUFFER			Renderer::m_LightData{};


ID3D11DepthStencilState* Renderer::m_DepthStateEnable{};
ID3D11DepthStencilState* Renderer::m_DepthStateDisable{};
ID3D11DepthStencilState* Renderer::m_DepthStateReadOnly{};


ID3D11BlendState*		Renderer::m_BlendState{};
ID3D11BlendState*		Renderer::m_BlendStateATC{};
ID3D11BlendState* Renderer::m_BlendStateOpaque{};
ID3D11BlendState* Renderer::m_BlendStateAdd{};

ID3D11RasterizerState* Renderer::m_RasterizerStateCullBack{};
ID3D11RasterizerState* Renderer::m_RasterizerStateCullFront{};
ID3D11RasterizerState* Renderer::m_RasterizerStateCullNone{};

ID3D11Buffer* Renderer::m_PostEffectBuffer{};
POST_EFFECT_PARAMS Renderer::m_PostEffectData{};

ID3D11Buffer* Renderer::m_CircleWipeBuffer{};
ID3D11Buffer* Renderer::m_OutlineParamsBuffer{};

ID3D11VertexShader* Renderer::m_DebugLineVS{};
ID3D11PixelShader* Renderer::m_DebugLinePS{};
ID3D11InputLayout* Renderer::m_DebugLineLayout{};
ID3D11Buffer* Renderer::m_DebugLineVertexBuffer{};
UINT Renderer::m_DebugLineVertexBufferaxVerts{};


void Renderer::Init()
{
	HRESULT hr = S_OK;




	// デバイス、スワップチェーン作成
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = SCREEN_WIDTH;
	swapChainDesc.BufferDesc.Height = SCREEN_HEIGHT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = GetWindow();
	swapChainDesc.SampleDesc.Count = 4;		// マルチサンプリング
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain( NULL,
										D3D_DRIVER_TYPE_HARDWARE,
										NULL,
										0,
										NULL,
										0,
										D3D11_SDK_VERSION,
										&swapChainDesc,
										&m_SwapChain,
										&m_Device,
										&m_FeatureLevel,
										&m_DeviceContext );






	// レンダーターゲットビュー作成
	ID3D11Texture2D* renderTarget{};
	m_SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&renderTarget );
	m_Device->CreateRenderTargetView( renderTarget, NULL, &m_RenderTargetView );
	renderTarget->Release();


	// デプスステンシルバッファ作成
	ID3D11Texture2D* depthStencile{};
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = swapChainDesc.BufferDesc.Width;
	textureDesc.Height = swapChainDesc.BufferDesc.Height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D16_UNORM;
	textureDesc.SampleDesc = swapChainDesc.SampleDesc;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	m_Device->CreateTexture2D(&textureDesc, NULL, &depthStencile);

	// デプスステンシルビュー作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = textureDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depthStencilViewDesc.Flags = 0;
	m_Device->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &m_DepthStencilView);
	depthStencile->Release();


	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);





	// ビューポート設定
	D3D11_VIEWPORT viewport;
	viewport.Width = (FLOAT)SCREEN_WIDTH;
	viewport.Height = (FLOAT)SCREEN_HEIGHT;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	m_DeviceContext->RSSetViewports( 1, &viewport );



	// ラスタライザステート設定
	{
		D3D11_RASTERIZER_DESC rd{};
		rd.FillMode = D3D11_FILL_SOLID;
		rd.DepthClipEnable = TRUE;
		rd.MultisampleEnable = FALSE;

		rd.CullMode = D3D11_CULL_BACK;
		m_Device->CreateRasterizerState(&rd, &m_RasterizerStateCullBack);

		rd.CullMode = D3D11_CULL_FRONT;
		m_Device->CreateRasterizerState(&rd, &m_RasterizerStateCullFront);

		rd.CullMode = D3D11_CULL_NONE;
		m_Device->CreateRasterizerState(&rd, &m_RasterizerStateCullNone);

		// デフォルト
		m_DeviceContext->RSSetState(m_RasterizerStateCullBack);
	}




	// ブレンドステート設定
	D3D11_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_Device->CreateBlendState( &blendDesc, &m_BlendState );

	blendDesc.AlphaToCoverageEnable = TRUE;
	m_Device->CreateBlendState( &blendDesc, &m_BlendStateATC );

	float blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff );

	{
		D3D11_BLEND_DESC bd{};
		bd.AlphaToCoverageEnable = FALSE;
		bd.IndependentBlendEnable = FALSE;
		bd.RenderTarget[0].BlendEnable = FALSE;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		m_Device->CreateBlendState(&bd, &m_BlendStateOpaque);
	}

	{
		D3D11_BLEND_DESC add{};
		add.AlphaToCoverageEnable = FALSE;
		add.IndependentBlendEnable = FALSE;
		add.RenderTarget[0].BlendEnable = TRUE;
		add.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		add.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		add.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		add.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		add.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		add.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		add.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		m_Device->CreateBlendState(&add, &m_BlendStateAdd);
	}

	// デプスステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = FALSE;

	m_Device->CreateDepthStencilState( &depthStencilDesc, &m_DepthStateEnable );//深度有効ステート

	depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	m_Device->CreateDepthStencilState( &depthStencilDesc, &m_DepthStateDisable );//深度無効ステート

	m_DeviceContext->OMSetDepthStencilState( m_DepthStateEnable, NULL );

	{
		D3D11_DEPTH_STENCIL_DESC ds{};
		ds.DepthEnable = TRUE;
		ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		ds.DepthFunc = D3D11_COMPARISON_LESS;
		ds.StencilEnable = FALSE;
		m_Device->CreateDepthStencilState(&ds, &m_DepthStateReadOnly);
	}


	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState{};
	m_Device->CreateSamplerState( &samplerDesc, &samplerState );

	m_DeviceContext->PSSetSamplers( 0, 1, &samplerState );



	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	m_Device->CreateBuffer( &bufferDesc, NULL, &m_WorldBuffer );
	m_DeviceContext->VSSetConstantBuffers( 0, 1, &m_WorldBuffer);

	m_Device->CreateBuffer( &bufferDesc, NULL, &m_ViewBuffer );
	m_DeviceContext->VSSetConstantBuffers( 1, 1, &m_ViewBuffer );

	m_Device->CreateBuffer( &bufferDesc, NULL, &m_ProjectionBuffer );
	m_DeviceContext->VSSetConstantBuffers( 2, 1, &m_ProjectionBuffer );


	bufferDesc.ByteWidth = sizeof(MATERIAL_DATA);

	m_Device->CreateBuffer( &bufferDesc, NULL, &m_MaterialBuffer );
	m_DeviceContext->VSSetConstantBuffers( 3, 1, &m_MaterialBuffer );
	m_DeviceContext->PSSetConstantBuffers( 3, 1, &m_MaterialBuffer );


	bufferDesc.ByteWidth = sizeof(LIGHT_BUFFER);

	m_Device->CreateBuffer( &bufferDesc, NULL, &m_LightBuffer );
	m_DeviceContext->VSSetConstantBuffers( 4, 1, &m_LightBuffer );
	m_DeviceContext->PSSetConstantBuffers( 4, 1, &m_LightBuffer );

	ZeroMemory(&m_LightData, sizeof(LIGHT_BUFFER));
	m_LightData.num_lights = 0;
	Renderer::SetLights(nullptr, 0);



	// ライト初期化
	LIGHT light{};
	light.Enable = true;
	light.Direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	light.Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = XMFLOAT4(1.5f, 1.5f, 1.5f, 1.0f);
	SetLights(&light, 1);



	// マテリアル初期化
	MATERIAL_DATA material{};
	material.m_diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.m_ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	{
		// ポストエフェクト用定数バッファ作成
		D3D11_BUFFER_DESC bd{};
		bd.ByteWidth = sizeof(POST_EFFECT_PARAMS);
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;

		HRESULT hr = m_Device->CreateBuffer(&bd, nullptr, &m_PostEffectBuffer);
		assert(SUCCEEDED(hr));

		// 初期値
		ZeroMemory(&m_PostEffectData, sizeof(m_PostEffectData));
		m_PostEffectData.param0 = XMFLOAT4(0, 0, static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), static_cast<float>(POST_EFFECT_MODE::POST_EFFECT_MODE_NONE));
		m_PostEffectData.param1 = XMFLOAT4(0.5f, 0.5f, 0, 0);
		m_PostEffectData.color = XMFLOAT4(0, 0, 0, 1);

		m_DeviceContext->UpdateSubresource(m_PostEffectBuffer, 0, nullptr, &m_PostEffectData, 0, 0);

		// PSレジスタ6にセット
		m_DeviceContext->PSSetConstantBuffers(6, 1, &m_PostEffectBuffer);
	}

	{
		// サークルワイプ用定数バッファ作成
		D3D11_BUFFER_DESC bd{};
		bd.ByteWidth = sizeof(POST_EFFECT_PARAMS);
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		HRESULT hr = m_Device->CreateBuffer(&bd, nullptr, &m_CircleWipeBuffer);
		assert(SUCCEEDED(hr));
		// PSレジスタ7にセット
		m_DeviceContext->PSSetConstantBuffers(7, 1, &m_CircleWipeBuffer);
	}

	{
		// アウトライン用定数バッファ作成
		D3D11_BUFFER_DESC bd{};
		bd.ByteWidth = sizeof(OUTLINE_PARAMS);
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		HRESULT hr = m_Device->CreateBuffer(&bd, nullptr, &m_OutlineParamsBuffer);
		assert(SUCCEEDED(hr));
		// PSレジスタ8にセット
		m_DeviceContext->PSSetConstantBuffers(8, 1, &m_OutlineParamsBuffer);
		// VSレジスタ8にセット
		m_DeviceContext->VSSetConstantBuffers(8, 1, &m_OutlineParamsBuffer);
	}

	if (!m_BackBufferRenderTarget)
	{
		m_BackBufferRenderTarget = new RENDER_TARGET();
	}

	DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
	m_SwapChain->GetDesc(&swap_chain_desc);
	m_BackBufferRenderTarget->width = swap_chain_desc.BufferDesc.Width;
	m_BackBufferRenderTarget->height = swap_chain_desc.BufferDesc.Height;

	m_BackBufferRenderTarget->rtv = m_RenderTargetView;
	m_BackBufferRenderTarget->dsv = m_DepthStencilView;

	InitDebugLine();
}



void Renderer::Uninit()
{

	m_WorldBuffer->Release();
	m_ViewBuffer->Release();
	m_ProjectionBuffer->Release();
	m_LightBuffer->Release();
	m_MaterialBuffer->Release();
	

	m_DeviceContext->ClearState();
	m_RenderTargetView->Release();
	m_SwapChain->Release();
	m_DeviceContext->Release();
	m_Device->Release();

	m_PostEffectBuffer->Release();

	m_DebugLineVertexBuffer->Release();
	m_DebugLineLayout->Release();
	m_DebugLinePS->Release();
	m_DebugLineVS->Release();
}




void Renderer::Begin()
{
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView( m_RenderTargetView, clearColor );
	m_DeviceContext->ClearDepthStencilView( m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}



void Renderer::End()
{
	m_SwapChain->Present( 1, 0 );
}

void Renderer::InitDebugLine()
{
	CreateVertexShader(&m_DebugLineVS, &m_DebugLineLayout, "shader\\debugLineVS.cso");
	CreatePixelShader(&m_DebugLinePS, "shader\\debugLinePS.cso");

	m_DebugLineVertexBufferaxVerts = 65536;
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(DEBUG_LINE_VERTEX) * m_DebugLineVertexBufferaxVerts;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = m_Device->CreateBuffer(&bd, NULL, &m_DebugLineVertexBuffer);
	assert(SUCCEEDED(hr));
}

void Renderer::DrawDebugLines(DEBUG_LINE* lines, size_t line_count, bool depth_test)
{
	if (!lines || line_count == 0)
	{
		return;
	}

	size_t vert_count = line_count * 2;

	if (vert_count > m_DebugLineVertexBufferaxVerts)
	{
		vert_count = m_DebugLineVertexBufferaxVerts;
	}

	D3D11_MAPPED_SUBRESOURCE ms{};
	HRESULT hr = m_DeviceContext->Map(m_DebugLineVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
	assert(SUCCEEDED(hr));

	DEBUG_LINE_VERTEX* v = (DEBUG_LINE_VERTEX*)ms.pData;
	for (size_t i = 0; i < vert_count / 2; i++)
	{
		v[i * 2 + 0].Position = XMFLOAT3(lines[i].a.x, lines[i].a.y, lines[i].a.z);
		v[i * 2 + 0].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
		v[i * 2 + 0].Diffuse = lines[i].color;
		v[i * 2 + 0].TexCoord = XMFLOAT2(0.0f, 0.0f);
		

		v[i * 2 + 1].Position = XMFLOAT3(lines[i].b.x, lines[i].b.y, lines[i].b.z);
		v[i * 2 + 1].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
		v[i * 2 + 1].Diffuse = lines[i].color;
		v[i * 2 + 1].TexCoord = XMFLOAT2(0.0f, 0.0f);
	}

	m_DeviceContext->Unmap(m_DebugLineVertexBuffer, 0);

	UINT stride = sizeof(DEBUG_LINE_VERTEX);
	UINT offset = 0;
	m_DeviceContext->IASetVertexBuffers(0, 1, &m_DebugLineVertexBuffer, &stride, &offset);
	m_DeviceContext->IASetInputLayout(m_DebugLineLayout);
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	m_DeviceContext->VSSetShader(m_DebugLineVS, NULL, 0);
	m_DeviceContext->PSSetShader(m_DebugLinePS, NULL, 0);

	if (depth_test)
	{
		SetDepthMode(DEPTH_MODE::DEPTH_MODE_READ_ONLY);
	}
	else
	{
		SetDepthMode(DEPTH_MODE::DEPTH_MODE_OFF);
	}

	SetBlendMode(BLEND_MODE::BLEND_MODE_ALPHA);

	Renderer::SetWorldMatrix(XMMatrixIdentity());

	m_DeviceContext->Draw((UINT)vert_count, 0);

	SetDepthMode(DEPTH_MODE::DEPTH_MODE_READ_WRITE);
	SetBlendMode(BLEND_MODE::BLEND_MODE_OPAQUE);
}


void Renderer::BeginCameraPass(const CAMERA_PASS_DESC& desc)
{
	RENDER_TARGET* rt = desc.render_target ? desc.render_target : m_BackBufferRenderTarget;
	UnbindAllPS_SRV(m_DeviceContext);

	ID3D11RenderTargetView* rtv = rt->rtv;
	m_DeviceContext->OMSetRenderTargets(1, &rtv, rt->dsv);

	ApplyViewport(m_DeviceContext, desc.viewport_rect, rt->width, rt->height);

	if (desc.clear_params.clear_color)
	{
		m_DeviceContext->ClearRenderTargetView(rtv, desc.clear_params.Color);
	}
	if (desc.clear_params.clear_depth)
	{
		m_DeviceContext->ClearDepthStencilView(rt->dsv, D3D11_CLEAR_DEPTH, desc.clear_params.Depth, desc.clear_params.Stencil);
	}
}

void Renderer::EndCameraPass()
{
	// 特に何もしない
}




void Renderer::SetDepthEnable( bool Enable )
{
	if( Enable )
		m_DeviceContext->OMSetDepthStencilState( m_DepthStateEnable, NULL );
	else
		m_DeviceContext->OMSetDepthStencilState( m_DepthStateDisable, NULL );

}



void Renderer::SetATCEnable( bool Enable )
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (Enable)
		m_DeviceContext->OMSetBlendState(m_BlendStateATC, blendFactor, 0xffffffff);
	else
		m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff);

}

void Renderer::SetWorldViewProjection2D()
{
	SetWorldMatrix(XMMatrixIdentity());
	SetViewMatrix(XMMatrixIdentity());

	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	SetProjectionMatrix(projection);
}


void Renderer::SetWorldMatrix(XMMATRIX WorldMatrix)
{
	XMFLOAT4X4 worldf;
	XMStoreFloat4x4(&worldf, XMMatrixTranspose(WorldMatrix));
	m_DeviceContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &worldf, 0, 0);
}

void Renderer::SetViewMatrix(XMMATRIX ViewMatrix)
{
	XMFLOAT4X4 viewf;
	XMStoreFloat4x4(&viewf, XMMatrixTranspose(ViewMatrix));
	m_DeviceContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &viewf, 0, 0);
}

void Renderer::SetProjectionMatrix(XMMATRIX ProjectionMatrix)
{
	XMFLOAT4X4 projectionf;
	XMStoreFloat4x4(&projectionf, XMMatrixTranspose(ProjectionMatrix));
	m_DeviceContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projectionf, 0, 0);

}



void Renderer::SetMaterial( MATERIAL_DATA material_data )
{
	m_DeviceContext->UpdateSubresource( m_MaterialBuffer, 0, NULL, &material_data, 0, 0 );
}

void Renderer::SetLights(const LIGHT* lights, UINT num_lights)
{
	if (num_lights > MAX_LIGHTS)
	{
		num_lights = MAX_LIGHTS;
	}

	// バッファクリア
	ZeroMemory(&m_LightData, sizeof(LIGHT_BUFFER));

	// ライト情報コピー
	for (UINT i = 0; i < num_lights; i++)
	{
		m_LightData.lights[i] = lights[i];
	}
	m_LightData.num_lights = num_lights;

	m_DeviceContext->UpdateSubresource(m_LightBuffer, 0, NULL, &m_LightData, 0, 0);
}



void Renderer::UnbindAllPS_SRV(ID3D11DeviceContext* ctx)
{
	ID3D11ShaderResourceView* null_srvs[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = {};
	ctx->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, null_srvs);
}

void Renderer::ApplyViewport(ID3D11DeviceContext* ctx, const VIEWPORT_RECT& vp, UINT target_w, UINT target_h)
{
	D3D11_VIEWPORT viewport{};
	viewport.TopLeftX = vp.x;
	viewport.TopLeftY = vp.y;
	viewport.Width = vp.w;
	viewport.Height = vp.h;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	ctx->RSSetViewports(1, &viewport);
}

void Renderer::CreateVertexShader( ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName )
{

	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	assert(file);

	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreateVertexShader(buffer, fsize, NULL, VertexShader);


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	m_Device->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		VertexLayout);

	delete[] buffer;
}



void Renderer::CreatePixelShader( ID3D11PixelShader** PixelShader, const char* FileName )
{
	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	assert(file);

	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreatePixelShader(buffer, fsize, NULL, PixelShader);

	delete[] buffer;
}

RENDER_TARGET* Renderer::CreateRenderTarget(const RENDER_TARGET_DESC& desc)
{
	auto rt = new RENDER_TARGET();
	rt->width = desc.width;
	rt->height = desc.height;

	D3D11_TEXTURE2D_DESC td{};
	td.Width = desc.width;
	td.Height = desc.height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = desc.color_format;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_RENDER_TARGET | (desc.shader_resource ? D3D11_BIND_SHADER_RESOURCE : 0);
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	HRESULT hr = m_Device->CreateTexture2D(&td, NULL, &rt->color_texture);
	assert(SUCCEEDED(hr));

	hr = m_Device->CreateRenderTargetView(rt->color_texture, NULL, &rt->rtv);
	assert(SUCCEEDED(hr));

	if (desc.shader_resource)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC sd{};
		sd.Format = desc.color_format;
		sd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		sd.Texture2D.MostDetailedMip = 0;
		sd.Texture2D.MipLevels = 1;

		hr = m_Device->CreateShaderResourceView(rt->color_texture, &sd, &rt->srv);
		assert(SUCCEEDED(hr));
	}

	if (desc.has_depth)
	{
		D3D11_TEXTURE2D_DESC depthDesc{};
		depthDesc.Width = desc.width;
		depthDesc.Height = desc.height;
		depthDesc.MipLevels = 1;
		depthDesc.ArraySize = 1;
		depthDesc.Format = desc.depth_format;
		depthDesc.SampleDesc.Count = 1;
		depthDesc.SampleDesc.Quality = 0;
		depthDesc.Usage = D3D11_USAGE_DEFAULT;
		depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthDesc.CPUAccessFlags = 0;
		depthDesc.MiscFlags = 0;

		hr = m_Device->CreateTexture2D(&depthDesc, NULL, &rt->depth_texture);
		assert(SUCCEEDED(hr));

		D3D11_DEPTH_STENCIL_VIEW_DESC dsd{};
		dsd.Format = desc.depth_format;
		dsd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsd.Flags = 0;

		hr = m_Device->CreateDepthStencilView(rt->depth_texture, &dsd, &rt->dsv);
		assert(SUCCEEDED(hr));
	}

	return rt;
}

void Renderer::DestroyRenderTarget(RENDER_TARGET* render_target)
{
	if (render_target)
	{
		render_target->Release();
		delete render_target;
	}
}

void Renderer::SetDepthMode(DEPTH_MODE mode)
{
	switch (mode)
	{
	case DEPTH_MODE::DEPTH_MODE_READ_WRITE:
		m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable, 0);
		break;
	case DEPTH_MODE::DEPTH_MODE_READ_ONLY:
		m_DeviceContext->OMSetDepthStencilState(m_DepthStateReadOnly, 0);
		break;
		break;
	case DEPTH_MODE::DEPTH_MODE_OFF:
		m_DeviceContext->OMSetDepthStencilState(m_DepthStateDisable, 0);
		break;
	}
}

void Renderer::SetBlendMode(BLEND_MODE mode)
{
	float blendFactor[4] = { 0,0,0,0 };
	switch (mode)
	{
	case BLEND_MODE::BLEND_MODE_OPAQUE:
		m_DeviceContext->OMSetBlendState(m_BlendStateOpaque, blendFactor, 0xffffffff);
		break;
	case BLEND_MODE::BLEND_MODE_ALPHA:
		m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff);
		break;
	case BLEND_MODE::BLEND_MODE_ADD:
		m_DeviceContext->OMSetBlendState(m_BlendStateAdd, blendFactor, 0xffffffff);
		break;
	}
}

void Renderer::SetCullMode(CULL_MODE mode)
{
	switch (mode)
	{
	case CULL_MODE::CULL_MODE_BACK:
		m_DeviceContext->RSSetState(m_RasterizerStateCullBack);
		break;
	case CULL_MODE::CULL_MODE_FRONT:
		m_DeviceContext->RSSetState(m_RasterizerStateCullFront);
		break;
	case CULL_MODE::CULL_MODE_NONE:
		m_DeviceContext->RSSetState(m_RasterizerStateCullNone);
		break;
	}
}

void Renderer::SetPostEffectParams(const POST_EFFECT_PARAMS& params)
{
	m_PostEffectData = params;
	m_PostEffectData.param0.z = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	m_DeviceContext->UpdateSubresource(m_PostEffectBuffer, 0, nullptr, &m_PostEffectData, 0, 0);
}

void Renderer::DisablePostEffect()
{
	m_PostEffectData.param0.w = (float)POST_EFFECT_MODE::POST_EFFECT_MODE_NONE;
	m_DeviceContext->UpdateSubresource(m_PostEffectBuffer, 0, nullptr, &m_PostEffectData, 0, 0);
}

void Renderer::SetCircleWipeParams(const CB_CIRCLE_WIPE& params)
{
	m_DeviceContext->UpdateSubresource(m_CircleWipeBuffer, 0, nullptr, &params, 0, 0);
}

void Renderer::SetOutlineParams(const OUTLINE_PARAMS& params)
{
	m_DeviceContext->UpdateSubresource(m_OutlineParamsBuffer, 0, nullptr, &params, 0, 0);
	m_DeviceContext->PSSetConstantBuffers(8, 1, &m_OutlineParamsBuffer);
	m_DeviceContext->VSSetConstantBuffers(8, 1, &m_OutlineParamsBuffer);
}
