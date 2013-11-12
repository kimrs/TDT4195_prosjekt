// ReadFile.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>

#include "ReadFile.h"

int _tmain(int argc, _TCHAR* argv[])
{	
	/*		--- CODE COPIED FROM ---
	 * http://support.microsoft.com/kb/99115
	 */

	hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	if(!GetConsoleScreenBufferInfo(hStdOutput, &csbi))
	{
		printf("GetConsoleScreenBufferInfo failed: %d\n", GetLastError());
		return 1;
	}

    bUsePause = ((!csbi.dwCursorPosition.X) &&
                (!csbi.dwCursorPosition.Y));
	
	_TCHAR * filename = argv[1];
	char file_name[100];

	wcstombs(file_name, filename, wcslen(argv[1]) + 1);
	printf("%s\n", file_name);

	int *params = read(file_name);

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
			printf("%d", params[i*4 + j]);

		printf("\n");

	}

	if (bUsePause)
    {
        int ch;
        printf("\n\tPress any key to exit...\n");
        ch = getch();
    }
	return 0;
}


int *read(char* filename)
{
	int n;
	long elapsed_seconds;
	char line[80];

	//int parameters[400]; //100 units per session allowed. 4 parameters per unit. 
	int parameterCounter = 0;
	int data[400];

	fr = fopen (filename, "rt");  /* open the file for reading */
	/* elapsed.dta is the name of the file */
	/* "rt" means open the file for reading text */

	while(fgets(line, 80, fr) != NULL)
	{
		/* get a line, up to 80 chars from fr.  done if NULL */
		sscanf (line, "%ld", &elapsed_seconds);
		/* convert the string to a long int */
		//printf ("%ld\n", elapsed_seconds);
		//printf("%s\n", line);

		if(line[0] == 35) //if '#'
		{
			printf("comment\n");
		} else
		{
			int color = line[0] - '0';
			int scale = line[1] - '0';
			int x = (line[2]-'0') * 1000 + 
					(line[3]-'0') * 100 + 
					(line[4]-'0') * 10 + 
					(line[5]-'0');
			int y = (line[6]-'0') * 1000 + 
					(line[7]-'0') * 100 + 
					(line[8]-'0') * 10 + 
					(line[9]-'0');

			data[0 + parameterCounter] = color;
			data[1 + parameterCounter] = scale;
			data[2 + parameterCounter] = x;
			data[3 + parameterCounter] = y;

			parameterCounter += 4;
		}
   }
   fclose(fr);  /* close the file prior to exiting the routine */
   return data;
}
