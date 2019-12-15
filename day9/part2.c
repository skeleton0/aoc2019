#include <stdio.h>
#include <bigbrainvector.h>
#include <string.h>

#define MAX_OPCODE_LEN 5
#define FIVE_FACTORIAL 120
#define INPUT 2
generate_vector(int);

typedef enum BinaryOp
{
	ADD,
	MUL,
	LESS,
	EQUAL
} BinaryOp;

typedef struct IntcodeAmp
{
	int_vec memory;
	int iptr;
	int phase_setting_is_set;
	int waiting_for_signal;
} IntcodeAmp;

int64_t read_val(int addr, int mode);
void write_val(int addr, int mode, int64_t val);
void binary_op(int param1_mode, int param2_mode, int intparam3_mode, BinaryOp op);
void input_op(int param1_mode, int input);
int64_t output_op(int param1_mode);
void jmp_op(int param1_mode, int param2_mode, int jmp_condition);
void adjust_base(int param1_mode);

void generate_permutations(char set[], char full_set[], int n, char permutations[]);

enum
{
	POS_MODE,
	VAL_MODE,
	REL_MODE
};

int iptr;
int relativebase;
int64_t intcodemem[10000];

int main() 
{
	FILE* intcodemem_file = fopen("input.txt", "r");
	if (!intcodemem_file)
	{
		printf("Failed to open input.txt\n");
		exit(EXIT_FAILURE);
	}


	int64_t mem_value;
	int idx = 0;
	while (fscanf(intcodemem_file, "%ld,", &mem_value) != EOF)
	{
		intcodemem[idx] = mem_value;
		++idx;
	}

	fclose(intcodemem_file);

	int64_t output = 0;
	int halt = 0;
	while (!halt)
	{
		char instruction[MAX_OPCODE_LEN];
		if (snprintf(instruction, MAX_OPCODE_LEN+1, "%ld", intcodemem[iptr]) == -1)
		{
			printf("Error reading opcode to string: %ld\n", intcodemem[iptr]);
			exit(EXIT_FAILURE);
		}

		int opcode;
		int param1_mode = POS_MODE;
		int param2_mode = POS_MODE;
		int param3_mode = POS_MODE;
		switch (strlen(instruction))
		{
			case 1:
			case 2:
				sscanf(instruction, "%d", &opcode);
				break;
			case 3:
				sscanf(&instruction[1], "%d", &opcode);
				param1_mode = instruction[0] - '0';
				break;
			case 4:
				sscanf(&instruction[2], "%d", &opcode);
				param1_mode = instruction[1] - '0';
				param2_mode = instruction[0] - '0';
				break;
			case 5:
				sscanf(&instruction[3], "%d", &opcode);
				param1_mode = instruction[2] - '0';
				param2_mode = instruction[1] - '0';
				param3_mode = instruction[0] - '0';
				break;
			default:
				printf("Unknown instruction: %s\n", instruction);
				exit(EXIT_FAILURE);
		}

		switch (opcode)
		{
			case 1:
				binary_op(param1_mode, param2_mode, param3_mode, ADD);
				break;
			case 2:
				binary_op(param1_mode, param2_mode, param3_mode, MUL);
				break;
			case 3:
				input_op(param1_mode, INPUT);
				break;
			case 4:
				output = output_op(param1_mode);
				break;
			case 5:
				jmp_op(param1_mode, param2_mode, 1);
				break;
			case 6:
				jmp_op(param1_mode, param2_mode, 0);
				break;
			case 7:
				binary_op(param1_mode, param2_mode, param3_mode, LESS);
				break;
			case 8:
				binary_op(param1_mode, param2_mode, param3_mode, EQUAL);
				break;
			case 9:
				adjust_base(param1_mode);
				break;
			case 99:
				halt = 1;
				break;
		}
	}

	printf("Output: %ld\n", output);
	return 0;
}

int64_t read_val(int addr, int mode)
{
	switch (mode)
	{
		case POS_MODE:
			return intcodemem[intcodemem[addr]];
		case VAL_MODE:
			return intcodemem[addr];
		case REL_MODE:
			return intcodemem[relativebase + intcodemem[addr]];
		default:
			printf("Unknown parameter mode\n");
			exit(1);
	}
}

void write_val(int addr, int mode, int64_t val)
{
	switch (mode)
	{
		case POS_MODE:
			intcodemem[intcodemem[addr]] = val;
			break;
		case VAL_MODE:
			printf("Tried to write value to value mode parameter\n");
			exit(1);
		case REL_MODE:
			intcodemem[relativebase + intcodemem[addr]] = val;
			break;
		default:
			printf("Unknown parameter mode\n");
			exit(1);
	}
}

void binary_op(int param1_mode, int param2_mode, int param3_mode, BinaryOp op)
{	
	int64_t a = read_val(iptr+1, param1_mode);
	int64_t b = read_val(iptr+2, param2_mode);

	int64_t result;
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

	write_val(iptr+3, param3_mode, result);
	iptr += 4;
}

void input_op(int param1_mode, int input)
{
	write_val(iptr+1, param1_mode, input);
	iptr += 2;
}

int64_t output_op(int param1_mode)
{
	int64_t output = read_val(iptr+1, param1_mode);
	iptr += 2;
	return output;
}

void jmp_op(int param1_mode, int param2_mode, int jmp_condition)
{
	int64_t condition = read_val(iptr+1, param1_mode);

	//a true condition can be any non-zero value
	if (condition)
		condition = 1;

	if (condition == jmp_condition)
		iptr = read_val(iptr+2, param2_mode);
	else
		iptr += 3;
}

void adjust_base(int param1_mode)
{
	relativebase += read_val(iptr+1, param1_mode);
	iptr += 2;
}

void generate_permutations(char set[], char full_set[], int n, char permutations[])
{
        if (n == 1)
        {
                strcat(permutations, full_set);
                return;
        }

        for (int i = 0; i < n; ++i)
        {
                char tmp = *set;
                *set = set[i];
                set[i] = tmp;

                generate_permutations(&set[1], full_set, n-1, permutations);

                //return swap to previous state
                tmp = *set;
                *set = set[i];
                set[i] = tmp;
        }
}
