//-----------------------------------------------
// Scene.cpp
// 制作日：2025/06/25
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "scene\Manager.h"
#include "renderer/Renderer.h"
#include "scene/Scene.h"
#include "object/Camera.h"
#include "renderer/CameraSystem.h"
#include "component/CameraComponent.h"
#include "component/renderer/RendererComponent.h"
#include "renderer/RenderTarget.h"
#include "entity/GameObject.h"
#include "renderer/LightManager.h"

void SCENE::Uninit()
{
	for (int i = 0; i < (int)LAYER::LAYER_MAX; i++)
	{
		for (auto p_object : m_game_object[i])
		{
			p_object->Uninit();
			delete p_object;
		}

		m_game_object[i].clear();
	}
}

void SCENE::Update()
{
	for (int i = 0; i < (int)LAYER::LAYER_MAX; i++)
	{
		for (auto p_object : m_game_object[i])
		{
			p_object->Update();
		}

		m_game_object[i].remove_if([](GAME_OBJECT* p_object)
			{
				return p_object->Destroy();
			});
	}
}

void SCENE::Draw()
{
	for (auto cam : CAMERA_SYSTEM::GetInstance().GetAllCameras())
	{
		cam->Update();
		Renderer::SetViewMatrix(cam->GetViewMatrix());
		Renderer::SetProjectionMatrix(cam->GetProjectionMatrix());

		Renderer::BeginCameraPass(cam->ToPassDesc());

		DrawByRenderPass(cam);

		Renderer::EndCameraPass();
	}
}

void SCENE::DrawByRenderPass(CAMERA_COMPONENT* p_camera)
{
	std::vector<RENDER_ENTRY> opaque, transparent, particle, ui;

	VECTOR3 cam_pos = p_camera->GetOwner()->GetTransform()->GetPos();
	VECTOR3 cam_forward = p_camera->GetOwner()->GetForward();

	// 全レイヤーを走査
	for (int i = 0; i < (int)LAYER::LAYER_MAX; ++i)
	{
		for (auto* obj : m_game_object[i])
		{
			auto comps = obj->GetComponents<RENDERER_COMPONENT>();
			for (auto* rc : comps)
			{
				RENDER_ENTRY e;
				e.p_renderer_component = rc;
				e.p_owner = obj;
				e.z = obj->GetZ(cam_pos, cam_forward);

				switch (rc->GetRenderPass())
				{
				case RENDER_PASS::RENDER_PASS_OPAQUE:
					opaque.push_back(e);
					break;
				case RENDER_PASS::RENDER_PASS_TRANSPARENT:
					transparent.push_back(e);
					break;
				case RENDER_PASS::RENDER_PASS_PARTICLE:
					particle.push_back(e);
					break;
				case RENDER_PASS::RENDER_PASS_UI:
					ui.push_back(e);
					break;
				}
			}
		}
	}

	// ソート：Opaqueはソート不要

	// Transparent/Particleは order -> z の順でソート
	auto sortTransparent = [&](const RENDER_ENTRY& a, const RENDER_ENTRY& b) {
		if (a.p_renderer_component->GetOrderInPass() != b.p_renderer_component->GetOrderInPass())
			return a.p_renderer_component->GetOrderInPass() < b.p_renderer_component->GetOrderInPass();
		return a.z > b.z;
		};
	std::sort(transparent.begin(), transparent.end(), sortTransparent);
	std::sort(particle.begin(), particle.end(), sortTransparent);

	// UI は order だけ（zは使わない）
	std::sort(ui.begin(), ui.end(), [&](const RENDER_ENTRY& a, const RENDER_ENTRY& b) {
		return a.p_renderer_component->GetOrderInPass() < b.p_renderer_component->GetOrderInPass();
		});

	// 描画

	Renderer::SetBlendMode(BLEND_MODE::BLEND_MODE_OPAQUE);
	Renderer::SetDepthMode(DEPTH_MODE::DEPTH_MODE_READ_WRITE);
	for (auto& e : opaque)
	{
		auto* owner = e.p_owner;
		LIGHT light[4]{};
		UINT light_count = 0;
		LIGHT_MANAGER::GetInstance().GatherLights(owner->GetTransform()->GetPos(), light, light_count);
		Renderer::SetLights(light, light_count);
		e.p_renderer_component->Draw();
	}

	Renderer::SetBlendMode(BLEND_MODE::BLEND_MODE_ALPHA);
	Renderer::SetDepthMode(DEPTH_MODE::DEPTH_MODE_READ_ONLY);
	for (auto& e : transparent)
	{
		auto* owner = e.p_owner;
		LIGHT light[4]{};
		UINT light_count = 0;
		LIGHT_MANAGER::GetInstance().GatherLights(owner->GetTransform()->GetPos(), light, light_count);
		Renderer::SetLights(light, light_count);
		e.p_renderer_component->Draw();
	}

	Renderer::SetBlendMode(BLEND_MODE::BLEND_MODE_ADD);
	Renderer::SetDepthMode(DEPTH_MODE::DEPTH_MODE_READ_ONLY);
	for (auto& e : particle)
	{
		e.p_renderer_component->Draw();
	}

	Renderer::SetBlendMode(BLEND_MODE::BLEND_MODE_ALPHA);
	Renderer::SetDepthMode(DEPTH_MODE::DEPTH_MODE_OFF);
	for (auto& e : ui)
	{
		e.p_renderer_component->Draw();
	}

	Renderer::SetViewMatrix(p_camera->GetViewMatrix());
	Renderer::SetProjectionMatrix(p_camera->GetProjectionMatrix());
}