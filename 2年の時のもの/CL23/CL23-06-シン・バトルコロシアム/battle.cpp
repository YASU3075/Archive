//----------------------------------------------
// battle.cpp
// 制作日:2024/09/06
// 制作者:安田晴人
//----------------------------------------------

#include "battle.h"

Battle::Battle(Fighter* pFighterA, Fighter* pFighterB)
{
	m_Fighter[0] = pFighterA;
	m_Fighter[1] = pFighterB;
}

void Battle::Fight()
{
	int Hp[2];

	Hp[0] = m_Fighter[0]->GetHp();
	Hp[1] = m_Fighter[1]->GetHp();

	int turn = 0;

	for (;;)
	{
		int damage = m_Fighter[turn]->Damage() - m_Fighter[1 - turn]->GetDef() / 2;
		if (damage <= 0)
		{
			damage = 1;
		}
		m_Fighter[turn]->Attack();
		m_Fighter[1 - turn]->Defence();
		std::cout << m_Fighter[1 - turn]->GetName() << " に";
		std::cout << damage << "のダメージ!!\n";

		//ダメージ計算
		Hp[1 - turn] -= damage;

		if (Hp[1 - turn] <= 0)
		{
			std::cout << m_Fighter[1 - turn]->GetName() << " は倒れた…\n";
			break;
		}

		turn = 1 - turn;
		std::rewind(stdin);
		std::cin.get();
		std::system("cls");
	}
}