//-----------------------------------------------
// [player.h]
// 制作日：2024/05/24
// 制作者：安田晴人
//-----------------------------------------------

#ifndef PLAYER_H
#define PLAYER_H

#include <string>

//プレイヤークラス
class Player
{
private:
	//メンバ変数
	std::string m_name;		//名前
	std::string m_gender;		//性別
	std::string  m_job;				//職業
	int m_level;			//レベル
	int m_hp;				//体力
	int m_mp;				//魔法力
	int m_atk;				//攻撃力
	int m_def;				//防御力
	int m_agi;				//敏捷性
	int m_dex;				//器用さ
	int m_wis;				//賢さ（信仰度の変わり）

public:
	//メンバ関数

	//セッター
	void SetName(std::string name);
	void SetGender(std::string gender);
	void SetJob(std::string job);
	void SetLevel(int level);
	void SetHp(int hp);
	void SetMp(int mp);
	void SetAtk(int atk);
	void SetDef(int def);
	void SetAgi(int agi);
	void SetDex(int dex);
	void SetWis(int wis);

	//ゲッター
	std::string GetName();
	std::string GetGender();
	std::string GetJob();
	int GetLevel();
	int GetHp();
	int GetMp();
	int GetAtk();
	int GetDef();
	int GetAgi();
	int GetDex();
	int GetWis();


};




#endif

