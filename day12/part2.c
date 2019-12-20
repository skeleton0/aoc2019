#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MOONS 4
#define DIMENSIONS 3

long long get_lcm(long long a, long long b);

struct moon
{
	int x, y, z; 	//position
	int vx, vy, vz;	//velocity
};

int main()
{
	int periods[DIMENSIONS] = {0};
	struct moon initial[] = {{5, 4, 4, 0, 0, 0},
		                 {-11, -11, -3, 0, 0, 0},
			         {0, 7, 0, 0, 0, 0},
			         {-13, 2, 10, 0, 0, 0}};
	struct moon moons[MOONS];
	memcpy(moons, initial, sizeof(moons));

	int steps = 0;
	while (!periods[0] || !periods[1] || !periods[2])
	{
		for (int a = 0; a < MOONS; ++a)
		{
			int gravity[DIMENSIONS] = {0};				
			
			for (int b = 0; b < MOONS; ++b)
			{
				//if same moons
				if (a == b)
					continue;

				int pos_deltas[] = {moons[b].x - moons[a].x, 
					 	    moons[b].y - moons[a].y, 
						    moons[b].z - moons[a].z};

				for (int j = 0; j < DIMENSIONS; ++j)
				{
					if (pos_deltas[j] > 0)
						gravity[j] += 1;
					else if (pos_deltas[j] < 0)
						gravity[j] -= 1;
				}

			}

			//update velocity
			moons[a].vx += gravity[0];
			moons[a].vy += gravity[1];
			moons[a].vz += gravity[2];
		}

		//update position
		for (int j = 0; j < MOONS; ++j)
		{
			moons[j].x += moons[j].vx;
			moons[j].y += moons[j].vy;
			moons[j].z += moons[j].vz;
		}

		++steps;

		int matches[] = {1, 1, 1};
		for (int j = 0; j < MOONS; ++j)
		{
			if (moons[j].x != initial[j].x || moons[j].vx)
			{
				matches[0] = 0;
			}

			if (moons[j].y != initial[j].y || moons[j].vy)
			{
				matches[1] = 0;
			}

			if (moons[j].z != initial[j].z || moons[j].vz)
			{
				matches[2] = 0;
			}
		}

		for (int j = 0; j < DIMENSIONS; ++j)
		{
			if (!periods[j] && matches[j])
				periods[j] = steps;
		}
	}
	long long lcm = get_lcm(periods[0], periods[1]);
	lcm = get_lcm(lcm, periods[2]);

	printf("LCM: %lld\n", lcm);

	return 0;
}

long long get_lcm(long long a, long long b)
{
	//small brain way of calculating LCM
	int hcf;
	for (int i = 1; i <= a && i <= b; ++i)
		if (a % i + b % i == 0)
			hcf = i;

	return (long long int)a * b / hcf;
}
