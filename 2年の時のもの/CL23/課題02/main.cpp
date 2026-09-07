//-----------------------------------------------
// CL23-02-使ったら責任もって後片付け
// main.cpp
// 制作日：2024/06/23
// 制作者：安田晴人
//-----------------------------------------------

#include <iostream>
#include <string>
#include "character.h"

int main()
{
    //①
    Character* pCharacter = new Character;

    pCharacter->SetName("やすだ");
    pCharacter->SetHp(20);

    std::cout << "名前："<< pCharacter->GetName() << std::endl;
    std::cout << "HP："<< pCharacter->GetHp() << std::endl << std::endl;

    delete pCharacter;

    Character* pChara = new Character[10];

    //リュウ
    (pChara + 2)->SetName("リュウ");
    (pChara + 2)->SetHp(40);
    (pChara + 2)->SetAtk(30);
    (pChara + 2)->SetDef(10);

    //ケン
    (pChara + 7)->SetName("ケン");
    (pChara + 7)->SetHp(10);
    (pChara + 7)->SetAtk(40);
    (pChara + 7)->SetDef(20);

    //ケンからリュウへの攻撃
    int damage;     //ダメージ数
    damage = (pChara + 7)->GetAtk() - (pChara + 2)->GetDef();

    int battleHp;       //一時的のHP
    battleHp = (pChara + 2)->GetHp() - damage;

    //ダメージ処理
    (pChara + 2)->SetHp(battleHp);

    std::cout << "名前：" << (pChara + 2)->GetName() << std::endl;
    std::cout << "HP：" << (pChara + 2)->GetHp() << std::endl;

    //削除
    delete[] pChara;

    rewind(stdin);
    (void)getchar();

    return 0;
}

