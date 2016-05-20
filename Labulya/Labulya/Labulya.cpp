#include "stdafx.h"
#include "MachineWorker.h"
#include "FileReader.h"
#include "lec.h"

int main(char* name)
{
	name = "test.txt";
	mFileReader* test_reader = new mFileReader(name);
	printf("File: \"%s\"\n", name);
	while (!test_reader->EndFile())
	{	
		char* temp = test_reader->ReadNextLine();
		printf("String Number %d: \"%s\"\n", test_reader->CurrentLine(), temp);	
	}
	printf("|-----------------|\n");

	List* temp_lexes = new List(sizeof(lexeme));
	MachineWorker().Work("test.txt", temp_lexes);
	
	int lcount = temp_lexes->count();
	for (int j = 0; j < lcount; j++)
	{
		lexeme* temp = (lexeme*)temp_lexes->get(j);
		temp->print();
	}
	return 0;
}

