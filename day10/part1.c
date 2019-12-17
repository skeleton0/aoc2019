#include <stdio.h>
#include <math.h>

#define WIDTH 34
#define HEIGHT 34
#define ASTEROID '#'

char map[WIDTH][HEIGHT];
void populate_map();

int main()
{
	populate_map();

	int x = 0, y = 0;
	int bestx = 0, besty = 0;
	int most_viewed_asteroids = 0;
	while (y < HEIGHT)
	{
		if (map[x][y] == ASTEROID)
		{
			int asteroid_view_count = 0;
			int targetx = 0, targety = 0;
			while (targety < HEIGHT)
			{
				if (map[targetx][targety] == ASTEROID && (targetx != x || targety != y))
				{
					int xdiff = targetx - x;
					int ydiff = targety - y;

					//handle special case for asteroids directly north or south
					if (xdiff == 0)
					{
						int incr = ydiff > 0 ? 1 : -1;

						for (int visiony = y + incr;; visiony += incr)
						{
							if (map[x][visiony] == ASTEROID)
							{
								if (visiony == targety)
									++asteroid_view_count;

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

							int realx = visionx + x;
							int realy = visiony + y;
							if (map[realx][realy] == ASTEROID)
							{
								if (realx == targetx)
									++asteroid_view_count;

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

			if (asteroid_view_count > most_viewed_asteroids)
			{
				most_viewed_asteroids = asteroid_view_count;
				bestx = x;
				besty = y;
			}
		}

		if ((++x) == WIDTH)
		{
			x = 0;
			++y;
		}
	}

	printf("Best position for tracking station is at %d, %d\n", bestx, besty);
	printf("From this asteroid position you can see %d other asteroids\n", most_viewed_asteroids);

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
				break;

			case '\n':
			        ++y;	
				x = 0;
		}
	}
}
