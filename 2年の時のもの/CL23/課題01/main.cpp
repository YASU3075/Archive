//-----------------------------------------------
// [main.cpp]
// 制作日：2024/05/24
// 制作者：安田晴人
//-----------------------------------------------
#include <iostream>
#include "player.h"
#include <string>


int main()
{
    Player player;

    //名前
    std::string name;

    std::cout << "プレイヤーの名前はなんですか？" << std::endl;
    std::cin >> name;

    player.SetName(name);

    //性別
    std::string gender;

    std::cout << "プレイヤーの性別はなんですか？" << std::endl;
    std::cin >> gender;

    player.SetGender(gender);

    //職業
    std::string job;

    std::cout << "プレイヤーの職業はなんですか？" << std::endl;
    std::cin >> job;

    player.SetJob(job);

    //レベル
    int level;

    std::cout << "プレイヤーのレベルはいくつですか？(0～100)" << std::endl;
    std::cin >> level;

    player.SetLevel(level);

    //体力
    int hp;

    std::cout << "プレイヤーの体力はいくつですか？(0～100)" << std::endl;
    std::cin >> hp;

    player.SetHp(hp);

    //魔法力
    int mp;

    std::cout << "プレイヤーの魔法力はいくつですか？(0～100)" << std::endl;
    std::cin >> mp;

    player.SetMp(mp);

    //攻撃力
    int atk;

    std::cout << "プレイヤーの攻撃力はいくつですか？(0～100)" << std::endl;
    std::cin >> atk;

    player.SetAtk(atk);

    //防御力
    int def;

    std::cout << "プレイヤーの防御力はいくつですか？(0～100)" << std::endl;
    std::cin >> def;

    player.SetDef(def);

    //敏捷性
    int agi;

    std::cout << "プレイヤーの敏捷性はいくつですか？(0～100)" << std::endl;
    std::cin >> agi;

    player.SetAgi(agi);

    //器用度
    int dex;

    std::cout << "プレイヤーの器用度はいくつですか？(0～100)" << std::endl;
    std::cin >> dex;

    player.SetDex(dex);
    
    //賢さ
    int wis;

    std::cout << "プレイヤーの賢さはいくつですか？(0～100)" << std::endl;
    std::cin >> wis;

    player.SetWis(wis);

    //表示
    std::cout << "\n\nこのプレイヤーのステータスは以下の通りです\n\n";

    std::cout << "名前　：" << player.GetName() << std::endl;
    std::cout << "性別　：" << player.GetGender() << std::endl;
    std::cout << "職業　：" << player.GetJob() << std::endl;
    std::cout << "レベル：" << player.GetLevel() << std::endl;
    std::cout << "体力　：" << player.GetHp() << std::endl;
    std::cout << "魔法力：" << player.GetMp() << std::endl;
    std::cout << "攻撃力：" << player.GetAtk() << std::endl;
    std::cout << "防御力：" << player.GetDef() << std::endl;
    std::cout << "敏捷性：" << player.GetAgi() << std::endl;
    std::cout << "器用度：" << player.GetDex() << std::endl;
    std::cout << "賢さ　：" << player.GetWis() << std::endl;

    //入力待ち
    rewind(stdin);
    (void)getchar();
    
    return 0;
}
