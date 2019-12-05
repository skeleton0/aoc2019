#include <stdio.h>
#include <bigbrainvector.h>
#include <string.h>

struct Range 
{
	int low;
	int high;
};

typedef struct Point
{
	int x;
	int y;
} Point;

generate_vector(Point);

void populate_Point_vec(Point_vec* vec)
{
	char direction;
	int path_length;

	Point current_point = {0, 0};
	append_Point_vec(vec, current_point);

	char path_string[2000];
	fgets(path_string, 2000, stdin);

	char* token_begin = strtok(path_string, ",");

	if (!token_begin)
	{
		printf("path_string unexpectedly small\n");
		exit(EXIT_FAILURE);
	}

	do
	{
		if (sscanf(token_begin, "%c%d", &direction, &path_length) != 2)
		{
			printf("Failure extracting direction and path length from token\n");
			exit(EXIT_FAILURE);
		}

		switch(direction)
		{
			case 'U':
				current_point.y += path_length;
				break;
			case 'R':
				current_point.x += path_length;
				break;
			case 'D':
				current_point.y -= path_length;
				break;
			case 'L':
				current_point.x -= path_length;
				break;
			default:
				printf("Encountered unknown direction from input text\n");
				exit(EXIT_FAILURE);
		}

		append_Point_vec(vec, current_point);
	} while (token_begin = strtok(NULL, ",\n"));	
}

struct Range get_range(int a, int b)
{
	struct Range range = {a, b};
	
	if (a > b)
		range = (struct Range){b, a};

	return range;
}

int main()
{
	Point_vec wire1 = build_Point_vec();
	Point_vec wire2 = build_Point_vec();

	populate_Point_vec(&wire1);
	populate_Point_vec(&wire2);

	Point quickest_intersect = {0, 0};
	int lowest_combined_steps = 0;

	int wire1_steps = 0;
	for (int i = 0; i < wire1.size-1; ++i)
	{
		int path1_is_y_direction = wire1.data[i].x == wire1.data[i+1].x;
		int path1_const_dimension;
		struct Range path1_range;

		if (path1_is_y_direction)
		{
			path1_const_dimension = wire1.data[i].x;
			path1_range = get_range(wire1.data[i].y, wire1.data[i+1].y);
		}
		else
		{
			path1_const_dimension = wire1.data[i].y;
			path1_range = get_range(wire1.data[i].x, wire1.data[i+1].x);
		}

		int wire2_steps = 0;
		for (int j = 0; j < wire2.size-1; ++j)
		{
			int path2_is_y_direction = wire2.data[j].x == wire2.data[j+1].x;

			int path2_const_dimension;
			struct Range path2_range;
			if (path2_is_y_direction)
			{
				path2_const_dimension = wire2.data[j].x;
				path2_range = get_range(wire2.data[j].y, wire2.data[j+1].y);
			}
			else
			{
				path2_const_dimension = wire2.data[j].y;
				path2_range = get_range(wire2.data[j].x, wire2.data[j+1].x);
			}

			if (path1_is_y_direction != path2_is_y_direction &&
			    path1_range.low < path2_const_dimension &&
			    path2_const_dimension < path1_range.high &&
			    path2_range.low < path1_const_dimension &&
			    path1_const_dimension < path2_range.high)
			{
				Point intersect;
				int combined_steps = wire1_steps + wire2_steps;

				if (path1_is_y_direction)
				{
					intersect.x = path1_const_dimension;
					intersect.y = path2_const_dimension;
					combined_steps += abs(wire1.data[i].y - path2_const_dimension) + 
						          abs(wire2.data[j].x - path1_const_dimension);
				}
				else
				{
					intersect.x = path2_const_dimension;
					intersect.y = path1_const_dimension;
					combined_steps += abs(wire1.data[i].x - path2_const_dimension) +
							  abs(wire2.data[j].y - path1_const_dimension);
				}

				if (combined_steps < lowest_combined_steps || lowest_combined_steps == 0)
				{
					quickest_intersect = intersect;
					lowest_combined_steps = combined_steps;
				}
			}

			wire2_steps += abs(path2_range.high - path2_range.low);
		}

		wire1_steps += abs(path1_range.high - path1_range.low);
	}

	printf("Quickest intersect = {%d, %d}\n", quickest_intersect.x, quickest_intersect.y);
	printf("Combined steps to reach intersection = %d\n", lowest_combined_steps);

	free(wire1.data);
	free(wire2.data);
	return 0;
}
