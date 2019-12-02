#include <stdio.h>
#include <string.h>

#define MAX_PROG_SIZE 150
#define DESIRED_OUTPUT 19690720

int main()
{	
	int intcode[MAX_PROG_SIZE];
	memset(intcode, 0, MAX_PROG_SIZE);

	for (int i = 0; i < MAX_PROG_SIZE; ++i) 
	{
		if (scanf("%d%*[,\n]", &intcode[i]) == EOF)
			break;
	}

	int noun = 0;
	int verb = 0;
	int break_flag = 0;

	for (int i = 0; i < 100 && !break_flag; ++i)
	{
		for (int j = 0; j < 100 && !break_flag; ++j)
		{
			int intcode_copy[MAX_PROG_SIZE];
			memcpy(intcode_copy, intcode, sizeof(int[MAX_PROG_SIZE]) );

			intcode_copy[1] = i;
			intcode_copy[2] = j;

			for (int instruct_ptr = 0; intcode_copy[instruct_ptr] != 99; instruct_ptr += 4)
			{
				switch (intcode[instruct_ptr])
				{
					case 1: 
						intcode_copy[intcode_copy[instruct_ptr + 3]] = 
						intcode_copy[intcode_copy[instruct_ptr + 1]] + 
						intcode_copy[intcode_copy[instruct_ptr + 2]];
						break;
					case 2: 
						intcode_copy[intcode_copy[instruct_ptr + 3]] = 
						intcode_copy[intcode_copy[instruct_ptr + 1]] * 
						intcode_copy[intcode_copy[instruct_ptr + 2]];
						break;
					default: 
						printf("Encountered bad opcode\n");
						return 1;
				}
			}

			if (intcode_copy[0] == DESIRED_OUTPUT)
			{
				noun = i;
				verb = j;
				break_flag = 1;
			}
		}
	}


	printf("100 * noun + verb = %d\n", 100 * noun + verb);
	return 0;
}
