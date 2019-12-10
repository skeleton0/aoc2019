#include <stdio.h>
#include <bigbrainvector.h>
#include <string.h>

#define MAX_OPCODE_LEN 5
#define FIVE_FACTORIAL 120
generate_vector(int);

typedef enum BinaryOp
{
	ADD,
	MUL,
	LESS,
	EQUAL
} BinaryOp;

void binary_op(int_vec* intcode_mem, int* instr_ptr, int parm1_mode, int parm2_mode, BinaryOp op);
void input_op(int_vec* intcode_mem, int* instr_ptr, int input);
int output_op(int_vec* intcode_mem, int* instr_ptr, int parm1_mode);
void jmp_op(int_vec* intcode_mem, int* instr_ptr, int parm1_mode, int parm2_mode, int jmp_condition);

void generate_permutations(char set[], char full_set[], int n, char permutations[]);

int main() 
{
	FILE* intcode_mem_file = fopen("input.txt", "r");
	if (!intcode_mem_file)
	{
		printf("Failed to open input.txt\n");
		exit(EXIT_FAILURE);
	}

	int_vec intcode_mem_initial = build_int_vec();

	int mem_value;
	while (fscanf(intcode_mem_file, "%d%*[,\n]", &mem_value) != EOF)
	{
		append_int_vec(&intcode_mem_initial, mem_value);
	}

	fclose(intcode_mem_file);

	//create copy of intcode memory for use
        int_vec intcode_mem = build_int_vec();
        for (int i = 0; i < intcode_mem_initial.size; ++i)
        {
                append_int_vec(&intcode_mem, intcode_mem_initial.data[i]);
        }

        char set[] = {'0', '1', '2', '3', '4', '\0'};
        char permutations[FIVE_FACTORIAL * 5 + 1] = "";
        generate_permutations(set, set, 5, permutations);

        int largest_thruster_signal = 0;

        for (int i = 0; i < FIVE_FACTORIAL; ++i)
        {
		int input = 0;
		int prev_amp_output = 0;

		//run program 5 times for each amp
		for (int j = 0; j < 5; ++j)
		{
			int halt = 0;
			int instr_ptr = 0;
			int phase_setting_set = 0;

			while (!halt)
			{
				char instruction[MAX_OPCODE_LEN];
				if (snprintf(instruction, MAX_OPCODE_LEN, "%d", intcode_mem.data[instr_ptr]) == -1)
				{
					printf("Error reading opcode to string: %d\n", intcode_mem.data[instr_ptr]);
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
						if (phase_setting_set)
						{
							input_op(&intcode_mem, &instr_ptr, prev_amp_output);
						}
						else
						{
							input_op(&intcode_mem, &instr_ptr, permutations[i * 5 + input] - '0');
							++input;
						}

						phase_setting_set = !phase_setting_set;
						break;
					case 4:
						prev_amp_output = output_op(&intcode_mem, &instr_ptr, parm1_mode);
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
			
			//return intcode memory to initial state
			for (int k = 0; k < intcode_mem.size; ++k)
			{
				intcode_mem.data[k] = intcode_mem_initial.data[k];
			}
		}

			if (prev_amp_output > largest_thruster_signal)
				largest_thruster_signal = prev_amp_output;

			printf("prev_amp_output = %d\n", prev_amp_output);
	}

	printf("Largest thruster signal = %d\n", largest_thruster_signal);

	free(intcode_mem.data);
	free(intcode_mem_initial.data);
	return 0;
}

void binary_op(int_vec* intcode_mem, int* instr_ptr, int parm1_mode, int parm2_mode, BinaryOp op)
{	

	int a = parm1_mode ? intcode_mem->data[*instr_ptr+1] : 
	                     intcode_mem->data[intcode_mem->data[*instr_ptr+1]];

	int b = parm2_mode ? intcode_mem->data[*instr_ptr+2] : 
	                     intcode_mem->data[intcode_mem->data[*instr_ptr+2]];

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

	intcode_mem->data[intcode_mem->data[*instr_ptr+3]] = result;
	*instr_ptr += 4;
}

void input_op(int_vec* intcode_mem, int* instr_ptr, int input)
{
	intcode_mem->data[intcode_mem->data[*instr_ptr+1]] = input;
	*instr_ptr += 2;
}

int output_op(int_vec* intcode_mem, int* instr_ptr, int parm1_mode)
{
	int output = parm1_mode ? intcode_mem->data[*instr_ptr+1] : 
		                  intcode_mem->data[intcode_mem->data[*instr_ptr+1]];
	*instr_ptr += 2;

	return output;
}

void jmp_op(int_vec* intcode_mem, int* instr_ptr, int parm1_mode, int parm2_mode, int jmp_condition)
{
	int condition = parm1_mode ? intcode_mem->data[*instr_ptr+1] : 
		                     intcode_mem->data[intcode_mem->data[*instr_ptr+1]];

	//a true condition can be any non-zero value
	if (condition)
		condition = 1;

	if (condition == jmp_condition)
	{
		*instr_ptr = parm2_mode ? intcode_mem->data[*instr_ptr+2] :
			                  intcode_mem->data[intcode_mem->data[*instr_ptr+2]];
	}
	else
	{
		*instr_ptr += 3;
	}
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
