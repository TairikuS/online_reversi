#include<ctime>
#include"reversi.h"

Reversi::Reversi()
{
	std::string input;
	bool retry = false;
	unsigned int inputNumber = 0, pow;

	do {
		retry = false;

		std::cout << "あなたはサーバー側ですか？クライアント側ですか？\n";
		std::cout << "server -> 0, client -> 1\n";
		std::getline(std::cin, input);

		if (input == "server" || input == "0")
		{
			state = PlayerState::server;
		}
		else if (input == "client" || input == "1")
		{
			state = PlayerState::client;
		}
		else
		{
			std::cout << "正常に読み取れませんでした。0もしくは1で入力してください。\n";
			retry = true;
		}
	} while (retry);

	if (state == PlayerState::server)
	{
		result = WSAStartup(MAKEWORD(1, 1), &wsaData);
		if (result) {
			std::cout << "WSAStartupの失敗。\n";
			//失敗
		}

		listenSoc = socket(AF_INET, SOCK_STREAM, 0);
		if (listenSoc < 0) {
			std::cout << "ソケットオープンエラー\n";
			WSACleanup();
			//失敗
		}

		do {
			retry = false;
			std::cout << "ポート番号(49152～65535)：";
			std::getline(std::cin, input);

			if (input[0] == '\0')
			{
				for (int i = 0; i < 5; i++)
				{
					buffer[i] = '5';
				}
			}
			else
			{
				if (input.length() != 5)
				{
					std::cout << "入力エラーです。49152～65535の値を入力してください。\n";
					inputNumber = 0;
					retry = true;
				}
				else
				{
					pow = 1;
					for (int i = 0; i < 5; i++)
					{
						inputNumber += pow * (input[4 - i] - '0');
						pow *= 10;
					}

					if (inputNumber < 49152 || inputNumber > 65535)
					{
						std::cout << "入力エラーです。49152～65535の値を入力してください。\n";
						inputNumber = 0;
						retry = true;
					}
					else
					{
						for (int i = 0; i < 5; i++)
						{
							buffer[i] = input[i];
						}
					}
				}
			}
		} while (retry);
		portNum = atoi(buffer);

		ZeroMemory(&saddr, sizeof(SOCKADDR_IN));
		saddr.sin_family = AF_INET;
		saddr.sin_port = htons(portNum);
		saddr.sin_addr.s_addr = INADDR_ANY;
		if (bind(listenSoc, (struct sockaddr*)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
			std::cout << "bindのエラー\n";
			closesocket(listenSoc);
			WSACleanup();
			//失敗
		}

		if (listen(listenSoc, 0) == SOCKET_ERROR) {
			std::cout << "listen error.\n";
			closesocket(listenSoc);
			WSACleanup();
			//失敗
		}
		std::cout << "対戦相手を待っています。しばらくお待ちください。\n";

		fromlen = sizeof(from);
		soc = accept(listenSoc, (SOCKADDR*)&from, &fromlen);

		if (soc == INVALID_SOCKET) {
			std::cout << "accept error.\n";
			closesocket(listenSoc);
			WSACleanup();
			//失敗
		}
		else
			std::cout << inet_ntoa(from.sin_addr) << "が接続してきました。\n";

		closesocket(listenSoc);

		do {
			retry = false;
			std::cout << "どちらが先行プレイヤーになりますか？※入力されなかった場合ランダムで決定されます(server -> 0,client -> 1)：";
			std::getline(std::cin, input);

			if (input[0] == '\0')
			{
				int randomNumber;
				std::srand(static_cast<unsigned int>(std::time(nullptr)));

				randomNumber = rand() % 2;

				if (randomNumber == 0)
				{
					firstPlayer = true;
					buffer[0] = '0';
					send(soc, buffer, int(strlen(buffer)), 0);
					std::cout << "あなたは先攻です。\n";
				}
				else
				{
					firstPlayer = false;
					buffer[0] = '1';
					send(soc, buffer, int(strlen(buffer)), 0);
					std::cout << "あなたは後攻です。\n";
				}
			}
			else if (input == "server" || input == "0")
			{
				firstPlayer = true;
				buffer[0] = '0';
				send(soc, buffer, int(strlen(buffer)), 0);
				std::cout << "あなたは先攻です。\n";
			}
			else if (input == "client" || input == "1")
			{
				firstPlayer = false;
				buffer[0] = '1';
				send(soc, buffer, int(strlen(buffer)), 0);
				std::cout << "あなたは後攻です。\n";
			}
			else
			{
				std::cout << "正常に読み取れませんでした。0もしくは1で入力してください。\n";
				retry = true;
			}
		} while (retry);
	}
	else
	{
		do {
			retry = false;
			std::cout << "ポート番号(49152～65535)：";
			std::getline(std::cin, input);
			
			if (input[0] == '\0')
			{
				for (int i = 0; i < 5; i++)
				{
					buffer[i] = '5';
				}
			}
			else
			{
				if (input.length() != 5)
				{
					std::cout << "入力エラーです。49152～65535の値を入力してください。\n";
					inputNumber = 0;
					retry = true;
				}
				else
				{
					pow = 1;
					for (int i = 0; i < 5; i++)
					{
						inputNumber += pow * (input[4 - i] - '0');
						pow *= 10;
					}

					if (inputNumber < 49152 || inputNumber > 65535)
					{
						std::cout << "入力エラーです。49152～65535の値を入力してください。\n";
						inputNumber = 0;
						retry = true;
					}
					else
					{
						for (int i = 0; i < 5; i++)
						{
							buffer[i] = input[i];
						}
					}
				}
			}
		} while (retry);
		portNum = atoi(buffer);
		
		//bufferの中身をきれいにする
		for (int i = 0; i < 5; i++)
		{
			buffer[i] = '\0';
		}
		std::cout << "サーバーのIPアドレス：";
		std::getline(std::cin, input);

		//enter入力の時は自分自信を示すアドレスを自動で入力
		if (input[0] == '\0')
		{
			serverIP[0] = '1';
			serverIP[1] = '2';
			serverIP[2] = '7';
			serverIP[3] = '.';
			serverIP[4] = '0';
			serverIP[5] = '.';
			serverIP[6] = '0';
			serverIP[7] = '.';
			serverIP[8] = '1';
		}
		else
		{
			for (int i = 0; i < static_cast<int>(input.length()); i++)
			{
				serverIP[i] = input[i];
			}
		}

		result = WSAStartup(MAKEWORD(1, 1), &wsaData);
		if (result) {
			std::cout << "WSAStartupの失敗。\n";
			//失敗
		}

		soc = socket(AF_INET, SOCK_STREAM, 0);
		if (soc < 0) {
			std::cout << "ソケットオープンエラー\n";
			WSACleanup();
			//失敗
		}

		lpHost = gethostbyname(serverIP);
		if (lpHost == NULL) {
			addr = inet_addr(serverIP);
			lpHost = gethostbyaddr((char*)&addr, 4, AF_INET);
		}
		if (lpHost == NULL) {
			std::cout << "gethostbyaddrのエラー\n";
			closesocket(soc);
			WSACleanup();
			//失敗
		}

		ZeroMemory(&saddr, sizeof(SOCKADDR_IN));

		if (lpHost != NULL)
		{
			saddr.sin_family = lpHost->h_addrtype;
			saddr.sin_port = htons(portNum);
			saddr.sin_addr.s_addr = *((u_long*)lpHost->h_addr);
		}

		if (connect(soc, (SOCKADDR*)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
			std::cout << "connectのエラー\n";
			closesocket(soc);
			WSACleanup();
			//失敗
		}
		else
			std::cout << "接続完了。現在serverがプレイ順を決めています。\n";

		rcv = recv(soc, buffer, sizeof(buffer) - 1, 0);
		if (rcv == SOCKET_ERROR)
		{
			std::cout << "エラーです。\n";
		}
		if (buffer[0] == '0')
		{
			std::cout << "あなたは後攻です。\n";
			firstPlayer = false;
		}
		else
		{
			std::cout << "あなたは先攻です。\n";
			firstPlayer = true;
		}
	}


	//一応全部Noneで初期化
	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; j++)
		{
			stones[i][j] = Stone::None;
		}
	}

	//オセロの初期配置
	stones[boardSize / 2 - 1][boardSize / 2 - 1] = Stone::White;
	stones[boardSize / 2][boardSize / 2] = Stone::White;

	stones[boardSize / 2 - 1][boardSize / 2] = Stone::Black;
	stones[boardSize / 2][boardSize / 2 - 1] = Stone::Black;

	//石の数などの初期化
	whiteStoneCount = 2;
	blackStoneCount = 2;
	noneSquareCount = boardSize * boardSize - 4;
}

