#include <stdio.h>

#define MAX_PROG_SIZE 500

int main()
{	
	int intcode_memory[MAX_PROG_SIZE];
	for (int i = 0; i < MAX_PROG_SIZE; ++i) 
	{
		if (scanf("%d%*[,\n]", &intcode_memory[i]) == EOF)
			break;
	}

	//restore memory to state before intcode machine caught fire
	intcode_memory[1] = 12;
	intcode_memory[2] = 2;

	for (int address = 0; intcode_memory[address] != 99; address += 4)
	{
		switch (intcode_memory[address])
		{
			case 1: 
				intcode_memory[intcode_memory[address + 3]] = 
				intcode_memory[intcode_memory[address + 1]] + 
				intcode_memory[intcode_memory[address + 2]];
				break;
			case 2: 
				intcode_memory[intcode_memory[address + 3]] = 
				intcode_memory[intcode_memory[address + 1]] * 
				intcode_memory[intcode_memory[address + 2]];
				break;
			default: 
				printf("Encountered bad opcode\n");
				return 1;
		}
	}

	printf("Value at address 0 = %d\n", intcode_memory[0]);
	return 0;
}
