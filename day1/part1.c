#include <stdio.h>

int main()
{
	int fuel_req = 0;
	for (int i = 0; i < 100; ++i)
	{
		int mass = 0;
		if (scanf("%d", &mass) == 1)
		{
			fuel_req += mass / 3 - 2;
		}
		else
		{
			printf("Input error!\n");
			return 1;
		}
		
	}
	
	printf("Total fuel requirement for modules is %d\n", fuel_req);
	return 0;
}
	