void Reversi::PrintBoard(Stone nextPutStoneColor)
{
	int k = 0;
	for (int i = 0; i < boardSize; i++)
	{
		std::cout << "";
		if (i == 0)
		{
			std::cout << " ";
			for (int j = 0; j < boardSize; j++)
			{
				std::cout << "   " << j + 1;
			}
			std::cout << "   横" << '\n';

		}

		//形を整えるための空白
		std::cout << "  ";

		//列の間の線を表示する
		for (int j = 0; j < boardSize; j++)
		{
			std::cout << "+---";
		}
		std::cout << '+' << '\n';

		//縦のマス数の表示
		std::cout << ' ' << i + 1;

		//マスにどの石が置かれているのかを表示
		for (int j = 0; j < boardSize; j++)
		{
			if (stones[j][i] == Stone::White)
			{
				std::cout << "| ●";
			}
			else if (stones[j][i] == Stone::Black)
			{
				std::cout << "| 〇";
			}
			else if (CheckAllLine(j, i, nextPutStoneColor, false))
			{
				std::cout << "| " << k + 1;
				if (k < 9)
				{
					std::cout<<' ';
				}
				k++;
			}
			else 
			{
				std::cout << "|   ";
			}
		}
		std::cout << '|' << '\n';
	}
	//ボードの一番下を閉じる線
	std::cout << "  ";
	for (int j = 0; j < boardSize; j++)
	{
		std::cout << "+---";
	}
	std::cout << '+' << '\n' << "縦\n";
	std::cout << "[〇×" <<  Reversi::blackStoneCount<< "個]　[●×" << Reversi::whiteStoneCount << "個]\n\n";
}

