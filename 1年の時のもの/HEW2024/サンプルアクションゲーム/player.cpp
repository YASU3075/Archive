//-----------------------------------------------
// player.cpp アクションゲームサンプル
// 制作日：2024/01/16 制作者：安田晴人
//-----------------------------------------------
#include "main.h"
#include "player.h"
#include "field.h"
#include "bullet.h"
#include "enemybullet.h"


#define CONIOEX
#include "conioex.h"

//-----------------------------------------------
// グローバル変数
//-----------------------------------------------
PLAYER player;		//プレイヤーの実体


//-----------------------------------------------
// プレイヤー初期化
//-----------------------------------------------
void InitializePlayer(void) 
{
	player.PosX = 
	player.OldPosX = INITIAL_PLAYER_X;
	player.PosY = 
	player.OldPosY = INITIAL_PLAYER_Y;

	player.VelY = 0.0f;

	player.isTrigger = false;
	player.isRelease = true;

	player.Hp = 5;
	player.OldHp = 5;
	player.isDamage = false;
	player.DamageFrameCnt = 0;

	player.isAttack = false;
	player.AttackFrameCnt = 0;

	textbackground(RED);

	//描画位置設定
	gotoxy(2, 25);
	printf("HP:");
	//色設定
	textbackground(WHITE);
	textcolor(RED);
	for (int i = 0; i < player.Hp; i++)
	{
		printf("辛");
	}
	//色をもとに戻す
	textbackground(BLACK);
	textcolor(WHITE);

	//ダメージ音
	strcpy(player.Damagevoice, "MUSICDATA\\player_damage.mp3");
	player.DamageSoundHandle = opensound(player.Damagevoice);
	if (player.DamageSoundHandle)
	{
		setvolume(player.DamageSoundHandle, 60);
	}

	//回復音
	strcpy(player.Healvoice, "MUSICDATA\\heal.mp3");
	player.HealSoundHandle = opensound(player.Healvoice);
	if (player.HealSoundHandle)
	{
		setvolume(player.HealSoundHandle, 60);
	}

}

