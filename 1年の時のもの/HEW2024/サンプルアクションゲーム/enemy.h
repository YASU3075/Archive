//-----------------------------------------------
// enemy.h アクションゲームサンプル
// 制作日：2024/01/22 制作者：安田晴人
//-----------------------------------------------

#ifndef _ENEMY_H_		//インクルードガード
#define _ENEMY_H_

//-----------------------------------------------
// マクロ定義
//-----------------------------------------------
#define MAX_ENEMY	(100)		//敵の最大数
#define MAX_ENEMY_X	(80.0f)		//敵のX座標の最大値
#define MIN_ENEMY_X	(1.0f)		//敵のX座標の最小値
#define MAX_ENEMY_Y	(19.0f)		//敵のY座標の最大値
#define MIN_ENEMY_Y	(1.0f)		//敵のY座標の最小値

#define ENEMY_SIZE_X	(10)	//敵のX方向のサイズ
#define ENEMY_SIZE_Y	(5)		//敵のY方向のサイズ

//-----------------------------------------------
// 列挙体宣言
//-----------------------------------------------
enum ENEMYTYPE
{
	ENEMYTYPE_NORMAL = 0,	//勝手にカウントアップしてくれる
	ENEMYTYPE_REVERSE,		//1
	ENEMYTYPE_TURN,
	ENEMYTYPE_ESCAPE,
	ENEMYTYPE_MAX,
};

//-----------------------------------------------
// 構造体宣言
//-----------------------------------------------
struct ENEMY
{//敵構造体
	float PosX;		//X座標
	float PosY;		//Y座標
	float OldPosX;	//１フレーム前のX座標
	float OldPosY;	//１フレーム前のY座標

	float VelX;		//X移動地
	float VelY;		//Y移動地

	bool isUse;		//使用フラグ

	ENEMYTYPE type;	//敵の種類
	int MoveFrameCnt;	//移動のフレームカウンター

	int SizeX;		//X方向のサイズ
	int SizeY;		//Y方向のサイズ

	bool isMoveX;	//X方向に動くか
	bool isMoveY;	//Y方向に動くか

	int ColorNum;	//敵の色の番号

	int Hp;			//敵のHP
	int MaxHp;		//敵の最大HP
	bool isDamage;	//ダメージをくらってるかどうか
	int DamageFrameCnt;		//ダメージ受けたときのフレームカウンター

	int AttackFrameCnt;		//攻撃のフレームカウンター
	int AttackInterval;		//攻撃のインターバル

	ENEMYBULLETTYPE bType;	//弾の種類

	int TimeCnt;			//タイムカウント
};

struct ENEMYBGM
{
	char Damagevoice[32];	//ダメージのBGM
	int DamageSoundHandle;	//ダメージのサウンドハンドル

	char Deadvoice[32];		//死んだときのBGM
	int DeadSoundHandle;	//死んだときのサウンドハンドル
};

//-----------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------
void InitializeEnemy(void);		//敵初期化
void UpdateEnemy(void);			//敵更新
void DrawEnemy(void);			//敵描画
void FinalizeEnemy(void);		//敵終了
void SetEnemy(float px, float py, float vx = 0.0f, float vy = 0.0f, int hp = 10, int time = 10, int atk = 3, 
	ENEMYTYPE t = ENEMYTYPE_NORMAL, ENEMYBULLETTYPE bT = ENEMYBULLETTYPE_NORMAL);		//敵セット


#endif
