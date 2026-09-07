//-----------------------------------------------
// enemy.cpp アクションゲームサンプル
// 制作日：2024/01/22 制作者：安田晴人
//-----------------------------------------------
#include "main.h"
#include "player.h"
#include "game.h"
#include "enemybullet.h"
#include "enemy.h"
#include "field.h"
#include "bullet.h"
#include "explosion.h"

#define CONIOEX
#include "conioex.h"

//-----------------------------------------------
// グローバル変数
//-----------------------------------------------
ENEMY enemy[MAX_ENEMY];		//敵の実体
ENEMYBGM eBgm;
int EnemyCnt;				//敵が消えたり倒されたりしたときのカウント
int OldEnemyCnt;			//1個前の敵が消えたり倒されたりしたときのカウント

//-----------------------------------------------
// 敵初期化
//-----------------------------------------------
void InitializeEnemy(void) 
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		enemy[i].PosX =
		enemy[i].OldPosX = 0.0f;
		enemy[i].PosY =
		enemy[i].OldPosY = 0.0f;
		enemy[i].VelX =
		enemy[i].VelY = 0.0f;

		enemy[i].type = ENEMYTYPE_NORMAL;
		enemy[i].MoveFrameCnt = 0;

		enemy[i].SizeX = ENEMY_SIZE_X;
		enemy[i].SizeY = ENEMY_SIZE_Y;

		enemy[i].isMoveX = true;
		enemy[i].isMoveY = false;

		enemy[i].ColorNum = 0;

		enemy[i].Hp = 0;
		enemy[i].MaxHp = 0;
		enemy[i].isDamage = false;
		enemy[i].DamageFrameCnt = 0;

		enemy[i].AttackFrameCnt = 0;
		enemy[i].AttackInterval = 0;

		enemy[i].TimeCnt = 0;

		enemy[i].isUse = false;
	}

	//ダメージ効果音
	strcpy(eBgm.Damagevoice, "MUSICDATA\\damage.mp3");
	eBgm.DamageSoundHandle = opensound(eBgm.Damagevoice);
	if (eBgm.DamageSoundHandle)
	{
		setvolume(eBgm.DamageSoundHandle, 60);
	}

	//死んだときの効果音
	strcpy(eBgm.Deadvoice, "MUSICDATA\\dead.mp3");
	eBgm.DeadSoundHandle = opensound(eBgm.Deadvoice);
	if (eBgm.DeadSoundHandle)
	{
		setvolume(eBgm.DeadSoundHandle, 70);
	}

	EnemyCnt = 0;
	OldEnemyCnt = 0;

	//移動無し、弾普通
	SetEnemy(30, 3, 0.0f, 0.0f, 1, 10, 3, ENEMYTYPE_NORMAL, ENEMYBULLETTYPE_NORMAL);
}

