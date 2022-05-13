#include<ctime>
#include"reversi.h"

Reversi::Reversi()
{
	std::string input;
	bool retry = false;
	unsigned int inputNumber = 0, pow;

	do {
		retry = false;

		std::cout << "���Ȃ��̓T�[�o�[���ł����H�N���C�A���g���ł����H\n";
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
			std::cout << "����ɓǂݎ��܂���ł����B0��������1�œ��͂��Ă��������B\n";
			retry = true;
		}
	} while (retry);

	if (state == PlayerState::server)
	{
		result = WSAStartup(MAKEWORD(1, 1), &wsaData);
		if (result) {
			std::cout << "WSAStartup�̎��s�B\n";
			//���s
		}

		listenSoc = socket(AF_INET, SOCK_STREAM, 0);
		if (listenSoc < 0) {
			std::cout << "�\�P�b�g�I�[�v���G���[\n";
			WSACleanup();
			//���s
		}

		do {
			retry = false;
			std::cout << "�|�[�g�ԍ�(49152�`65535)�F";
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
					std::cout << "���̓G���[�ł��B49152�`65535�̒l����͂��Ă��������B\n";
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
						std::cout << "���̓G���[�ł��B49152�`65535�̒l����͂��Ă��������B\n";
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
			std::cout << "bind�̃G���[\n";
			closesocket(listenSoc);
			WSACleanup();
			//���s
		}

		if (listen(listenSoc, 0) == SOCKET_ERROR) {
			std::cout << "listen error.\n";
			closesocket(listenSoc);
			WSACleanup();
			//���s
		}
		std::cout << "�ΐ푊���҂��Ă��܂��B���΂炭���҂����������B\n";

		fromlen = sizeof(from);
		soc = accept(listenSoc, (SOCKADDR*)&from, &fromlen);

		if (soc == INVALID_SOCKET) {
			std::cout << "accept error.\n";
			closesocket(listenSoc);
			WSACleanup();
			//���s
		}
		else
			std::cout << inet_ntoa(from.sin_addr) << "���ڑ����Ă��܂����B\n";

		closesocket(listenSoc);

		do {
			retry = false;
			std::cout << "�ǂ��炪��s�v���C���[�ɂȂ�܂����H�����͂���Ȃ������ꍇ�����_���Ō��肳��܂�(server -> 0,client -> 1)�F";
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
					std::cout << "���Ȃ��͐�U�ł��B\n";
				}
				else
				{
					firstPlayer = false;
					buffer[0] = '1';
					send(soc, buffer, int(strlen(buffer)), 0);
					std::cout << "���Ȃ��͌�U�ł��B\n";
				}
			}
			else if (input == "server" || input == "0")
			{
				firstPlayer = true;
				buffer[0] = '0';
				send(soc, buffer, int(strlen(buffer)), 0);
				std::cout << "���Ȃ��͐�U�ł��B\n";
			}
			else if (input == "client" || input == "1")
			{
				firstPlayer = false;
				buffer[0] = '1';
				send(soc, buffer, int(strlen(buffer)), 0);
				std::cout << "���Ȃ��͌�U�ł��B\n";
			}
			else
			{
				std::cout << "����ɓǂݎ��܂���ł����B0��������1�œ��͂��Ă��������B\n";
				retry = true;
			}
		} while (retry);
	}
	else
	{
		do {
			retry = false;
			std::cout << "�|�[�g�ԍ�(49152�`65535)�F";
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
					std::cout << "���̓G���[�ł��B49152�`65535�̒l����͂��Ă��������B\n";
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
						std::cout << "���̓G���[�ł��B49152�`65535�̒l����͂��Ă��������B\n";
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
		
		//buffer�̒��g�����ꂢ�ɂ���
		for (int i = 0; i < 5; i++)
		{
			buffer[i] = '\0';
		}
		std::cout << "�T�[�o�[��IP�A�h���X�F";
		std::getline(std::cin, input);

		//enter���͂̎��͎������M�������A�h���X�������œ���
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
			std::cout << "WSAStartup�̎��s�B\n";
			//���s
		}

		soc = socket(AF_INET, SOCK_STREAM, 0);
		if (soc < 0) {
			std::cout << "�\�P�b�g�I�[�v���G���[\n";
			WSACleanup();
			//���s
		}

		lpHost = gethostbyname(serverIP);
		if (lpHost == NULL) {
			addr = inet_addr(serverIP);
			lpHost = gethostbyaddr((char*)&addr, 4, AF_INET);
		}
		if (lpHost == NULL) {
			std::cout << "gethostbyaddr�̃G���[\n";
			closesocket(soc);
			WSACleanup();
			//���s
		}

		ZeroMemory(&saddr, sizeof(SOCKADDR_IN));

		if (lpHost != NULL)
		{
			saddr.sin_family = lpHost->h_addrtype;
			saddr.sin_port = htons(portNum);
			saddr.sin_addr.s_addr = *((u_long*)lpHost->h_addr);
		}

		if (connect(soc, (SOCKADDR*)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
			std::cout << "connect�̃G���[\n";
			closesocket(soc);
			WSACleanup();
			//���s
		}
		else
			std::cout << "�ڑ������B����server���v���C�������߂Ă��܂��B\n";

		rcv = recv(soc, buffer, sizeof(buffer) - 1, 0);
		if (rcv == SOCKET_ERROR)
		{
			std::cout << "�G���[�ł��B\n";
		}
		if (buffer[0] == '0')
		{
			std::cout << "���Ȃ��͌�U�ł��B\n";
			firstPlayer = false;
		}
		else
		{
			std::cout << "���Ȃ��͐�U�ł��B\n";
			firstPlayer = true;
		}
	}


	//�ꉞ�S��None�ŏ�����
	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; j++)
		{
			stones[i][j] = Stone::None;
		}
	}

	//�I�Z���̏����z�u
	stones[boardSize / 2 - 1][boardSize / 2 - 1] = Stone::White;
	stones[boardSize / 2][boardSize / 2] = Stone::White;

	stones[boardSize / 2 - 1][boardSize / 2] = Stone::Black;
	stones[boardSize / 2][boardSize / 2 - 1] = Stone::Black;

	//�΂̐��Ȃǂ̏�����
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
			std::cout << "   ��" << '\n';

		}

		//�`�𐮂��邽�߂̋�
		std::cout << "  ";

		//��̊Ԃ̐���\������
		for (int j = 0; j < boardSize; j++)
		{
			std::cout << "+---";
		}
		std::cout << '+' << '\n';

		//�c�̃}�X���̕\��
		std::cout << ' ' << i + 1;

		//�}�X�ɂǂ̐΂��u����Ă���̂���\��
		for (int j = 0; j < boardSize; j++)
		{
			if (stones[j][i] == Stone::White)
			{
				std::cout << "| ��";
			}
			else if (stones[j][i] == Stone::Black)
			{
				std::cout << "| �Z";
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
	//�{�[�h�̈�ԉ�������
	std::cout << "  ";
	for (int j = 0; j < boardSize; j++)
	{
		std::cout << "+---";
	}
	std::cout << '+' << '\n' << "�c\n";
	std::cout << "[�Z�~" <<  Reversi::blackStoneCount<< "��]�@[���~" << Reversi::whiteStoneCount << "��]\n\n";
}

bool Reversi::PutStone(Stone stoneColor)
{
	bool canPutStone = true;
	std::vector<int>* canPutPos;
	int putPos;

	//�n���ꂽ�F�ɂ���Ďg��vector��ς���
	if (stoneColor == Stone::Black)
	{
		canPutPos = &blackCanPutPos;
	}
	else
	{
		canPutPos = &whiteCanPutPos;
	}

	CheckPutPos(canPutPos, stoneColor);

	//�΂��u���Ȃ��ꍇ�u������̓X�L�b�v
	if (canPutPos->size() == 0)
	{
		std::cout << "�΂�u����ꏊ������܂���ł���\n";
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
				std::cout << "���łɐ΂��u����Ă��܂�!\n";
				canPutStone = false;
			}
			else if (!CheckAllLine(x, y, stoneColor, false))
			{
				std::cout << "�Ђ�����Ԃ���΂�����܂���!\n";
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
	//�Ђ�����Ԃ��邩�ǂ����̃t���O
	bool canStoneTurnOver = false;
	int turnOverCount = 0;

	for (int i = 1; i <= boardSize; i++)
	{
		//�z��͈̔͊O�ɃA�N�Z�X���Ȃ��悤��
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

	//���ꂼ��̌������ƂɂЂ�����Ԃ��΂𒲂ׂĂ���
	//��[�[]
	canStoneTurnOver = CheckLine(x, y, 1, 0, checkStoneColor, turnOverFlag);
	if (canStoneTurnOver)
	{
		CheckLine(x, y, -1, 0, checkStoneColor, turnOverFlag);
	}
	else
	{
		canStoneTurnOver = CheckLine(x, y, -1, 0, checkStoneColor, turnOverFlag);
	}
	
	//�c[|]
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

	//�΂�[/]
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
	
	//�΂�[�_]
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
	//����u����ꏊ�����\�ς��̂ŏ��߂ɋ�ɂ���
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
		std::cout << "���̌��F" << whiteStoneCount << " �Z�̌��F" << blackStoneCount << '\n';
		if (whiteStoneCount > blackStoneCount)
		{
			if (firstPlayer)
			{
				std::cout << "���Ȃ��̏����I\n";
			
			}
			else
			{
				std::cout << "�ΐ푊��̏����I\n";
			}
			
		}
		else if (blackStoneCount > whiteStoneCount)
		{
			if (firstPlayer)
			{
				std::cout << "�ΐ푊��̏����I\n";
			}
			else
			{
				std::cout << "���Ȃ��̏����I\n";
			}
		}
		else
		{
			std::cout << "���������I\n";
		}
		end = true;
	}

	if (blackStoneCount == 0)
	{
		PrintBoard(Stone::None);
		std::cout << "gameend!\n"; 
		std::cout << "���̌��F" << whiteStoneCount << " �Z�̌��F" << blackStoneCount << '\n';
		if (firstPlayer)
		{
			std::cout << "�������I���Ȃ��̈�����!\n";
		}
		else
		{
			std::cout << "�ΐ푊��̈���\n";
		}
		end = true;
	}

	if (whiteStoneCount == 0)
	{
		PrintBoard(Stone::None);
		std::cout << "gameend!\n";
		std::cout << "���̌��F" << whiteStoneCount << " �Z�̌��F" << blackStoneCount << '\n';
		if (firstPlayer)
		{
			std::cout << "�ΐ푊��̈���\n";
		}
		else
		{
			std::cout << "�������I���Ȃ��̈�����!\n";
		}
		end = true;
	}

	if (end)
	{
		shutdown(soc, SD_BOTH);
		closesocket(soc);

		result = WSACleanup();
		if (result) {
			std::cout << "WSACleanup�̎��s�B\n";
			//���s
		}
	}
	return end;
}

void Reversi::WaitPutStone(Stone stoneColor)
{
	int putNum = -1;
	int x, y;
	std::vector<int>* canPutPos;

	//�n���ꂽ�F�ɂ���Ďg��vector��ς���
	if (stoneColor == Stone::Black)
	{
		canPutPos = &blackCanPutPos;
	}
	else
	{
		canPutPos = &whiteCanPutPos;
	}

	CheckPutPos(canPutPos, stoneColor);

	//�΂��u���Ȃ��ꍇ�u������̓X�L�b�v
	if (canPutPos->size() == 0)
	{
		std::cout << "�΂�u����ꏊ������܂���ł���\n";
	}
	else
	{
		std::cout << "�ΐ푊��̓��͂�҂��Ă��܂��B\n";
		rcv = recv(soc, buffer, sizeof(buffer) - 1, 0);
		if (rcv == SOCKET_ERROR) {
			std::cout << "�G���[�ł��B\n";
		}

		if (rcv < sizeof(buffer))
			buffer[rcv] = '\0';


		if (buffer[0] == 'e')
		{
			std::cout << "�ΐ푊�肪�~�Q���܂����B\n";
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
				std::cout << "�G���[���������܂����B\n";
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
	
	//�����_���ȗv�f�̒l��Ԃ�
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
		std::cout << "�΂�u���}�X��I��ł��������B\n";
		std::cout << "�Ֆʂ̑Ή�����}�X�ɕ\������Ă��鐔�l����͂��Ă��������B(1�`" << position->size() << "):";
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
			std::cout << "�����_����" << inputNumber << "�Ԃ̃}�X�ɂ����܂���\n";
		}
		else if (position->size() < 10)
		{
			if (input.length() > 1)
			{
				std::cout << "���͂��ꂽ���l���傫�����܂��B\n";
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
				std::cout << "���͂��ꂽ���l���傫�����܂��B\n";
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
			std::cout << "���͂��ꂽ���l���傫�����܂��B\n";
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
