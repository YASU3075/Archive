//-----------------------------------------------
// player.h アクションゲームサンプル
// 制作日：2024/01/16 制作者：安田晴人
//-----------------------------------------------

#ifndef _PLAYER_H_		//インクルードガード
#define _PLAYER_H_

//-----------------------------------------------
// マクロ定義
//-----------------------------------------------
#define INITIAL_PLAYER_X		(5.0f)		//プレイヤーの初期X座標
#define INITIAL_PLAYER_Y		(3.0f)		//プレイヤーの初期Y座標
#define MAX_PLAYER_X			(100.0f)	//プレイヤーのX座標の最大値
#define MIN_PLAYER_X			(1.0f)		//プレイヤーのX座標の最小値
#define MAX_PLAYER_Y			(50.0f)		//プレイヤーのY座標の最大値
#define MIN_PLAYER_Y			(1.0f)		//プレイヤーのY座標の最小値
#define PLAYER_X_SPEED			(0.5f)		//プレイヤーのX座標の動く速さ
#define PLAYER_Y_SPEED			(0.25f)		//プレイヤーのY座標の動く速さ

//-----------------------------------------------
// 構造体宣言
//-----------------------------------------------
struct PLAYER
{//プレイヤー構造体
	float PosX;		//X座標
	float PosY;		//Y座標
	float OldPosX;	//１フレーム前のX座標
	float OldPosY;	//１フレーム前のY座標
};

//-----------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------
void InitializePlayer(void);		//プレイヤー初期化
void UpdatePlayer(void);			//プレイヤー更新
void DrawPlayer(void);				//プレイヤー描画
void FinalizePlayer(void);			//プレイヤー終了





#endif