bool Reversi::PutStone(Stone stoneColor)
{
	bool canPutStone = true;
	std::vector<int>* canPutPos;
	int putPos;

	//渡された色によって使うvectorを変える
	if (stoneColor == Stone::Black)
	{
		canPutPos = &blackCanPutPos;
	}
	else
	{
		canPutPos = &whiteCanPutPos;
	}

	CheckPutPos(canPutPos, stoneColor);

	//石が置けない場合置く動作はスキップ
	if (canPutPos->size() == 0)
	{
		std::cout << "石を置ける場所がありませんでした\n";
		canPutStone = false;
	}
	else
	{
		if (inputAuto)
		{
			Computer com;
			putPos = com.ChoosePos(canPutPos);
		}
		else
		{
			Player player;
			putPos = player.ChoosePos(canPutPos);
			inputAuto = player.InputAuto();
			end = player.End();
		}
		if (!end)
		{
			int x = putPos % boardSize;
			int y = putPos / boardSize;

			if (stones[x][y] != Stone::None)
			{
				std::cout << "すでに石が置かれています!\n";
				canPutStone = false;
			}
			else if (!CheckAllLine(x, y, stoneColor, false))
			{
				std::cout << "ひっくり返せる石がありません!\n";
				canPutStone = false;
			}
			else
			{
				if (putPos > 10)
				{
					buffer[0] = putPos / 10 + '0';
					buffer[1] = putPos % 10 + '0';
					buffer[2] = '\0';
				}
				else
				{
					buffer[0] = putPos + '0';
					buffer[1] = '\0';
				}

				send(soc, buffer, int(strlen(buffer)),0);

				CheckAllLine(x, y, stoneColor, true);
				stones[x][y] = stoneColor;
				if (stoneColor == Stone::Black)
				{
					blackStoneCount++;
					noneSquareCount--;
				}
				else
				{
					whiteStoneCount++;
					noneSquareCount--;
				}
			}
		}
		else
		{
			buffer[0] = 'e';
			buffer[1] = 'n';
			buffer[2] = 'd';
			buffer[3] = '\0';
			send(soc, buffer, int(strlen(buffer)), 0);
		}
	}
	return canPutStone;
}

