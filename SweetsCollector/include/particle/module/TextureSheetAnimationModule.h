//-----------------------------------------------
// TextureSheetAnimationModule.h
// 制作日：2025/07/03
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include "ParticleModule.h"

class TEXTURE_SHEET_ANIMATION_MODULE : public PARTICLE_MODULE
{
private:
	int m_tile_x = 1;						//タイル数X
	int m_tile_y = 1;						//タイル数Y
	int m_total_frames = 1;			//総フレーム数
	bool m_looping = true;					//ループするかどうか
	bool m_reverse = false;					//逆再生するかどうか

public:
	TEXTURE_SHEET_ANIMATION_MODULE(int tile_x = 1, int tile_y = 1, float total_frames = 1.0f, bool looping = true, bool reverse = false)
		: m_tile_x(tile_x), m_tile_y(tile_y), m_total_frames(total_frames), m_looping(looping), m_reverse(reverse) {}

	void Apply(PARTICLE& particle, float delta_time) override
	{
		if (!particle.m_alive)
		{
			return; // パーティクルが生きていない場合は何もしない
		}

		// フレームの寿命に対する割合を計算
		float frame_over_life = particle.m_frame_over_life;

		if (frame_over_life < 0.0f)
		{
			frame_over_life = 0.0f; // 負の値は無効なので0にクリップ
		}
		else if (frame_over_life > 1.0f)
		{
			frame_over_life = 1.0f; // 1を超える値は無効なので1にクリップ
		}

		// 逆再生の場合、割合を反転
		if (m_reverse)
		{
			frame_over_life = 1.0f - frame_over_life;
		}

		// 総フレーム数に基づいてスプライトインデックスを計算
		int sprite_index = static_cast<int>(frame_over_life * m_total_frames) % (m_tile_x * m_tile_y);

		if (!m_looping && frame_over_life >= 1.0f)
		{
			sprite_index = m_total_frames - 1; // 最後のフレームに固定
			//particle.m_alive = false; // ループしない場合はパーティクルを終了
		}

		particle.m_tile_x = m_tile_x; // タイル数Xをパーティクルに設定
		particle.m_tile_y = m_tile_y; // タイル数Yをパーティクルに設定

		// スプライトインデックスをパーティクルに設定
		particle.m_sprite_index = sprite_index;
	}

	std::unique_ptr<PARTICLE_MODULE> Clone() const override
	{
		return std::make_unique<TEXTURE_SHEET_ANIMATION_MODULE>(m_tile_x, m_tile_y, m_total_frames, m_looping, m_reverse);
	}
};