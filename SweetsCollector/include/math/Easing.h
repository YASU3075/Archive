//-----------------------------------------------
// Easing.h
// 制作日：2026/01/19
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <cmath>

#include <DirectXMath.h>
using namespace DirectX;

enum class EASE_TYPE
{
	EASE_IN_SINE,
	EASE_OUT_SINE,
	EASE_IN_OUT_SINE,
	EASE_IN_QUAD,
	EASE_OUT_QUAD,
	EASE_IN_OUT_QUAD,
	EASE_IN_CUBIC,
	EASE_OUT_CUBIC,
	EASE_IN_OUT_CUBIC,
	EASE_IN_QUART,
	EASE_OUT_QUART,
	EASE_IN_OUT_QUART,
	EASE_IN_CIRC,
	EASE_OUT_CIRC,
	EASE_IN_OUT_CIRC,
	EASE_IN_BACK,
	EASE_OUT_BACK,
	EASE_IN_OUT_BACK,
	EASE_IN_ELASTIC,
	EASE_OUT_ELASTIC,
	EASE_IN_OUT_ELASTIC,
	EASE_IN_BOUNCE,
	EASE_OUT_BOUNCE,
	EASE_IN_OUT_BOUNCE,
};


namespace EASING
{
	// イージング関数群(見た目でわかるサイト)
	// https://easings.net/ja

	// イーズイン・サイン
	static float EaseInSine(float t)
	{
		return 1.0f - cosf((t * XM_PI) * 0.5f);
	}

	// イーズアウト・サイン
	static float EaseOutSine(float t)
	{
		return sinf((t * XM_PI) * 0.5f);
	}

	// イーズイン・アウト・サイン
	static float EaseInOutSine(float t)
	{
		return -0.5f * (cosf(XM_PI * t) - 1.0f);
	}

	// イーズイン・クアドラティック
	static float EaseInQuad(float t)
	{
		return t * t;
	}

	// イーズアウト・クアドラティック
	static float EaseOutQuad(float t)
	{
		return 1.0f - (1.0f - t) * (1.0f - t);
	}

	// イーズイン・アウト・クアドラティック
	static float EaseInOutQuad(float t)
	{
		if (t < 0.5f)
		{
			return 2.0f * t * t;
		}
		else
		{
			return 1.0f - powf(-2.0f * t + 2.0f, 2.0f) * 0.5f;
		}
	}

	// イーズイン・キュービック
	static float EaseInCubic(float t)
	{
		return t * t * t;
	}

	// イーズアウト・キュービック
	static float EaseOutCubic(float t)
	{
		return 1.0f - powf(1.0f - t, 3.0f);
	}

	// イーズイン・アウト・キュービック
	static float EaseInOutCubic(float t)
	{
		if (t < 0.5f)
		{
			return 4.0f * t * t * t;
		}
		else
		{
			return 1.0f - powf(-2.0f * t + 2.0f, 3.0f) * 0.5f;
		}
	}

	// イーズイン・クォーティック
	static float EaseInQuart(float t)
	{
		return t * t * t * t;
	}

	// イーズアウト・クォーティック
	static float EaseOutQuart(float t)
	{
		return 1.0f - powf(1.0f - t, 4.0f);
	}

	// イーズイン・アウト・クォーティック
	static float EaseInOutQuart(float t)
	{
		if (t < 0.5f)
		{
			return 8.0f * t * t * t * t;
		}
		else
		{
			return 1.0f - powf(-2.0f * t + 2.0f, 4.0f) * 0.5f;
		}
	}

	// イーズイン・サイクル
	static float EaseInCirc(float t)
	{
		return 1.0f - sqrtf(1.0f - powf(t, 2.0f));
	}

	// イーズアウト・サイクル
	static float EaseOutCirc(float t)
	{
		return sqrtf(1.0f - powf(t - 1.0f, 2.0f));
	}

	// イーズイン・アウト・サイクル
	static float EaseInOutCirc(float t)
	{
		if (t < 0.5f)
		{
			return (1.0f - sqrtf(1.0f - powf(2.0f * t, 2.0f))) * 0.5f;
		}
		else
		{
			return (sqrtf(1.0f - powf(-2.0f * t + 2.0f, 2.0f)) + 1.0f) * 0.5f;
		}
	}

	// イーズイン・バック
	static float EaseInBack(float t)
	{
		const float c1 = 1.70158f;
		const float c3 = c1 + 1.0f;
		return c3 * t * t * t - c1 * t * t;
	}

	// イーズアウト・バック
	static float EaseOutBack(float t)
	{
		const float c1 = 1.70158f;
		const float c3 = c1 + 1.0f;

		return 1.0f + c3 * powf(t - 1.0f, 3.0f) + c1 * powf(t - 1.0f, 2.0f);
	}

	// イーズインアウト・バック
	static float EaseInOutBack(float t)
	{
		const float c1 = 1.70158f;
		const float c2 = c1 * 1.525f;
		if (t < 0.5f)
		{
			return powf(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2) * 0.5f;
		}
		else
		{
			return (powf(2.0f * t - 2.0f, 2.0f) * ((c2 + 1) * (t * 2.0f - 2.0f) + c2) + 2.0f) * 0.5f;
		}
	}

