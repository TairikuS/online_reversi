#ifndef REVERSI_H
#define REVERSI_H
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<iostream>
#include <vector>
#include<WinSock2.h>
#include<string>

class Reversi
{
public:
	enum class Stone
	{
		None, Black, White,
	};

	enum class PlayerState
	{
		server,client,
	};

	class Player
	{
		bool inputAuto = false;
		bool end= false;
	public :
		bool End(){ return end; };
		int ChoosePos(std::vector<int>*);
		bool InputAuto() { return inputAuto; }
	};

	class Computer
	{
	public :
		int ChoosePos(std::vector<int>*);
	};

private:
	
	PlayerState state;
	WSADATA wsaData;
	int result;
	char serverIP[40];
	HOSTENT* lpHost;
	SOCKET listenSoc, soc;
	SOCKADDR_IN saddr, from;
	int fromlen, rcv;
	char buffer[1000];
	unsigned int addr;
	const unsigned short myPortNumber = 55555;
	unsigned short portNum = myPortNumber;
	bool firstPlayer;

	static const int boardSize = 8;
	const int colMax = boardSize;
	const int rowMax = boardSize;
	Stone stones[boardSize][boardSize] = {};

	bool inputAuto = false;

	int whiteStoneCount;
	int blackStoneCount;
	int noneSquareCount;

	std::vector<int> whiteCanPutPos;
	std::vector<int> blackCanPutPos;

	bool end = false;

public:
	Reversi();
	bool FirstPlayer()  const { return firstPlayer; }
	void PrintBoard(Stone nextPutStoneColor);
	bool PutStone(Stone stoneColor);
	bool CheckLine(int x, int y, int directionX, int directionY, Stone checkStoneColor, bool turnOverFlag = true);
	bool CheckAllLine(int x, int y, Stone checkStoneColor, bool turnOverFlag = true);
	void CheckPutPos(std::vector<int>* canPutPos, Stone stoneColor);
	bool CheckEnd();
	void WaitPutStone(Stone stoneColor);
	void SetInputAuto(bool flag) { inputAuto = flag; }
};

#endif