//-----------------------------------------------
// [monster.h]
// 制作日：2024/07/10
// 制作者：安田晴人
//-----------------------------------------------
#ifndef _MONSTER
#define _MONSTER

#include <string>
#include "player.h"

class Monster :public Player
{
private:
	//メンバ変数
	std::string m_race;		//種族
	int m_exp;				//経験値量

public:
	//メンバ関数
	void SetRace(std::string race);
	void SetExp(int exp);

	std::string GetRace(void);
	int GetExp(void);
};



#endif


