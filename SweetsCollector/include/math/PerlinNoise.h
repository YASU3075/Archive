//-----------------------------------------------
// PerlinNoise.h
// 制作日：2025/07/02
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <random>

class PERLIN_NOISE
{
private:
	std::vector<int> permutation;

	int Hash(int x, int y) const
	{
		return permutation[permutation[x] + y];
	};

	float Fade(float t) const
	{//スムーズステップ
		return t * t * t * (t * (t * 6 - 15) + 10);
	};

	float Lerp(float a, float b, float t) const
	{
		return a + t * (b - a);
	};

	float Grad(int hash, float x, float y) const
	{
		switch (hash & 3)
		{
		case 0: return x + y;  // (1,1,0)
		case 1: return -x + y;  // (-1.0,1)
		case 2: return x - y; // (1,-1,0)
		case 3: return -x - y; // (-1,-1,0)
		}
		return 0.0f;
	};

public:

	PERLIN_NOISE(unsigned int seed = std::random_device{}())
	{
		permutation.resize(256);
		std::iota(permutation.begin(), permutation.end(), 0); // 0から255までの整数を生成
		std::shuffle(permutation.begin(), permutation.end(), std::mt19937(seed)); // シャッフル
		permutation.insert(permutation.end(), permutation.begin(), permutation.end()); // 256から512に拡張
	}

	float Noise(float x, float y) const
	{
		int xi = static_cast<int>(std::floor(x)) & 255;
		int yi = static_cast<int>(std::floor(y)) & 255;
		float xf = x - std::floor(x);
		float yf = y - std::floor(y);

		float u = Fade(xf);
		float v = Fade(yf);

		int aa = Hash(xi, yi);
		int ab = Hash(xi, yi + 1);
		int ba = Hash(xi + 1, yi);
		int bb = Hash(xi + 1, yi + 1);

		float x1 = Lerp(Grad(aa, xf, yf), Grad(ba, xf - 1, yf), u);
		float x2 = Lerp(Grad(ab, xf, yf - 1), Grad(bb, xf - 1, yf - 1), u);

		return Lerp(x1, x2, v);
	}

};