	// イーズイン・エラスティック
	static float EaseInElastic(float t)
	{
		const float c4 = (2.0f * XM_PI) / 3;

		return powf(2.0f, 10.0f * t - 10.0f) * sinf((t * 10.0f - 10.75f) * c4);
	}

	// イーズアウト・エラスティック
	static float EaseOutElastic(float t)
	{
		const float c4 = (2.0f * XM_PI) / 3;
		return 1.0f - powf(2.0f, -10.0f * t) * sinf((t * 10.0f - 0.75f) * c4);
	}

	// イーズイン・アウト・エラスティック
	static float EaseInOutElastic(float t)
	{
		const float c5 = (2.0f * XM_PI) / 4.5f;
		if (t < 0.5f)
		{
			return (powf(2.0f, 20.0f * t - 10.0f) * sinf((20.0f * t - 11.125f) * c5)) * 0.5f;
		}
		else
		{
			return (2.0f - powf(2.0f, -20.0f * t + 10.0f) * sinf((20.0f * t - 11.125f) * c5)) * 0.5f;
		}
	}

	// イーズアウト・バウンス
	static float EaseOutBounce(float t)
	{
		const float n1 = 7.5625f;
		const float d1 = 2.75f;
		if (t < 1.0f / d1)
		{
			return n1 * t * t;
		}
		else if (t < 2.0f / d1)
		{
			return n1 * (t -= 1.5 / d1) * t + 0.75f;
		}
		else if (t < 2.5f / d1)
		{
			return n1 * (t -= 2.25f / d1) * t + 0.9375f;
		}
		else
		{
			return n1 * (t -= 2.625f / d1) * t + 0.984375;
		}
	}

	// イーズイン・バウンス
	static float EaseInBounce(float t)
	{
		return 1.0f - EaseOutBounce(1.0f - t);
	}

	// イーズイン・アウト・バウンス
	static float EaseInOutBounce(float t)
	{
		if (t < 0.5f)
		{
			return (1.0f - EaseOutBounce(1.0f - 2.0f * t)) * 0.5f;
		}
		else
		{
			return (1.0f + EaseOutBounce(2.0f * t - 1.0f)) * 0.5f;
		}
	}

	static float Smooth01(float t)
	{
		return t * t * (3.0f - 2.0f * t);
	}

	static float Ease(EASE_TYPE type, float t)
	{
		switch (type)
		{
		case EASE_TYPE::EASE_IN_SINE:		 return EaseInSine(t);
		case EASE_TYPE::EASE_OUT_SINE:		 return EaseOutSine(t);
		case EASE_TYPE::EASE_IN_OUT_SINE:	 return EaseInOutSine(t);
		case EASE_TYPE::EASE_IN_QUAD:		 return EaseInQuad(t);
		case EASE_TYPE::EASE_OUT_QUAD:		 return EaseOutQuad(t);
		case EASE_TYPE::EASE_IN_OUT_QUAD:	 return EaseInOutQuad(t);
		case EASE_TYPE::EASE_IN_CUBIC:		 return EaseInCubic(t);
		case EASE_TYPE::EASE_OUT_CUBIC:		 return EaseOutCubic(t);
		case EASE_TYPE::EASE_IN_OUT_CUBIC:	 return EaseInOutCubic(t);
		case EASE_TYPE::EASE_IN_QUART:		 return EaseInQuart(t);
		case EASE_TYPE::EASE_OUT_QUART:		 return EaseOutQuart(t);
		case EASE_TYPE::EASE_IN_OUT_QUART:	 return EaseInOutQuart(t);
		case EASE_TYPE::EASE_IN_CIRC:		 return EaseInCirc(t);
		case EASE_TYPE::EASE_OUT_CIRC:		 return EaseOutCirc(t);
		case EASE_TYPE::EASE_IN_OUT_CIRC:	 return EaseInOutCirc(t);
		case EASE_TYPE::EASE_IN_BACK:		 return EaseInBack(t);
		case EASE_TYPE::EASE_OUT_BACK:		 return EaseOutBack(t);
		case EASE_TYPE::EASE_IN_OUT_BACK:	 return EaseInOutBack(t);
		case EASE_TYPE::EASE_IN_ELASTIC:	 return EaseInElastic(t);
		case EASE_TYPE::EASE_OUT_ELASTIC:	 return EaseOutElastic(t);
		case EASE_TYPE::EASE_IN_OUT_ELASTIC: return EaseInOutElastic(t);
		case EASE_TYPE::EASE_IN_BOUNCE:	 return EaseInBounce(t);
		case EASE_TYPE::EASE_OUT_BOUNCE:	 return EaseOutBounce(t);
		case EASE_TYPE::EASE_IN_OUT_BOUNCE: return EaseInOutBounce(t);
		default:							 return t;
		}
	}
}