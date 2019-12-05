#include <stdio.h>
#include <bigbrainvector.h>
#include <string.h>

#define MAX_OPCODE_LEN 5
generate_vector(int);

typedef enum BinaryOp
{
	ADD,
	MUL,
	LESS,
	EQUAL
} BinaryOp;

void binary_op(int_vec* intcode_mem, int* instr_ptr, int parm1_mode, int parm2_mode, BinaryOp op);
void input_op(int_vec* intcode_mem, int* instr_ptr);
void output_op(int_vec* intcode_mem, int* instr_ptr, int parm1_mode);
void jmp_op(int_vec* intcode_mem, int* instr_ptr, int parm1_mode, int parm2_mode, int jmp_condition);

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
				binary_op(&intcode_mem, &instr_ptr, parm1_mode, parm2_mode, ADD);
				break;
			case 2:
				binary_op(&intcode_mem, &instr_ptr, parm1_mode, parm2_mode, MUL);
				break;
			case 3:
				input_op(&intcode_mem, &instr_ptr);
				break;
			case 4:
				output_op(&intcode_mem, &instr_ptr, parm1_mode);
				break;
			case 5:
				jmp_op(&intcode_mem, &instr_ptr, parm1_mode, parm2_mode, 1);
				break;
			case 6:
				jmp_op(&intcode_mem, &instr_ptr, parm1_mode, parm2_mode, 0);
				break;
			case 7:
				binary_op(&intcode_mem, &instr_ptr, parm1_mode, parm2_mode, LESS);
				break;
			case 8:
				binary_op(&intcode_mem, &instr_ptr, parm1_mode, parm2_mode, EQUAL);
				break;
			case 99:
				halt = 1;
				break;
		}
	}

	free(intcode_mem.elements);
	return 0;
}

void binary_op(int_vec* intcode_mem, int* instr_ptr, int parm1_mode, int parm2_mode, BinaryOp op)
{	

	int a = parm1_mode ? intcode_mem->elements[*instr_ptr+1] : 
	                     intcode_mem->elements[intcode_mem->elements[*instr_ptr+1]];

	int b = parm2_mode ? intcode_mem->elements[*instr_ptr+2] : 
	                     intcode_mem->elements[intcode_mem->elements[*instr_ptr+2]];

	int result;
	switch (op) 
	{
		case ADD:
		       result = a + b;
		       break;
		case MUL:
		       result = a * b;
		       break;
		case LESS:
		       result = a < b;
		       break;
		case EQUAL:
		       result = a == b;
		       break;
	}	       

	intcode_mem->elements[intcode_mem->elements[*instr_ptr+3]] = result;
	*instr_ptr += 4;
}

void input_op(int_vec* intcode_mem, int* instr_ptr)
{
	int input;
	printf("Input integer: ");
	while (scanf("%d", &input) != 1)
		printf("Invalid input.\n> ");

	intcode_mem->elements[intcode_mem->elements[*instr_ptr+1]] = input;
	*instr_ptr += 2;
}

void output_op(int_vec* intcode_mem, int* instr_ptr, int parm1_mode)
{
	int output = parm1_mode ? intcode_mem->elements[*instr_ptr+1] : 
		                  intcode_mem->elements[intcode_mem->elements[*instr_ptr+1]];
	printf("Output: %d\n", output);
	*instr_ptr += 2;
}

void jmp_op(int_vec* intcode_mem, int* instr_ptr, int parm1_mode, int parm2_mode, int jmp_condition)
{
	int condition = parm1_mode ? intcode_mem->elements[*instr_ptr+1] : 
		                     intcode_mem->elements[intcode_mem->elements[*instr_ptr+1]];

	//a true condition can be any non-zero value
	if (condition)
		condition = 1;

	if (condition == jmp_condition)
	{
		*instr_ptr = parm2_mode ? intcode_mem->elements[*instr_ptr+2] :
			                  intcode_mem->elements[intcode_mem->elements[*instr_ptr+2]];
	}
	else
	{
		*instr_ptr += 3;
	}
}
