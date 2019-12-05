#include <stdio.h>
#include <bigbrainvector.h>
#include <string.h>

#define MAX_OPCODE_LEN 5
generate_vector(int);

typedef enum ArithmeticOp 
{
	ADD,
	MUL
} ArithmeticOp;

void arithmetic_op(int_vec* intcode_mem, int instr_ptr, int parm1_mode, int parm2_mode, ArithmeticOp op);
void input_op(int_vec* intcode_mem, int instr_ptr);
void output_op(int_vec* intcode_mem, int instr_ptr, int parm1_mode);

int main() 
{
	FILE* intcode_mem_file = fopen("input.txt", "r");
	if (!intcode_mem_file)
	{
		printf("Failed to open input.txt\n");
		exit(EXIT_FAILURE);
	}

	int_vec intcode_mem = build_int_vec();

	int mem_value;
	while (fscanf(intcode_mem_file, "%d%*[,\n]", &mem_value) != EOF)
	{
		append_int_vec(&intcode_mem, mem_value);
	}

	fclose(intcode_mem_file);

	int halt = 0;
	int instr_ptr = 0;
	while (!halt)
	{
		char instruction[MAX_OPCODE_LEN];
		if (snprintf(instruction, MAX_OPCODE_LEN, "%d", intcode_mem.elements[instr_ptr]) == -1)
		{
			printf("Error reading opcode to string: %d\n", intcode_mem.elements[instr_ptr]);
			exit(EXIT_FAILURE);
		}

		int opcode;
		int parm1_mode = 0;
		int parm2_mode = 0;
		switch (strlen(instruction))
		{
			case 1:
			case 2:
				sscanf(instruction, "%d", &opcode);
				break;
			case 3:
				sscanf(&instruction[1], "%d", &opcode);
				parm1_mode = 1;
				break;
			case 4:
				sscanf(&instruction[2], "%d", &opcode);
				parm1_mode = instruction[1] - '0';
				parm2_mode = instruction[0] - '0';
				break;
			default:
				printf("Unknown instruction: %s\n", instruction);
				exit(EXIT_FAILURE);
		}

		int params;
		switch (opcode)
		{
			case 1:
				arithmetic_op(&intcode_mem, instr_ptr, parm1_mode, parm2_mode, ADD);
				instr_ptr += 4;
				break;
			case 2:
				arithmetic_op(&intcode_mem, instr_ptr, parm1_mode, parm2_mode, MUL);
				instr_ptr += 4;
				break;
			case 3:
				input_op(&intcode_mem, instr_ptr);
				instr_ptr += 2;
				break;
			case 4:
				output_op(&intcode_mem, instr_ptr, parm1_mode);
				instr_ptr += 2;
				break;
			case 99:
				halt = 1;
				break;
		}
	}

	free(intcode_mem.elements);
	return 0;
}

void arithmetic_op(int_vec* intcode_mem, int instr_ptr, int parm1_mode, int parm2_mode, ArithmeticOp op) {
	int val1;
	int val2;
	int result;

	val1 = parm1_mode ? intcode_mem->elements[instr_ptr+1] : 
	                    intcode_mem->elements[intcode_mem->elements[instr_ptr+1]];

	val2 = parm2_mode ? intcode_mem->elements[instr_ptr+2] : 
	                    intcode_mem->elements[intcode_mem->elements[instr_ptr+2]];

	switch (op) 
	{
		case ADD:
		       result = val1+val2;
		       break;
		case MUL:
		       result = val1*val2;
		       break;
	}	       

	intcode_mem->elements[intcode_mem->elements[instr_ptr+3]] = result;
}

void input_op(int_vec* intcode_mem, int instr_ptr)
{
	int input;
	printf("Input integer: ");
	while (scanf("%d", &input) != 1)
		printf("Invalid input.\n> ");

	intcode_mem->elements[intcode_mem->elements[instr_ptr+1]] = input;
}

void output_op(int_vec* intcode_mem, int instr_ptr, int parm1_mode)
{
	int output = parm1_mode ? intcode_mem->elements[instr_ptr+1] : 
		                  intcode_mem->elements[intcode_mem->elements[instr_ptr+1]];
	printf("Output: %d\n", output);
}
