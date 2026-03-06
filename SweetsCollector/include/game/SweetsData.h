//-----------------------------------------------
// SweetsData.h
// 制作日：2025/12/28
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

enum class SWEETS_TYPE
{
	SWEETS_TYPE_NONE = 0,
	SWEETS_TYPE_CAKE,		// ケーキ
	SWEETS_TYPE_CHOCOLATE,	// チョコレート
	SWEETS_TYPE_COOKIE,		// クッキー
	SWEETS_TYPE_CUPCAKE,	// カップケーキ
	SWEETS_TYPE_DONUT,		// ドーナツ
	SWEETS_TYPE_ICE_CREAM,	// アイスクリーム
	SWEETS_TYPE_LOLLYPOP,	// ペロペロキャンディ
	SWEETS_TYPE_PANCAKE,	// パンケーキ
	SWEETS_TYPE_POPSICLE,	// 棒アイス
	SWEETS_TYPE_PUDDING,	// プリン
	SWEETS_TYPE_MAX
};

struct SWEETS_DATA
{
	SWEETS_TYPE type = SWEETS_TYPE::SWEETS_TYPE_NONE;	// お菓子の種類
	int weight = 0; // 重さ
};