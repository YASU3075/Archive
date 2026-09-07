//-----------------------------------------------
// menu.h
// 制作日：2024/07/17
// 制作者：安田晴人
//-----------------------------------------------

#ifndef _MENU
#define _MENU

#include <string>
#include "guild.h"

class Menu : public Guild
{
private:
	int m_num = 0;		//選択した番号

public:
	
	Menu(int memberMax) : Guild(memberMax)
	{

	}

	void SetNum(int num);	
	int GetNum();			

	void DispMenu();		//メニューの表示

	void CreateCharacter();
	void DispCharacter();
	void SearchCharacter();
	void DeleteCharacter();
	void BattleCharacter();
};



#endif


