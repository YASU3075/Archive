//---------------------------------------------- -
// main.cpp
// 制作日：2024/06/28
// 制作者：安田晴人
//-----------------------------------------------
#include <iostream>
#include <string>
#include "menu.h"
#include "guild.h"

int main()
{
    Menu menu(MEMBER_MAX);
    
    //初期設定
    menu.SetNum(-1);
    int selectNum = menu.GetNum();
    bool isFinish = false;

    while (!isFinish)
    {
        //メニューを表示
        menu.DispMenu();
        selectNum = menu.GetNum();
        std::cout << std::endl << "===============================================================================" << std::endl << std::endl;


        switch (selectNum)
        {
        case 1: //作成
            menu.CreateCharacter();
            break;

        case 2: //表示
            menu.DispCharacter();
            break;

        case 3: //検索
            menu.SearchCharacter();
            break;

        case 4: //削除
            menu.DeleteCharacter();
            break;

        case 5: //セーブ
            menu.SaveMembers();
            break;

        case 6: //ロード
            menu.LoadMembers();
            break;

        case 0: //終了
            isFinish = true;
            break;

        default:
            std::cout << "無効な数字が入力されました" << std::endl;
            break;
        }

        if (!isFinish)
        {
            std::cout << "メインメニューに戻ります" << std::endl;
            std::cout << std::endl << "===============================================================================" << std::endl << std::endl;

            (void)std::getc(stdin);
            (void)std::getc(stdin);

            std::system("cls");

            menu.SetNum(-1);
            selectNum = menu.GetNum();
        }
    }
}
