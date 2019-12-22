#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_OPCODE_LEN 5
#define SCREEN_WIDTH 42
#define SCREEN_HEIGHT 25

typedef enum BinaryOp
{
	ADD,
	MUL,
	LESS,
	EQUAL
} BinaryOp;

enum parameter_mode
{
	POS_MODE,
	VAL_MODE,
	REL_MODE
};

enum output_state
{
	X,
	Y,
	TILE_ID
};

struct tile
{
	int x;
	int y;
	int tileid;
};

int read_val(int addr, int mode);
void write_val(int addr, int mode, int val);
void binary_op(int param1_mode, int param2_mode, int intparam3_mode, BinaryOp op);
void input_op(int param1_mode, int input);
int output_op(int param1_mode);
void jmp_op(int param1_mode, int param2_mode, int jmp_condition);
void adjust_base(int param1_mode);

int iptr;
int relativebase;
int intcodemem[10000];

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

	char screen[SCREEN_WIDTH][SCREEN_HEIGHT];
	memset(screen, 0, sizeof(screen));

	struct tile output_tile;
	enum output_state curr_state = X;
	int halt = 0;
	while (!halt)
	{
		char instruction[MAX_OPCODE_LEN];
		if (snprintf(instruction, MAX_OPCODE_LEN+1, "%d", intcodemem[iptr]) == -1)
		{
			printf("Error reading opcode to string: %d\n", intcodemem[iptr]);
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
				//no input required for this program
				break;
			case 4:
				{
					int output = output_op(param1_mode);

					switch (curr_state)
					{
						case X:
							output_tile.x = output;
							curr_state = Y;
							break;
						case Y:
							output_tile.y = output;
							curr_state = TILE_ID;
							break;
						case TILE_ID:
							output_tile.tileid = output;
							curr_state = X;
							screen[output_tile.x][output_tile.y] = output_tile.tileid;
							break;
					}

					break;
				}
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

	int blocktiles = 0;
	for (int i = 0; i < SCREEN_HEIGHT; ++i)
		for (int j = 0; j < SCREEN_WIDTH; ++j)
			if (screen[j][i] == 2)
				++blocktiles;

	printf("Block tiles on screen: %d\n", blocktiles);

	return 0;
}

int read_val(int addr, int mode)
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

void write_val(int addr, int mode, int val)
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

int output_op(int param1_mode)
{
	int output = read_val(iptr+1, param1_mode);
	iptr += 2;
	return output;
}

void jmp_op(int param1_mode, int param2_mode, int jmp_condition)
{
	int condition = read_val(iptr+1, param1_mode);

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
