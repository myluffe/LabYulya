#include "stdafx.h"
#include "MachineWorker.h"
#include "FileReader.h"
#include "lec.h"

int main(char* name)
{	
	
	
	//»м€ обрабатываемого файла
	name = "test.txt";

	//¬ывод обрабатываемого текста из файла:
	mFileReader* test_reader = new mFileReader(name);
	printf("File: \"%s\"\n", name);
	while (!test_reader->EndFile())
	{	
		char* temp = test_reader->ReadNextLine();
		printf("String Number %d: \"%s\"\n", test_reader->CurrentLine(), temp);	
	}
	printf("\n-------------\n");
	test_reader->~mFileReader();
	//конец вывода

	List* temp_lexes = new List(sizeof(lexeme));
	
	//обработка текста:
	MachineWorker().Work(name, temp_lexes);

	
	//вывод обработанных лексем
	int lcount = temp_lexes->count();
	for (int j = 0; j < lcount; j++)
	{
		lexeme tl = *(lexeme*)temp_lexes->get(j);
		tl.Print();
	}
	//конец вывода

	getc(stdin);
	return 0;
}

