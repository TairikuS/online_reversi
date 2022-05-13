#include<iostream>
#include <chrono>
#include <thread>
#include "reversi.h"

using namespace std;

int main()
{
	Reversi reversi;
	//これをtrueにするとコンピュータ同士がやりあってくれる;
	const bool debugMode = false;
	reversi.SetInputAuto(debugMode);
	while (true)
	{
		if (reversi.FirstPlayer())
		{
			cout << "\nプレイヤーの石は";
			if (reversi.FirstPlayer())
			{
				cout << "●です。\n";
			}
			else
			{
				cout << "〇です。\n";
			}
			cout << "石が置ける場所は盤面上の数値が書かれているところのみです。\n";
			cout << "autoと入力すると以降は自動で石を置く位置が選択されます。\n";
			cout << "endと入力すると降参します。\n\n";
			cout << "あなたの番です\n";
			reversi.PrintBoard(Reversi::Stone::White);
			reversi.PutStone(Reversi::Stone::White);
			//終わりかどうかの判定
			if (reversi.CheckEnd())
				break;

			cout << "対戦相手の番です\n";
			reversi.PrintBoard(Reversi::Stone::Black);
			reversi.WaitPutStone(Reversi::Stone::Black);

			if (reversi.CheckEnd())
				break;
		}
		else
		{
			cout << "対戦相手の番です\n";
			reversi.PrintBoard(Reversi::Stone::White);
			reversi.WaitPutStone(Reversi::Stone::White);

			if (reversi.CheckEnd())
				break;

			cout << "\nプレイヤーの石は";
			if (reversi.FirstPlayer())
			{
				cout << "●です。\n";
			}
			else
			{
				cout << "〇です。\n";
			}
			cout << "石が置ける場所は盤面上の数値が書かれているところのみです。\n";
			cout << "endと入力すると降参します。\n\n";
			cout << "あなたの番です\n";
			reversi.PrintBoard(Reversi::Stone::Black);
			reversi.PutStone(Reversi::Stone::Black);
			//終わりかどうかの判定
			if (reversi.CheckEnd())
				break;
		}
	}
}