//-----------------------------------------------
// menu.cpp
// 制作日：2024/07/17
// 制作者：安田晴人
//-----------------------------------------------

#include "menu.h"
#include "guild.h"
#include "battle.h"


void Menu::SetNum(int num)
{
	m_num = num;
}

int Menu::GetNum()
{
	return m_num;
}

void Menu::DispMenu()
{
	int num = 0;
	std::cout << "「コマンドを選んでください」" << std::endl;
	std::cout << "１：キャラクターを作成" << std::endl;
	std::cout << "２：キャラクターを表示" << std::endl;
	std::cout << "３：キャラクターを検索" << std::endl;
	std::cout << "４：キャラクターを削除" << std::endl;
	std::cout << "５：キャラクターをセーブ" << std::endl;
	std::cout << "６：キャラクターをロード" << std::endl;
	std::cout << "７：キャラクターでバトル" << std::endl;
	std::cout << "０：終了" << std::endl << std::endl;

	std::cout << "数字を入力してね＝＞";
	std::cin >> num;
	SetNum(num);

}

void Menu::CreateCharacter()
{
   
    int success;

    //名前
    std::string name;

    std::cout << "キャラクターの名前はなんですか？" << std::endl;
    std::cin >> name;

    //性別
    std::string gender;

    std::cout << "キャラクターの性別はなんですか？" << std::endl;
    std::cin >> gender;

    //職業
    std::string job;

    std::cout << "キャラクターの職業はなんですか？" << std::endl;
    std::cin >> job;

    //レベル
    int level;

    std::cout << "キャラクターのレベルはいくつですか？(0～100)" << std::endl;
    std::cin >> level;

    //体力
    int hp;

    std::cout << "キャラクターの体力はいくつですか？(0～100)" << std::endl;
    std::cin >> hp;

    //魔法力
    int mp;

    std::cout << "キャラクターの魔法力はいくつですか？(0～100)" << std::endl;
    std::cin >> mp;

    //攻撃力
    int atk;

    std::cout << "キャラクターの攻撃力はいくつですか？(0～100)" << std::endl;
    std::cin >> atk;

    //防御力
    int def;

    std::cout << "キャラクターの防御力はいくつですか？(0～100)" << std::endl;
    std::cin >> def;

    //敏捷性
    int agi;

    std::cout << "キャラクターの敏捷性はいくつですか？(0～100)" << std::endl;
    std::cin >> agi;

    //器用度
    int dex;

    std::cout << "キャラクターの器用度はいくつですか？(0～100)" << std::endl;
    std::cin >> dex;

    //賢さ
    int wis;

    std::cout << "キャラクターの賢さはいくつですか？(0～100)" << std::endl;
    std::cin >> wis;

    success = AddMember(name, gender, job, level, hp, mp, atk, def, agi, dex, wis);

    if (success == -1)
    {
        std::cout << "キャラクターを追加できませんでした" << std::endl;
    }

    else
    {
        std::cout << "キャラクターの作成に成功しました！" << std::endl;
        std::cout << "ID:" << success + 1 << std::endl;
    }

}

void Menu::DispCharacter()
{
    int selectNum;

    std::cout << "1～" << MEMBER_MAX + 1 << "：任意のキャラクターを１体表示する" << std::endl;
    std::cout << "99：すべてのキャラクターを表示" << std::endl;
    std::cout << "0：キャンセル" << std::endl;
    std::cout << "数字を入力してください＝＞";
    std::cin >> selectNum;

    if (selectNum >= 1 && selectNum <= MEMBER_MAX + 1)
    {//任意のキャラクター1体
        ShowCharacter(selectNum - 1);
    }

    else if (selectNum == 99)
    {//すべてのキャラクター
        ShowMembers();

        std::cout << "キャラクターをレベル順にソートします" << std::endl;

        SortMemberByLevel();

        ShowMembers();

    }

    else if (selectNum == 0)
    {//キャンセル
        std::cout << "キャンセルされました" << std::endl;
        std::cout << "メインメニューに戻ります" << std::endl;
    }

    else
    {//無効な入力
        std::cout << "無効な数字が入力されました" << std::endl;
        std::cout << "メインメニューに戻ります" << std::endl;
    }
    std::cout << std::endl;
}

void Menu::SearchCharacter()
{
    std::string name;
    int success;

    std::cout << "検索したいキャラクターの名前を入力してね" << std::endl;
    std::cin >> name;

  
    success = GetMemberByName(name, 0);

    if (success != -1)
    {
        ShowCharacter(success);
    }

    else
    {
        std::cout << "その名前のキャラクターは見つかりませんでした" << std::endl;
        std::cout << "メインメニューに戻ります" << std::endl;
    }
}

void Menu::DeleteCharacter()
{
    int id;

    std::cout << "削除したいキャラクターのIDを入力してください" << std::endl;
    std::cin >> id;
    DeleteMember(id - 1);

    std::cout << "削除完了！\n";
}

void Menu::BattleCharacter()
{
    int id[2] = {};

    ShowMembers();
    std::cout << "バトルさせたい2体のキャラクターを選んでください" << std::endl;
    for (;;)
    {
        std::cout << "1体目のID＝＞";
        std::cin >> id[0];
        if (GetCharacter(id[0] - 1) == nullptr)
        {
            std::cout << "そのIDのキャラクターは存在しません\n";
            std::cout << "もう一度IDを入力してください\n";
        }

        else
        {
            break;
        }
    }

    for (;;)
    {
        std::cout << "2体目のID＝＞";
        std::cin >> id[1];
        std::cout << std::endl;

        if (GetCharacter(id[1] - 1) == nullptr)
        {
            std::cout << "そのIDのキャラクターは存在しません\n";
            std::cout << "もう一度IDを入力してください\n";
        }

        else if (id[0] == id[1])
        {
            std::cout << "同じキャラクター同士は戦えません\n";
        }

        else
        {
            break;
        }
    }

    Fighter* pFighter[2] = {};
    pFighter[0] = (Fighter*)GetCharacter(id[0] - 1);
    pFighter[1] = (Fighter*)GetCharacter(id[1] - 1);

    Battle battle(pFighter[0], pFighter[1]);

    std::cout << "それではバトルを開始します!!\n";

    battle.Fight();
}