//-----------------------------------------------
// 敵更新
//-----------------------------------------------
void UpdateEnemy(void)
{
	if (EnemyCnt != OldEnemyCnt)
	{//敵のカウントが増加したとき

		OldEnemyCnt = EnemyCnt;

		switch (EnemyCnt)
		{//左から順にX座標、Y座標、X移動値、Y移動値、HP、時間、攻撃速度、タイプ
		case 1:
			//横移動、弾普通
			SetEnemy(40, 5, 0.2f, 0.0f, 1, 15, 2, ENEMYTYPE_REVERSE, ENEMYBULLETTYPE_NORMAL);
			break;

		case 2:
			//縦移動、弾普通
			SetEnemy(10, 3, 0.0f, 0.1f, 2, 15, 2, ENEMYTYPE_REVERSE, ENEMYBULLETTYPE_NORMAL);
			//縦移動、弾普通
			SetEnemy(60, 3, 0.0f, 0.1f, 2, 15, 2, ENEMYTYPE_REVERSE, ENEMYBULLETTYPE_NORMAL);
			break;

		case 4:
			//一周、弾普通
			SetEnemy(40, 5, 0.2f, 0.1f, 2, 20, 2, ENEMYTYPE_TURN, ENEMYBULLETTYPE_NORMAL);
			break;

		case 5:
			//ジグザグ移動、弾普通
			SetEnemy(40, 5, 0.2f, 0.1f, 2, 25, 2, ENEMYTYPE_REVERSE, ENEMYBULLETTYPE_NORMAL);
			break;

		case 6:	//ここまで甘口
			//横移動、弾普通
			SetEnemy(5, 3, 0.2f, 0.0f, 2, 25, 2, ENEMYTYPE_REVERSE, ENEMYBULLETTYPE_NORMAL);
			//横移動、弾普通
			SetEnemy(60, 10, 0.2f, 0.0f, 2, 25, 2, ENEMYTYPE_REVERSE, ENEMYBULLETTYPE_NORMAL);
			break;

		case 8:	//ここから中辛
			//移動無し、弾早い
			SetEnemy(30, 3, 0.0f, 0.0f, 2, 10, 2, ENEMYTYPE_NORMAL, ENEMYBULLETTYPE_FAST);
			break;

		case 9:
			//横移動、弾早い
			SetEnemy(10, 8, 0.2f, 0.0f, 2, 10, 2, ENEMYTYPE_REVERSE, ENEMYBULLETTYPE_FAST);
			break;

		case 10:
			//横移動、弾変速
			SetEnemy(10, 3, 0.1f, 0.0f, 2, 10, 2, ENEMYTYPE_REVERSE, ENEMYBULLETTYPE_SPEEDCHANGE);
			break;

		case 11:
			//横移動、弾カーブ
			SetEnemy(60, 5, 0.1f, 0.0f, 2, 10, 3, ENEMYTYPE_REVERSE, ENEMYBULLETTYPE_CURVE);
			break;

		case 12:
			//縦移動、弾早い
			SetEnemy(10, 3, 0.0f, 0.1f, 2, 15, 2, ENEMYTYPE_REVERSE, ENEMYBULLETTYPE_FAST);
			//縦移動、弾カーブ
			SetEnemy(60, 3, 0.0f, 0.1f, 2, 15, 2, ENEMYTYPE_REVERSE, ENEMYBULLETTYPE_CURVE);
			break;

		case 14:	//ここから辛口
			//移動無し、弾早い
			SetEnemy(10, 3, 0.0f, 0.0f, 2, 10, 2, ENEMYTYPE_NORMAL, ENEMYBULLETTYPE_FAST);
			//移動無し、弾カーブ
			SetEnemy(35, 3, 0.0f, 0.1f, 1, 10, 3, ENEMYTYPE_NORMAL, ENEMYBULLETTYPE_CURVE);
			//移動無し、弾早い
			SetEnemy(60, 3, 0.0f, 0.0f, 2, 10, 2, ENEMYTYPE_NORMAL, ENEMYBULLETTYPE_FAST);
			break;

		case 17:
			//一周、弾早い
			SetEnemy(10, 2, 0.2f, 0.1f, 2, 10, 2, ENEMYTYPE_TURN, ENEMYBULLETTYPE_FAST);
			//移動無し、弾変速
			SetEnemy(35, 9, 0.0f, 0.0f, 2, 10, 1, ENEMYTYPE_NORMAL, ENEMYBULLETTYPE_SPEEDCHANGE);
			break;

		case 19:
			//ジグザグ移動、弾カーブ
			SetEnemy(40, 5, 0.2f, 0.1f, 2, 20, 4, ENEMYTYPE_REVERSE, ENEMYBULLETTYPE_CURVE);
			break;

		case 20:
			//移動無し、弾普通
			SetEnemy(10, 3, 0.0f, 0.0f, 2, 20, 2, ENEMYTYPE_NORMAL, ENEMYBULLETTYPE_NORMAL);
			//移動無し、弾カーブ
			SetEnemy(35, 3, 0.0f, 0.1f, 1, 20, 3, ENEMYTYPE_NORMAL, ENEMYBULLETTYPE_CURVE);
			//移動無し、弾普通
			SetEnemy(60, 3, 0.0f, 0.0f, 2, 20, 2, ENEMYTYPE_NORMAL, ENEMYBULLETTYPE_NORMAL);
			//移動無し、弾カーブ
			SetEnemy(10, 10, 0.0f, 0.0f, 2, 20, 2, ENEMYTYPE_NORMAL, ENEMYBULLETTYPE_CURVE);
			//移動無し、弾普通
			SetEnemy(35, 10, 0.0f, 0.1f, 1, 20, 3, ENEMYTYPE_NORMAL, ENEMYBULLETTYPE_NORMAL);
			//移動無し、弾カーブ
			SetEnemy(60, 10, 0.0f, 0.0f, 2, 20, 2, ENEMYTYPE_NORMAL, ENEMYBULLETTYPE_CURVE);
			break;

		case 26:
			//一周、弾早い
			SetEnemy(10, 2, 1.0f, 0.5f, 2, 10, 2, ENEMYTYPE_TURN, ENEMYBULLETTYPE_FAST);
			break;

		case 27:
			SetScene(SCENE_RESULT);
			break;

		default:break;
		}

	}


	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy[i].isUse)
		{//使用していたら

			if (!enemy[i].isDamage)
			{//ダメージを食らっていなかったら
				//前フレームの座標を格納
				enemy[i].OldPosX = enemy[i].PosX;
				enemy[i].OldPosY = enemy[i].PosY;

				//移動タイプ
				switch (enemy[i].type)
				{
				case ENEMYTYPE_NORMAL:
					break;

				case ENEMYTYPE_REVERSE:
					//移動
					enemy[i].PosX += enemy[i].VelX;

					if (
						enemy[i].OldPosX >= MIN_ENEMY_X + 1 && enemy[i].PosX <= MIN_ENEMY_X + 1 ||
						enemy[i].OldPosX + enemy[i].SizeX + 1 <= MAX_ENEMY_X && enemy[i].PosX + enemy[i].SizeX + 1 >= MAX_ENEMY_X
						)
					{//画面の左右の端に行ったら
						enemy[i].PosX = enemy[i].OldPosX;
						enemy[i].VelX *= -1;
					}

					enemy[i].PosY += enemy[i].VelY;

					if (
						enemy[i].OldPosY - 1 >= MIN_ENEMY_Y && enemy[i].PosY - 1 <= MIN_ENEMY_Y ||
						enemy[i].OldPosY + enemy[i].SizeY <= MAX_ENEMY_Y && enemy[i].PosY + enemy[i].SizeY >= MAX_ENEMY_Y
						)
					{//画面の上下の端に行ったら
						enemy[i].PosY = enemy[i].OldPosY;
						enemy[i].VelY *= -1;
					}


					break;

				case ENEMYTYPE_TURN:
					//一周

					if (enemy[i].isMoveX)
					{//X方向に動くとき
						//移動
						enemy[i].PosX += enemy[i].VelX;

						if (
							enemy[i].OldPosX >= MIN_ENEMY_X + 1 && enemy[i].PosX <= MIN_ENEMY_X + 1 ||
							enemy[i].OldPosX + enemy[i].SizeX + 1 <= MAX_ENEMY_X && enemy[i].PosX + enemy[i].SizeX + 1 >= MAX_ENEMY_X
							)
						{//画面の左右の端に行ったら
							enemy[i].PosX = enemy[i].OldPosX;
							enemy[i].VelX *= -1;
							enemy[i].isMoveX = false;
							enemy[i].isMoveY = true;
						}
					}

					else if (enemy[i].isMoveY)
					{//Y方向に動くとき
						//移動
						enemy[i].PosY += enemy[i].VelY;

						if (
							enemy[i].OldPosY - 1 >= MIN_ENEMY_Y && enemy[i].PosY - 1 <= MIN_ENEMY_Y ||
							enemy[i].OldPosY + enemy[i].SizeY <= MAX_ENEMY_Y && enemy[i].PosY + enemy[i].SizeY >= MAX_ENEMY_Y
							)
						{//画面の上下の端に行ったら
							enemy[i].PosY = enemy[i].OldPosY;
							enemy[i].VelY *= -1;
							enemy[i].isMoveX = true;
							enemy[i].isMoveY = false;
						}

					}

					break;

				case ENEMYTYPE_ESCAPE:
					//逃げる

					enemy[i].VelY = -0.1f;
					enemy[i].VelX = 0.0f;

					enemy[i].PosX += enemy[i].VelX;
					enemy[i].PosY += enemy[i].VelY;

					break;

				default:break;

				}

				//壁判定
				if (enemy[i].PosX <= MIN_ENEMY_X || enemy[i].PosX + enemy[i].SizeX >= MAX_ENEMY_X ||
					enemy[i].PosY <= MIN_ENEMY_Y || enemy[i].PosY + enemy[i].SizeY >= MAX_ENEMY_Y)
				{
					//消す
					enemy[i].isUse = false;

					//カウント増加
					EnemyCnt++;

					//残像を消す
					for (int j = 0; j < 5; j++)
					{
						//残像の描画位置設定
						gotoxy((int)enemy[i].OldPosX, (int)enemy[i].OldPosY + j);

						printf("          ");
					}

				}

				//弾の発射処理
				if (enemy[i].AttackFrameCnt >= 60 * enemy[i].AttackInterval)
				{
					//プレイヤーのアドレス取得
					PLAYER* pPlayer = GetPlayer();

					float BulletVelX = 0.0f;		//敵の弾のX方向のスピード
					float BulletVelY = 0.0f;		//敵の弾のY方向のスピード
					int BulletPosX = ENEMY_SIZE_X / 2 + (-1 + rand() % 3) * 2;		//敵の弾のX座標の発射位置
					int BulletPosY = ENEMY_SIZE_Y / 2;		//敵の弾のY座標の発射位置

					//弾のX、Y方向のスピード計算式
					BulletVelX = (pPlayer->PosX - (enemy[i].PosX + BulletPosX)) / 60 * ENEMYBULLET_SPEED;
					BulletVelY = (pPlayer->PosY - (enemy[i].PosY + BulletPosY)) / 60 * ENEMYBULLET_SPEED;

					//弾の発射（）
					SetEnemyBullet(enemy[i].PosX + BulletPosX, enemy[i].PosY + BulletPosY, BulletVelX, BulletVelY, enemy[i].ColorNum / 16, enemy[i].bType);

					//フレームカウンターをリセット
					enemy[i].AttackFrameCnt = 0;
				}

				//弾と敵の当たり判定処理
				BULLET* pBullet = GetBullet();		//弾実体の先頭アドレスを取得

				for (int j = 0; j < MAX_BULLET; j++)
				{
					if ((pBullet + j)->isUse)
					{//敵も弾もどちらも使っている場合


						//弾
						int ATop = (int)(pBullet + j)->PosY;
						int ABottom = (int)(pBullet + j)->PosY + (1.0f - 0.1f);
						int ARight = (int)(pBullet + j)->PosX + (BULLET_SIZE_X - 0.1f);
						int ALeft = (int)(pBullet + j)->PosX;

						int ATopOld = (int)(pBullet + j)->OldPosY;
						int ABottomOld = (int)(pBullet + j)->OldPosY + (1.0f - 0.1f);
						int ARightOld = (int)(pBullet + j)->OldPosX + (BULLET_SIZE_X - 0.1f);
						int ALeftOld = (int)(pBullet + j)->OldPosX;


						//敵
						int BTop = (int)enemy[i].PosY;
						int BBottom = (int)enemy[i].PosY + (ENEMY_SIZE_Y - 0.1f);
						int BRight = (int)enemy[i].PosX + (ENEMY_SIZE_X - 0.1f);
						int BLeft = (int)enemy[i].PosX;

						int BTopOld = (int)enemy[i].OldPosY;
						int BBottomOld = (int)enemy[i].OldPosY + (ENEMY_SIZE_Y - 0.1f);
						int BRightOld = (int)enemy[i].OldPosX + (ENEMY_SIZE_X - 0.1f);
						int BLeftOld = (int)enemy[i].OldPosX;


						//当たり判定式
						if (
							(ATop <= BBottom && ABottom >= BTop && ARight >= BLeft && ALeft <= BRight) ||		//重なったとき
							(ARightOld < BLeft && ARight >= BLeft && ATop <= BBottom && ABottom >= BTop) ||		//左から右すり抜けたとき
							(ALeftOld > BRight && ALeft <= BRight && ATop <= BBottom && ABottom >= BTop) ||		//右から左すり抜けたとき
							(ARight >= BLeft && ALeft <= BRight && ABottomOld < BTop && ATop > BBottom) ||	//上から下すり抜けたとき
							(ARight >= BLeft && ALeft <= BRight && ATopOld > BBottom && ABottom < BTop)		//下から上すり抜けたとき
							)
						{

							//弾を消す
							(pBullet + j)->isUse = false;

							//残像の描画位置設定
							gotoxy((int)(pBullet + j)->OldPosX, (int)(pBullet + j)->OldPosY);

							//残像を消す
							printf(" ");

							if (!enemy[i].isDamage)
							{//ダメージをくらっていなかったら（点滅してなかったら）
								//敵のHP減少
								enemy[i].Hp--;

								//ダメージ受けた
								enemy[i].isDamage = true;
							}
						}


					}
				}

				//攻撃のフレームカウンター増加
				enemy[i].AttackFrameCnt++;

			}

			if (enemy[i].Hp <= 0)
			{//敵のHPが無くなったら

				ScoreAdd(enemy[i].type + 1 * enemy[i].MaxHp);

				//敵を消す
				enemy[i].isUse = false;
				
				//死んだときの効果音
				playsound(eBgm.DeadSoundHandle, 0);

				//カウント増加
				EnemyCnt++;

				//残像を消す
				for (int j = 0; j < ENEMY_SIZE_Y; j++)
				{
					//残像の描画位置設定
					gotoxy((int)enemy[i].OldPosX, (int)enemy[i].OldPosY + j);

					printf("          ");
				}

				//爆発アニメーションの設置
				for (int j = 0; j < ENEMY_SIZE_X; j++)
				{
					for (int k = 0; k < ENEMY_SIZE_Y; k++)
					{
						SetExplosion(enemy[i].PosX + j, enemy[i].PosY + k);
					}
				}

			}

			if (enemy[i].isDamage)
			{//ダメージをくらってたら

				if (enemy[i].DamageFrameCnt == 0)
				{//最初の1フレーム
					//ダメージ効果音
					playsound(eBgm.DamageSoundHandle, 0);
				}

				//弾と敵の当たり判定処理
				BULLET* pBullet = GetBullet();		//弾実体の先頭アドレスを取得

				for (int j = 0; j < MAX_BULLET; j++)
				{
					if ((pBullet + j)->isUse)
					{//敵も弾もどちらも使っている場合


						//弾
						int ATop = (int)(pBullet + j)->PosY;
						int ABottom = (int)(pBullet + j)->PosY + (1.0f - 0.1f);
						int ARight = (int)(pBullet + j)->PosX + (BULLET_SIZE_X - 0.1f);
						int ALeft = (int)(pBullet + j)->PosX;

						int ATopOld = (int)(pBullet + j)->OldPosY;
						int ABottomOld = (int)(pBullet + j)->OldPosY + (1.0f - 0.1f);
						int ARightOld = (int)(pBullet + j)->OldPosX + (BULLET_SIZE_X - 0.1f);
						int ALeftOld = (int)(pBullet + j)->OldPosX;


						//敵
						int BTop = (int)enemy[i].PosY;
						int BBottom = (int)enemy[i].PosY + (ENEMY_SIZE_Y - 0.1f);
						int BRight = (int)enemy[i].PosX + (ENEMY_SIZE_X - 0.1f);
						int BLeft = (int)enemy[i].PosX;

						int BTopOld = (int)enemy[i].OldPosY;
						int BBottomOld = (int)enemy[i].OldPosY + (ENEMY_SIZE_Y - 0.1f);
						int BRightOld = (int)enemy[i].OldPosX + (ENEMY_SIZE_X - 0.1f);
						int BLeftOld = (int)enemy[i].OldPosX;


						//当たり判定式
						if (
							(ATop <= BBottom && ABottom >= BTop && ARight >= BLeft && ALeft <= BRight) ||		//重なったとき
							(ARightOld < BLeft && ARight >= BLeft && ATop <= BBottom && ABottom >= BTop) ||		//左から右すり抜けたとき
							(ALeftOld > BRight && ALeft <= BRight && ATop <= BBottom && ABottom >= BTop) ||		//右から左すり抜けたとき
							(ARight >= BLeft && ALeft <= BRight && ABottomOld < BTop && ATop > BBottom) ||	//上から下すり抜けたとき
							(ARight >= BLeft && ALeft <= BRight && ATopOld > BBottom && ABottom < BTop)		//下から上すり抜けたとき
							)
						{

							//弾を消す
							(pBullet + j)->isUse = false;

							//残像の描画位置設定
							gotoxy((int)(pBullet + j)->OldPosX, (int)(pBullet + j)->OldPosY);

							//残像を消す
							printf(" ");

						}
					}
				}

				//フレーム数増加
				enemy[i].DamageFrameCnt++;

				if (enemy[i].DamageFrameCnt > 30)
				{//30フレーム経ったら
					enemy[i].DamageFrameCnt = 0;
					enemy[i].isDamage = false;
				}
			}

			//タイム増加
			enemy[i].TimeCnt--;

			if (enemy[i].TimeCnt < 0)
			{
				enemy[i].type = ENEMYTYPE_ESCAPE;
			}

		}
	}
}

