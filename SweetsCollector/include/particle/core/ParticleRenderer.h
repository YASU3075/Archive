//-----------------------------------------------
// ParticleRenderer.h
// 制作日：2025/06/27
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include <memory>
#include <vector>
#include "particle/core/ComUniquePtr.h"

class PARTICLE;
class MATERIAL;

class PARTICLE_RENDERER
{
private:
	ComUniquePtr<ID3D11Buffer> m_vertex_buffer;

	size_t m_vertex_capacity = 0;

	MATERIAL* m_material = nullptr;

	void EnsureCapacity(size_t required_capacity);

public:
	PARTICLE_RENDERER() = default;
	~PARTICLE_RENDERER() = default;

	PARTICLE_RENDERER(const PARTICLE_RENDERER&) = delete;
	PARTICLE_RENDERER& operator=(const PARTICLE_RENDERER&) = delete;

	PARTICLE_RENDERER(PARTICLE_RENDERER&&) noexcept = default;
	PARTICLE_RENDERER& operator=(PARTICLE_RENDERER&&) noexcept = default;

	void Init(const char* material_file);
	void Draw(const std::vector<PARTICLE>& particles);

	void SetMaterial(MATERIAL* material) { m_material = material; }
};