bool Reversi::CheckLine(int x, int y, int directionX, int directionY, Stone checkStoneColor, bool turnOverFlag)
{
	//ひっくり返せるかどうかのフラグ
	bool canStoneTurnOver = false;
	int turnOverCount = 0;

	for (int i = 1; i <= boardSize; i++)
	{
		//配列の範囲外にアクセスしないように
		if (x + (directionX * i) >= 0 && y + (directionY * i) >= 0 && x + (directionX * i) < boardSize && y + (directionY * i) < boardSize)
		{
			if (stones[x + (directionX * i)][y + (directionY * i)] != checkStoneColor && stones[x + (directionX * i)][y + (directionY * i)] != Stone::None)
			{
				turnOverCount++;
			}
			else if(stones[x + (directionX * i)][y + (directionY * i)] == checkStoneColor)
			{
				if (turnOverCount > 0)
				{
					canStoneTurnOver = true;
				}
				break;
			}
			else 
			{
				turnOverCount = 0;
				break;
			}
		}
		else
		{
			turnOverCount = 0;
			break;
		}
	}

	if (turnOverFlag)
	{
		for (int i = 1; i <= turnOverCount; i++)
		{
			if (checkStoneColor == Stone::Black)
			{
				stones[x + (directionX * i)][y + (directionY * i)] = Stone::Black;

				blackStoneCount++;
				whiteStoneCount--;
			}
			else
			{
				stones[x + (directionX * i)][y + (directionY * i)] = Stone::White;
				whiteStoneCount++;
				blackStoneCount--;
			}
		}
	}
	return canStoneTurnOver;
}
bool Reversi::CheckAllLine(int x, int y, Stone checkStoneColor,bool turnOverFlag)
{
	bool canStoneTurnOver = false;

	//それぞれの向きごとにひっくり返す石を調べていく
	//横[ー]
	canStoneTurnOver = CheckLine(x, y, 1, 0, checkStoneColor, turnOverFlag);
	if (canStoneTurnOver)
	{
		CheckLine(x, y, -1, 0, checkStoneColor, turnOverFlag);
	}
	else
	{
		canStoneTurnOver = CheckLine(x, y, -1, 0, checkStoneColor, turnOverFlag);
	}
	
	//縦[|]
	if (canStoneTurnOver)
	{
		CheckLine(x, y, 0, 1, checkStoneColor, turnOverFlag);
	}
	else
	{
		canStoneTurnOver = CheckLine(x, y, 0, 1, checkStoneColor, turnOverFlag);
	}

	if (canStoneTurnOver)
	{
		CheckLine(x, y, 0, -1, checkStoneColor, turnOverFlag);
	}
	else
	{
		canStoneTurnOver = CheckLine(x, y, 0, -1, checkStoneColor, turnOverFlag);
	}

	//斜め[/]
	if (canStoneTurnOver)
	{
		CheckLine(x, y, 1, -1, checkStoneColor, turnOverFlag);
	}
	else
	{
		canStoneTurnOver = CheckLine(x, y, 1, -1, checkStoneColor, turnOverFlag);
	}

	if (canStoneTurnOver)
	{
		CheckLine(x, y, -1, 1, checkStoneColor, turnOverFlag);
	}
	else
	{
		canStoneTurnOver = CheckLine(x, y, -1, 1, checkStoneColor, turnOverFlag);
	}
	
	//斜め[＼]
	if (canStoneTurnOver)
	{
		CheckLine(x, y, 1, 1, checkStoneColor, turnOverFlag);
	}
	else
	{
		canStoneTurnOver = CheckLine(x, y, 1, 1, checkStoneColor, turnOverFlag);
	}

	if (canStoneTurnOver)
	{
		CheckLine(x, y, -1, -1, checkStoneColor, turnOverFlag);
	}
	else
	{
		canStoneTurnOver = CheckLine(x, y, -1, -1, checkStoneColor, turnOverFlag);
	}

	return canStoneTurnOver;
}

void Reversi::CheckPutPos(std::vector<int> *canPutPos,Stone stoneColor)
{
	//毎回置ける場所が結構変わるので初めに空にする
	canPutPos->clear();

	for (int j = 0; j < boardSize; j++)
	{
		for (int i = 0; i < boardSize; i++)
		{
			if (CheckAllLine(i, j, stoneColor, false) && stones[i][j] == Stone::None)
			{
				canPutPos->push_back(j * boardSize + i);
			}
		}
	}
}

bool Reversi::CheckEnd()
{
	if (noneSquareCount == 0)
	{
		PrintBoard(Stone::None);
		std::cout << "gameend!\n";
		std::cout << "●の個数：" << whiteStoneCount << " 〇の個数：" << blackStoneCount << '\n';
		if (whiteStoneCount > blackStoneCount)
		{
			if (firstPlayer)
			{
				std::cout << "あなたの勝利！\n";
			
			}
			else
			{
				std::cout << "対戦相手の勝利！\n";
			}
			
		}
		else if (blackStoneCount > whiteStoneCount)
		{
			if (firstPlayer)
			{
				std::cout << "対戦相手の勝利！\n";
			}
			else
			{
				std::cout << "あなたの勝利！\n";
			}
		}
		else
		{
			std::cout << "引き分け！\n";
		}
		end = true;
	}

	if (blackStoneCount == 0)
	{
		PrintBoard(Stone::None);
		std::cout << "gameend!\n"; 
		std::cout << "●の個数：" << whiteStoneCount << " 〇の個数：" << blackStoneCount << '\n';
		if (firstPlayer)
		{
			std::cout << "すごい！あなたの圧勝だ!\n";
		}
		else
		{
			std::cout << "対戦相手の圧勝\n";
		}
		end = true;
	}

	if (whiteStoneCount == 0)
	{
		PrintBoard(Stone::None);
		std::cout << "gameend!\n";
		std::cout << "●の個数：" << whiteStoneCount << " 〇の個数：" << blackStoneCount << '\n';
		if (firstPlayer)
		{
			std::cout << "対戦相手の圧勝\n";
		}
		else
		{
			std::cout << "すごい！あなたの圧勝だ!\n";
		}
		end = true;
	}

	if (end)
	{
		shutdown(soc, SD_BOTH);
		closesocket(soc);

		result = WSACleanup();
		if (result) {
			std::cout << "WSACleanupの失敗。\n";
			//失敗
		}
	}
	return end;
}