//-----------------------------------------------
// 敵描画
//-----------------------------------------------
void DrawEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy[i].isUse)
		{//使用していたら

		//もし描画位置が動いていたら
			if ((int)enemy[i].OldPosX != (int)enemy[i].PosX || (int)enemy[i].OldPosY != (int)enemy[i].PosY)
			{
				
				//残像を消す
				for (int j = 0; j < 5; j++)
				{
					//残像の描画位置設定
					gotoxy((int)enemy[i].OldPosX - 1, (int)enemy[i].OldPosY + j);

					printf("           ");
				}
			}

			//色設定
			textcolor(BLUE);

			//敵実体の描画
			{

				if (enemy[i].isDamage && (enemy[i].DamageFrameCnt % 6) == 0)
				{//ダメージをくらってるかつフレーム数が6の倍数の時とき

					//白く点滅
					textbackground(WHITE);

					gotoxy((int)enemy[i].PosX + 1, (int)enemy[i].PosY);
					printf("        ");
					for (int j = 0; j < 3; j++)
					{
						gotoxy((int)enemy[i].PosX, (int)enemy[i].PosY + j + 1);
						printf("          ");
					}

					gotoxy((int)enemy[i].PosX + 1, (int)enemy[i].PosY + 4);
					printf("        ");

					//色をもとに戻す
					textbackground(BLACK);

				}

				else
				{
					//体の部分
					textattr(enemy[i].ColorNum);
					gotoxy((int)enemy[i].PosX + 1, (int)enemy[i].PosY);
					printf("        ");
					for (int j = 0; j < 3; j++)
					{
						gotoxy((int)enemy[i].PosX, (int)enemy[i].PosY + j + 1);
						printf("          ");
					}

					gotoxy((int)enemy[i].PosX + 1, (int)enemy[i].PosY + 4);
					printf("        ");

					//体力表示
					textattr(enemy[i].ColorNum / 16);
					gotoxy((int)enemy[i].PosX, (int)enemy[i].PosY);
					printf("%d", enemy[i].Hp);

					//目の白い部分
					textattr(0xFF);

					gotoxy((int)enemy[i].PosX + 3, (int)enemy[i].PosY + 1);
					printf("    ");

					gotoxy((int)enemy[i].PosX + 2, (int)enemy[i].PosY + 2);
					printf("      ");

					gotoxy((int)enemy[i].PosX + 3, (int)enemy[i].PosY + 3);
					printf("    ");

					//目の黒い部分
					textattr(0x00);
					gotoxy((int)enemy[i].PosX + 4, (int)enemy[i].PosY + 2);
					printf("  ");
				}
			}

			//色をもとに戻す
			textcolor(WHITE);

			textbackground(BLACK);
		}
	}
}

