// =============================================================================
// explanation.h アクション説明サンプル
// 制作日：2024/02/26 制作者：安田晴人
// =============================================================================
#ifndef _EXPLANATION_H_
#define _EXPLANATION_H_

//-----------------------------------------------
// 構造体宣言
//-----------------------------------------------
struct EXPLANATION
{
	bool isPushW;			//移動したかどうか
	bool isPushA;			//移動したかどうか
	bool isPushS;			//移動したかどうか
	bool isPushD;			//移動したかどうか
	bool isLeftClick;		//左クリックしたかどうか
	bool isRightClick;		//右クリックしたかどうか

	int FrameCnt;			//フレームカウンター
	int State;				//今の状態
	int OldState;			//一個前の状態

	char text[3][4][64];	//テキスト

	int row;	//列
	int col;	//行

	char sound[32];		//テキスト表示音
	int SoundHandle;	//サウンドハンドル

};

//-----------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------
void InitializeExplanation(void);		//説明シーン初期化
void UpdateExplanation(void);			//説明シーン更新
void DrawExplanation(void);				//説明シーン描画
void FinalizeExplanation(void);			//説明シーン終了

#endif