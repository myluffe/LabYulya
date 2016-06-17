#pragma once

#include "string.h"

class lexeme
{
private:
	char* _name; //�������� �������. ��� ��� ����������
	char* _data; //�������� �������, ��������� �� �����. � ������ ���������� ���������� �� � ��������.
	int _type; //��� �������. � ������ ���������� ��� ��������� ������� ���������� (Number ��� Variable) ����� �� ���� ��������.
	int _line; //����� ������, �� ������� ��������� ������� � �������������� �����.
	int _startposition; //������� ������� ������������ ������ ������.
	int _priority; //���������. �� ��������� 100, �����, ���� ������� ���. ���������, � ����. ������� ������������ � ��������� �� �������: http://cppstudio.com/post/302/
	//������ "(" � ")" ����� ��������� 0.
	int _rank = 0; //������� ������� (���� �� ������, �� 0).
public: 
	lexeme(char* name, char* type, char* data, int line, int startposition, int priority);
	lexeme(char* name, int type, char* data, int line, int startposition, int priority);
	lexeme mass(lexeme *name, int type, List values, int rank);
	~lexeme();

	//��������� ����� �������:
	char* Data() { return _data; }
	char* Name() { return _name; }
	int Type() { return _type; }
	int Line() { return _line; }
	int Start_Position() { return _startposition; }
	int Priority() { return _priority; }

	bool GetNumber(double* value); //�������� �������� ������� (���� Data). � ������������ �������� ��������� � ������ � ������� Parser
	bool SetNumber(double value); //����� �������� ������� (���� Data).

	void DataChange(char* newdata); //����� �������� ������� (���� Data).
	void Print();

	//���� ��� ��������
	List* valuse = nullptr;
	int Rank() { return _rank; }
};