//-----------------------------------------------
// プレイヤー更新
//-----------------------------------------------
void UpdatePlayer(void)
{
	//前フレームの座標を格納
	player.OldPosX = player.PosX;
	player.OldPosY = player.PosY;

	//前フレームのHPを格納
	player.OldHp = player.Hp;

	//移動処理
	{

		if (inport(PK_D))
		{//Dキー（右入力）
			player.PosX += PLAYER_X_SPEED;
		}

		if (inport(PK_A))
		{//Aキー（左入力）
			player.PosX -= PLAYER_X_SPEED;
		}

		if (inport(PK_W))
		{//Wキー入力（上入力）
			player.PosY -= PLAYER_Y_SPEED;
		}

		if (inport(PK_S))
		{//Sキー入力（下入力）
			player.PosY += PLAYER_Y_SPEED;
		}

		//壁判定
		{
			if (player.PosX >= MAX_FIELD_X - 2)
			{//フィールド右
				player.PosX = MAX_FIELD_X - 2;
			}

			if (player.PosX <= MIN_FIELD_X + 2)
			{//フィールド左
				player.PosX = MIN_FIELD_X + 2;
			}

			if (player.PosY <= FIELD_POSY + 1)
			{//フィールド上限
				player.PosY = FIELD_POSY + 1;
			}

			if (player.PosY >= 24.0f)
			{//フィールド下限
				player.PosY = 24.0f;
			}

		}

	}

	//プレイヤーと敵の弾の当たり判定処理
	ENEMYBULLET* pEnemyBullet = GetEnemyBullet();		//敵の弾実体の先頭アドレスを取得

	for (int i = 0; i < MAX_ENEMYBULLET; i++)
	{
		if ((pEnemyBullet + i)->isUse)
		{//敵の弾が使っている場合

			//弾
			int ATop = (int)(pEnemyBullet + i)->PosY;
			int ABottom = (int)(pEnemyBullet + i)->PosY;
			int ARight = (int)(pEnemyBullet + i)->PosX + (ENEMYBULLET_SIZE - 0.1f);
			int ALeft = (int)(pEnemyBullet + i)->PosX;

			int ATopOld = (int)(pEnemyBullet + i)->OldPosY;
			int ABottomOld = (int)(pEnemyBullet + i)->OldPosY;
			int ARightOld = (int)(pEnemyBullet + i)->OldPosX + (ENEMYBULLET_SIZE - 0.1f);
			int ALeftOld = (int)(pEnemyBullet + i)->OldPosX;


			//プレイヤー
			int BTop = (int)player.PosY;
			int BBottom = (int)player.PosY;
			int BRight = (int)player.PosX + (PLAYER_SIZE - 0.1f);
			int BLeft = (int)player.PosX;

			int BTopOld = (int)player.OldPosY;
			int BBottomOld = (int)player.OldPosY;
			int BRightOld = (int)player.OldPosX + (PLAYER_SIZE - 0.1f);
			int BLeftOld = (int)player.OldPosX;


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
				(pEnemyBullet + i)->isUse = false;

				//残像の描画位置設定
				gotoxy((int)(pEnemyBullet + i)->OldPosX, (int)(pEnemyBullet + i)->OldPosY);

				//残像を消す
				printf(" ");

				if (
					((int)pEnemyBullet[i].OldPosX >= MIN_FIELD_X && (int)pEnemyBullet[i].OldPosX <= MAX_FIELD_X) &&
					((int)pEnemyBullet[i].OldPosY == FIELD_POSY)
					)
				{//フィールドと重なった時
					//残像の描画位置設定
					gotoxy(MIN_FIELD_X, FIELD_POSY);

					//色設定
					textcolor(WHITE);
					//フィールド描画
					printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
					//色をもとに戻す
					textcolor(WHITE);
				}

				if (!player.isDamage)
				{//ダメージをくらっていなかったら（点滅してなかったら）
					
				 //プレイヤーのHP減少
					player.Hp--;

					//ダメージ受けた
					player.isDamage = true;

					playsound(player.DamageSoundHandle, 0);
				}

			}
		}
	}

	//弾と敵の当たり判定処理
	BULLET* pBullet = GetBullet();		//弾実体の先頭アドレスを取得

	for (int i = 0; i < MAX_BULLET; i++)
	{
		if ((pBullet + i)->isUse)
		{//敵も弾もどちらも使っている場合


			//弾
			int ATop = (int)(pBullet + i)->PosY;
			int ABottom = (int)(pBullet + i)->PosY + (1.0f - 0.1f);
			int ARight = (int)(pBullet + i)->PosX + (BULLET_SIZE_X - 0.1f);
			int ALeft = (int)(pBullet + i)->PosX;

			int ATopOld = (int)(pBullet + i)->OldPosY;
			int ABottomOld = (int)(pBullet + i)->OldPosY + (1.0f - 0.1f);
			int ARightOld = (int)(pBullet + i)->OldPosX + (BULLET_SIZE_X - 0.1f);
			int ALeftOld = (int)(pBullet + i)->OldPosX;


			//プレイヤー
			int BTop = (int)player.PosY;
			int BBottom = (int)player.PosY;
			int BRight = (int)player.PosX + (PLAYER_SIZE - 0.1f);
			int BLeft = (int)player.PosX;

			int BTopOld = (int)player.OldPosY;
			int BBottomOld = (int)player.OldPosY;
			int BRightOld = (int)player.OldPosX + (PLAYER_SIZE - 0.1f);
			int BLeftOld = (int)player.OldPosX;


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
				(pBullet + i)->isUse = false;

				//残像の描画位置設定
				gotoxy((int)(pBullet + i)->OldPosX, (int)(pBullet + i)->OldPosY);

				//残像を消す
				printf(" ");

				if (
					((int)pBullet[i].OldPosX >= MIN_FIELD_X && (int)pBullet[i].OldPosX <= MAX_FIELD_X) &&
					((int)pBullet[i].OldPosY == FIELD_POSY)
					)
				{//フィールドと重なった時
					//残像の描画位置設定
					gotoxy(MIN_FIELD_X, FIELD_POSY);

					//色設定
					textcolor(WHITE);
					//フィールド描画
					printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
					//色をもとに戻す
					textcolor(WHITE);
				}

				if (player.Hp < MAX_PLAYER_HP)
				{//最大HPより少なかったら
					//体力回復
					player.Hp++;

					//回復音
					playsound(player.HealSoundHandle, 0);
				}

			}
		}
	}

	if (player.Hp <= 0)
	{//プレイヤーのHPが無くなったら
		SetScene(SCENE_RESULT);
	}

	if (player.isDamage)
	{//ダメージをくらってたら

		//フレーム数増加
		player.DamageFrameCnt++;

		if (player.DamageFrameCnt > 30)
		{//30フレーム経ったら
			player.DamageFrameCnt = 0;
			player.isDamage = false;
		}
	}

	if (inport(PM_LEFT))
	{//マウス左ボタンを押しているとき

		//Press処理
		player.isRelease = false;

		if (!player.isTrigger)
		{//Trigger処理
			player.isTrigger = true;

			//攻撃処理
			player.isAttack = true;

		}
	}

	else
	{//マウスの左ボタンを押していないとき
		player.isTrigger = false;

		if (!player.isRelease)
		{//Release処理

			player.isRelease = true;
		}

	}


	if (player.isAttack)
	{//攻撃していたら
		//攻撃フレームカウンター増加
		player.AttackFrameCnt++;

		if (player.AttackFrameCnt > 10)
		{
			player.isAttack = false;
			player.AttackFrameCnt = 0;
		}

	}


}

