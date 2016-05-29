#include "stdafx.h"
#include "MachineWorker.h"
#include "FileReader.h"
#include "lec.h"
#include "hash.h"

int main(char* name)
{
	Diction* dtest = new Diction();
	dtest->auto_create("ine");
	dtest->auto_create("zwine");
	dtest->auto_create("dryine");

	/*
	name = "test.txt";
	mFileReader* test_reader = new mFileReader(name);
	printf("File: \"%s\"\n", name);
	while (!test_reader->EndFile())
	{	
		char* temp = test_reader->ReadNextLine();
		printf("String Number %d: \"%s\"\n", test_reader->CurrentLine(), temp);	
	}
	printf("\n-------------\n");

	List* temp_lexes = new List(sizeof(lexeme));
	MachineWorker().Work("test.txt", temp_lexes);
	
	int lcount = temp_lexes->count();
	for (int j = 0; j < lcount; j++)
	{
		lexeme tl = *(lexeme*)temp_lexes->get(j);
		tl.Print();
	}
	*/

	getc(stdin);
	return 0;
}