void Reversi::WaitPutStone(Stone stoneColor)
{
	int putNum = -1;
	int x, y;
	std::vector<int>* canPutPos;

	//渡された色によって使うvectorを変える
	if (stoneColor == Stone::Black)
	{
		canPutPos = &blackCanPutPos;
	}
	else
	{
		canPutPos = &whiteCanPutPos;
	}

	CheckPutPos(canPutPos, stoneColor);

	//石が置けない場合置く動作はスキップ
	if (canPutPos->size() == 0)
	{
		std::cout << "石を置ける場所がありませんでした\n";
	}
	else
	{
		std::cout << "対戦相手の入力を待っています。\n";
		rcv = recv(soc, buffer, sizeof(buffer) - 1, 0);
		if (rcv == SOCKET_ERROR) {
			std::cout << "エラーです。\n";
		}

		if (rcv < sizeof(buffer))
			buffer[rcv] = '\0';


		if (buffer[0] == 'e')
		{
			std::cout << "対戦相手が降参しました。\n";
			end = true;
		}
		else
		{
			if (rcv == 2)
			{
				putNum = (buffer[0] - '0') * 10 + buffer[1] - '0';
			}
			else if (rcv == 1)
			{
				putNum = buffer[0] - '0';
			}

			x = putNum % boardSize;
			y = putNum / boardSize;
			CheckAllLine(x, y, stoneColor, true);
			if (x < boardSize && x >= 0 && y >= 0 && y < boardSize)
			{
				stones[x][y] = stoneColor;
			}
			else
			{
				std::cout << "エラーが発生しました。\n";
				end = true;
			}
			if (stoneColor == Stone::Black)
			{
				blackStoneCount++;
				noneSquareCount--;
			}
			else
			{
				whiteStoneCount++;
				noneSquareCount--;
			}
		}
	}
}

int Reversi::Computer::ChoosePos(std::vector<int>* position)
{
	int randomNumber;
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	randomNumber = rand() % position->size();
	
	//ランダムな要素の値を返す
	return position->at(randomNumber);
}

int Reversi::Player::ChoosePos(std::vector<int>* position)
{
	int x = 0;
	int y = 0;
	int inputNumber = boardSize * boardSize;
	bool retry;

	std::string input;

	do {
		retry = false;
		std::cout << "石を置くマスを選んでください。\n";
		std::cout << "盤面の対応するマスに表示されている数値を入力してください。(1～" << position->size() << "):";
		std::getline(std::cin, input);

		if (input == "end" || input == "END" || input == "End")
		{
			end = true;
			break;
		}

		if (input == "auto" || input == "AUTO" || input == "Auto")
		{
			inputAuto = true;
		}

		if (input[0] == '\0' || inputAuto)
		{
			std::srand(static_cast<unsigned int>(std::time(nullptr)));
			inputNumber = rand() % position->size() + 1;
			std::cout << "ランダムで" << inputNumber << "番のマスにおきました\n";
		}
		else if (position->size() < 10)
		{
			if (input.length() > 1)
			{
				std::cout << "入力された数値が大きすぎます。\n";
				retry = true;
			}
			else
			{
				inputNumber = input[0] - '0';
			}
		}
		else
		{
			if (input.length() > 2)
			{
				std::cout << "入力された数値が大きすぎます。\n";
				retry = true;
			}
			else if (input.length() > 1)
			{
				inputNumber = (input[0] - '0') * 10 + input[1] - '0';
			}
			else
			{
				inputNumber = input[0] - '0';
			}
		}

		if (inputNumber > static_cast<int>( position->size()))
		{
			std::cout << "入力された数値が大きすぎます。\n";
			retry = true;
		}

	} while (retry);
	
	if (end)
	{
		return -1;
	}
	else
	{
		return position->at(inputNumber - 1);
	}
}
