#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHEM_NAMELEN 5
#define UNIQUE_CHEMICALS 57
#define MAX_INPUT_LINELEN 100

struct chemical
{
	char name[MAX_CHEM_NAMELEN + 1];
	int quantity;
};

struct reaction
{
	struct chemical result;
	struct chemical* ingredients;
	int ingredients_len;
};

struct reaction reactions[UNIQUE_CHEMICALS];
struct chemical leftovers[UNIQUE_CHEMICALS];

void parse_input();
struct reaction* find_reaction(char* name);
struct chemical* find_leftover(char* name);
long long ore_cost(char* chemical, long long quantity);

int main()
{
	parse_input();

	long long ore = 1000000000000; //one trillion

	//get maximum possible ore cost for a single fuel production (i.e. no leftovers used)
	int ore_per_fuel = ore_cost("FUEL", 1);
	ore -=  ore_per_fuel;
	int fuel = 1;

	int fuel_production_est = ore / ore_per_fuel;
	while (ore > 0)
	{
		ore -= ore_cost("FUEL", fuel_production_est);

		if (ore >= 0)
			fuel += fuel_production_est;

		fuel_production_est = ore / ore_per_fuel;
		if (fuel_production_est < 1)
			fuel_production_est = 1;
	}

	printf("Fuel produced: %d\n", fuel);
	
	return 0;
}

void parse_input()
{
	for (int i = 0; i < UNIQUE_CHEMICALS; ++i)
	{
		char buffer[MAX_INPUT_LINELEN];
		if (fgets(buffer, MAX_INPUT_LINELEN, stdin) == NULL)
		{
			perror("Failed to read input");
			exit(1);
		}

		//calculate amount of ingredients reaction requires
		struct reaction* reaction = &reactions[i];
		reaction->ingredients_len = 1;

		for (int c = 0; c < strlen(buffer); ++c)
			if (buffer[c] == ',')
				++reaction->ingredients_len;

		reaction->ingredients = (struct chemical*) malloc(reaction->ingredients_len * sizeof(struct chemical));

		//parse line into reaction structure
		int ingredient_idx = 0;
		for (char* token = strtok(buffer, ","); token != NULL; token = strtok(NULL, ","))
		{
			struct chemical* ingredient = &reaction->ingredients[ingredient_idx];
			if (sscanf(token, "%d %s", &ingredient->quantity, ingredient->name) != 2)
			{
				perror("Failed to parse ingredient");
				exit(1);
			}

			++ingredient_idx;
			if (ingredient_idx == reaction->ingredients_len)
			{
				token = strtok(token, "=>");
				token = strtok(NULL, "=>");

				if (token == NULL || sscanf(token, "%d %s", &reaction->result.quantity, reaction->result.name) != 2)
				{
					fprintf(stderr, "Failed parsing result chemical\n");
					exit(1);
				}
			}
		}	
	}

	//add chemicals to leftovers
	for (int i = 0; i < UNIQUE_CHEMICALS; ++i)
	{
		leftovers[i] = reactions[i].result;
		leftovers[i].quantity = 0;
	}
}

struct reaction* find_reaction(char* name)
{
	for (int i = 0; i < UNIQUE_CHEMICALS; ++i)
	{
		if (strcmp(reactions[i].result.name, name) == 0)
			return &reactions[i];
	}

	fprintf(stderr, "Failed to find reaction\n");
	exit(1);	
}

struct chemical* find_leftover(char* name)
{
	for (int i = 0; i < UNIQUE_CHEMICALS; ++i)
	{
		if (strcmp(leftovers[i].name, name) == 0)
			return &leftovers[i];
	}

	fprintf(stderr, "Failed to find leftover\n");
	exit(1);	
}

long long ore_cost(char* name, long long quantity)
{
	if (!quantity || strcmp(name, "ORE") == 0)
		return quantity;

	struct reaction* reaction = find_reaction(name);
	struct chemical* leftover = find_leftover(name);

	//calculate quantities of this chemical we need
	int from_leftovers = leftover->quantity;
	leftover->quantity = 0;
	long long remaining = quantity - from_leftovers;

	long long ingredient_factor = remaining / reaction->result.quantity;
	if (ingredient_factor * reaction->result.quantity < remaining)
		++ingredient_factor;
	
	//calculate ore required
	long long ore = 0;
	for (int i = 0; i < reaction->ingredients_len; ++i)
	{
		struct chemical* ingredient = &reaction->ingredients[i];
		ore += ore_cost(ingredient->name, ingredient->quantity * ingredient_factor);
	}

	//add leftovers
	long long from_reaction = ingredient_factor * reaction->result.quantity;
	leftover->quantity += from_leftovers + from_reaction  - quantity;

	return ore;
}
