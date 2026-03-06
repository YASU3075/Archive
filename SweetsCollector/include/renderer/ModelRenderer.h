#pragma once



// マテリアル構造体
struct MODEL_MATERIAL
{
	char						Name[256];
	MATERIAL_DATA					Material;
	char						TextureName[256];
	ID3D11ShaderResourceView*	Texture;

};


// 描画サブセット構造体
struct SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	MODEL_MATERIAL	Material;
};


// モデル構造体
struct MODEL_OBJ
{
	VERTEX_3D		*VertexArray;
	unsigned int	VertexNum;

	unsigned int	*IndexArray;
	unsigned int	IndexNum;

	SUBSET			*SubsetArray;
	unsigned int	SubsetNum;
};

struct MODEL
{
	ID3D11Buffer*	VertexBuffer;
	ID3D11Buffer*	IndexBuffer;

	SUBSET*			SubsetArray;
	unsigned int	SubsetNum;
};


#include "component/Component.h"
#include <string>
#include <unordered_map>


class ModelRenderer : public COMPONENT
{
private:

	static std::unordered_map<std::string, MODEL*> m_ModelPool;

	static void LoadModel(const char *FileName, MODEL *Model);
	static void LoadObj( const char *FileName, MODEL_OBJ *ModelObj );
	static void LoadMaterial( const char *FileName, MODEL_MATERIAL **MaterialArray, unsigned int *MaterialNum );

	MODEL* m_Model{};

	bool m_use_model_material = true;

public:

	static void Preload( const char *FileName );
	static void UnloadAll();

	void SetUseModelMaterial(bool use)
	{
		m_use_model_material = use;
	}
	bool GetUseModelMaterial() const
	{
		return m_use_model_material;
	}

	using COMPONENT::COMPONENT;

	void BindGeometry() const;
	void DrawGeometyrOnly() const;

	void Load( const char *FileName );
	void Draw() override;
};