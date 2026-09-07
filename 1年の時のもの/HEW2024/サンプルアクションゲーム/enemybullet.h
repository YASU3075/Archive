//-----------------------------------------------
// enemybullet.h アクションゲームサンプル
// 制作日：2024/02/09 制作者：安田晴人
//-----------------------------------------------

#ifndef _ENEMYBULLET_H_		//インクルードガード
#define _ENEMYBULLET_H_

//-----------------------------------------------
// マクロ定義
//-----------------------------------------------
#define MAX_ENEMYBULLET		(100)		//敵の弾の最大数
#define MAX_ENEMYBULLET_X	(80.0f)		//敵の弾のX座標の最大値
#define MIN_ENEMYBULLET_X	(1.0f)		//敵の弾のX座標の最小値
#define MAX_ENEMYBULLET_Y	(25.0f)		//敵の弾のY座標の最大値
#define MIN_ENEMYBULLET_Y	(1.0f)		//敵の弾のY座標の最小値
#define ENEMYBULLET_SIZE	(2)			//敵の弾の大きさ
#define ENEMYBULLET_SPEED	(0.8f)			//敵の弾のスピード
#define ENEMYBULLET_CHANGE_SPEED	(0.25f)	//敵の弾の変速スピード


//-----------------------------------------------
// 列挙体宣言
//-----------------------------------------------
enum ENEMYBULLETTYPE
{
	ENEMYBULLETTYPE_NORMAL = 0,
	ENEMYBULLETTYPE_FAST,
	ENEMYBULLETTYPE_CURVE,
	ENEMYBULLETTYPE_SPEEDCHANGE,
	ENEMYBULLETTYPE_MAX,
};


//-----------------------------------------------
// 構造体宣言
//-----------------------------------------------
struct ENEMYBULLET
{//敵の弾構造体
	float PosX;		//X座標
	float PosY;		//Y座標
	float OldPosX;	//１フレーム前のX座標
	float OldPosY;	//１フレーム前のY座標

	float VelX;		//X移動地
	float VelY;		//Y移動地

	int ColorNum;	//弾の色

	int HitWallCnt;	//壁に当たった回数

	ENEMYBULLETTYPE type;	//弾の種類

	int FrameCnt;	//フレームカウンター

	bool isUse;		//使用フラグ
};

struct ENEMYBULLETSOUND
{
	char CollisionSound[32];	//衝突のBGM
	int CollisionSoundHandle;	//衝突のサウンドハンドル
};

//-----------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------
void InitializeEnemyBullet(void);		//敵の弾初期化
void UpdateEnemyBullet(void);			//敵の弾更新
void DrawEnemyBullet(void);				//敵の弾描画
void FinalizeEnemyBullet(void);			//敵の弾終了
void SetEnemyBullet(float px, float py, float vx, float vy, int color, ENEMYBULLETTYPE t = ENEMYBULLETTYPE_NORMAL);		//敵の弾セット
ENEMYBULLET* GetEnemyBullet(void);			//敵の弾ゲット

#endif
