#include "Main.h"
#include "scene/Manager.h"
#include "renderer/Renderer.h"
#include "input/Input.h"
#include "input/GamePad.h"
#include "entity/GameObject.h"

#include "physics/Physics3D.h"
#include "physics/RigidBody.h"
#include "physics/PhysicsDebugDraw3D.h"

#include "audio.h"

#include "scene/Scene.h"

#include "scene/Title.h"
#include "scene/Game.h"
#include "scene/Result.h"

#include "renderer/TextureManager.h"
#include "renderer/ShaderManager.h"
#include "renderer/MaterialManager.h"




SCENE* Manager::m_p_scene = nullptr;
SCENE* Manager::m_p_next_scene = nullptr;
int Manager::m_score = 0;
bool Manager::m_hit_stop = false;
float Manager::m_hit_stop_timer = 0.0f;

void Manager::Init()
{
	Renderer::Init();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplWin32_Init(GetWindow());
	ImGui_ImplDX11_Init(Renderer::GetDevice(), Renderer::GetDeviceContext());


	Input::Init();

	AUDIO::InitMaster();

	PHYSICS_3D::GetInstance().Init();

	m_p_scene = new TITLE();
	m_p_scene->Init();
}

void Manager::Uninit()
{
	m_p_scene->Uninit();
	delete m_p_scene;

	Input::Uninit();

	AUDIO::UninitMaster();
	
	PHYSICS_3D::GetInstance().Uninit();

	TEXTURE_MANAGER::ReleaseAll();
	SHADER_MANAGER::ReleaseAll();
	MATERIAL_MANAGER::ReleaseAll();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Renderer::Uninit();
}

void Manager::Update()
{
	if (m_hit_stop)
	{
		m_hit_stop_timer -= 1.0f / 60.0f;
		if (m_hit_stop_timer <= 0.0f)
		{
			m_hit_stop = false;
		}
		else
		{
			return;
		}
	}
	else
	{
		Input::Update();

		PHYSICS_3D::GetInstance().Simulate(1.0f / 60.0f);

		m_p_scene->Update();
	}

#if _DEBUG
	if (Input::GetKeyTrigger('L'))
	{
		PHYSICS_3D::GetInstance().SetDebugDraw(!PHYSICS_3D::GetInstance().IsDebugDraw());
	}
#endif
}

void Manager::Draw()
{
	Renderer::Begin();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	/*ImGui::Begin("Renderer");

	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

	float draw_time = 1000.0f / ImGui::GetIO().Framerate;
	ImGui::Text("Frame: %.1f[ms]", draw_time);

	static float value[180];
	for (int i = 0; i < 179; i++)
	{
		value[i] = value[i + 1];
	}

	value[179] = draw_time;

	ImGui::PlotLines("", value, sizeof(value) / sizeof(float), 0, nullptr, 0.0f, 100.0f, ImVec2(0, 50));
	ImGui::End();*/

	m_p_scene->Draw();
	PHYSICS_DEBUG_DRAW_3D::Flush();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	Renderer::End();

	//‰æ–Ê‘JˆÚ
	if (m_p_next_scene)
	{
		PHYSICS_3D::GetInstance().Uninit();

		m_p_scene->Uninit();
		delete m_p_scene;

		PHYSICS_3D::GetInstance().Init();

		m_p_scene = m_p_next_scene;
		m_p_scene->Init();

		m_p_next_scene = nullptr;
	}
}