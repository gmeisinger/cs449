// George Meisinger
// gem56
#include <stdio.h>
#include <ctype.h>

int main(int argc, char** argv)
{
	unsigned char buffer[5];
	int counter = 0;
	int tempByte = 0;

	FILE* f = fopen(argv[1], "rb");
	//check if file exists
	if(f == NULL)
	{
		printf("File does not exist.\n");
		printf("argv[1] = '%s'\n", argv[1]);
		return 1;
	}

	//read file loop
	while(!feof(f))
	{
		//read in a byte
		//fread(&tempByte, sizeof(char), 1, f);
		tempByte = getc(f);
		if(isprint(tempByte))
		{
			buffer[counter] = (unsigned char)tempByte;
			counter++;
			//buffer[counter] = NULL;
		}
		else
		{
			counter = 0;
		}
		if(counter > 3)
		{
			printf("%s", buffer);
			//fread(&tempByte, sizeof(char), 1, f);
			tempByte = getc(f);
			while(isprint(tempByte))
			{
				printf("%c", (unsigned char)tempByte);
				//fread(&tempByte, sizeof(char), 1, f);
				tempByte = getc(f);
			}
			printf("\n");
			counter = 0;
			//buffer[0] = NULL;
		}
	}

	fclose(f);
}