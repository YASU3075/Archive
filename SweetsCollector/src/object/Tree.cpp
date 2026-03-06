//-----------------------------------------------
// Tree.cpp
// 制作日：2025/12/17
// 制作者：安田晴人
//-----------------------------------------------
#include "Main.h"
#include "object/Tree.h"
#include "renderer/LSystem.h"
#include "renderer/TreeGenerator.h"

#include "renderer/Renderer.h"
#include "renderer/MaterialManager.h"

#include "component/renderer/MeshRendererComponent.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void TREE::Init()
{
	m_transform.SetSca(VECTOR3(3.0f, 3.0f, 3.0f));

	// L-Systemの定義
	LSYSTEM lsystem("F");
	lsystem.AddRule('F', "F[&F][^F][+F][-F][\\F][/F]");
	// 木の生成
	TREE_GENERATOR treeGenerator(lsystem);
	MESH_RENDERER_DATA treeMesh = treeGenerator.GenerateTree(3);
	// メッシュレンダラーコンポーネントの追加
	auto meshRenderer = this->AddComponent<MESH_RENDERER_COMPONENT>();
	meshRenderer->SetMeshData(treeMesh);
	meshRenderer->SetMaterial(MATERIAL_MANAGER::GetInstance().LoadFromCustomFile("asset\\material\\TreeMaterial.mtl"));
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void TREE::Draw()
{
	GetComponent<MESH_RENDERER_COMPONENT>()->Draw();
}