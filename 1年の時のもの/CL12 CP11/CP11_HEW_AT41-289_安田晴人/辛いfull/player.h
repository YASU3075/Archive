//-----------------------------------------------
// player.h アクションゲームサンプル
// 制作日：2024/01/16 制作者：安田晴人
//-----------------------------------------------

#ifndef _PLAYER_H_		//インクルードガード
#define _PLAYER_H_

//-----------------------------------------------
// マクロ定義
//-----------------------------------------------
#define INITIAL_PLAYER_X		(40.0f)		//プレイヤーの初期X座標
#define INITIAL_PLAYER_Y		(22.0f)		//プレイヤーの初期Y座標
#define PLAYER_X_SPEED			(0.2f)		//プレイヤーのX座標の動く速さ
#define PLAYER_Y_SPEED			(0.1f)		//プレイヤーのY座標の動く速さ
#define PLAYER_SIZE				(2)			//プレイヤーの大きさ
#define MAX_PLAYER_HP			(5)			//プレイヤーの体力の最大値

//-----------------------------------------------
// 構造体宣言
//-----------------------------------------------
struct PLAYER
{//プレイヤー構造体
	float PosX;		//X座標
	float PosY;		//Y座標
	float OldPosX;	//１フレーム前のX座標
	float OldPosY;	//１フレーム前のY座標
	
	float VelX;		//X方向の速さ
	float VelY;		//Y方向の速さ

	bool isTrigger;	//トリガー
	bool isRelease;	//リリース

	int Hp;			//体力
	int OldHp;		//1フレーム前のHP
	bool isDamage;	//ダメージフラグ
	int DamageFrameCnt;	//ダメージのフレームカウンター

	bool isAttack;		//攻撃フラグ
	int AttackFrameCnt;	//攻撃のフレームカウンター

	char Damagevoice[32];	//ダメージのBGM
	int DamageSoundHandle;	//ダメージのサウンドハンドル

	char Healvoice[32];		//回復のBGM
	int HealSoundHandle;	//ヒールのサウンドハンドル

};

//-----------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------
void InitializePlayer(void);		//プレイヤー初期化
void UpdatePlayer(void);			//プレイヤー更新
void DrawPlayer(void);				//プレイヤー描画
void FinalizePlayer(void);			//プレイヤー終了
PLAYER* GetPlayer(void);			//プレイヤーのゲッター





#endif
