#include<iostream>
#include <chrono>
#include <thread>
#include "reversi.h"

using namespace std;

int main()
{
	Reversi reversi;
	//�����true�ɂ���ƃR���s���[�^���m����肠���Ă����;
	const bool debugMode = false;
	reversi.SetInputAuto(debugMode);
	while (true)
	{
		if (reversi.FirstPlayer())
		{
			cout << "\n�v���C���[�̐΂�";
			if (reversi.FirstPlayer())
			{
				cout << "���ł��B\n";
			}
			else
			{
				cout << "�Z�ł��B\n";
			}
			cout << "�΂��u����ꏊ�͔Ֆʏ�̐��l��������Ă���Ƃ���݂̂ł��B\n";
			cout << "auto�Ɠ��͂���ƈȍ~�͎����Ő΂�u���ʒu���I������܂��B\n";
			cout << "end�Ɠ��͂���ƍ~�Q���܂��B\n\n";
			cout << "���Ȃ��̔Ԃł�\n";
			reversi.PrintBoard(Reversi::Stone::White);
			reversi.PutStone(Reversi::Stone::White);
			//�I��肩�ǂ����̔���
			if (reversi.CheckEnd())
				break;

			cout << "�ΐ푊��̔Ԃł�\n";
			reversi.PrintBoard(Reversi::Stone::Black);
			reversi.WaitPutStone(Reversi::Stone::Black);

			if (reversi.CheckEnd())
				break;
		}
		else
		{
			cout << "�ΐ푊��̔Ԃł�\n";
			reversi.PrintBoard(Reversi::Stone::White);
			reversi.WaitPutStone(Reversi::Stone::White);

			if (reversi.CheckEnd())
				break;

			cout << "\n�v���C���[�̐΂�";
			if (reversi.FirstPlayer())
			{
				cout << "���ł��B\n";
			}
			else
			{
				cout << "�Z�ł��B\n";
			}
			cout << "�΂��u����ꏊ�͔Ֆʏ�̐��l��������Ă���Ƃ���݂̂ł��B\n";
			cout << "end�Ɠ��͂���ƍ~�Q���܂��B\n\n";
			cout << "���Ȃ��̔Ԃł�\n";
			reversi.PrintBoard(Reversi::Stone::Black);
			reversi.PutStone(Reversi::Stone::Black);
			//�I��肩�ǂ����̔���
			if (reversi.CheckEnd())
				break;
		}
	}
}