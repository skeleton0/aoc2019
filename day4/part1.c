#include <stdio.h>
#include <string.h>

int add_match(int*);

int main()
{
	int password[] = {2,7,7,7,7,7};
	int upper_bound[] = {7,7,9,9,9,9};

	int matches = 1; //initial value is a match

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
			
			//previous for loop will always produce repeated adjacent digits
			++matches;
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
	return password[0] == password[1] || 
	       password[1] == password[2] ||
	       password[2] == password[3] ||
	       password[3] == password[4] ||
	       password[4] == password[5];
}
