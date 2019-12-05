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

	Point closest_intersect = {0, 0};

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

		for (int j = 0; j < wire2.size-1; ++j)
		{
			int path2_is_y_direction = wire2.data[j].x == wire2.data[j+1].x;

			if (path1_is_y_direction == path2_is_y_direction)
				continue;

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

			if (path1_range.low < path2_const_dimension &&
			    path2_const_dimension < path1_range.high &&
			    path2_range.low < path1_const_dimension &&
			    path1_const_dimension < path2_range.high)
			{
				Point intersect;

				if (path1_is_y_direction)
				{
					intersect.x = path1_const_dimension;
					intersect.y = path2_const_dimension;
				}
				else
				{
					intersect.x = path2_const_dimension;
					intersect.y = path1_const_dimension;
				}

				int closest_manhattan = abs(closest_intersect.x) + abs(closest_intersect.y);
				int manhattan = abs(intersect.x) + abs(intersect.y);

				if (manhattan < closest_manhattan || closest_manhattan == 0)
					closest_intersect = intersect;
			}
		}
	}

	printf("Closest intersect = {%d, %d}\n", closest_intersect.x, closest_intersect.y);
	printf("Manhattan distance = %d\n", abs(closest_intersect.x) + abs(closest_intersect.y));

	free(wire1.data);
	free(wire2.data);
	return 0;
}
