//-----------------------------------------------
// bullet.h アクションゲームサンプル
// 制作日：2024/01/17 制作者：安田晴人
//-----------------------------------------------

#ifndef _BULLET_H_		//インクルードガード
#define _BULLET_H_

//-----------------------------------------------
// マクロ定義
//-----------------------------------------------
#define MAX_BULLET		(100)		//弾の最大数
#define MAX_BULLET_X	(80.0f)		//弾のX座標の最大値
#define MIN_BULLET_X	(1.0f)		//弾のX座標の最小値
#define MAX_BULLET_Y	(25.0f)		//弾のY座標の最大値
#define MIN_BULLET_Y	(1.0f)		//弾のY座標の最小値
#define BULLET_SIZE_X	(2)		//弾の大きさ
#define BULLET_SPEED	(1)			//弾のスピード
#define MAX_HIT_COUNT	(4)			//壁で跳ね返る最大値

//-----------------------------------------------
// 構造体宣言
//-----------------------------------------------
struct BULLET
{//弾構造体
	float PosX;		//X座標
	float PosY;		//Y座標
	float OldPosX;	//１フレーム前のX座標
	float OldPosY;	//１フレーム前のY座標

	float VelX;		//X移動地
	float VelY;		//Y移動地

	int HitWallCnt;	//壁に当たった回数

	bool isUse;		//使用フラグ
	
};

//-----------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------
void InitializeBullet(void);		//弾初期化
void UpdateBullet(void);			//弾更新
void DrawBullet(void);				//弾描画
void FinalizeBullet(void);			//弾終了
void SetBullet(float px, float py, float vx, float vy);		//弾セット
BULLET* GetBullet(void);			//弾ゲット

#endif
