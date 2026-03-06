#pragma once

#include "renderer/RenderTarget.h"

struct VERTEX_3D
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
};

struct DEBUG_LINE_VERTEX
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
};

struct DEBUG_LINE
{
	VECTOR3 a;
	VECTOR3 b;
	XMFLOAT4 color;
};


class MATERIAL_DATA
{
public:
	XMFLOAT4	m_ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	XMFLOAT4	m_diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4	m_specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4	m_emission = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	float		m_shininess = 1.0f;
	BOOL		m_texture_enable = FALSE;
	
	XMFLOAT2	m_uv_scale = XMFLOAT2(1.0f, 1.0f);
	XMFLOAT2	m_uv_offset = XMFLOAT2(0.0f, 0.0f);

	float		m_dummy[2];
};


enum class LIGHT_TYPE : UINT
{
	DIRECTIONAL = 0,
	POINT = 1,
	SPOT = 2,
};

struct LIGHT
{
	BOOL		Enable;
	UINT		Type;
	UINT		Dummy0;
	UINT		Dummy1;

	XMFLOAT4 	Position;
	XMFLOAT4	Direction;

	XMFLOAT4	Diffuse;
	XMFLOAT4	Ambient;

	FLOAT		Range;
	FLOAT		SpotInner;
	FLOAT		SpotOuter;
	FLOAT		Dummy2;
};

struct LIGHT_BUFFER
{
	LIGHT lights[4];
	UINT  num_lights;
	FLOAT dummy[3];
};

enum class POST_EFFECT_MODE
{
	POST_EFFECT_MODE_NONE = 0,
	POST_EFFECT_MODE_FADE,
	POST_EFFECT_MODE_IRIS,
	POST_EFFECT_MODE_MAX,
};

struct POST_EFFECT_PARAMS
{
	XMFLOAT4 param0;		// x = 進捗（0～1）, y = ぼかし幅, z = 解像度, w = モード
	XMFLOAT4 param1;		// x = 中心X, y = 中心Y, z = 予備, w = 予備
	XMFLOAT4 color;			// 色
};

struct CB_CIRCLE_WIPE
{
	XMFLOAT4 param0;		// x = 進捗（0～1）, y = ぼかし幅, z = 解像度, w = 予備
	XMFLOAT4 param1;		// x = 中心X, y = 中心Y, z = 予備, w = 予備
};

struct OUTLINE_PARAMS
{
	XMFLOAT4 outline_color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	float outline_width = 1.0f;
	float padding[3];
};

struct CLEAR_PARAMS
{
	bool clear_color = true;
	bool clear_depth = true;
	FLOAT       Color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	FLOAT       Depth = 1.0f;
	UINT8       Stencil = 0;
};

struct VIEWPORT_RECT
{
	float x = 0.0f;
	float y = 0.0f;
	float w = 1.0f;
	float h = 1.0f;
};

struct CAMERA_PASS_DESC
{
	RENDER_TARGET* render_target = nullptr;
	VIEWPORT_RECT viewport_rect;
	CLEAR_PARAMS clear_params;
};

enum class DEPTH_MODE
{
	DEPTH_MODE_READ_WRITE,
	DEPTH_MODE_READ_ONLY,
	DEPTH_MODE_OFF
};

enum class BLEND_MODE
{
	BLEND_MODE_OPAQUE,
	BLEND_MODE_ALPHA,
	BLEND_MODE_ADD
};

enum class CULL_MODE
{
	CULL_MODE_BACK,
	CULL_MODE_FRONT,
	CULL_MODE_NONE
};

class Renderer
{
private:

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device*           m_Device;
	static ID3D11DeviceContext*    m_DeviceContext;
	static IDXGISwapChain*         m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;

	static RENDER_TARGET*			m_BackBufferRenderTarget;

	static ID3D11Buffer*			m_WorldBuffer;
	static ID3D11Buffer*			m_ViewBuffer;
	static ID3D11Buffer*			m_ProjectionBuffer;
	static ID3D11Buffer*			m_MaterialBuffer;
	static ID3D11Buffer*			m_LightBuffer;

	static LIGHT_BUFFER				m_LightData;


	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;
	static ID3D11DepthStencilState* m_DepthStateReadOnly;

	static ID3D11BlendState*		m_BlendState;
	static ID3D11BlendState*		m_BlendStateATC;
	static ID3D11BlendState*		m_BlendStateOpaque;
	static ID3D11BlendState*		m_BlendStateAdd;

	static ID3D11RasterizerState* m_RasterizerStateCullBack;
	static ID3D11RasterizerState* m_RasterizerStateCullFront;
	static ID3D11RasterizerState* m_RasterizerStateCullNone;

	static ID3D11Buffer* m_PostEffectBuffer;
	static POST_EFFECT_PARAMS m_PostEffectData;

	static ID3D11Buffer* m_CircleWipeBuffer;
	static ID3D11Buffer* m_OutlineParamsBuffer;

	static ID3D11VertexShader* m_DebugLineVS;
	static ID3D11PixelShader* m_DebugLinePS;
	static ID3D11InputLayout* m_DebugLineLayout;
	static ID3D11Buffer* m_DebugLineVertexBuffer;

	static UINT m_DebugLineVertexBufferaxVerts;

	static const UINT MAX_LIGHTS = 4;


public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	static void InitDebugLine();
	static void DrawDebugLines(DEBUG_LINE* lines, size_t line_count, bool depth_test);

	static void BeginCameraPass(const CAMERA_PASS_DESC& desc);
	static void EndCameraPass();

	static void SetDepthEnable(bool Enable);
	static void SetATCEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(XMMATRIX WorldMatrix);
	static void SetViewMatrix(XMMATRIX ViewMatrix);
	static void SetProjectionMatrix(XMMATRIX ProjectionMatrix);
	static void SetMaterial(MATERIAL_DATA material_data);
	static void SetLights(const LIGHT* lights, UINT num_lights);

	static ID3D11Device* GetDevice( void ){ return m_Device; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return m_DeviceContext; }

	static void UnbindAllPS_SRV(ID3D11DeviceContext* ctx);
	static void ApplyViewport(ID3D11DeviceContext* ctx, const VIEWPORT_RECT& vp, UINT target_w, UINT target_h);

	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);

	static RENDER_TARGET* CreateRenderTarget(const RENDER_TARGET_DESC& desc);
	static void DestroyRenderTarget(RENDER_TARGET* render_target);

	static void SetDepthMode(DEPTH_MODE mode);
	static void SetBlendMode(BLEND_MODE mode);
	static void SetCullMode(CULL_MODE mode);

	static void SetPostEffectParams(const POST_EFFECT_PARAMS& params);
	static void DisablePostEffect();

	static void SetCircleWipeParams(const CB_CIRCLE_WIPE& params);
	static void SetOutlineParams(const OUTLINE_PARAMS& params);
};
