#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define WIDTH 34
#define HEIGHT 34
#define ASTEROID '#'
#define LASER_X 26
#define LASER_Y 28
#define VAPORIZE_MARK 'X'
#define MAX_LOCS WIDTH * HEIGHT

//vaporize location
struct vloc
{
	int x;
	int y;
	double angle;
};

char map[WIDTH][HEIGHT];
void populate_map();
int comp_vloc(const void* a, const void* b);

int main()
{
	populate_map();

	int vaporized = 0;
	while (vaporized < 200)
	{
		int vaporize_map_idx = 0;
		struct vloc vaporize_map[MAX_LOCS];

		int targetx = 0, targety = 0;
		while (targety < HEIGHT)
		{
			if (map[targetx][targety] == ASTEROID && (targetx != LASER_X || targety != LASER_Y))
			{
				int xdiff = targetx - LASER_X;
				int ydiff = targety - LASER_Y;

				//handle special case for asteroids directly north or south
				if (xdiff == 0)
				{
					int incr = ydiff > 0 ? 1 : -1;

					for (int visiony = LASER_Y + incr;; visiony += incr)
					{
						if (map[LASER_X][visiony] == ASTEROID)
						{
							if (visiony == targety)
							{
								struct vloc* v = &vaporize_map[vaporize_map_idx];
								v->x = targetx;
								v->y = targety;
								v->angle = ydiff > 0 ? M_PI : 0;

								++vaporize_map_idx;
							}

							break;
						}
					}
				}
				else
				{
					//calculate gradient
					double m = (double)ydiff / (double)xdiff;
					int incr = xdiff > 0 ? 1 : -1;

					for (int visionx = 0 + incr;; visionx += incr)
					{
						float visiony = m * (double)visionx;
						
						//if visiony is not an integer, it cannot be on an asteroid position
						if (floorf(visiony) != visiony)
							continue;

						int realx = visionx + LASER_X;
						int realy = visiony + LASER_Y;
						if (map[realx][realy] == ASTEROID)
						{
							if (realx == targetx)
							{
								struct vloc* v = &vaporize_map[vaporize_map_idx];
								v->x = targetx;
								v->y = targety;

								if (ydiff)
								{
									double c = sqrtf(powf(xdiff, 2) + powf(ydiff, 2));

									if (xdiff > 0 && ydiff < 0)
										v->angle = acos(((double)abs(ydiff)) / c);
									else if (xdiff > 0 && ydiff > 0)
										v->angle = acos(((double)abs(xdiff)) / c) + M_PI/2;
									else if (xdiff < 0 && ydiff > 0)
										v->angle = acos(((double)abs(ydiff)) / c) + M_PI;
									else
										v->angle = acos(((double)abs(xdiff)) / c) + M_PI*1.5l; 
								}
								else
									v->angle = xdiff > 0 ? M_PI/2 : M_PI*1.5;

								++vaporize_map_idx;
							}

							break;
						}
					}
				}
			}

			if ((++targetx) == WIDTH)
			{
				targetx = 0;
				++targety;
			}
		}

		qsort(vaporize_map, vaporize_map_idx, sizeof(struct vloc), comp_vloc);
		for (int i = 0; i < vaporize_map_idx; ++i)
		{
			struct vloc* vap = &vaporize_map[i];
			++vaporized;

			if (vaporized == 200)
			{
				printf("The 200th asteroid vaporized was at coordinate %d, %d\n", vap->x, vap->y);
				return 0;
			}

			printf("%dth vaporized asteroid is at %d, %d\n", vaporized, vap->x, vap->y);
			map[vap->x][vap->y] = '.';
		}
	}


	return 0;
}

void populate_map()
{
	int x = 0, y = 0;
	int c;
	while ((c = getchar()) != EOF)
	{
		switch (c)
		{
			case '#':
			case '.':
				map[x][y] = c;	
				++x;
				printf("%c", c);
				break;

			case '\n':
			        ++y;	
				x = 0;
				printf("\n");
		}
	}

	
}

int comp_vloc(const void* a, const void* b)
{
	struct vloc* vloc_a = (struct vloc*) a;
	struct vloc* vloc_b = (struct vloc*) b;

	if (vloc_a->angle > vloc_b->angle)
		return 1;
	else if (vloc_a->angle < vloc_b->angle)
		return -1;
	else
		return 0;	
}
