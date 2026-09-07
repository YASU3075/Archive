//-----------------------------------------------
// guild.h
// 制作日：2024/06/28
// 制作者：安田晴人
//-----------------------------------------------

#ifndef _GUILD
#define _GUILD

#include <string>
#include <fstream>
#include "character.h"

//-----------------------------------------------
// マクロ定義
//-----------------------------------------------
#define MEMBER_MAX      (10)

class Guild
{
private:
	int m_memberMax = 0;		//ギルドメンバーの最大数
	Character** m_memberList = nullptr;

public:
	Guild(int memberMax);
	~Guild();

	//メンバーの地下
	//戻り値：メンバーのID。追加できなかった場合-1
	int AddMember(std::string name, std::string gender, std::string job, int level,
		int hp, int mp, int atk, int def, int agi, int dex, int wis);

	//メンバーの削除
	void DeleteMember(int id);

	//名前での検索
	int GetMemberByName(std::string name, int startId);

	//指定したキャラクターのすべての数値を表示する
	void ShowCharacterAllStatus(int id);

	//指定したキャラクターを表示
	void ShowCharacter(int id);

	//全キャラクターを表示
	void ShowMembers();
	
	//レベルでのソート
	void SortMemberByLevel();

	//セーブ
	void SaveMembers();

	//ロード
	void LoadMembers();

	//体力での検索
	bool CheckMemberByHp(int hp, int id, int range);
};


#endif
