#include "stdafx.h"
#include "MachineWorker.h"
#include "FileReader.h"
#include "lec.h"
#include <locale.h>
#include "Parser.h"

int main(char* name)
{	
	setlocale(LC_CTYPE, "");

	//��� ��������������� �����
	name = "test.txt";

	//����� ��������������� ������ �� �����:
	mFileReader* test_reader = new mFileReader(name);
	printf("File: \"%s\"\n", name);
	while (!test_reader->EndFile())
	{	
		char* temp = test_reader->ReadNextLine();
		printf("String Number %d: \"%s\"\n", test_reader->CurrentLine(), temp);	
	}
	//printf_s("\n|---------------|\n");
	test_reader->~mFileReader();
	//����� ������

	List* temp_lexes = new List(sizeof(lexeme));
	
	//��������� ������:
	MachineWorker().Work(name, temp_lexes);

	
	//����� ������������ ������
	int lcount = temp_lexes->count();
	for (int j = 0; j < lcount; j++)
	{
		lexeme tl = *(lexeme*)temp_lexes->get(j);
		printf("%d. ", j);
		tl.Print();
	}
	//����� ������

	getc(stdin);
	return 0;
}

