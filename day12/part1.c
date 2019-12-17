#include <stdlib.h>
#include <stdio.h>

#define MOONS 4
#define DIMENSIONS 3
#define STEPS 1000

struct moon
{
	int x, y, z; 	//position
	int vx, vy, vz;	//velocity
};

int main()
{
	struct moon moons[] = {{5, 4, 4, 0, 0, 0},
		               {-11, -11, -3, 0, 0, 0},
			       {0, 7, 0, 0, 0, 0},
			       {-13, 2, 10, 0, 0, 0}};
	
	for (int i = 0; i < STEPS; ++i)
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
	}

	//calculate total energy of system
	int energy = 0;
	for (int i = 0; i < MOONS; ++i)
	{
		int ep = abs(moons[i].x) + abs(moons[i].y) + abs(moons[i].z);
		int ek = abs(moons[i].vx) + abs(moons[i].vy) + abs(moons[i].vz);	
		energy += ep * ek;
	}

	printf("Total energy of system: %d\n", energy);

	return 0;
}
