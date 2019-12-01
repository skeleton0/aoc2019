#include <stdio.h>

int calc_fuel(int mass)
{
	int fuel_req = mass / 3 - 2;
	if (fuel_req <= 0)
		return 0;
	
	return fuel_req + calc_fuel(fuel_req);
}

int main()
{
	int fuel_req = 0;
	for (int i = 0; i < 100; ++i)
	{
		int mass = 0;
		if (scanf("%d", &mass) != 1)
		{
			printf("Input error!\n");
			return 1;
		}
		
		fuel_req += calc_fuel(mass);
	}
	
	printf("Total fuel requirement for modules is %d\n", fuel_req);
	return 0;
}
	
