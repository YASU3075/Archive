//-----------------------------------------------
// SkyBall.h
// 制作日：2025/02/19
// 制作者：安田晴人
//-----------------------------------------------

#include "SkyBall.h"
#include "model.h"
#include "Camera.h"

SKY_BALL_OBJECT g_sky_ball;
MODEL* g_sky_ball_model;

SKY_BALL_OBJECT* GetSkyBall()
{
	return &g_sky_ball;
}

void InitSkyBall()
{
	g_sky_ball.use = true;
	g_sky_ball.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_sky_ball.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_sky_ball.sca = XMFLOAT3(100.0f, 100.0f, 100.0f);
	g_sky_ball_model = ModelLoad("asset\\model\\sky.fbx");
}

void UninitSkyBall()
{
	ModelRelease(g_sky_ball_model);
}

void UpdateSkyBall()
{
	g_sky_ball.pos.x = g_sky_ball.p_camera->GetPos().x;
	g_sky_ball.pos.y = g_sky_ball.p_camera->GetPos().y;
	g_sky_ball.pos.z = g_sky_ball.p_camera->GetPos().z;

	g_sky_ball.rot.y += 0.008f;

	return;
}

void DrawSkyBall()
{
	XMMATRIX translation_matrix = XMMatrixTranslation(g_sky_ball.pos.x, g_sky_ball.pos.y, g_sky_ball.pos.z);
	XMMATRIX rotation_matrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_sky_ball.rot.x), XMConvertToRadians(g_sky_ball.rot.y), XMConvertToRadians(g_sky_ball.rot.z));
	XMMATRIX scaling_matrix = XMMatrixScaling(g_sky_ball.sca.x, g_sky_ball.sca.y, g_sky_ball.sca.z);

	XMMATRIX world_matrix = scaling_matrix * rotation_matrix * translation_matrix;
	SetWorldMatrix(world_matrix);

	MATERIAL material;
	ZeroMemory(&material, sizeof(MATERIAL));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	ModelDraw(g_sky_ball_model);
}