//-----------------------------------------------
// 敵終了
//-----------------------------------------------
void FinalizeEnemy(void)
{
	closesound(eBgm.DamageSoundHandle);
	closesound(eBgm.DeadSoundHandle);
	//２年生になってテクスチャーとかモデルとかを読み込んだ時に解放するのに使う
	//今のところはあまり使わないが癖づけておく
}

//-----------------------------------------------
// 敵のセッター
//-----------------------------------------------
void SetEnemy(float px, float py, float vx, float vy, int hp, int time, int atk, ENEMYTYPE t, ENEMYBULLETTYPE bT)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (!enemy[i].isUse)
		{
			enemy[i].PosX = px;
			enemy[i].OldPosX = px;
			enemy[i].PosY = py;
			enemy[i].OldPosY = py;

			enemy[i].VelX = vx;
			enemy[i].VelY = vy;

			enemy[i].type = t;
			
			enemy[i].MoveFrameCnt = 0;

			enemy[i].ColorNum = ((rand() % 14) + 1) * 15 + 15; //白と黒を除いた色を乱数で設定

			enemy[i].Hp = hp;
			enemy[i].MaxHp = hp;
			enemy[i].DamageFrameCnt = 0;

			enemy[i].AttackFrameCnt = 0;
			enemy[i].AttackInterval = atk;

			enemy[i].TimeCnt = time * 60;

			enemy[i].bType = bT;

			enemy[i].isUse = true;
			break;

		}
	}
}
