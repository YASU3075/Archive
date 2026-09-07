//-----------------------------------------------
// bat.h アクションゲームサンプル
// 制作日：2024/02/16 制作者：安田晴人
//-----------------------------------------------

#ifndef _BAT_H_		//インクルードガード
#define _BAT_H_

//-----------------------------------------------
// マクロ定義
//-----------------------------------------------
#define MAX_BAT_X	(80.0f)		//バットのX座標の最大値
#define MIN_BAT_X	(1.0f)		//バットのX座標の最小値
#define MAX_BAT_Y	(25.0f)		//バットのY座標の最大値
#define MIN_BAT_Y	(1.0f)		//バットのY座標の最小値
#define BAT_SIZE_X	(2.0f)		//バットの大きさ
#define BAT_SPEED	(1)			//バットのスピード

//-----------------------------------------------
// 構造体宣言
//-----------------------------------------------
struct BAT
{//バット構造体
	float PosX;		//X座標
	float PosY;		//Y座標
	float OldPosX;	//１フレーム前のX座標
	float OldPosY;	//１フレーム前のY座標

	float VelX;		//X移動地
	float VelY;		//Y移動地

	//左
	int LeftFrameCnt;	//フレームカウンター
	bool isLeftTriger;	//トリガーフラグ
	bool isLeftUse;		//使用フラグ

	//右
	int RightFrameCnt;	//フレームカウンター
	bool isRightTriger;	//トリガーフラグ
	bool isRightUse;	//使用フラグ

	char sound[32];			//反射したとき
	int SoundHandle;		//サウンドハンドル

};

//-----------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------
void InitializeBat(void);		//バット初期化
void UpdateBat(void);			//バット更新
void DrawBat(void);				//バット描画
void FinalizeBat(void);			//バット終了
void SetBat(float px, float py, float vx, float vy);		//バットセット
BAT* GetBat(void);			//バットゲット

#endif
