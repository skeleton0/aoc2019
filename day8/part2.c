#include <stdio.h>

int main()
{
	int image[150];
	for (int i = 0; i < 150; ++i)
		image[i] = 2;

	int pixel = 0;
	char c = getchar();
	while (c != '\n')
	{
		if (image[pixel] == 2)
			image[pixel] = c - '0';

		++pixel;
		c = getchar();

		if (pixel == 150)
		{
			pixel = 0;
		}
	}

	for (int i = 0; i < 150; ++i)
	{
		if (i % 25 == 0)
			printf("\n");

		printf("%d", image[i]);
	}

	printf("\n");

	return 0;
}