//-----------------------------------------------
// プレイヤー描画
//-----------------------------------------------
void DrawPlayer(void)
{

	//もし描画位置が動いていたら残像処理
	if ((int)player.OldPosX != (int)player.PosX || (int)player.OldPosY != (int)player.PosY)
	{
		//残像の描画位置設定
		gotoxy((int)player.OldPosX, (int)player.OldPosY);

		//残像を消す
		printf(" ");
	}

	if (player.isDamage && (player.DamageFrameCnt % 6) == 0)
	{//プレイヤーがダメージをくらっているかつフレーム数が６の倍数の時

		//色設定
		textcolor(WHITE);

		//プレイヤー実体の描画位置設定
		gotoxy((int)player.PosX, (int)player.PosY);

		//画面表示
		printf("辛");

		//色をもとに戻す
		textcolor(WHITE);

	}

	else
	{
		//色設定
		textcolor(RED);

		//プレイヤー実体の描画位置設定
		gotoxy((int)player.PosX, (int)player.PosY);

		//画面表示
		printf("辛");

		//色をもとに戻す
		textcolor(WHITE);
	}


	if (player.OldHp != player.Hp)
	{//HPが増減したとき
		
		//残像処理
		gotoxy(5, 25);
		textbackground(WHITE);
		for (int i = 0; i < MAX_PLAYER_HP; i++)
		{
			printf("　");
		}
		textbackground(BLACK);

		textbackground(RED);

		//描画位置設定
		gotoxy(2, 25);
		printf("HP:");
		//色設定
		textbackground(WHITE);
		textcolor(RED);
		for (int i = 0; i < player.Hp; i++)
		{
			printf("辛");
		}
		//色をもとに戻す
		textbackground(BLACK);
		textcolor(WHITE);
	}

#ifdef _DEBUG
	/*gotoxy(1, 2);
	printf("PosX:%0.1f", player.PosX);
	gotoxy(1, 3);
	printf("PosY:%0.1f", player.PosY);
	gotoxy(1, 4);
	printf("VelX:%0.1f", player.VelX);*/
#endif

}

//-----------------------------------------------
// プレイヤー終了
//-----------------------------------------------
void FinalizePlayer(void)
{
	closesound(player.DamageSoundHandle);
	closesound(player.HealSoundHandle);
}

//-----------------------------------------------
// プレイヤーのゲッター
//-----------------------------------------------
PLAYER* GetPlayer(void)
{
	return &player;
}
