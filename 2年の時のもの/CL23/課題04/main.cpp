//-----------------------------------------------
// [main.cpp]
// 制作日：2024/07/10
// 制作者：安田晴人
//-----------------------------------------------
#include <iostream>
#include "monster.h"
#include <string>


int main()
{
    Monster monster;

    //名前
    std::string name;

    std::cout << "モンスターの名前はなんですか？" << std::endl;
    std::cin >> name;

    monster.SetName(name);

    //性別
    std::string gender;

    std::cout << "モンスターの性別はなんですか？" << std::endl;
    std::cin >> gender;

    monster.SetGender(gender);

    //職業
    std::string job;

    std::cout << "モンスターの職業はなんですか？" << std::endl;
    std::cin >> job;

    monster.SetJob(job);

    //レベル
    int level;

    std::cout << "モンスターのレベルはいくつですか？(0～100)" << std::endl;
    std::cin >> level;

    monster.SetLevel(level);

    //体力
    int hp;

    std::cout << "モンスターの体力はいくつですか？(0～100)" << std::endl;
    std::cin >> hp;

    monster.SetHp(hp);

    //魔法力
    int mp;

    std::cout << "モンスターの魔法力はいくつですか？(0～100)" << std::endl;
    std::cin >> mp;

    monster.SetMp(mp);

    //攻撃力
    int atk;

    std::cout << "モンスターの攻撃力はいくつですか？(0～100)" << std::endl;
    std::cin >> atk;

    monster.SetAtk(atk);

    //防御力
    int def;

    std::cout << "モンスターの防御力はいくつですか？(0～100)" << std::endl;
    std::cin >> def;

    monster.SetDef(def);

    //敏捷性
    int agi;

    std::cout << "モンスターの敏捷性はいくつですか？(0～100)" << std::endl;
    std::cin >> agi;

    monster.SetAgi(agi);

    //器用度
    int dex;

    std::cout << "モンスターの器用度はいくつですか？(0～100)" << std::endl;
    std::cin >> dex;

    monster.SetDex(dex);
    
    //賢さ
    int wis;

    std::cout << "モンスターの賢さはいくつですか？(0～100)" << std::endl;
    std::cin >> wis;

    monster.SetWis(wis);

    //種族
    std::string race;

    std::cout << "モンスターの種族はなんですか？" << std::endl;
    std::cin >> race;

    monster.SetRace(race);

    //経験値量
    int exp;

    std::cout << "モンスターの経験値量はいくつですか？(0～100)" << std::endl;
    std::cin >> exp;

    monster.SetExp(exp);


    //表示
    std::cout << "\n\nこのモンスターのステータスは以下の通りです\n\n";

    std::cout << "名前　　：" << monster.GetName() << std::endl;
    std::cout << "性別　　：" << monster.GetGender() << std::endl;
    std::cout << "職業　　：" << monster.GetJob() << std::endl;
    std::cout << "レベル　：" << monster.GetLevel() << std::endl;
    std::cout << "体力　　：" << monster.GetHp() << std::endl;
    std::cout << "魔法力　：" << monster.GetMp() << std::endl;
    std::cout << "攻撃力　：" << monster.GetAtk() << std::endl;
    std::cout << "防御力　：" << monster.GetDef() << std::endl;
    std::cout << "敏捷性　：" << monster.GetAgi() << std::endl;
    std::cout << "器用度　：" << monster.GetDex() << std::endl;
    std::cout << "賢さ　　：" << monster.GetWis() << std::endl;
    std::cout << "種族　　：" << monster.GetRace() << std::endl;
    std::cout << "経験値量：" << monster.GetExp() << std::endl;

    //入力待ち
    rewind(stdin);
    (void)getchar();
    
    return 0;
}
