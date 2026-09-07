#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <io.h>
#include <conio.h>
#include <stdio.h>
#include <chrono>
#include <thread>

#include "Manager.h"
#include "Title.h"
#include "GameScene.h"

Manager* Manager::m_instance = nullptr; // シングルトンインスタンスの初期化

/* この実習で学んだ様々なものを使って簡単なCUIのゲームを作ってみよう。
 * 作成するものは何でもよいが下記の要件を満たす事。
 * タイトル ゲームシーケンス 結果表示が存在しそれらがステートマシンによってコントロールされていること（任意)
 * クラスの多態性を用いたオブジェクト管理が入っていること(任意)
 * 自分が作成した Poolアロケータが使われていること(必須)
 * 何かしらの処理にスレッドを用いた並行処理が入っていること(任意)
 * ゲームエンジンの使用は不可
 * 使用する言語は C ++
 */

int main()
{
	// エスケープシーケンスを有効に
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode = 0;
	GetConsoleMode(hStdOut, &mode);
	SetConsoleMode(hStdOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

	// カーソルを消す
	printf("\x1b[?25l");

	const int FPS = 60;
	const int FRAME_TIME = 1000 / FPS; // フレーム時間（ミリ秒）

	// マネージャーのインスタンスを取得
	Manager* manager = Manager::GetInstance();
	manager->SetQuit(false);

	// タイトルシーンを設定
	manager->SetCurrentScene(new Title()); // タイトルシーンを設定

	{
		do
		{
			auto start_time = std::chrono::high_resolution_clock::now(); // フレーム開始時間

			

			manager->UpdateCurrentScene(); // 現在のシーンを更新
			manager->DrawCurrentScene(); // 現在のシーンを描画

			manager->SwitchScene(); // シーンの切り替え処理

			auto end_time = std::chrono::high_resolution_clock::now(); // フレーム終了時間
			std::chrono::duration<double, std::milli> frame_duration = end_time - start_time; // フレーム時間の計算
			int sleep_time = FRAME_TIME - static_cast<int>(frame_duration.count()); // 残りの時間を計算
			if (sleep_time > 0) // 残り時間が正の場合
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time)); // スリープ
			}
		} while (!Manager::GetInstance()->IsQuit());
	}

	printf("\x1b[2J");	// 画面をクリア

	printf("\x1b[%d;%dHまた遊んでね!", 5, 5);

	// カーソルを表示
	printf("\x1b[?25h");

	_getch();

	return EXIT_SUCCESS;
}
