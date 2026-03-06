//-----------------------------------------------
// PerlinNoise3D.h
// 制作日：2025/07/02
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <random>

class PERLIN_NOISE_3D
{
private:
	std::vector<int> permutation;

	int Hash(int x, int y, int z) const
	{
		return permutation[permutation[permutation[x] + y] + z];
	};

	float Fade(float t) const
	{
		return t * t * t * (t * (t * 6 - 15) + 10);
	};

	float Lerp(float a, float b, float t) const
	{
		return a + t * (b - a);
	};

	float Grad(int hash, float x, float y, float z) const
	{
		switch (hash & 15) // 0-15の範囲でグラデーションを決定
		{
		case 0: return x + y;
		case 1: return -x + y;
		case 2: return x - y;
		case 3: return -x - y;
		case 4: return x + z;
		case 5: return -x + z;
		case 6: return x - z;
		case 7: return -x - z;
		case 8: return y + z;
		case 9: return -y + z;
		case 10: return y - z;
		case 11: return -y - z;
		case 12: return x + y;
		case 13: return -x + z;
		case 14: return y - z;
		case 15: return -y - x;
		}
		return 0.0f;
	}

public:
	PERLIN_NOISE_3D(unsigned int seed = std::random_device{}())
	{
		permutation.resize(256);
		std::iota(permutation.begin(), permutation.end(), 0); // 0から255までの整数を生成
		std::shuffle(permutation.begin(), permutation.end(), std::mt19937(seed)); // シャッフル
		permutation.insert(permutation.end(), permutation.begin(), permutation.end()); // 256から512に拡張
	}

	float Noise(float x, float y, float z) const
	{
		int xi = static_cast<int>(std::floor(x)) & 255;
		int yi = static_cast<int>(std::floor(y)) & 255;
		int zi = static_cast<int>(std::floor(z)) & 255;

		float xf = x - std::floor(x);
		float yf = y - std::floor(y);
		float zf = z - std::floor(z);

		float u = Fade(xf);
		float v = Fade(yf);
		float w = Fade(zf);

		int aaa = Hash(xi, yi, zi);
		int aba = Hash(xi, yi + 1, zi);
		int aab = Hash(xi, yi, zi + 1);
		int abb = Hash(xi, yi + 1, zi + 1);
		int baa = Hash(xi + 1, yi, zi);
		int bba = Hash(xi + 1, yi + 1, zi);
		int bab = Hash(xi + 1, yi, zi + 1);
		int bbb = Hash(xi + 1, yi + 1, zi + 1);

		float x1 = Lerp(Grad(aaa, xf, yf, zf), Grad(baa, xf - 1, yf, zf), u);
		float x2 = Lerp(Grad(aba, xf, yf - 1, zf), Grad(bba, xf - 1, yf - 1, zf), u);
		float y1 = Lerp(x1, x2, v);

		float x3 = Lerp(Grad(aab, xf, yf, zf - 1), Grad(bab, xf - 1, yf, zf - 1), u);
		float x4 = Lerp(Grad(abb, xf, yf - 1, zf - 1), Grad(bbb, xf - 1, yf - 1, zf - 1), u);
		float y2 = Lerp(x3, x4, v);

		return Lerp(y1, y2, w);
	}

};