#include "stdafx.h"
#include "MachineWorker.h"
#include "FileReader.h"

int main(char* name)
{
	name = "test.txt";
	mFileReader* test_reader = new mFileReader(name);
	printf("File: \"%s\"\n", name);
	while (!test_reader->EndFile())
	{	
		char* temp = test_reader->ReadNextLine();
		//temp[strlen(temp) - 1] = '\0';
		printf("String Number %d: \" ", test_reader->CurrentLine());
		for (int i = 0; i < strlen(temp); i++)
		{
			printf("\'%c\'", temp[i]);
		}
		printf(" \"\n");
	}
	printf("-----------\n");
	lexeme();
	MachineWorker().Work("test.txt");
	return 0;
}

