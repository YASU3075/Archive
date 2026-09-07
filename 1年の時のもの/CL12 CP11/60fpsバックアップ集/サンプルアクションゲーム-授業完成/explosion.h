//-----------------------------------------------
// explosion.h アクションゲームサンプル
// 制作日：2024/01/30 制作者：安田晴人
//-----------------------------------------------

#ifndef _EXPLOSION_H_		//インクルードガード
#define _EXPLOSION_H_

//-----------------------------------------------
// マクロ定義
//-----------------------------------------------
#define MAX_EXPLOSION		(100)		//爆発アニメーションの最大数
#define MAX_EXPLOSION_X		(60.0f)		//爆発アニメーションのX座標の最大値
#define MIN_EXPLOSION_X		(1.0f)		//爆発アニメーションのX座標の最小値
#define MAX_EXPLOSION_Y		(30.0f)		//爆発アニメーションのY座標の最大値
#define MIN_EXPLOSION_Y		(1.0f)		//爆発アニメーションのY座標の最小値
#define EXPLOSION_SIZE		(1.0f)			//爆発アニメーションの大きさ

//-----------------------------------------------
// 構造体宣言
//-----------------------------------------------
struct EXPLOSION
{//爆発アニメーション構造体
	float PosX;		//X座標
	float PosY;		//Y座標
	
	int frameCnt;	//フレームカウント

	bool isUse;		//使用フラグ
};

//-----------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------
void InitializeExplosion(void);		//爆発アニメーション初期化
void UpdateExplosion(void);			//爆発アニメーション更新
void DrawExplosion(void);				//爆発アニメーション描画
void FinalizeExplosion(void);			//爆発アニメーション終了

void SetExplosion(float px, float py);		//爆発アニメーションセット
EXPLOSION* GetExplosion(void);			//爆発アニメーションゲット

#endif
