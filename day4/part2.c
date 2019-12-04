#include <stdio.h>
#include <string.h>

int add_match(int*);
void print_password(int*);

int main()
{
	int password[] = {2,7,7,7,7,7};
	int upper_bound[] = {7,7,9,9,9,9};

	int matches = 0;

	int curr_digit = 5;
	while (memcmp(password, upper_bound, sizeof(password)))
	{
		if (password[curr_digit] == 9)
		{
			--curr_digit;
			continue;
		}
		else if (curr_digit != 5)
		{
			++password[curr_digit];
			//set following digits to same value
			for (int i = curr_digit+1; i < 6; ++i)
				password[i] = password[curr_digit];
			
			matches += add_match(password);
			curr_digit = 5;
			continue;
		}

		++password[5];
		matches += add_match(password);
	}

	printf("Possible matches = %d\n", matches);
	return 0;
}

int add_match(int* password)
{
	for (int i = 0; i <= 5; i++)
	{
		int val = password[i];
		int left_neighbour = i-1 >= 0 && password[i-1] == val; 
		int right_neighbour = i+1 <= 5 && password[i+1] == val;

		if (left_neighbour == right_neighbour)
			continue;

		if (left_neighbour && (i-2 < 0 || password[i-2] != val))
			return 1;

		if (right_neighbour && (i+2 > 5 || password[i+2] != val))
			return 1;
	}

	return 0;
}

void print_password(int* password)
{
	for (int i = 0; i < 6; i++)
	{
		printf("%d", password[i]);
	}

	printf("\n");
}
