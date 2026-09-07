//-----------------------------------------------
// menu.cpp
// 制作日：2024/07/17
// 制作者：安田晴人
//-----------------------------------------------

#include "menu.h"
#include "guild.h"



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
        std::cout << "キャラクターの作成に成功しました！" << std::endl << std::endl;
        std::cout << "ID:" << success + 1 << std::endl;
    }

}

void Menu::DispCharacter()
{
    int selectNum;

    std::cout << "1～" << MEMBER_MAX + 1 << "：入力したIDのキャラクターを表示する" << std::endl;
    std::cout << "99：すべてのキャラクターを表示" << std::endl;
    std::cout << "0：キャンセル" << std::endl;
    std::cout << "数字を入力してください＝＞";
    std::cin >> selectNum;

    if (selectNum >= 1 && selectNum <= MEMBER_MAX + 1)
    {//任意のキャラクター1体
        ShowCharacterAllStatus(selectNum - 1);
    }

    else if (selectNum == 99)
    {//すべてのキャラクター
        ShowMembers();

        std::cout << "キャラクターをレベル順（昇順）にソートしますか？" << std::endl;
        std::cout << "[1:はい]  [2:いいえ]" << std::endl;
        std::cin >> selectNum;

        if (selectNum == 1)
        {
            SortMemberByLevel();
            std::cout << "ソートした結果がこちらです" << std::endl;
            ShowMembers();
        }

        else
        {
            std::cout << "ソートせずに終了します" << std::endl;
        }

    }

    else if (selectNum == 0)
    {//キャンセル
        std::cout << "キャンセルされました" << std::endl;
    }

    else
    {//無効な入力
        std::cout << "無効な数字が入力されました" << std::endl;
    }
    std::cout << std::endl;
}

void Menu::SearchCharacter()
{
    int selectNum = 0;

    std::cout << "検索したい方法を選んでね" << std::endl;
    std::cout << "１：名前で検索" << std::endl;
    std::cout << "２：体力の範囲で検索" << std::endl;
    std::cout << "数字を入力してね＝＞";
    std::cin >> selectNum;

    if (selectNum == 1)
    {
        std::string name;
        int success;

        std::cout << "検索したいキャラクターの名前を入力してね" << std::endl;
        std::cin >> name;


        success = GetMemberByName(name, 0);

        if (success != -1)
        {
            std::cout << std::endl << "キャラクターが見つかりました" << std::endl;
            ShowCharacter(success);
        }

        else
        {
            std::cout << "その名前のキャラクターは見つかりませんでした" << std::endl;
        }
    }

    else if (selectNum == 2)
    {
        int hp = -1;
        int range = -1;
        bool isSearch = false;

        std::cout << "検索する基準となる体力の数値を入力してください" << std::endl;
        std::cout << "数字を入力してください＝＞";
        std::cin >> hp;

        std::cout << std::endl << "次にその数値以上か以下のどちらで知らべるか選んでください（[以上：０] [以下：１]）" << std::endl;
        std::cout << "数字を入力してください＝＞";
        std::cin >> range;

        std::cout << std::endl << "検索結果はこちらになります" << std::endl;

        for (int i = 0; i < MEMBER_MAX; i++)
        {
            
            bool success = CheckMemberByHp(hp, i, range);
            
            if (success)
            {
                ShowCharacter(i);
                isSearch = true;
            }

        }

        if (!isSearch)
        {
            std::cout << "その条件に合うキャラクターは見つかりませんでした" << std::endl;
        }

    }

}

void Menu::DeleteCharacter()
{
    int selectNum;

    std::cout << "1～" << MEMBER_MAX + 1 << "：入力したIDのキャラクターを削除する" << std::endl;
    std::cout << "99：すべてのキャラクターを削除" << std::endl;
    std::cout << "0：キャンセル" << std::endl;
    std::cout << "数字を入力してください＝＞";
    std::cin >> selectNum;
   
    if (selectNum >= 1 && selectNum <= MEMBER_MAX + 1)
    {
        DeleteMember(selectNum - 1);
    }

    else if (selectNum == 99)
    {
        for (int i = 0; i < MEMBER_MAX; i++)
        {
            DeleteMember(i);
        }
    }

    else
    {
        std::cout << "削除がキャンセルされました" << std::endl;
    }

    std::cout << "削除完了！";
}
