//-----------------------------------------------
// block.h アクションゲームサンプル
// 制作日：2024/01/24 制作者：安田晴人
//-----------------------------------------------

#ifndef _BLOCK_H_		//インクルードガード
#define _BLOCK_H_

//-----------------------------------------------
// マクロ定義
//-----------------------------------------------
#define MAX_BLOCK	(100)		//ブロックの最大数
#define MAX_BLOCK_X	(60.0f)		//ブロックのX座標の最大値
#define MIN_BLOCK_X	(1.0f)		//ブロックのX座標の最小値
#define MAX_BLOCK_Y	(30.0f)		//ブロックのY座標の最大値
#define MIN_BLOCK_Y	(1.0f)		//ブロックのY座標の最小値
#define BLOCK_SIZE	(1.0f)		//ブロックの大きさ

//-----------------------------------------------
// 列挙体宣言
//-----------------------------------------------
enum BLOCKTYPE
{
	BLOCKTYPE_NORMAL = 0,	//勝手にカウントアップしてくれる
	BLOCKTYPE_REVERSE,		//1
	BLOCKTYPE_MAX,			//2
};

//-----------------------------------------------
// 構造体宣言
//-----------------------------------------------
struct BLOCK
{//ブロック構造体
	float PosX;		//X座標
	float PosY;		//Y座標
	float OldPosX;	//１フレーム前のX座標
	float OldPosY;	//１フレーム前のY座標

	float VelX;		//X移動地

	int Size;		//横幅

	bool isUse;		//使用フラグ

	BLOCKTYPE type;	//ブロックの種類
	int frameCnt;	//フレームカウンター
};

//-----------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------
void InitializeBlock(void);		//ブロック初期化
void UpdateBlock(void);			//ブロック更新
void DrawBlock(void);			//ブロック描画
void FinalizeBlock(void);		//ブロック終了
void SetBlock(float px, float py, int s, float vx = 0.0f, BLOCKTYPE t = BLOCKTYPE_NORMAL);		//ブロックセット
BLOCK* GetBlock(void);		//ブロック取得

#endif
